
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      NULL },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

      ngx_null_command
};


static ngx_http_module_t  ngx_http_ssl_module_ctx = {
    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif


static ngx_int_t
ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_ssl_variable_handler_pt  handler = (ngx_ssl_variable_handler_pt) data;

    size_t     len;
    ngx_str_t  s;

    if (r->connection->ssl) {

        (void) handler(r->connection, NULL, &s);

        v->data = s.data;

        for (len = 0; v->data[len]; len++) { /* void */ }

        v->len = len;
        v->valid = 1;
        v->no_cacheable = 0;
        v->not_found = 0;

        return NGX_OK;
    }

    v->not_found = 1;

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssl_variable(ngx_http_request_t *r, ngx_http_variable_value_t *v,
    uintptr_t data)
{
    ngx_ssl_variable_handler_pt  handler = (ngx_ssl_variable_handler_pt) data;

    ngx_str_t  s;

    if (r->connection->ssl) {

        if (handler(r->connection, r->pool, &s) != NGX_OK) {
            return NGX_ERROR;
        }

        v->len = s.len;
        v->data = s.data;

        if (v->len) {
            v->valid = 1;
            v->no_cacheable = 0;
            v->not_found = 0;

            return NGX_OK;
        }
    }

    v->not_found = 1;

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssl_add_variables(ngx_conf_t *cf)
{
    ngx_http_variable_t  *var, *v;

    for (v = ngx_http_ssl_vars; v->name.len; v++) {
        var = ngx_http_add_variable(cf, &v->name, v->flags);
        if (var == NULL) {
            return NGX_ERROR;
        }

        var->get_handler = v->get_handler;
        var->data = v->data;
    }

    return NGX_OK;
}


static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


static char *
ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssl_srv_conf_t *prev = parent;
    ngx_http_ssl_srv_conf_t *conf = child;

    ngx_pool_cleanup_t  *cln;

    if (conf->enable == NGX_CONF_UNSET) {
        if (prev->enable == NGX_CONF_UNSET) {
            conf->enable = 0;

        } else {
            conf->enable = prev->enable;
            conf->file = prev->file;
            conf->line = prev->line;
        }
    }

    ngx_conf_merge_value(conf->session_timeout,
                         prev->session_timeout, 300);

    ngx_conf_merge_value(conf->prefer_server_ciphers,
                         prev->prefer_server_ciphers, 0);

    ngx_conf_merge_bitmask_value(conf->protocols, prev->protocols,
                         (NGX_CONF_BITMASK_SET|NGX_SSL_TLSv1
                          |NGX_SSL_TLSv1_1|NGX_SSL_TLSv1_2));

    ngx_conf_merge_size_value(conf->buffer_size, prev->buffer_size,
                         NGX_SSL_BUFSIZE);

    ngx_conf_merge_uint_value(conf->verify, prev->verify, 0);
    ngx_conf_merge_uint_value(conf->verify_depth, prev->verify_depth, 1);

    ngx_conf_merge_ptr_value(conf->certificates, prev->certificates, NULL);
    ngx_conf_merge_ptr_value(conf->certificate_keys, prev->certificate_keys,
                         NULL);

    ngx_conf_merge_ptr_value(conf->passwords, prev->passwords, NULL);

    ngx_conf_merge_str_value(conf->dhparam, prev->dhparam, "");

    ngx_conf_merge_str_value(conf->client_certificate, prev->client_certificate,
                         "");
    ngx_conf_merge_str_value(conf->trusted_certificate,
                         prev->trusted_certificate, "");
    ngx_conf_merge_str_value(conf->crl, prev->crl, "");

    ngx_conf_merge_str_value(conf->ecdh_curve, prev->ecdh_curve,
                         NGX_DEFAULT_ECDH_CURVE);

    ngx_conf_merge_str_value(conf->ciphers, prev->ciphers, NGX_DEFAULT_CIPHERS);

    ngx_conf_merge_value(conf->stapling, prev->stapling, 0);
    ngx_conf_merge_value(conf->stapling_verify, prev->stapling_verify, 0);
    ngx_conf_merge_str_value(conf->stapling_file, prev->stapling_file, "");
    ngx_conf_merge_str_value(conf->stapling_responder,
                         prev->stapling_responder, "");

    conf->ssl.log = cf->log;

    if (conf->enable) {

        if (conf->certificates == NULL) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                          "no \"ssl_certificate\" is defined for "
                          "the \"ssl\" directive in %s:%ui",
                          conf->file, conf->line);
            return NGX_CONF_ERROR;
        }

        if (conf->certificate_keys == NULL) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                          "no \"ssl_certificate_key\" is defined for "
                          "the \"ssl\" directive in %s:%ui",
                          conf->file, conf->line);
            return NGX_CONF_ERROR;
        }

        if (conf->certificate_keys->nelts < conf->certificates->nelts) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                          "no \"ssl_certificate_key\" is defined "
                          "for certificate \"%V\" and "
                          "the \"ssl\" directive in %s:%ui",
                          ((ngx_str_t *) conf->certificates->elts)
                          + conf->certificates->nelts - 1,
                          conf->file, conf->line);
            return NGX_CONF_ERROR;
        }

    } else {

        if (conf->certificates == NULL) {
            return NGX_CONF_OK;
        }

        if (conf->certificate_keys == NULL
            || conf->certificate_keys->nelts < conf->certificates->nelts)
        {
            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                          "no \"ssl_certificate_key\" is defined "
                          "for certificate \"%V\"",
                          ((ngx_str_t *) conf->certificates->elts)
                          + conf->certificates->nelts - 1);
            return NGX_CONF_ERROR;
        }
    }

    if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME

    if (SSL_CTX_set_tlsext_servername_callback(conf->ssl.ctx,
                                               ngx_http_ssl_servername)
        == 0)
    {
        ngx_log_error(NGX_LOG_WARN, cf->log, 0,
            "nginx was built with SNI support, however, now it is linked "
            "dynamically to an OpenSSL library which has no tlsext support, "
            "therefore SNI is not available");
    }

#endif

#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
    SSL_CTX_set_alpn_select_cb(conf->ssl.ctx, ngx_http_ssl_alpn_select, NULL);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
    SSL_CTX_set_next_protos_advertised_cb(conf->ssl.ctx,
                                          ngx_http_ssl_npn_advertised, NULL);
#endif

    cln = ngx_pool_cleanup_add(cf->pool, 0);
    if (cln == NULL) {
        return NGX_CONF_ERROR;
    }

    cln->handler = ngx_ssl_cleanup_ctx;
    cln->data = &conf->ssl;

    if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates,
                             conf->certificate_keys, conf->passwords)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers,
                        conf->prefer_server_ciphers)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    conf->ssl.buffer_size = conf->buffer_size;

    if (conf->verify) {

        if (conf->client_certificate.len == 0 && conf->verify != 3) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                          "no ssl_client_certificate for ssl_client_verify");
            return NGX_CONF_ERROR;
        }

        if (ngx_ssl_client_certificate(cf, &conf->ssl,
                                       &conf->client_certificate,
                                       conf->verify_depth)
            != NGX_OK)
        {
            return NGX_CONF_ERROR;
        }
    }

    if (ngx_ssl_trusted_certificate(cf, &conf->ssl,
                                    &conf->trusted_certificate,
                                    conf->verify_depth)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    ngx_conf_merge_value(conf->builtin_session_cache,
                         prev->builtin_session_cache, NGX_SSL_NONE_SCACHE);

    if (conf->shm_zone == NULL) {
        conf->shm_zone = prev->shm_zone;
    }

    if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx,
                              conf->builtin_session_cache,
                              conf->shm_zone, conf->session_timeout)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    ngx_conf_merge_value(conf->session_tickets, prev->session_tickets, 1);

