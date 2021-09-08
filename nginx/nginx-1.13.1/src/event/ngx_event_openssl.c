
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#define NGX_SSL_PASSWORD_BUFFER_SIZE  4096


typedef struct {
    ngx_uint_t  engine;   /* unsigned  engine:1; */
} ngx_openssl_conf_t;


static int ngx_ssl_password_callback(char *buf, int size, int rwflag,
    void *userdata);
static int ngx_ssl_verify_callback(int ok, X509_STORE_CTX *x509_store);
static void ngx_ssl_info_callback(const ngx_ssl_conn_t *ssl_conn, int where,
    int ret);
static void ngx_ssl_passwords_cleanup(void *data);
static void ngx_ssl_handshake_handler(ngx_event_t *ev);
static ngx_int_t ngx_ssl_handle_recv(ngx_connection_t *c, int n);
static void ngx_ssl_write_handler(ngx_event_t *wev);
static void ngx_ssl_read_handler(ngx_event_t *rev);
static void ngx_ssl_shutdown_handler(ngx_event_t *ev);
static void ngx_ssl_connection_error(ngx_connection_t *c, int sslerr,
    ngx_err_t err, char *text);
static void ngx_ssl_clear_error(ngx_log_t *log);

static ngx_int_t ngx_ssl_session_id_context(ngx_ssl_t *ssl,
    ngx_str_t *sess_ctx);
ngx_int_t ngx_ssl_session_cache_init(ngx_shm_zone_t *shm_zone, void *data);
static int ngx_ssl_new_session(ngx_ssl_conn_t *ssl_conn,
    ngx_ssl_session_t *sess);
static ngx_ssl_session_t *ngx_ssl_get_cached_session(ngx_ssl_conn_t *ssl_conn,
#if OPENSSL_VERSION_NUMBER >= 0x10100003L
    const
#endif
    u_char *id, int len, int *copy);
static void ngx_ssl_remove_session(SSL_CTX *ssl, ngx_ssl_session_t *sess);
static void ngx_ssl_expire_sessions(ngx_ssl_session_cache_t *cache,
    ngx_slab_pool_t *shpool, ngx_uint_t n);
static void ngx_ssl_session_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);

#ifdef SSL_CTRL_SET_TLSEXT_TICKET_KEY_CB
static int ngx_ssl_session_ticket_key_callback(ngx_ssl_conn_t *ssl_conn,
    unsigned char *name, unsigned char *iv, EVP_CIPHER_CTX *ectx,
    HMAC_CTX *hctx, int enc);
#endif

#ifndef X509_CHECK_FLAG_ALWAYS_CHECK_SUBJECT
static ngx_int_t ngx_ssl_check_name(ngx_str_t *name, ASN1_STRING *str);
#endif

static time_t ngx_ssl_parse_time(
#if OPENSSL_VERSION_NUMBER > 0x10100000L
    const
#endif
    ASN1_TIME *asn1time);