#ifdef SSL_OP_NO_TICKET
    if (!conf->session_tickets) {
        SSL_CTX_set_options(conf->ssl.ctx, SSL_OP_NO_TICKET);
    }
#endif

    ngx_conf_merge_ptr_value(conf->session_ticket_keys,
                         prev->session_ticket_keys, NULL);

    if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    if (conf->stapling) {

        if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file,
                             &conf->stapling_responder, conf->stapling_verify)
            != NGX_OK)
        {
            return NGX_CONF_ERROR;
        }

    }

    return NGX_CONF_OK;
}


static char *
ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_ssl_srv_conf_t *sscf = conf;

    char  *rv;

    rv = ngx_conf_set_flag_slot(cf, cmd, conf);

    if (rv != NGX_CONF_OK) {
        return rv;
    }

    sscf->file = cf->conf_file->file.name.data;
    sscf->line = cf->conf_file->line;

    return NGX_CONF_OK;
}


static char *
ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_ssl_srv_conf_t *sscf = conf;

    ngx_str_t  *value;

    if (sscf->passwords != NGX_CONF_UNSET_PTR) {
        return "is duplicate";
    }

    value = cf->args->elts;

    sscf->passwords = ngx_ssl_read_password_file(cf, &value[1]);

    if (sscf->passwords == NULL) {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static char *
ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_ssl_srv_conf_t *sscf = conf;

    size_t       len;
    ngx_str_t   *value, name, size;
    ngx_int_t    n;
    ngx_uint_t   i, j;

    value = cf->args->elts;

    for (i = 1; i < cf->args->nelts; i++) {

        if (ngx_strcmp(value[i].data, "off") == 0) {
            sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
            continue;
        }

        if (ngx_strcmp(value[i].data, "none") == 0) {
            sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
            continue;
        }

        if (ngx_strcmp(value[i].data, "builtin") == 0) {
            sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
            continue;
        }

        if (value[i].len > sizeof("builtin:") - 1
            && ngx_strncmp(value[i].data, "builtin:", sizeof("builtin:") - 1)
               == 0)
        {
            n = ngx_atoi(value[i].data + sizeof("builtin:") - 1,
                         value[i].len - (sizeof("builtin:") - 1));

            if (n == NGX_ERROR) {
                goto invalid;
            }

            sscf->builtin_session_cache = n;

            continue;
        }

        if (value[i].len > sizeof("shared:") - 1
            && ngx_strncmp(value[i].data, "shared:", sizeof("shared:") - 1)
               == 0)
        {
            len = 0;

            for (j = sizeof("shared:") - 1; j < value[i].len; j++) {
                if (value[i].data[j] == ':') {
                    break;
                }

                len++;
            }

            if (len == 0) {
                goto invalid;
            }

            name.len = len;
            name.data = value[i].data + sizeof("shared:") - 1;

            size.len = value[i].len - j - 1;
            size.data = name.data + len + 1;

            n = ngx_parse_size(&size);

            if (n == NGX_ERROR) {
                goto invalid;
            }

            if (n < (ngx_int_t) (8 * ngx_pagesize)) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "session cache \"%V\" is too small",
                                   &value[i]);

                return NGX_CONF_ERROR;
            }

            sscf->shm_zone = ngx_shared_memory_add(cf, &name, n,
                                                   &ngx_http_ssl_module);
            if (sscf->shm_zone == NULL) {
                return NGX_CONF_ERROR;
            }

            sscf->shm_zone->init = ngx_ssl_session_cache_init;

            continue;
        }

        goto invalid;
    }

    if (sscf->shm_zone && sscf->builtin_session_cache == NGX_CONF_UNSET) {
        sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
    }

    return NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid session cache \"%V\"", &value[i]);

    return NGX_CONF_ERROR;
}


static ngx_int_t
ngx_http_ssl_init(ngx_conf_t *cf)
{
    ngx_uint_t                   s;
    ngx_http_ssl_srv_conf_t     *sscf;
    ngx_http_core_loc_conf_t    *clcf;
    ngx_http_core_srv_conf_t   **cscfp;
    ngx_http_core_main_conf_t   *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
    cscfp = cmcf->servers.elts;

    for (s = 0; s < cmcf->servers.nelts; s++) {

        sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];

        if (sscf->ssl.ctx == NULL || !sscf->stapling) {
            continue;
        }

        clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];

        if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver,
                                      clcf->resolver_timeout)
            != NGX_OK)
        {
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}
;
}


static ngx_int_t
ngx_http_ssi_output(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx)
{
    ngx_int_t     rc;
    ngx_buf_t    *b;
    ngx_chain_t  *cl;

#if 1
    b = NULL;
    for (cl = ctx->out; cl; cl = cl->next) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "ssi out: %p %p", cl->buf, cl->buf->pos);
        if (cl->buf == b) {
            ngx_log_error(NGX_LOG_ALERT, r->connection->log, 0,
                          "the same buf was used in ssi");
            ngx_debug_point();
            return NGX_ERROR;
        }
        b = cl->buf;
    }
#endif

    rc = ngx_http_next_body_filter(r, ctx->out);

    if (ctx->busy == NULL) {
        ctx->busy = ctx->out;

    } else {
        for (cl = ctx->busy; cl->next; cl = cl->next) { /* void */ }
        cl->next = ctx->out;
    }

    ctx->out = NULL;
    ctx->last_out = &ctx->out;

    while (ctx->busy) {

        cl = ctx->busy;
        b = cl->buf;

        if (ngx_buf_size(b) != 0) {
            break;
        }

        if (b->shadow) {
            b->shadow->pos = b->shadow->last;
        }

        ctx->busy = cl->next;

        if (ngx_buf_in_memory(b) || b->in_file) {
            /* add data bufs only to the free buf chain */

            cl->next = ctx->free;
            ctx->free = cl;
        }
    }

    ngx_http_ssi_buffered(r, ctx);

    return rc;
}


static void
ngx_http_ssi_buffered(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx)
{
    if (ctx->in || ctx->buf) {
        r->buffered |= NGX_HTTP_SSI_BUFFERED;

    } else {
        r->buffered &= ~NGX_HTTP_SSI_BUFFERED;
    }
}