static void *ngx_openssl_create_conf(ngx_cycle_t *cycle);
static char *ngx_openssl_engine(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static void ngx_openssl_exit(ngx_cycle_t *cycle);


static ngx_command_t  ngx_openssl_commands[] = {

    { ngx_string("ssl_engine"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_openssl_engine,
      0,
      0,
      NULL },

      ngx_null_command
};


static ngx_core_module_t  ngx_openssl_module_ctx = {
    ngx_string("openssl"),
    ngx_openssl_create_conf,
    NULL
};


ngx_module_t  ngx_openssl_module = {
    NGX_MODULE_V1,
    &ngx_openssl_module_ctx,               /* module context */
    ngx_openssl_commands,                  /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    ngx_openssl_exit,                      /* exit master */
    NGX_MODULE_V1_PADDING
};


int  ngx_ssl_connection_index;
int  ngx_ssl_server_conf_index;
int  ngx_ssl_session_cache_index;
int  ngx_ssl_session_ticket_keys_index;
int  ngx_ssl_certificate_index;
int  ngx_ssl_next_certificate_index;
int  ngx_ssl_certificate_name_index;
int  ngx_ssl_stapling_index;


ngx_int_t
ngx_ssl_init(ngx_log_t *log)
{
#if OPENSSL_VERSION_NUMBER >= 0x10100003L

    if (OPENSSL_init_ssl(OPENSSL_INIT_LOAD_CONFIG, NULL) == 0) {
        ngx_ssl_error(NGX_LOG_ALERT, log, 0, "OPENSSL_init_ssl() failed");
        return NGX_ERROR;
    }

    /*
     * OPENSSL_init_ssl() may leave errors in the error queue
     * while returning success
     */

    ERR_clear_error();

#else

    OPENSSL_config(NULL);

    SSL_library_init();
    SSL_load_error_strings();

    OpenSSL_add_all_algorithms();

#endif

#if OPENSSL_VERSION_NUMBER >= 0x0090800fL
#ifndef SSL_OP_NO_COMPRESSION
    {
    /*
     * Disable gzip compression in OpenSSL prior to 1.0.0 version,
     * this saves about 522K per connection.
     */
    int                  n;
    STACK_OF(SSL_COMP)  *ssl_comp_methods;

    ssl_comp_methods = SSL_COMP_get_compression_methods();
    n = sk_SSL_COMP_num(ssl_comp_methods);

    while (n--) {
        (void) sk_SSL_COMP_pop(ssl_comp_methods);
    }
    }
#endif
#endif

    ngx_ssl_connection_index = SSL_get_ex_new_index(0, NULL, NULL, NULL, NULL);

    if (ngx_ssl_connection_index == -1) {
        ngx_ssl_error(NGX_LOG_ALERT, log, 0, "SSL_get_ex_new_index() failed");
        return NGX_ERROR;
    }

    ngx_ssl_server_conf_index = SSL_CTX_get_ex_new_index(0, NULL, NULL, NULL,
                                                         NULL);
    if (ngx_ssl_server_conf_index == -1) {
        ngx_ssl_error(NGX_LOG_ALERT, log, 0,
                      "SSL_CTX_get_ex_new_index() failed");
        return NGX_ERROR;
    }

    ngx_ssl_session_cache_index = SSL_CTX_get_ex_new_index(0, NULL, NULL, NULL,
                                                           NULL);
    if (ngx_ssl_session_cache_index == -1) {
        ngx_ssl_error(NGX_LOG_ALERT, log, 0,
                      "SSL_CTX_get_ex_new_index() failed");
        return NGX_ERROR;
    }

    ngx_ssl_session_ticket_keys_index = SSL_CTX_get_ex_new_index(0, NULL, NULL,
                                                                 NULL, NULL);
    if (ngx_ssl_session_ticket_keys_index == -1) {
        ngx_ssl_error(NGX_LOG_ALERT, log, 0,
                      "SSL_CTX_get_ex_new_index() failed");
        return NGX_ERROR;
    }

    ngx_ssl_certificate_index = SSL_CTX_get_ex_new_index(0, NULL, NULL, NULL,
                                                         NULL);
    if (ngx_ssl_certificate_index == -1) {
        ngx_ssl_error(NGX_LOG_ALERT, log, 0,
                      "SSL_CTX_get_ex_new_index() failed");
        return NGX_ERROR;
    }

    ngx_ssl_next_certificate_index = X509_get_ex_new_index(0, NULL, NULL, NULL,
                                                           NULL);
    if (ngx_ssl_next_certificate_index == -1) {
        ngx_ssl_error(NGX_LOG_ALERT, log, 0, "X509_get_ex_new_index() failed");
        return NGX_ERROR;
    }

    ngx_ssl_certificate_name_index = X509_get_ex_new_index(0, NULL, NULL, NULL,
                                                           NULL);

    if (ngx_ssl_certificate_name_index == -1) {
        ngx_ssl_error(NGX_LOG_ALERT, log, 0, "X509_get_ex_new_index() failed");
        return NGX_ERROR;
    }

    ngx_ssl_stapling_index = X509_get_ex_new_index(0, NULL, NULL, NULL, NULL);

    if (ngx_ssl_stapling_index == -1) {
        ngx_ssl_error(NGX_LOG_ALERT, log, 0, "X509_get_ex_new_index() failed");
        return NGX_ERROR;
    }

    return NGX_OK;
}


ngx_int_t
ngx_ssl_create(ngx_ssl_t *ssl, ngx_uint_t protocols, void *data)
{
    ssl->ctx = SSL_CTX_new(SSLv23_method());

    if (ssl->ctx == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0, "SSL_CTX_new() failed");
        return NGX_ERROR;
    }

    if (SSL_CTX_set_ex_data(ssl->ctx, ngx_ssl_server_conf_index, data) == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_set_ex_data() failed");
        return NGX_ERROR;
    }

    if (SSL_CTX_set_ex_data(ssl->ctx, ngx_ssl_certificate_index, NULL) == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_set_ex_data() failed");
        return NGX_ERROR;
    }

    ssl->buffer_size = NGX_SSL_BUFSIZE;

    /* client side options */

#ifdef SSL_OP_MICROSOFT_SESS_ID_BUG
    SSL_CTX_set_options(ssl->ctx, SSL_OP_MICROSOFT_SESS_ID_BUG);
#endif

#ifdef SSL_OP_NETSCAPE_CHALLENGE_BUG
    SSL_CTX_set_options(ssl->ctx, SSL_OP_NETSCAPE_CHALLENGE_BUG);
#endif

    /* server side options */

#ifdef SSL_OP_SSLREF2_REUSE_CERT_TYPE_BUG
    SSL_CTX_set_options(ssl->ctx, SSL_OP_SSLREF2_REUSE_CERT_TYPE_BUG);
#endif

#ifdef SSL_OP_MICROSOFT_BIG_SSLV3_BUFFER
    SSL_CTX_set_options(ssl->ctx, SSL_OP_MICROSOFT_BIG_SSLV3_BUFFER);
#endif

#ifdef SSL_OP_MSIE_SSLV2_RSA_PADDING
    /* this option allow a potential SSL 2.0 rollback (CAN-2005-2969) */
    SSL_CTX_set_options(ssl->ctx, SSL_OP_MSIE_SSLV2_RSA_PADDING);
#endif

#ifdef SSL_OP_SSLEAY_080_CLIENT_DH_BUG
    SSL_CTX_set_options(ssl->ctx, SSL_OP_SSLEAY_080_CLIENT_DH_BUG);
#endif

#ifdef SSL_OP_TLS_D5_BUG
    SSL_CTX_set_options(ssl->ctx, SSL_OP_TLS_D5_BUG);
#endif

#ifdef SSL_OP_TLS_BLOCK_PADDING_BUG
    SSL_CTX_set_options(ssl->ctx, SSL_OP_TLS_BLOCK_PADDING_BUG);
#endif

#ifdef SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS
    SSL_CTX_set_options(ssl->ctx, SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS);
#endif

    SSL_CTX_set_options(ssl->ctx, SSL_OP_SINGLE_DH_USE);

#ifdef SSL_CTRL_CLEAR_OPTIONS
    /* only in 0.9.8m+ */
    SSL_CTX_clear_options(ssl->ctx,
                          SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3|SSL_OP_NO_TLSv1);
#endif

    if (!(protocols & NGX_SSL_SSLv2)) {
        SSL_CTX_set_options(ssl->ctx, SSL_OP_NO_SSLv2);
    }
    if (!(protocols & NGX_SSL_SSLv3)) {
        SSL_CTX_set_options(ssl->ctx, SSL_OP_NO_SSLv3);
    }
    if (!(protocols & NGX_SSL_TLSv1)) {
        SSL_CTX_set_options(ssl->ctx, SSL_OP_NO_TLSv1);
    }
#ifdef SSL_OP_NO_TLSv1_1
    SSL_CTX_clear_options(ssl->ctx, SSL_OP_NO_TLSv1_1);
    if (!(protocols & NGX_SSL_TLSv1_1)) {
        SSL_CTX_set_options(ssl->ctx, SSL_OP_NO_TLSv1_1);
    }
#endif
#ifdef SSL_OP_NO_TLSv1_2
    SSL_CTX_clear_options(ssl->ctx, SSL_OP_NO_TLSv1_2);
    if (!(protocols & NGX_SSL_TLSv1_2)) {
        SSL_CTX_set_options(ssl->ctx, SSL_OP_NO_TLSv1_2);
    }
#endif
#ifdef SSL_OP_NO_TLSv1_3
    SSL_CTX_clear_options(ssl->ctx, SSL_OP_NO_TLSv1_3);
    if (!(protocols & NGX_SSL_TLSv1_3)) {
        SSL_CTX_set_options(ssl->ctx, SSL_OP_NO_TLSv1_3);
    }
#endif

#ifdef SSL_OP_NO_COMPRESSION
    SSL_CTX_set_options(ssl->ctx, SSL_OP_NO_COMPRESSION);
#endif

#ifdef SSL_MODE_RELEASE_BUFFERS
    SSL_CTX_set_mode(ssl->ctx, SSL_MODE_RELEASE_BUFFERS);
#endif

#ifdef SSL_MODE_NO_AUTO_CHAIN
    SSL_CTX_set_mode(ssl->ctx, SSL_MODE_NO_AUTO_CHAIN);
#endif

    SSL_CTX_set_read_ahead(ssl->ctx, 1);

    SSL_CTX_set_info_callback(ssl->ctx, ngx_ssl_info_callback);

    return NGX_OK;
}


ngx_int_t
ngx_ssl_certificates(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_array_t *certs,
    ngx_array_t *keys, ngx_array_t *passwords)
{
    ngx_str_t   *cert, *key;
    ngx_uint_t   i;

    cert = certs->elts;
    key = keys->elts;

    for (i = 0; i < certs->nelts; i++) {

        if (ngx_ssl_certificate(cf, ssl, &cert[i], &key[i], passwords)
            != NGX_OK)
        {
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}


ngx_int_t
ngx_ssl_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *cert,
    ngx_str_t *key, ngx_array_t *passwords)
{
    BIO         *bio;
    X509        *x509;
    u_long       n;
    ngx_str_t   *pwd;
    ngx_uint_t   tries;

    if (ngx_conf_full_name(cf->cycle, cert, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    /*
     * we can't use SSL_CTX_use_certificate_chain_file() as it doesn't
     * allow to access certificate later from SSL_CTX, so we reimplement
     * it here
     */

    bio = BIO_new_file((char *) cert->data, "r");
    if (bio == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "BIO_new_file(\"%s\") failed", cert->data);
        return NGX_ERROR;
    }

    x509 = PEM_read_bio_X509_AUX(bio, NULL, NULL, NULL);
    if (x509 == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "PEM_read_bio_X509_AUX(\"%s\") failed", cert->data);
        BIO_free(bio);
        return NGX_ERROR;
    }

    if (SSL_CTX_use_certificate(ssl->ctx, x509) == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_use_certificate(\"%s\") failed", cert->data);
        X509_free(x509);
        BIO_free(bio);
        return NGX_ERROR;
    }

    if (X509_set_ex_data(x509, ngx_ssl_certificate_name_index, cert->data)
        == 0)
    {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0, "X509_set_ex_data() failed");
        X509_free(x509);
        BIO_free(bio);
        return NGX_ERROR;
    }

    if (X509_set_ex_data(x509, ngx_ssl_next_certificate_index,
                      SSL_CTX_get_ex_data(ssl->ctx, ngx_ssl_certificate_index))
        == 0)
    {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0, "X509_set_ex_data() failed");
        X509_free(x509);
        BIO_free(bio);
        return NGX_ERROR;
    }

    if (SSL_CTX_set_ex_data(ssl->ctx, ngx_ssl_certificate_index, x509)
        == 0)
    {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_set_ex_data() failed");
        X509_free(x509);
        BIO_free(bio);
        return NGX_ERROR;
    }

    /* read rest of the chain */

    for ( ;; ) {

        x509 = PEM_read_bio_X509(bio, NULL, NULL, NULL);
        if (x509 == NULL) {
            n = ERR_peek_last_error();

            if (ERR_GET_LIB(n) == ERR_LIB_PEM
                && ERR_GET_REASON(n) == PEM_R_NO_START_LINE)
            {
                /* end of file */
                ERR_clear_error();
                break;
            }

            /* some real error */

            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "PEM_read_bio_X509(\"%s\") failed", cert->data);
            BIO_free(bio);
            return NGX_ERROR;
        }

#ifdef SSL_CTRL_CHAIN_CERT

        /*
         * SSL_CTX_add0_chain_cert() is needed to add chain to
         * a particular certificate when multiple certificates are used;
         * only available in OpenSSL 1.0.2+
         */

        if (SSL_CTX_add0_chain_cert(ssl->ctx, x509) == 0) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "SSL_CTX_add0_chain_cert(\"%s\") failed",
                          cert->data);
            X509_free(x509);
            BIO_free(bio);
            return NGX_ERROR;
        }