static ngx_int_t
ngx_http_ssi_parse(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx)
{
    u_char                *p, *value, *last, *copy_end, ch;
    size_t                 looked;
    ngx_http_ssi_state_e   state;

    state = ctx->state;
    looked = ctx->looked;
    last = ctx->buf->last;
    copy_end = ctx->copy_end;

    for (p = ctx->pos; p < last; p++) {

        ch = *p;

        if (state == ssi_start_state) {

            /* the tight loop */

            for ( ;; ) {
                if (ch == '<') {
                    copy_end = p;
                    looked = 1;
                    state = ssi_tag_state;

                    goto tag_started;
                }

                if (++p == last) {
                    break;
                }

                ch = *p;
            }

            ctx->state = state;
            ctx->pos = p;
            ctx->looked = looked;
            ctx->copy_end = p;

            if (ctx->copy_start == NULL) {
                ctx->copy_start = ctx->buf->pos;
            }

            return NGX_AGAIN;

        tag_started:

            continue;
        }

        switch (state) {

        case ssi_start_state:
            /* not reached */
            break;

        case ssi_tag_state:
            switch (ch) {
            case '!':
                looked = 2;
                state = ssi_comment0_state;
                break;

            case '<':
                copy_end = p;
                break;

            default:
                copy_end = p;
                looked = 0;
                state = ssi_start_state;
                break;
            }

            break;

        case ssi_comment0_state:
            switch (ch) {
            case '-':
                looked = 3;
                state = ssi_comment1_state;
                break;

            case '<':
                copy_end = p;
                looked = 1;
                state = ssi_tag_state;
                break;

            default:
                copy_end = p;
                looked = 0;
                state = ssi_start_state;
                break;
            }

            break;

        case ssi_comment1_state:
            switch (ch) {
            case '-':
                looked = 4;
                state = ssi_sharp_state;
                break;

            case '<':
                copy_end = p;
                looked = 1;
                state = ssi_tag_state;
                break;

            default:
                copy_end = p;
                looked = 0;
                state = ssi_start_state;
                break;
            }

            break;

        case ssi_sharp_state:
            switch (ch) {
            case '#':
                if (p - ctx->pos < 4) {
                    ctx->saved = 0;
                }
                looked = 0;
                state = ssi_precommand_state;
                break;

            case '<':
                copy_end = p;
                looked = 1;
                state = ssi_tag_state;
                break;

            default:
                copy_end = p;
                looked = 0;
                state = ssi_start_state;
                break;
            }

            break;

        case ssi_precommand_state:
            switch (ch) {
            case ' ':
            case CR:
            case LF:
            case '\t':
                break;

            default:
                ctx->command.len = 1;
                ctx->command.data = ngx_pnalloc(r->pool,
                                                NGX_HTTP_SSI_COMMAND_LEN);
                if (ctx->command.data == NULL) {
                    return NGX_ERROR;
                }

                ctx->command.data[0] = ch;

                ctx->key = 0;
                ctx->key = ngx_hash(ctx->key, ch);

                ctx->params.nelts = 0;

                state = ssi_command_state;
                break;
            }

            break;

        case ssi_command_state:
            switch (ch) {
            case ' ':
            case CR:
            case LF:
            case '\t':
                state = ssi_preparam_state;
                break;

            case '-':
                state = ssi_comment_end0_state;
                break;

            default:
                if (ctx->command.len == NGX_HTTP_SSI_COMMAND_LEN) {
                    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                                  "the \"%V%c...\" SSI command is too long",
                                  &ctx->command, ch);

                    state = ssi_error_state;
                    break;
                }

                ctx->command.data[ctx->command.len++] = ch;
                ctx->key = ngx_hash(ctx->key, ch);
            }

            break;

        case ssi_preparam_state:
            switch (ch) {
            case ' ':
            case CR:
            case LF:
            case '\t':
                break;

            case '-':
                state = ssi_comment_end0_state;
                break;

            default:
                ctx->param = ngx_array_push(&ctx->params);
                if (ctx->param == NULL) {
                    return NGX_ERROR;
                }

                ctx->param->key.len = 1;
                ctx->param->key.data = ngx_pnalloc(r->pool,
                                                   NGX_HTTP_SSI_PARAM_LEN);
                if (ctx->param->key.data == NULL) {
                    return NGX_ERROR;
                }

                ctx->param->key.data[0] = ch;

                ctx->param->value.len = 0;

                if (ctx->value_buf == NULL) {
                    ctx->param->value.data = ngx_pnalloc(r->pool,
                                                         ctx->value_len + 1);
                    if (ctx->param->value.data == NULL) {
                        return NGX_ERROR;
                    }

                } else {
                    ctx->param->value.data = ctx->value_buf;
                }

                state = ssi_param_state;
                break;
            }

            break;

        case ssi_param_state:
            switch (ch) {
            case ' ':
            case CR:
            case LF:
            case '\t':
                state = ssi_preequal_state;
                break;

            case '=':
                state = ssi_prevalue_state;
                break;

            case '-':
                state = ssi_error_end0_state;

                ctx->param->key.data[ctx->param->key.len++] = ch;
                ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                              "invalid \"%V\" parameter in \"%V\" SSI command",
                              &ctx->param->key, &ctx->command);
                break;

            default:
                if (ctx->param->key.len == NGX_HTTP_SSI_PARAM_LEN) {
                    state = ssi_error_state;
                    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                                  "too long \"%V%c...\" parameter in "
                                  "\"%V\" SSI command",
                                  &ctx->param->key, ch, &ctx->command);
                    break;
                }

                ctx->param->key.data[ctx->param->key.len++] = ch;
            }

            break;

        case ssi_preequal_state:
            switch (ch) {
            case ' ':
            case CR:
            case LF:
            case '\t':
                break;

            case '=':
                state = ssi_prevalue_state;
                break;

            default:
                if (ch == '-') {
                    state = ssi_error_end0_state;
                } else {
                    state = ssi_error_state;
                }

                ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                              "unexpected \"%c\" symbol after \"%V\" "
                              "parameter in \"%V\" SSI command",
                              ch, &ctx->param->key, &ctx->command);
                break;
            }

            break;

        case ssi_prevalue_state:
            switch (ch) {
            case ' ':
            case CR:
            case LF:
            case '\t':
                break;

            case '"':
                state = ssi_double_quoted_value_state;
                break;

            case '\'':
                state = ssi_quoted_value_state;
                break;

            default:
                if (ch == '-') {
                    state = ssi_error_end0_state;
                } else {
                    state = ssi_error_state;
                }

                ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                              "unexpected \"%c\" symbol before value of "
                              "\"%V\" parameter in \"%V\" SSI command",
                              ch, &ctx->param->key, &ctx->command);
                break;
            }

            break;

        case ssi_double_quoted_value_state:
            switch (ch) {
            case '"':
                state = ssi_postparam_state;
                break;

            case '\\':
                ctx->saved_state = ssi_double_quoted_value_state;
                state = ssi_quoted_symbol_state;

                /* fall through */

            default:
                if (ctx->param->value.len == ctx->value_len) {
                    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                                  "too long \"%V%c...\" value of \"%V\" "
                                  "parameter in \"%V\" SSI command",
                                  &ctx->param->value, ch, &ctx->param->key,
                                  &ctx->command);
                    state = ssi_error_state;
                    break;
                }

                ctx->param->value.data[ctx->param->value.len++] = ch;
            }

            break;

        case ssi_quoted_value_state:
            switch (ch) {
            case '\'':
                state = ssi_postparam_state;
                break;

            case '\\':
                ctx->saved_state = ssi_quoted_value_state;
                state = ssi_quoted_symbol_state;

                /* fall through */

            default:
                if (ctx->param->value.len == ctx->value_len) {
                    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                                  "too long \"%V%c...\" value of \"%V\" "
                                  "parameter in \"%V\" SSI command",
                                  &ctx->param->value, ch, &ctx->param->key,
                                  &ctx->command);
                    state = ssi_error_state;
                    break;
                }

                ctx->param->value.data[ctx->param->value.len++] = ch;
            }

            break;

        case ssi_quoted_symbol_state:
            state = ctx->saved_state;

            if (ctx->param->value.len == ctx->value_len) {
                ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                              "too long \"%V%c...\" value of \"%V\" "
                              "parameter in \"%V\" SSI command",
                              &ctx->param->value, ch, &ctx->param->key,
                              &ctx->command);
                state = ssi_error_state;
                break;
            }

            ctx->param->value.data[ctx->param->value.len++] = ch;

            break;

        case ssi_postparam_state:

            if (ctx->param->value.len + 1 < ctx->value_len / 2) {
                value = ngx_pnalloc(r->pool, ctx->param->value.len + 1);
                if (value == NULL) {
                    return NGX_ERROR;
                }

                ngx_memcpy(value, ctx->param->value.data,
                           ctx->param->value.len);

                ctx->value_buf = ctx->param->value.data;
                ctx->param->value.data = value;

            } else {
                ctx->value_buf = NULL;
            }

            switch (ch) {
            case ' ':
            case CR:
            case LF:
            case '\t':
                state = ssi_preparam_state;
                break;

            case '-':
                state = ssi_comment_end0_state;
                break;

            default:
                ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                              "unexpected \"%c\" symbol after \"%V\" value "
                              "of \"%V\" parameter in \"%V\" SSI command",
                              ch, &ctx->param->value, &ctx->param->key,
                              &ctx->command);
                state = ssi_error_state;
                break;
            }

            break;

        case ssi_comment_end0_state:
            switch (ch) {
            case '-':
                state = ssi_comment_end1_state;
                break;

            default:
                ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                              "unexpected \"%c\" symbol in \"%V\" SSI command",
                              ch, &ctx->command);
                state = ssi_error_state;
                break;
            }

            break;

        case ssi_comment_end1_state:
            switch (ch) {
            case '>':
                ctx->state = ssi_start_state;
                ctx->pos = p + 1;
                ctx->looked = looked;
                ctx->copy_end = copy_end;

                if (ctx->copy_start == NULL && copy_end) {
                    ctx->copy_start = ctx->buf->pos;
                }

                return NGX_OK;

            default:
                ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                              "unexpected \"%c\" symbol in \"%V\" SSI command",
                              ch, &ctx->command);
                state = ssi_error_state;
                break;
            }

            break;

        case ssi_error_state:
            switch (ch) {
            case '-':
                state = ssi_error_end0_state;
                break;

            default:
                break;
            }

            break;

        case ssi_error_end0_state:
            switch (ch) {
            case '-':
                state = ssi_error_end1_state;
                break;

            default:
                state = ssi_error_state;
                break;
            }

            break;

        case ssi_error_end1_state:
            switch (ch) {
            case '>':
                ctx->state = ssi_start_state;
                ctx->pos = p + 1;
                ctx->looked = looked;
                ctx->copy_end = copy_end;

                if (ctx->copy_start == NULL && copy_end) {
                    ctx->copy_start = ctx->buf->pos;
                }

                return NGX_HTTP_SSI_ERROR;

            default:
                state = ssi_error_state;
                break;
            }

            break;
        }
    }

    ctx->state = state;
    ctx->pos = p;
    ctx->looked = looked;

    ctx->copy_end = (state == ssi_start_state) ? p : copy_end;

    if (ctx->copy_start == NULL && ctx->copy_end) {
        ctx->copy_start = ctx->buf->pos;
    }

    return NGX_AGAIN;
}