#else
        if (SSL_CTX_add_extra_chain_cert(ssl->ctx, x509) == 0) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "SSL_CTX_add_extra_chain_cert(\"%s\") failed",
                          cert->data);
            X509_free(x509);
            BIO_free(bio);
            return NGX_ERROR;
        }
#endif
    }

    BIO_free(bio);

    if (ngx_strncmp(key->data, "engine:", sizeof("engine:") - 1) == 0) {

#ifndef OPENSSL_NO_ENGINE

        u_char      *p, *last;
        ENGINE      *engine;
        EVP_PKEY    *pkey;

        p = key->data + sizeof("engine:") - 1;
        last = (u_char *) ngx_strchr(p, ':');

        if (last == NULL) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid syntax in \"%V\"", key);
            return NGX_ERROR;
        }

        *last = '\0';

        engine = ENGINE_by_id((char *) p);

        if (engine == NULL) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "ENGINE_by_id(\"%s\") failed", p);
            return NGX_ERROR;
        }

        *last++ = ':';

        pkey = ENGINE_load_private_key(engine, (char *) last, 0, 0);

        if (pkey == NULL) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "ENGINE_load_private_key(\"%s\") failed", last);
            ENGINE_free(engine);
            return NGX_ERROR;
        }

        ENGINE_free(engine);

        if (SSL_CTX_use_PrivateKey(ssl->ctx, pkey) == 0) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "SSL_CTX_use_PrivateKey(\"%s\") failed", last);
            EVP_PKEY_free(pkey);
            return NGX_ERROR;
        }

        EVP_PKEY_free(pkey);

        return NGX_OK;

#else

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "loading \"engine:...\" certificate keys "
                           "is not supported");
        return NGX_ERROR;

#endif
    }

    if (ngx_conf_full_name(cf->cycle, key, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    if (passwords) {
        tries = passwords->nelts;
        pwd = passwords->elts;

        SSL_CTX_set_default_passwd_cb(ssl->ctx, ngx_ssl_password_callback);
        SSL_CTX_set_default_passwd_cb_userdata(ssl->ctx, pwd);

    } else {
        tries = 1;
#if (NGX_SUPPRESS_WARN)
        pwd = NULL;
#endif
    }

    for ( ;; ) {

        if (SSL_CTX_use_PrivateKey_file(ssl->ctx, (char *) key->data,
                                        SSL_FILETYPE_PEM)
            != 0)
        {
            break;
        }

        if (--tries) {
            ERR_clear_error();
            SSL_CTX_set_default_passwd_cb_userdata(ssl->ctx, ++pwd);
            continue;
        }

        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_use_PrivateKey_file(\"%s\") failed", key->data);
        return NGX_ERROR;
    }

    SSL_CTX_set_default_passwd_cb(ssl->ctx, NULL);

    return NGX_OK;
}


static int
ngx_ssl_password_callback(char *buf, int size, int rwflag, void *userdata)
{
    ngx_str_t *pwd = userdata;

    if (rwflag) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "ngx_ssl_password_callback() is called for encryption");
        return 0;
    }

    if (pwd->len > (size_t) size) {
        ngx_log_error(NGX_LOG_ERR, ngx_cycle->log, 0,
                      "password is truncated to %d bytes", size);
    } else {
        size = pwd->len;
    }

    ngx_memcpy(buf, pwd->data, size);

    return size;
}


ngx_int_t
ngx_ssl_ciphers(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *ciphers,
    ngx_uint_t prefer_server_ciphers)
{
    if (SSL_CTX_set_cipher_list(ssl->ctx, (char *) ciphers->data) == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_set_cipher_list(\"%V\") failed",
                      ciphers);
        return NGX_ERROR;
    }

    if (prefer_server_ciphers) {
        SSL_CTX_set_options(ssl->ctx, SSL_OP_CIPHER_SERVER_PREFERENCE);
    }

#if (OPENSSL_VERSION_NUMBER < 0x10100001L && !defined LIBRESSL_VERSION_NUMBER)
    /* a temporary 512-bit RSA key is required for export versions of MSIE */
    SSL_CTX_set_tmp_rsa_callback(ssl->ctx, ngx_ssl_rsa512_key_callback);
#endif

    return NGX_OK;
}