static ngx_str_t *
ngx_http_ssi_get_variable(ngx_http_request_t *r, ngx_str_t *name,
    ngx_uint_t key)
{
    ngx_uint_t           i;
    ngx_list_part_t     *part;
    ngx_http_ssi_var_t  *var;
    ngx_http_ssi_ctx_t  *ctx;

    ctx = ngx_http_get_module_ctx(r->main, ngx_http_ssi_filter_module);

#if (NGX_PCRE)
    {
    ngx_str_t  *value;

    if (key >= '0' && key <= '9') {
        i = key - '0';

        if (i < ctx->ncaptures) {
            value = ngx_palloc(r->pool, sizeof(ngx_str_t));
            if (value == NULL) {
                return NULL;
            }

            i *= 2;

            value->data = ctx->captures_data + ctx->captures[i];
            value->len = ctx->captures[i + 1] - ctx->captures[i];

            return value;
        }
    }
    }
#endif

    if (ctx->variables == NULL) {
        return NULL;
    }

    part = &ctx->variables->part;
    var = part->elts;

    for (i = 0; /* void */ ; i++) {

        if (i >= part->nelts) {
            if (part->next == NULL) {
                break;
            }

            part = part->next;
            var = part->elts;
            i = 0;
        }

        if (name->len != var[i].name.len) {
            continue;
        }

        if (key != var[i].key) {
            continue;
        }

        if (ngx_strncmp(name->data, var[i].name.data, name->len) == 0) {
            return &var[i].value;
        }
    }

    return NULL;
}