ngx_int_t
ngx_ssl_client_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *cert,
    ngx_int_t depth)
{
    STACK_OF(X509_NAME)  *list;

    SSL_CTX_set_verify(ssl->ctx, SSL_VERIFY_PEER, ngx_ssl_verify_callback);

    SSL_CTX_set_verify_depth(ssl->ctx, depth);

    if (cert->len == 0) {
        return NGX_OK;
    }

    if (ngx_conf_full_name(cf->cycle, cert, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    if (SSL_CTX_load_verify_locations(ssl->ctx, (char *) cert->data, NULL)
        == 0)
    {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_load_verify_locations(\"%s\") failed",
                      cert->data);
        return NGX_ERROR;
    }

    /*
     * SSL_CTX_load_verify_locations() may leave errors in the error queue
     * while returning success
     */

    ERR_clear_error();

    list = SSL_load_client_CA_file((char *) cert->data);

    if (list == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_load_client_CA_file(\"%s\") failed", cert->data);
        return NGX_ERROR;
    }

    /*
     * before 0.9.7h and 0.9.8 SSL_load_client_CA_file()
     * always leaved an error in the error queue
     */

    ERR_clear_error();

    SSL_CTX_set_client_CA_list(ssl->ctx, list);

    return NGX_OK;
}


ngx_int_t
ngx_ssl_trusted_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *cert,
    ngx_int_t depth)
{
    SSL_CTX_set_verify_depth(ssl->ctx, depth);

    if (cert->len == 0) {
        return NGX_OK;
    }

    if (ngx_conf_full_name(cf->cycle, cert, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    if (SSL_CTX_load_verify_locations(ssl->ctx, (char *) cert->data, NULL)
        == 0)
    {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_load_verify_locations(\"%s\") failed",
                      cert->data);
        return NGX_ERROR;
    }

    /*
     * SSL_CTX_load_verify_locations() may leave errors in the error queue
     * while returning success
     */

    ERR_clear_error();

    return NGX_OK;
}


ngx_int_t
ngx_ssl_crl(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *crl)
{
    X509_STORE   *store;
    X509_LOOKUP  *lookup;

    if (crl->len == 0) {
        return NGX_OK;
    }

    if (ngx_conf_full_name(cf->cycle, crl, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    store = SSL_CTX_get_cert_store(ssl->ctx);

    if (store == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_get_cert_store() failed");
        return NGX_ERROR;
    }

    lookup = X509_STORE_add_lookup(store, X509_LOOKUP_file());

    if (lookup == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "X509_STORE_add_lookup() failed");
        return NGX_ERROR;
    }

    if (X509_LOOKUP_load_file(lookup, (char *) crl->data, X509_FILETYPE_PEM)
        == 0)
    {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "X509_LOOKUP_load_file(\"%s\") failed", crl->data);
        return NGX_ERROR;
    }

    X509_STORE_set_flags(store,
                         X509_V_FLAG_CRL_CHECK|X509_V_FLAG_CRL_CHECK_ALL);

    return NGX_OK;
}


static int
ngx_ssl_verify_callback(int ok, X509_STORE_CTX *x509_store)
{
#if (NGX_DEBUG)
    char              *subject, *issuer;
    int                err, depth;
    X509              *cert;
    X509_NAME         *sname, *iname;
    ngx_connection_t  *c;
    ngx_ssl_conn_t    *ssl_conn;

    ssl_conn = X509_STORE_CTX_get_ex_data(x509_store,
                                          SSL_get_ex_data_X509_STORE_CTX_idx());

    c = ngx_ssl_get_connection(ssl_conn);

    cert = X509_STORE_CTX_get_current_cert(x509_store);
    err = X509_STORE_CTX_get_error(x509_store);
    depth = X509_STORE_CTX_get_error_depth(x509_store);

    sname = X509_get_subject_name(cert);
    subject = sname ? X509_NAME_oneline(sname, NULL, 0) : "(none)";

    iname = X509_get_issuer_name(cert);
    issuer = iname ? X509_NAME_oneline(iname, NULL, 0) : "(none)";

    ngx_log_debug5(NGX_LOG_DEBUG_EVENT, c->log, 0,
                   "verify:%d, error:%d, depth:%d, "
                   "subject:\"%s\", issuer:\"%s\"",
                   ok, err, depth, subject, issuer);

    if (sname) {
        OPENSSL_free(subject);
    }

    if (iname) {
        OPENSSL_free(issuer);
    }
#endif

    return 1;
}


static void
ngx_ssl_info_callback(const ngx_ssl_conn_t *ssl_conn, int where, int ret)
{
    BIO               *rbio, *wbio;
    ngx_connection_t  *c;

    if ((where & SSL_CB_HANDSHAKE_START)
        && SSL_is_server((ngx_ssl_conn_t *) ssl_conn))
    {
        c = ngx_ssl_get_connection((ngx_ssl_conn_t *) ssl_conn);

        if (c->ssl->handshaked) {
            c->ssl->renegotiation = 1;
            ngx_log_debug0(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL renegotiation");
        }
    }

    if ((where & SSL_CB_ACCEPT_LOOP) == SSL_CB_ACCEPT_LOOP) {
        c = ngx_ssl_get_connection((ngx_ssl_conn_t *) ssl_conn);

        if (!c->ssl->handshake_buffer_set) {
            /*
             * By default OpenSSL uses 4k buffer during a handshake,
             * which is too low for long certificate chains and might
             * result in extra round-trips.
             *
             * To adjust a buffer size we detect that buffering was added
             * to write side of the connection by comparing rbio and wbio.
             * If they are different, we assume that it's due to buffering
             * added to wbio, and set buffer size.
             */

            rbio = SSL_get_rbio((ngx_ssl_conn_t *) ssl_conn);
            wbio = SSL_get_wbio((ngx_ssl_conn_t *) ssl_conn);

            if (rbio != wbio) {
                (void) BIO_set_write_buffer_size(wbio, NGX_SSL_BUFSIZE);
                c->ssl->handshake_buffer_set = 1;
            }
        }
    }
}


RSA *
ngx_ssl_rsa512_key_callback(ngx_ssl_conn_t *ssl_conn, int is_export,
    int key_length)
{
    static RSA  *key;

    if (key_length != 512) {
        return NULL;
    }

#if (OPENSSL_VERSION_NUMBER < 0x10100003L && !defined OPENSSL_NO_DEPRECATED)

    if (key == NULL) {
        key = RSA_generate_key(512, RSA_F4, NULL, NULL);
    }

#endif

    return key;
}


ngx_array_t *
ngx_ssl_read_password_file(ngx_conf_t *cf, ngx_str_t *file)
{
    u_char              *p, *last, *end;
    size_t               len;
    ssize_t              n;
    ngx_fd_t             fd;
    ngx_str_t           *pwd;
    ngx_array_t         *passwords;
    ngx_pool_cleanup_t  *cln;
    u_char               buf[NGX_SSL_PASSWORD_BUFFER_SIZE];

    if (ngx_conf_full_name(cf->cycle, file, 1) != NGX_OK) {
        return NULL;
    }

    cln = ngx_pool_cleanup_add(cf->temp_pool, 0);
    passwords = ngx_array_create(cf->temp_pool, 4, sizeof(ngx_str_t));

    if (cln == NULL || passwords == NULL) {
        return NULL;
    }

    cln->handler = ngx_ssl_passwords_cleanup;
    cln->data = passwords;

    fd = ngx_open_file(file->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);
    if (fd == NGX_INVALID_FILE) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_file_n " \"%s\" failed", file->data);
        return NULL;
    }

    len = 0;
    last = buf;

    do {
        n = ngx_read_fd(fd, last, NGX_SSL_PASSWORD_BUFFER_SIZE - len);

        if (n == -1) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_read_fd_n " \"%s\" failed", file->data);
            passwords = NULL;
            goto cleanup;
        }

        end = last + n;

        if (len && n == 0) {
            *end++ = LF;
        }

        p = buf;

        for ( ;; ) {
            last = ngx_strlchr(last, end, LF);

            if (last == NULL) {
                break;
            }

            len = last++ - p;

            if (len && p[len - 1] == CR) {
                len--;
            }

            if (len) {
                pwd = ngx_array_push(passwords);
                if (pwd == NULL) {
                    passwords = NULL;
                    goto cleanup;
                }

                pwd->len = len;
                pwd->data = ngx_pnalloc(cf->temp_pool, len);

                if (pwd->data == NULL) {
                    passwords->nelts--;
                    passwords = NULL;
                    goto cleanup;
                }

                ngx_memcpy(pwd->data, p, len);
            }

            p = last;
        }

        len = end - p;

        if (len == NGX_SSL_PASSWORD_BUFFER_SIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too long line in \"%s\"", file->data);
            passwords = NULL;
            goto cleanup;
        }

        ngx_memmove(buf, p, len);
        last = buf + len;

    } while (n != 0);

    if (passwords->nelts == 0) {
        pwd = ngx_array_push(passwords);
        if (pwd == NULL) {
            passwords = NULL;
            goto cleanup;
        }

        ngx_memzero(pwd, sizeof(ngx_str_t));
    }