static ngx_int_t
ngx_http_ssi_evaluate_string(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t *text, ngx_uint_t flags)
{
    u_char                      ch, *p, **value, *data, *part_data;
    size_t                     *size, len, prefix, part_len;
    ngx_str_t                   var, *val;
    ngx_int_t                   key;
    ngx_uint_t                  i, n, bracket, quoted;
    ngx_array_t                 lengths, values;
    ngx_http_variable_value_t  *vv;

    n = ngx_http_script_variables_count(text);

    if (n == 0) {

        data = text->data;
        p = data;

        if ((flags & NGX_HTTP_SSI_ADD_PREFIX) && text->data[0] != '/') {

            for (prefix = r->uri.len; prefix; prefix--) {
                if (r->uri.data[prefix - 1] == '/') {
                    break;
                }
            }

            if (prefix) {
                len = prefix + text->len;

                data = ngx_pnalloc(r->pool, len);
                if (data == NULL) {
                    return NGX_ERROR;
                }

                p = ngx_copy(data, r->uri.data, prefix);
            }
        }

        quoted = 0;

        for (i = 0; i < text->len; i++) {
            ch = text->data[i];

            if (!quoted) {

                if (ch == '\\') {
                    quoted = 1;
                    continue;
                }

            } else {
                quoted = 0;

                if (ch != '\\' && ch != '\'' && ch != '"' && ch != '$') {
                    *p++ = '\\';
                }
            }

            *p++ = ch;
        }

        text->len = p - data;
        text->data = data;

        return NGX_OK;
    }

    if (ngx_array_init(&lengths, r->pool, 8, sizeof(size_t *)) != NGX_OK) {
        return NGX_ERROR;
    }

    if (ngx_array_init(&values, r->pool, 8, sizeof(u_char *)) != NGX_OK) {
        return NGX_ERROR;
    }

    len = 0;
    i = 0;

    while (i < text->len) {

        if (text->data[i] == '$') {

            var.len = 0;

            if (++i == text->len) {
                goto invalid_variable;
            }

            if (text->data[i] == '{') {
                bracket = 1;

                if (++i == text->len) {
                    goto invalid_variable;
                }

                var.data = &text->data[i];

            } else {
                bracket = 0;
                var.data = &text->data[i];
            }

            for ( /* void */ ; i < text->len; i++, var.len++) {
                ch = text->data[i];

                if (ch == '}' && bracket) {
                    i++;
                    bracket = 0;
                    break;
                }

                if ((ch >= 'A' && ch <= 'Z')
                    || (ch >= 'a' && ch <= 'z')
                    || (ch >= '0' && ch <= '9')
                    || ch == '_')
                {
                    continue;
                }

                break;
            }

            if (bracket) {
                ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                              "the closing bracket in \"%V\" "
                              "variable is missing", &var);
                return NGX_HTTP_SSI_ERROR;
            }

            if (var.len == 0) {
                goto invalid_variable;
            }

            key = ngx_hash_strlow(var.data, var.data, var.len);

            val = ngx_http_ssi_get_variable(r, &var, key);

            if (val == NULL) {
                vv = ngx_http_get_variable(r, &var, key);
                if (vv == NULL) {
                    return NGX_ERROR;
                }

                if (vv->not_found) {
                    continue;
                }

                part_data = vv->data;
                part_len = vv->len;

            } else {
                part_data = val->data;
                part_len = val->len;
            }

        } else {
            part_data = &text->data[i];
            quoted = 0;

            for (p = part_data; i < text->len; i++) {
                ch = text->data[i];

                if (!quoted) {

                    if (ch == '\\') {
                        quoted = 1;
                        continue;
                    }

                    if (ch == '$') {
                        break;
                    }

                } else {
                    quoted = 0;

                    if (ch != '\\' && ch != '\'' && ch != '"' && ch != '$') {
                        *p++ = '\\';
                    }
                }

                *p++ = ch;
            }

            part_len = p - part_data;
        }

        len += part_len;

        size = ngx_array_push(&lengths);
        if (size == NULL) {
            return NGX_ERROR;
        }

        *size = part_len;

        value = ngx_array_push(&values);
        if (value == NULL) {
            return NGX_ERROR;
        }

        *value = part_data;
    }

    prefix = 0;

    size = lengths.elts;
    value = values.elts;

    if (flags & NGX_HTTP_SSI_ADD_PREFIX) {
        for (i = 0; i < values.nelts; i++) {
            if (size[i] != 0) {
                if (*value[i] != '/') {
                    for (prefix = r->uri.len; prefix; prefix--) {
                        if (r->uri.data[prefix - 1] == '/') {
                            len += prefix;
                            break;
                        }
                    }
                }

                break;
            }
        }
    }

    p = ngx_pnalloc(r->pool, len + ((flags & NGX_HTTP_SSI_ADD_ZERO) ? 1 : 0));
    if (p == NULL) {
        return NGX_ERROR;
    }

    text->len = len;
    text->data = p;

    p = ngx_copy(p, r->uri.data, prefix);

    for (i = 0; i < values.nelts; i++) {
        p = ngx_copy(p, value[i], size[i]);
    }

    return NGX_OK;

invalid_variable:

    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                  "invalid variable name in \"%V\"", text);

    return NGX_HTTP_SSI_ERROR;
}


static ngx_int_t
ngx_http_ssi_regex_match(ngx_http_request_t *r, ngx_str_t *pattern,
    ngx_str_t *str)
{
#if (NGX_PCRE)
    int                   rc, *captures;
    u_char               *p, errstr[NGX_MAX_CONF_ERRSTR];
    size_t                size;
    ngx_int_t             key;
    ngx_str_t            *vv, name, value;
    ngx_uint_t            i, n;
    ngx_http_ssi_ctx_t   *ctx;
    ngx_http_ssi_var_t   *var;
    ngx_regex_compile_t   rgc;

    ngx_memzero(&rgc, sizeof(ngx_regex_compile_t));

    rgc.pattern = *pattern;
    rgc.pool = r->pool;
    rgc.err.len = NGX_MAX_CONF_ERRSTR;
    rgc.err.data = errstr;

    if (ngx_regex_compile(&rgc) != NGX_OK) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "%V", &rgc.err);
        return NGX_HTTP_SSI_ERROR;
    }

    n = (rgc.captures + 1) * 3;

    captures = ngx_palloc(r->pool, n * sizeof(int));
    if (captures == NULL) {
        return NGX_ERROR;
    }

    rc = ngx_regex_exec(rgc.regex, str, captures, n);

    if (rc < NGX_REGEX_NO_MATCHED) {
        ngx_log_error(NGX_LOG_ALERT, r->connection->log, 0,
                      ngx_regex_exec_n " failed: %d on \"%V\" using \"%V\"",
                      rc, str, pattern);
        return NGX_HTTP_SSI_ERROR;
    }

    if (rc == NGX_REGEX_NO_MATCHED) {
        return NGX_DECLINED;
    }

    ctx = ngx_http_get_module_ctx(r->main, ngx_http_ssi_filter_module);

    ctx->ncaptures = rc;
    ctx->captures = captures;
    ctx->captures_data = str->data;

    if (rgc.named_captures > 0) {

        if (ctx->variables == NULL) {
            ctx->variables = ngx_list_create(r->pool, 4,
                                             sizeof(ngx_http_ssi_var_t));
            if (ctx->variables == NULL) {
                return NGX_ERROR;
            }
        }

        size = rgc.name_size;
        p = rgc.names;

        for (i = 0; i < (ngx_uint_t) rgc.named_captures; i++, p += size) {

            name.data = &p[2];
            name.len = ngx_strlen(name.data);

            n = 2 * ((p[0] << 8) + p[1]);

            value.data = &str->data[captures[n]];
            value.len = captures[n + 1] - captures[n];

            key = ngx_hash_strlow(name.data, name.data, name.len);

            vv = ngx_http_ssi_get_variable(r, &name, key);

            if (vv) {
                *vv = value;
                continue;
            }

            var = ngx_list_push(ctx->variables);
            if (var == NULL) {
                return NGX_ERROR;
            }

            var->name = name;
            var->key = key;
            var->value = value;
        }
    }

    return NGX_OK;

#else

    ngx_log_error(NGX_LOG_ALERT, r->connection->log, 0,
                  "the using of the regex \"%V\" in SSI requires PCRE library",
                  pattern);
    return NGX_HTTP_SSI_ERROR;

#endif
}


static ngx_int_t
ngx_http_ssi_include(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t **params)
{
    ngx_int_t                    rc, key;
    ngx_str_t                   *uri, *file, *wait, *set, *stub, args;
    ngx_buf_t                   *b;
    ngx_uint_t                   flags, i;
    ngx_chain_t                 *cl, *tl, **ll, *out;
    ngx_http_request_t          *sr;
    ngx_http_ssi_var_t          *var;
    ngx_http_ssi_ctx_t          *mctx;
    ngx_http_ssi_block_t        *bl;
    ngx_http_post_subrequest_t  *psr;

    uri = params[NGX_HTTP_SSI_INCLUDE_VIRTUAL];
    file = params[NGX_HTTP_SSI_INCLUDE_FILE];
    wait = params[NGX_HTTP_SSI_INCLUDE_WAIT];
    set = params[NGX_HTTP_SSI_INCLUDE_SET];
    stub = params[NGX_HTTP_SSI_INCLUDE_STUB];

    if (uri && file) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "inclusion may be either virtual=\"%V\" or file=\"%V\"",
                      uri, file);
        return NGX_HTTP_SSI_ERROR;
    }

    if (uri == NULL && file == NULL) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "no parameter in \"include\" SSI command");
        return NGX_HTTP_SSI_ERROR;
    }

    if (set && stub) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "\"set\" and \"stub\" cannot be used together "
                      "in \"include\" SSI command");
        return NGX_HTTP_SSI_ERROR;
    }

    if (wait) {
        if (uri == NULL) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "\"wait\" cannot be used with file=\"%V\"", file);
            return NGX_HTTP_SSI_ERROR;
        }

        if (wait->len == 2
            && ngx_strncasecmp(wait->data, (u_char *) "no", 2) == 0)
        {
            wait = NULL;

        } else if (wait->len != 3
                   || ngx_strncasecmp(wait->data, (u_char *) "yes", 3) != 0)
        {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "invalid value \"%V\" in the \"wait\" parameter",
                          wait);
            return NGX_HTTP_SSI_ERROR;
        }
    }

    if (uri == NULL) {
        uri = file;
        wait = (ngx_str_t *) -1;
    }

    rc = ngx_http_ssi_evaluate_string(r, ctx, uri, NGX_HTTP_SSI_ADD_PREFIX);

    if (rc != NGX_OK) {
        return rc;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ssi include: \"%V\"", uri);

    ngx_str_null(&args);
    flags = NGX_HTTP_LOG_UNSAFE;

    if (ngx_http_parse_unsafe_uri(r, uri, &args, &flags) != NGX_OK) {
        return NGX_HTTP_SSI_ERROR;
    }

    psr = NULL;

    mctx = ngx_http_get_module_ctx(r->main, ngx_http_ssi_filter_module);

    if (stub) {
        if (mctx->blocks) {
            bl = mctx->blocks->elts;
            for (i = 0; i < mctx->blocks->nelts; i++) {
                if (stub->len == bl[i].name.len
                    && ngx_strncmp(stub->data, bl[i].name.data, stub->len) == 0)
                {
                    goto found;
                }
            }
        }

        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "\"stub\"=\"%V\" for \"include\" not found", stub);
        return NGX_HTTP_SSI_ERROR;

    found:

        psr = ngx_palloc(r->pool, sizeof(ngx_http_post_subrequest_t));
        if (psr == NULL) {
            return NGX_ERROR;
        }

        psr->handler = ngx_http_ssi_stub_output;

        if (bl[i].count++) {

            out = NULL;
            ll = &out;

            for (tl = bl[i].bufs; tl; tl = tl->next) {

                if (ctx->free) {
                    cl = ctx->free;
                    ctx->free = ctx->free->next;
                    b = cl->buf;

                } else {
                    b = ngx_alloc_buf(r->pool);
                    if (b == NULL) {
                        return NGX_ERROR;
                    }

                    cl = ngx_alloc_chain_link(r->pool);
                    if (cl == NULL) {
                        return NGX_ERROR;
                    }

                    cl->buf = b;
                }

                ngx_memcpy(b, tl->buf, sizeof(ngx_buf_t));

                b->pos = b->start;

                *ll = cl;
                cl->next = NULL;
                ll = &cl->next;
            }

            psr->data = out;

        } else {
            psr->data = bl[i].bufs;
        }
    }

    if (wait) {
        flags |= NGX_HTTP_SUBREQUEST_WAITED;
    }

    if (set) {
        key = ngx_hash_strlow(set->data, set->data, set->len);

        psr = ngx_palloc(r->pool, sizeof(ngx_http_post_subrequest_t));
        if (psr == NULL) {
            return NGX_ERROR;
        }

        psr->handler = ngx_http_ssi_set_variable;
        psr->data = ngx_http_ssi_get_variable(r, set, key);

        if (psr->data == NULL) {

            if (mctx->variables == NULL) {
                mctx->variables = ngx_list_create(r->pool, 4,
                                                  sizeof(ngx_http_ssi_var_t));
                if (mctx->variables == NULL) {
                    return NGX_ERROR;
                }
            }

            var = ngx_list_push(mctx->variables);
            if (var == NULL) {
                return NGX_ERROR;
            }

            var->name = *set;
            var->key = key;
            var->value = ngx_http_ssi_null_string;
            psr->data = &var->value;
        }

        flags |= NGX_HTTP_SUBREQUEST_IN_MEMORY|NGX_HTTP_SUBREQUEST_WAITED;
    }

    if (ngx_http_subrequest(r, uri, &args, &sr, psr, flags) != NGX_OK) {
        return NGX_HTTP_SSI_ERROR;
    }

    if (wait == NULL && set == NULL) {
        return NGX_OK;
    }

    if (ctx->wait == NULL) {
        ctx->wait = sr;

        return NGX_AGAIN;

    } else {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "can only wait for one subrequest at a time");
    }

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssi_stub_output(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_chain_t  *out;

    if (rc == NGX_ERROR || r->connection->error || r->request_output) {
        return rc;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ssi stub output: \"%V?%V\"", &r->uri, &r->args);

    out = data;

    if (!r->header_sent) {
        r->headers_out.content_type_len =
                                      r->parent->headers_out.content_type_len;
        r->headers_out.content_type = r->parent->headers_out.content_type;

        if (ngx_http_send_header(r) == NGX_ERROR) {
            return NGX_ERROR;
        }
    }

    return ngx_http_output_filter(r, out);
}


static ngx_int_t
ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_str_t  *value = data;

    if (r->upstream) {
        value->len = r->upstream->buffer.last - r->upstream->buffer.pos;
        value->data = r->upstream->buffer.pos;
    }

    return rc;
}