cleanup:

    if (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_conf_log_error(NGX_LOG_ALERT, cf, ngx_errno,
                           ngx_close_file_n " \"%s\" failed", file->data);
    }

    ngx_memzero(buf, NGX_SSL_PASSWORD_BUFFER_SIZE);

    return passwords;
}


static void
ngx_ssl_passwords_cleanup(void *data)
{
    ngx_array_t *passwords = data;

    ngx_str_t   *pwd;
    ngx_uint_t   i;

    pwd = passwords->elts;

    for (i = 0; i < passwords->nelts; i++) {
        ngx_memzero(pwd[i].data, pwd[i].len);
    }
}


ngx_int_t
ngx_ssl_dhparam(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *file)
{
    DH   *dh;
    BIO  *bio;

    if (file->len == 0) {
        return NGX_OK;
    }

    if (ngx_conf_full_name(cf->cycle, file, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    bio = BIO_new_file((char *) file->data, "r");
    if (bio == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "BIO_new_file(\"%s\") failed", file->data);
        return NGX_ERROR;
    }

    dh = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
    if (dh == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "PEM_read_bio_DHparams(\"%s\") failed", file->data);
        BIO_free(bio);
        return NGX_ERROR;
    }

    SSL_CTX_set_tmp_dh(ssl->ctx, dh);

    DH_free(dh);
    BIO_free(bio);

    return NGX_OK;
}


ngx_int_t
ngx_ssl_ecdh_curve(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *name)
{
#if OPENSSL_VERSION_NUMBER >= 0x0090800fL
#ifndef OPENSSL_NO_ECDH

    /*
     * Elliptic-Curve Diffie-Hellman parameters are either "named curves"
     * from RFC 4492 section 5.1.1, or explicitly described curves over
     * binary fields.  OpenSSL only supports the "named curves", which provide
     * maximum interoperability.
     */

#if (defined SSL_CTX_set1_curves_list || defined SSL_CTRL_SET_CURVES_LIST)

    /*
     * OpenSSL 1.0.2+ allows configuring a curve list instead of a single
     * curve previously supported.  By default an internal list is used,
     * with prime256v1 being preferred by server in OpenSSL 1.0.2b+
     * and X25519 in OpenSSL 1.1.0+.
     *
     * By default a curve preferred by the client will be used for
     * key exchange.  The SSL_OP_CIPHER_SERVER_PREFERENCE option can
     * be used to prefer server curves instead, similar to what it
     * does for ciphers.
     */

    SSL_CTX_set_options(ssl->ctx, SSL_OP_SINGLE_ECDH_USE);

#if SSL_CTRL_SET_ECDH_AUTO
    /* not needed in OpenSSL 1.1.0+ */
    SSL_CTX_set_ecdh_auto(ssl->ctx, 1);
#endif

    if (ngx_strcmp(name->data, "auto") == 0) {
        return NGX_OK;
    }

    if (SSL_CTX_set1_curves_list(ssl->ctx, (char *) name->data) == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_set1_curves_list(\"%s\") failed", name->data);
        return NGX_ERROR;
    }

#else

    int      nid;
    char    *curve;
    EC_KEY  *ecdh;

    if (ngx_strcmp(name->data, "auto") == 0) {
        curve = "prime256v1";

    } else {
        curve = (char *) name->data;
    }

    nid = OBJ_sn2nid(curve);
    if (nid == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "OBJ_sn2nid(\"%s\") failed: unknown curve", curve);
        return NGX_ERROR;
    }

    ecdh = EC_KEY_new_by_curve_name(nid);
    if (ecdh == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "EC_KEY_new_by_curve_name(\"%s\") failed", curve);
        return NGX_ERROR;
    }

    SSL_CTX_set_options(ssl->ctx, SSL_OP_SINGLE_ECDH_USE);

    SSL_CTX_set_tmp_ecdh(ssl->ctx, ecdh);

    EC_KEY_free(ecdh);
#endif
#endif
#endif

    return NGX_OK;
}


ngx_int_t
ngx_ssl_create_connection(ngx_ssl_t *ssl, ngx_connection_t *c, ngx_uint_t flags)
{
    ngx_ssl_connection_t  *sc;

    sc = ngx_pcalloc(c->pool, sizeof(ngx_ssl_connection_t));
    if (sc == NULL) {
        return NGX_ERROR;
    }

    sc->buffer = ((flags & NGX_SSL_BUFFER) != 0);
    sc->buffer_size = ssl->buffer_size;

    sc->session_ctx = ssl->ctx;

    sc->connection = SSL_new(ssl->ctx);

    if (sc->connection == NULL) {
        ngx_ssl_error(NGX_LOG_ALERT, c->log, 0, "SSL_new() failed");
        return NGX_ERROR;
    }

    if (SSL_set_fd(sc->connection, c->fd) == 0) {
        ngx_ssl_error(NGX_LOG_ALERT, c->log, 0, "SSL_set_fd() failed");
        return NGX_ERROR;
    }

    if (flags & NGX_SSL_CLIENT) {
        SSL_set_connect_state(sc->connection);

    } else {
        SSL_set_accept_state(sc->connection);
    }

    if (SSL_set_ex_data(sc->connection, ngx_ssl_connection_index, c) == 0) {
        ngx_ssl_error(NGX_LOG_ALERT, c->log, 0, "SSL_set_ex_data() failed");
        return NGX_ERROR;
    }

    c->ssl = sc;

    return NGX_OK;
}


ngx_int_t
ngx_ssl_set_session(ngx_connection_t *c, ngx_ssl_session_t *session)
{
    if (session) {
        if (SSL_set_session(c->ssl->connection, session) == 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0, "SSL_set_session() failed");
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}


ngx_int_t
ngx_ssl_handshake(ngx_connection_t *c)
{
    int        n, sslerr;
    ngx_err_t  err;

    ngx_ssl_clear_error(c->log);

    n = SSL_do_handshake(c->ssl->connection);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL_do_handshake: %d", n);

    if (n == 1) {

        if (ngx_handle_read_event(c->read, 0) != NGX_OK) {
            return NGX_ERROR;
        }

        if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
            return NGX_ERROR;
        }

#if (NGX_DEBUG)
        {
        char         buf[129], *s, *d;
#if OPENSSL_VERSION_NUMBER >= 0x10000000L
        const
#endif
        SSL_CIPHER  *cipher;

        cipher = SSL_get_current_cipher(c->ssl->connection);

        if (cipher) {
            SSL_CIPHER_description(cipher, &buf[1], 128);

            for (s = &buf[1], d = buf; *s; s++) {
                if (*s == ' ' && *d == ' ') {
                    continue;
                }

                if (*s == LF || *s == CR) {
                    continue;
                }

                *++d = *s;
            }

            if (*d != ' ') {
                d++;
            }

            *d = '\0';

            ngx_log_debug2(NGX_LOG_DEBUG_EVENT, c->log, 0,
                           "SSL: %s, cipher: \"%s\"",
                           SSL_get_version(c->ssl->connection), &buf[1]);

            if (SSL_session_reused(c->ssl->connection)) {
                ngx_log_debug0(NGX_LOG_DEBUG_EVENT, c->log, 0,
                               "SSL reused session");
            }

        } else {
            ngx_log_debug0(NGX_LOG_DEBUG_EVENT, c->log, 0,
                           "SSL no shared ciphers");
        }
        }
#endif

        c->ssl->handshaked = 1;

        c->recv = ngx_ssl_recv;
        c->send = ngx_ssl_write;
        c->recv_chain = ngx_ssl_recv_chain;
        c->send_chain = ngx_ssl_send_chain;

#if OPENSSL_VERSION_NUMBER < 0x10100000L
#ifdef SSL3_FLAGS_NO_RENEGOTIATE_CIPHERS

        /* initial handshake done, disable renegotiation (CVE-2009-3555) */
        if (c->ssl->connection->s3 && SSL_is_server(c->ssl->connection)) {
            c->ssl->connection->s3->flags |= SSL3_FLAGS_NO_RENEGOTIATE_CIPHERS;
        }

#endif
#endif

        return NGX_OK;
    }

    sslerr = SSL_get_error(c->ssl->connection, n);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL_get_error: %d", sslerr);

    if (sslerr == SSL_ERROR_WANT_READ) {
        c->read->ready = 0;
        c->read->handler = ngx_ssl_handshake_handler;
        c->write->handler = ngx_ssl_handshake_handler;

        if (ngx_handle_read_event(c->read, 0) != NGX_OK) {
            return NGX_ERROR;
        }

        if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
            return NGX_ERROR;
        }

        return NGX_AGAIN;
    }

    if (sslerr == SSL_ERROR_WANT_WRITE) {
        c->write->ready = 0;
        c->read->handler = ngx_ssl_handshake_handler;
        c->write->handler = ngx_ssl_handshake_handler;

        if (ngx_handle_read_event(c->read, 0) != NGX_OK) {
            return NGX_ERROR;
        }

        if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
            return NGX_ERROR;
        }

        return NGX_AGAIN;
    }

    err = (sslerr == SSL_ERROR_SYSCALL) ? ngx_errno : 0;

    c->ssl->no_wait_shutdown = 1;
    c->ssl->no_send_shutdown = 1;
    c->read->eof = 1;

    if (sslerr == SSL_ERROR_ZERO_RETURN || ERR_peek_error() == 0) {
        ngx_connection_error(c, err,
                             "peer closed connection in SSL handshake");

        return NGX_ERROR;
    }

    c->read->error = 1;

    ngx_ssl_connection_error(c, sslerr, err, "SSL_do_handshake() failed");

    return NGX_ERROR;
}


static void
ngx_ssl_handshake_handler(ngx_event_t *ev)
{
    ngx_connection_t  *c;

    c = ev->data;

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0,
                   "SSL handshake handler: %d", ev->write);

    if (ev->timedout) {
        c->ssl->handler(c);
        return;
    }

    if (ngx_ssl_handshake(c) == NGX_AGAIN) {
        return;
    }

    c->ssl->handler(c);
}


ssize_t
ngx_ssl_recv_chain(ngx_connection_t *c, ngx_chain_t *cl, off_t limit)
{
    u_char     *last;
    ssize_t     n, bytes, size;
    ngx_buf_t  *b;

    bytes = 0;

    b = cl->buf;
    last = b->last;

    for ( ;; ) {
        size = b->end - last;

        if (limit) {
            if (bytes >= limit) {
                return bytes;
            }

            if (bytes + size > limit) {
                size = (ssize_t) (limit - bytes);
            }
        }

        n = ngx_ssl_recv(c, last, size);

        if (n > 0) {
            last += n;
            bytes += n;

            if (last == b->end) {
                cl = cl->next;

                if (cl == NULL) {
                    return bytes;
                }

                b = cl->buf;
                last = b->last;
            }

            continue;
        }

        if (bytes) {

            if (n == 0 || n == NGX_ERROR) {
                c->read->ready = 1;
            }

            return bytes;
        }

        return n;
    }
}


ssize_t
ngx_ssl_recv(ngx_connection_t *c, u_char *buf, size_t size)
{
    int  n, bytes;

    if (c->ssl->last == NGX_ERROR) {
        c->read->error = 1;
        return NGX_ERROR;
    }

    if (c->ssl->last == NGX_DONE) {
        c->read->ready = 0;
        c->read->eof = 1;
        return 0;
    }

    bytes = 0;

    ngx_ssl_clear_error(c->log);

    /*
     * SSL_read() may return data in parts, so try to read
     * until SSL_read() would return no data
     */

    for ( ;; ) {

        n = SSL_read(c->ssl->connection, buf, size);

        ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL_read: %d", n);

        if (n > 0) {
            bytes += n;
        }

        c->ssl->last = ngx_ssl_handle_recv(c, n);

        if (c->ssl->last == NGX_OK) {

            size -= n;

            if (size == 0) {
                c->read->ready = 1;
                return bytes;
            }

            buf += n;

            continue;
        }

        if (bytes) {
            if (c->ssl->last != NGX_AGAIN) {
                c->read->ready = 1;
            }

            return bytes;
        }

        switch (c->ssl->last) {

        case NGX_DONE:
            c->read->ready = 0;
            c->read->eof = 1;
            return 0;

        case NGX_ERROR:
            c->read->error = 1;

            /* fall through */

        case NGX_AGAIN:
            return c->ssl->last;
        }
    }
}