static ngx_int_t
ngx_http_ssi_echo(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t **params)
{
    u_char                     *p;
    uintptr_t                   len;
    ngx_int_t                   key;
    ngx_buf_t                  *b;
    ngx_str_t                  *var, *value, *enc, text;
    ngx_chain_t                *cl;
    ngx_http_variable_value_t  *vv;

    var = params[NGX_HTTP_SSI_ECHO_VAR];

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ssi echo \"%V\"", var);

    key = ngx_hash_strlow(var->data, var->data, var->len);

    value = ngx_http_ssi_get_variable(r, var, key);

    if (value == NULL) {
        vv = ngx_http_get_variable(r, var, key);

        if (vv == NULL) {
            return NGX_HTTP_SSI_ERROR;
        }

        if (!vv->not_found) {
            text.data = vv->data;
            text.len = vv->len;
            value = &text;
        }
    }

    if (value == NULL) {
        value = params[NGX_HTTP_SSI_ECHO_DEFAULT];

        if (value == NULL) {
            value = &ngx_http_ssi_none;

        } else if (value->len == 0) {
            return NGX_OK;
        }

    } else {
        if (value->len == 0) {
            return NGX_OK;
        }
    }

    enc = params[NGX_HTTP_SSI_ECHO_ENCODING];

    if (enc) {
        if (enc->len == 4 && ngx_strncmp(enc->data, "none", 4) == 0) {

            ctx->encoding = NGX_HTTP_SSI_NO_ENCODING;

        } else if (enc->len == 3 && ngx_strncmp(enc->data, "url", 3) == 0) {

            ctx->encoding = NGX_HTTP_SSI_URL_ENCODING;

        } else if (enc->len == 6 && ngx_strncmp(enc->data, "entity", 6) == 0) {

            ctx->encoding = NGX_HTTP_SSI_ENTITY_ENCODING;

        } else {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "unknown encoding \"%V\" in the \"echo\" command",
                          enc);
        }
    }

    p = value->data;

    switch (ctx->encoding) {

    case NGX_HTTP_SSI_URL_ENCODING:
        len = 2 * ngx_escape_uri(NULL, value->data, value->len,
                                 NGX_ESCAPE_HTML);

        if (len) {
            p = ngx_pnalloc(r->pool, value->len + len);
            if (p == NULL) {
                return NGX_HTTP_SSI_ERROR;
            }

            (void) ngx_escape_uri(p, value->data, value->len, NGX_ESCAPE_HTML);
        }

        len += value->len;
        break;

    case NGX_HTTP_SSI_ENTITY_ENCODING:
        len = ngx_escape_html(NULL, value->data, value->len);

        if (len) {
            p = ngx_pnalloc(r->pool, value->len + len);
            if (p == NULL) {
                return NGX_HTTP_SSI_ERROR;
            }

            (void) ngx_escape_html(p, value->data, value->len);
        }

        len += value->len;
        break;

    default: /* NGX_HTTP_SSI_NO_ENCODING */
        len = value->len;
        break;
    }

    b = ngx_calloc_buf(r->pool);
    if (b == NULL) {
        return NGX_HTTP_SSI_ERROR;
    }

    cl = ngx_alloc_chain_link(r->pool);
    if (cl == NULL) {
        return NGX_HTTP_SSI_ERROR;
    }

    b->memory = 1;
    b->pos = p;
    b->last = p + len;

    cl->buf = b;
    cl->next = NULL;
    *ctx->last_out = cl;
    ctx->last_out = &cl->next;

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssi_config(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t **params)
{
    ngx_str_t  *value;

    value = params[NGX_HTTP_SSI_CONFIG_TIMEFMT];

    if (value) {
        ctx->timefmt.len = value->len;
        ctx->timefmt.data = ngx_pnalloc(r->pool, value->len + 1);
        if (ctx->timefmt.data == NULL) {
            return NGX_HTTP_SSI_ERROR;
        }

        ngx_cpystrn(ctx->timefmt.data, value->data, value->len + 1);
    }

    value = params[NGX_HTTP_SSI_CONFIG_ERRMSG];

    if (value) {
        ctx->errmsg = *value;
    }

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssi_set(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t **params)
{
    ngx_int_t            key, rc;
    ngx_str_t           *name, *value, *vv;
    ngx_http_ssi_var_t  *var;
    ngx_http_ssi_ctx_t  *mctx;

    mctx = ngx_http_get_module_ctx(r->main, ngx_http_ssi_filter_module);

    if (mctx->variables == NULL) {
        mctx->variables = ngx_list_create(r->pool, 4,
                                          sizeof(ngx_http_ssi_var_t));
        if (mctx->variables == NULL) {
            return NGX_ERROR;
        }
    }

    name = params[NGX_HTTP_SSI_SET_VAR];
    value = params[NGX_HTTP_SSI_SET_VALUE];

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ssi set \"%V\" \"%V\"", name, value);

    rc = ngx_http_ssi_evaluate_string(r, ctx, value, 0);

    if (rc != NGX_OK) {
        return rc;
    }

    key = ngx_hash_strlow(name->data, name->data, name->len);

    vv = ngx_http_ssi_get_variable(r, name, key);

    if (vv) {
        *vv = *value;
        return NGX_OK;
    }

    var = ngx_list_push(mctx->variables);
    if (var == NULL) {
        return NGX_ERROR;
    }

    var->name = *name;
    var->key = key;
    var->value = *value;

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "set: \"%V\"=\"%V\"", name, value);

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssi_if(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t **params)
{
    u_char       *p, *last;
    ngx_str_t    *expr, left, right;
    ngx_int_t     rc;
    ngx_uint_t    negative, noregex, flags;

    if (ctx->command.len == 2) {
        if (ctx->conditional) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "the \"if\" command inside the \"if\" command");
            return NGX_HTTP_SSI_ERROR;
        }
    }

    if (ctx->output_chosen) {
        ctx->output = 0;
        return NGX_OK;
    }

    expr = params[NGX_HTTP_SSI_IF_EXPR];

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ssi if expr=\"%V\"", expr);

    left.data = expr->data;
    last = expr->data + expr->len;

    for (p = left.data; p < last; p++) {
        if (*p >= 'A' && *p <= 'Z') {
            *p |= 0x20;
            continue;
        }

        if ((*p >= 'a' && *p <= 'z')
             || (*p >= '0' && *p <= '9')
             || *p == '$' || *p == '{' || *p == '}' || *p == '_'
             || *p == '"' || *p == '\'')
        {
            continue;
        }

        break;
    }

    left.len = p - left.data;

    while (p < last && *p == ' ') {
        p++;
    }

    flags = 0;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "left: \"%V\"", &left);

    rc = ngx_http_ssi_evaluate_string(r, ctx, &left, flags);

    if (rc != NGX_OK) {
        return rc;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "evaluated left: \"%V\"", &left);

    if (p == last) {
        if (left.len) {
            ctx->output = 1;
            ctx->output_chosen = 1;

        } else {
            ctx->output = 0;
        }

        ctx->conditional = NGX_HTTP_SSI_COND_IF;

        return NGX_OK;
    }

    if (p < last && *p == '=') {
        negative = 0;
        p++;

    } else if (p + 1 < last && *p == '!' && *(p + 1) == '=') {
        negative = 1;
        p += 2;

    } else {
        goto invalid_expression;
    }

    while (p < last && *p == ' ') {
        p++;
    }

    if (p < last - 1 && *p == '/') {
        if (*(last - 1) != '/') {
            goto invalid_expression;
        }

        noregex = 0;
        flags = NGX_HTTP_SSI_ADD_ZERO;
        last--;
        p++;

    } else {
        noregex = 1;
        flags = 0;

        if (p < last - 1 && p[0] == '\\' && p[1] == '/') {
            p++;
        }
    }

    right.len = last - p;
    right.data = p;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "right: \"%V\"", &right);

    rc = ngx_http_ssi_evaluate_string(r, ctx, &right, flags);

    if (rc != NGX_OK) {
        return rc;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "evaluated right: \"%V\"", &right);

    if (noregex) {
        if (left.len != right.len) {
            rc = -1;

        } else {
            rc = ngx_strncmp(left.data, right.data, right.len);
        }

    } else {
        right.data[right.len] = '\0';

        rc = ngx_http_ssi_regex_match(r, &right, &left);

        if (rc == NGX_OK) {
            rc = 0;
        } else if (rc == NGX_DECLINED) {
            rc = -1;
        } else {
            return rc;
        }
    }

    if ((rc == 0 && !negative) || (rc != 0 && negative)) {
        ctx->output = 1;
        ctx->output_chosen = 1;

    } else {
        ctx->output = 0;
    }

    ctx->conditional = NGX_HTTP_SSI_COND_IF;

    return NGX_OK;