static ngx_int_t
ngx_ssl_handle_recv(ngx_connection_t *c, int n)
{
    int        sslerr;
    ngx_err_t  err;

    if (c->ssl->renegotiation) {
        /*
         * disable renegotiation (CVE-2009-3555):
         * OpenSSL (at least up to 0.9.8l) does not handle disabled
         * renegotiation gracefully, so drop connection here
         */

        ngx_log_error(NGX_LOG_NOTICE, c->log, 0, "SSL renegotiation disabled");

        while (ERR_peek_error()) {
            ngx_ssl_error(NGX_LOG_DEBUG, c->log, 0,
                          "ignoring stale global SSL error");
        }

        ERR_clear_error();

        c->ssl->no_wait_shutdown = 1;
        c->ssl->no_send_shutdown = 1;

        return NGX_ERROR;
    }

    if (n > 0) {

        if (c->ssl->saved_write_handler) {

            c->write->handler = c->ssl->saved_write_handler;
            c->ssl->saved_write_handler = NULL;
            c->write->ready = 1;

            if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
                return NGX_ERROR;
            }

            ngx_post_event(c->write, &ngx_posted_events);
        }

        return NGX_OK;
    }

    sslerr = SSL_get_error(c->ssl->connection, n);

    err = (sslerr == SSL_ERROR_SYSCALL) ? ngx_errno : 0;

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL_get_error: %d", sslerr);

    if (sslerr == SSL_ERROR_WANT_READ) {
        c->read->ready = 0;
        return NGX_AGAIN;
    }

    if (sslerr == SSL_ERROR_WANT_WRITE) {

        ngx_log_error(NGX_LOG_INFO, c->log, 0,
                      "peer started SSL renegotiation");

        c->write->ready = 0;

        if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
            return NGX_ERROR;
        }

        /*
         * we do not set the timer because there is already the read event timer
         */

        if (c->ssl->saved_write_handler == NULL) {
            c->ssl->saved_write_handler = c->write->handler;
            c->write->handler = ngx_ssl_write_handler;
        }

        return NGX_AGAIN;
    }

    c->ssl->no_wait_shutdown = 1;
    c->ssl->no_send_shutdown = 1;

    if (sslerr == SSL_ERROR_ZERO_RETURN || ERR_peek_error() == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, c->log, 0,
                       "peer shutdown SSL cleanly");
        return NGX_DONE;
    }

    ngx_ssl_connection_error(c, sslerr, err, "SSL_read() failed");

    return NGX_ERROR;
}


static void
ngx_ssl_write_handler(ngx_event_t *wev)
{
    ngx_connection_t  *c;

    c = wev->data;

    c->read->handler(c->read);
}


/*
 * OpenSSL has no SSL_writev() so we copy several bufs into our 16K buffer
 * before the SSL_write() call to decrease a SSL overhead.
 *
 * Besides for protocols such as HTTP it is possible to always buffer
 * the output to decrease a SSL overhead some more.
 */

ngx_chain_t *
ngx_ssl_send_chain(ngx_connection_t *c, ngx_chain_t *in, off_t limit)
{
    int          n;
    ngx_uint_t   flush;
    ssize_t      send, size;
    ngx_buf_t   *buf;

    if (!c->ssl->buffer) {

        while (in) {
            if (ngx_buf_special(in->buf)) {
                in = in->next;
                continue;
            }

            n = ngx_ssl_write(c, in->buf->pos, in->buf->last - in->buf->pos);

            if (n == NGX_ERROR) {
                return NGX_CHAIN_ERROR;
            }

            if (n == NGX_AGAIN) {
                return in;
            }

            in->buf->pos += n;

            if (in->buf->pos == in->buf->last) {
                in = in->next;
            }
        }

        return in;
    }


    /* the maximum limit size is the maximum int32_t value - the page size */

    if (limit == 0 || limit > (off_t) (NGX_MAX_INT32_VALUE - ngx_pagesize)) {
        limit = NGX_MAX_INT32_VALUE - ngx_pagesize;
    }

    buf = c->ssl->buf;

    if (buf == NULL) {
        buf = ngx_create_temp_buf(c->pool, c->ssl->buffer_size);
        if (buf == NULL) {
            return NGX_CHAIN_ERROR;
        }

        c->ssl->buf = buf;
    }

    if (buf->start == NULL) {
        buf->start = ngx_palloc(c->pool, c->ssl->buffer_size);
        if (buf->start == NULL) {
            return NGX_CHAIN_ERROR;
        }

        buf->pos = buf->start;
        buf->last = buf->start;
        buf->end = buf->start + c->ssl->buffer_size;
    }

    send = buf->last - buf->pos;
    flush = (in == NULL) ? 1 : buf->flush;

    for ( ;; ) {

        while (in && buf->last < buf->end && send < limit) {
            if (in->buf->last_buf || in->buf->flush) {
                flush = 1;
            }

            if (ngx_buf_special(in->buf)) {
                in = in->next;
                continue;
            }

            size = in->buf->last - in->buf->pos;

            if (size > buf->end - buf->last) {
                size = buf->end - buf->last;
            }

            if (send + size > limit) {
                size = (ssize_t) (limit - send);
            }

            ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0,
                           "SSL buf copy: %z", size);

            ngx_memcpy(buf->last, in->buf->pos, size);

            buf->last += size;
            in->buf->pos += size;
            send += size;

            if (in->buf->pos == in->buf->last) {
                in = in->next;
            }
        }

        if (!flush && send < limit && buf->last < buf->end) {
            break;
        }

        size = buf->last - buf->pos;

        if (size == 0) {
            buf->flush = 0;
            c->buffered &= ~NGX_SSL_BUFFERED;
            return in;
        }

        n = ngx_ssl_write(c, buf->pos, size);

        if (n == NGX_ERROR) {
            return NGX_CHAIN_ERROR;
        }

        if (n == NGX_AGAIN) {
            break;
        }

        buf->pos += n;

        if (n < size) {
            break;
        }

        flush = 0;

        buf->pos = buf->start;
        buf->last = buf->start;

        if (in == NULL || send == limit) {
            break;
        }
    }

    buf->flush = flush;

    if (buf->pos < buf->last) {
        c->buffered |= NGX_SSL_BUFFERED;

    } else {
        c->buffered &= ~NGX_SSL_BUFFERED;
    }

    return in;
}


ssize_t
ngx_ssl_write(ngx_connection_t *c, u_char *data, size_t size)
{
    int        n, sslerr;
    ngx_err_t  err;

    ngx_ssl_clear_error(c->log);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL to write: %uz", size);

    n = SSL_write(c->ssl->connection, data, size);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL_write: %d", n);

    if (n > 0) {

        if (c->ssl->saved_read_handler) {

            c->read->handler = c->ssl->saved_read_handler;
            c->ssl->saved_read_handler = NULL;
            c->read->ready = 1;

            if (ngx_handle_read_event(c->read, 0) != NGX_OK) {
                return NGX_ERROR;
            }

            ngx_post_event(c->read, &ngx_posted_events);
        }

        c->sent += n;

        return n;
    }

    sslerr = SSL_get_error(c->ssl->connection, n);

    err = (sslerr == SSL_ERROR_SYSCALL) ? ngx_errno : 0;

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL_get_error: %d", sslerr);

    if (sslerr == SSL_ERROR_WANT_WRITE) {
        c->write->ready = 0;
        return NGX_AGAIN;
    }

    if (sslerr == SSL_ERROR_WANT_READ) {

        ngx_log_error(NGX_LOG_INFO, c->log, 0,
                      "peer started SSL renegotiation");

        c->read->ready = 0;

        if (ngx_handle_read_event(c->read, 0) != NGX_OK) {
            return NGX_ERROR;
        }

        /*
         * we do not set the timer because there is already
         * the write event timer
         */

        if (c->ssl->saved_read_handler == NULL) {
            c->ssl->saved_read_handler = c->read->handler;
            c->read->handler = ngx_ssl_read_handler;
        }

        return NGX_AGAIN;
    }

    c->ssl->no_wait_shutdown = 1;
    c->ssl->no_send_shutdown = 1;
    c->write->error = 1;

    ngx_ssl_connection_error(c, sslerr, err, "SSL_write() failed");

    return NGX_ERROR;
}


static void
ngx_ssl_read_handler(ngx_event_t *rev)
{
    ngx_connection_t  *c;

    c = rev->data;

    c->write->handler(c->write);
}


void
ngx_ssl_free_buffer(ngx_connection_t *c)
{
    if (c->ssl->buf && c->ssl->buf->start) {
        if (ngx_pfree(c->pool, c->ssl->buf->start) == NGX_OK) {
            c->ssl->buf->start = NULL;
        }
    }
}


ngx_int_t
ngx_ssl_shutdown(ngx_connection_t *c)
{
    int        n, sslerr, mode;
    ngx_err_t  err;

    if (SSL_in_init(c->ssl->connection)) {
        /*
         * OpenSSL 1.0.2f complains if SSL_shutdown() is called during
         * an SSL handshake, while previous versions always return 0.
         * Avoid calling SSL_shutdown() if handshake wasn't completed.
         */

        SSL_free(c->ssl->connection);
        c->ssl = NULL;

        return NGX_OK;
    }

    if (c->timedout) {
        mode = SSL_RECEIVED_SHUTDOWN|SSL_SENT_SHUTDOWN;
        SSL_set_quiet_shutdown(c->ssl->connection, 1);

    } else {
        mode = SSL_get_shutdown(c->ssl->connection);

        if (c->ssl->no_wait_shutdown) {
            mode |= SSL_RECEIVED_SHUTDOWN;
        }

        if (c->ssl->no_send_shutdown) {
            mode |= SSL_SENT_SHUTDOWN;
        }

        if (c->ssl->no_wait_shutdown && c->ssl->no_send_shutdown) {
            SSL_set_quiet_shutdown(c->ssl->connection, 1);
        }
    }

    SSL_set_shutdown(c->ssl->connection, mode);

    ngx_ssl_clear_error(c->log);

    n = SSL_shutdown(c->ssl->connection);

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0, "SSL_shutdown: %d", n);

    sslerr = 0;

    /* before 0.9.8m SSL_shutdown() returned 0 instead of -1 on errors */

    if (n != 1 && ERR_peek_error()) {
        sslerr = SSL_get_error(c->ssl->connection, n);

        ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0,
                       "SSL_get_error: %d", sslerr);
    }

    if (n == 1 || sslerr == 0 || sslerr == SSL_ERROR_ZERO_RETURN) {
        SSL_free(c->ssl->connection);
        c->ssl = NULL;

        return NGX_OK;
    }

    if (sslerr == SSL_ERROR_WANT_READ || sslerr == SSL_ERROR_WANT_WRITE) {
        c->read->handler = ngx_ssl_shutdown_handler;
        c->write->handler = ngx_ssl_shutdown_handler;

        if (ngx_handle_read_event(c->read, 0) != NGX_OK) {
            return NGX_ERROR;
        }

        if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
            return NGX_ERROR;
        }

        if (sslerr == SSL_ERROR_WANT_READ) {
            ngx_add_timer(c->read, 30000);
        }

        return NGX_AGAIN;
    }

    err = (sslerr == SSL_ERROR_SYSCALL) ? ngx_errno : 0;

    ngx_ssl_connection_error(c, sslerr, err, "SSL_shutdown() failed");

    SSL_free(c->ssl->connection);
    c->ssl = NULL;

    return NGX_ERROR;
}


static void
ngx_ssl_shutdown_handler(ngx_event_t *ev)
{
    ngx_connection_t           *c;
    ngx_connection_handler_pt   handler;

    c = ev->data;
    handler = c->ssl->handler;

    if (ev->timedout) {
        c->timedout = 1;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, ev->log, 0, "SSL shutdown handler");

    if (ngx_ssl_shutdown(c) == NGX_AGAIN) {
        return;
    }

    handler(c);
}


static void
ngx_ssl_connection_error(ngx_connection_t *c, int sslerr, ngx_err_t err,
    char *text)
{
    int         n;
    ngx_uint_t  level;

    level = NGX_LOG_CRIT;

    if (sslerr == SSL_ERROR_SYSCALL) {

        if (err == NGX_ECONNRESET
            || err == NGX_EPIPE
            || err == NGX_ENOTCONN
            || err == NGX_ETIMEDOUT
            || err == NGX_ECONNREFUSED
            || err == NGX_ENETDOWN
            || err == NGX_ENETUNREACH
            || err == NGX_EHOSTDOWN
            || err == NGX_EHOSTUNREACH)
        {
            switch (c->log_error) {

            case NGX_ERROR_IGNORE_ECONNRESET:
            case NGX_ERROR_INFO:
                level = NGX_LOG_INFO;
                break;

            case NGX_ERROR_ERR:
                level = NGX_LOG_ERR;
                break;

            default:
                break;
            }
        }

    } else if (sslerr == SSL_ERROR_SSL) {

        n = ERR_GET_REASON(ERR_peek_error());

            /* handshake failures */
        if (n == SSL_R_BAD_CHANGE_CIPHER_SPEC                        /*  103 */
            || n == SSL_R_BLOCK_CIPHER_PAD_IS_WRONG                  /*  129 */
            || n == SSL_R_DIGEST_CHECK_FAILED                        /*  149 */
            || n == SSL_R_ERROR_IN_RECEIVED_CIPHER_LIST              /*  151 */
          