invalid_expression:

    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                  "invalid expression in \"%V\"", expr);

    return NGX_HTTP_SSI_ERROR;
}


static ngx_int_t
ngx_http_ssi_else(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t **params)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ssi else");

    if (ctx->output_chosen) {
        ctx->output = 0;
    } else {
        ctx->output = 1;
    }

    ctx->conditional = NGX_HTTP_SSI_COND_ELSE;

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssi_endif(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t **params)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ssi endif");

    ctx->output = 1;
    ctx->output_chosen = 0;
    ctx->conditional = 0;

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssi_block(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t **params)
{
    ngx_http_ssi_ctx_t    *mctx;
    ngx_http_ssi_block_t  *bl;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ssi block");

    mctx = ngx_http_get_module_ctx(r->main, ngx_http_ssi_filter_module);

    if (mctx->blocks == NULL) {
        mctx->blocks = ngx_array_create(r->pool, 4,
                                        sizeof(ngx_http_ssi_block_t));
        if (mctx->blocks == NULL) {
            return NGX_HTTP_SSI_ERROR;
        }
    }

    bl = ngx_array_push(mctx->blocks);
    if (bl == NULL) {
        return NGX_HTTP_SSI_ERROR;
    }

    bl->name = *params[NGX_HTTP_SSI_BLOCK_NAME];
    bl->bufs = NULL;
    bl->count = 0;

    ctx->output = 0;
    ctx->block = 1;

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssi_endblock(ngx_http_request_t *r, ngx_http_ssi_ctx_t *ctx,
    ngx_str_t **params)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ssi endblock");

    ctx->output = 1;
    ctx->block = 0;

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssi_date_gmt_local_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t gmt)
{
    time_t               now;
    ngx_http_ssi_ctx_t  *ctx;
    ngx_str_t           *timefmt;
    struct tm            tm;
    char                 buf[NGX_HTTP_SSI_DATE_LEN];

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;

    now = ngx_time();

    ctx = ngx_http_get_module_ctx(r, ngx_http_ssi_filter_module);

    timefmt = ctx ? &ctx->timefmt : &ngx_http_ssi_timefmt;

    if (timefmt->len == sizeof("%s") - 1
        && timefmt->data[0] == '%' && timefmt->data[1] == 's')
    {
        v->data = ngx_pnalloc(r->pool, NGX_TIME_T_LEN);
        if (v->data == NULL) {
            return NGX_ERROR;
        }

        v->len = ngx_sprintf(v->data, "%T", now) - v->data;

        return NGX_OK;
    }

    if (gmt) {
        ngx_libc_gmtime(now, &tm);
    } else {
        ngx_libc_localtime(now, &tm);
    }

    v->len = strftime(buf, NGX_HTTP_SSI_DATE_LEN,
                      (char *) timefmt->data, &tm);
    if (v->len == 0) {
        return NGX_ERROR;
    }

    v->data = ngx_pnalloc(r->pool, v->len);
    if (v->data == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(v->data, buf, v->len);

    return NGX_OK;
}


static ngx_int_t
ngx_http_ssi_preconfiguration(ngx_conf_t *cf)
{
    ngx_int_t                  rc;
    ngx_http_variable_t       *var, *v;
    ngx_http_ssi_command_t    *cmd;
    ngx_http_ssi_main_conf_t  *smcf;

    for (v = ngx_http_ssi_vars; v->name.len; v++) {
        var = ngx_http_add_variable(cf, &v->name, v->flags);
        if (var == NULL) {
            return NGX_ERROR;
        }

        var->get_handler = v->get_handler;
        var->data = v->data;
    }

    smcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_ssi_filter_module);

    for (cmd = ngx_http_ssi_commands; cmd->name.len; cmd++) {
        rc = ngx_hash_add_key(&smcf->commands, &cmd->name, cmd,
                              NGX_HASH_READONLY_KEY);

        if (rc == NGX_OK) {
            continue;
        }

        if (rc == NGX_BUSY) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "conflicting SSI command \"%V\"", &cmd->name);
        }

        return NGX_ERROR;
    }

    return NGX_OK;
}


static void *
ngx_http_ssi_create_main_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_main_conf_t  *smcf;

    smcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_main_conf_t));
    if (smcf == NULL) {
        return NULL;
    }

    smcf->commands.pool = cf->pool;
    smcf->commands.temp_pool = cf->temp_pool;

    if (ngx_hash_keys_array_init(&smcf->commands, NGX_HASH_SMALL) != NGX_OK) {
        return NULL;
    }

    return smcf;
}


static char *
ngx_http_ssi_init_main_conf(ngx_conf_t *cf, void *conf)
{
    ngx_http_ssi_main_conf_t *smcf = conf;

    ngx_hash_init_t  hash;

    hash.hash = &smcf->hash;
    hash.key = ngx_hash_key;
    hash.max_size = 1024;
    hash.bucket_size = ngx_cacheline_size;
    hash.name = "ssi_command_hash";
    hash.pool = cf->pool;
    hash.temp_pool = NULL;

    if (ngx_hash_init(&hash, smcf->commands.keys.elts,
                      smcf->commands.keys.nelts)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static void *
ngx_http_ssi_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_loc_conf_t  *slcf;

    slcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_loc_conf_t));
    if (slcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->types = { NULL };
     *     conf->types_keys = NULL;
     */

    slcf->enable = NGX_CONF_UNSET;
    slcf->silent_errors = NGX_CONF_UNSET;
    slcf->ignore_recycled_buffers = NGX_CONF_UNSET;
    slcf->last_modified = NGX_CONF_UNSET;

    slcf->min_file_chunk = NGX_CONF_UNSET_SIZE;
    slcf->value_len = NGX_CONF_UNSET_SIZE;

    return slcf;
}


static char *
ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssi_loc_conf_t *prev = parent;
    ngx_http_ssi_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_value(conf->silent_errors, prev->silent_errors, 0);
    ngx_conf_merge_value(conf->ignore_recycled_buffers,
                         prev->ignore_recycled_buffers, 0);
    ngx_conf_merge_value(conf->last_modified, prev->last_modified, 0);

    ngx_conf_merge_size_value(conf->min_file_chunk, prev->min_file_chunk, 1024);
    ngx_conf_merge_size_value(conf->value_len, prev->value_len, 255);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
                             &prev->types_keys, &prev->types,
                             ngx_http_html_default_types)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_ssi_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_ssi_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_ssi_body_filter;

    return NGX_OK;
}
