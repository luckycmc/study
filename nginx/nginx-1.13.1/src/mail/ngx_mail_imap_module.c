
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_mail.h>
#include <ngx_mail_imap_module.h>


static void *ngx_mail_imap_create_srv_conf(ngx_conf_t *cf);
static char *ngx_mail_imap_merge_srv_conf(ngx_conf_t *cf, void *parent,
    void *child);


static ngx_str_t  ngx_mail_imap_default_capabilities[] = {
    ngx_string("IMAP4"),
    ngx_string("IMAP4rev1"),
    ngx_string("UIDPLUS"),
    ngx_null_string
};


static ngx_conf_bitmask_t  ngx_mail_imap_auth_methods[] = {
    { ngx_string("plain"), NGX_MAIL_AUTH_PLAIN_ENABLED },
    { ngx_string("login"), NGX_MAIL_AUTH_LOGIN_ENABLED },
    { ngx_string("cram-md5"), NGX_MAIL_AUTH_CRAM_MD5_ENABLED },
    { ngx_string("external"), NGX_MAIL_AUTH_EXTERNAL_ENABLED },
    { ngx_null_string, 0 }
};


static ngx_str_t  ngx_mail_imap_auth_methods_names[] = {
    ngx_string("AUTH=PLAIN"),
    ngx_string("AUTH=LOGIN"),
    ngx_null_string,  /* APOP */
    ngx_string("AUTH=CRAM-MD5"),
    ngx_string("AUTH=EXTERNAL"),
    ngx_null_string   /* NONE */
};


static ngx_mail_protocol_t  ngx_mail_imap_protocol = {
    ngx_string("imap"),
    { 143, 993, 0, 0 },
    NGX_MAIL_IMAP_PROTOCOL,

    ngx_mail_imap_init_session,
    ngx_mail_imap_init_protocol,
    ngx_mail_imap_parse_command,
    ngx_mail_imap_auth_state,

    ngx_string("* BAD internal server error" CRLF),
    ngx_string("* BYE SSL certificate error" CRLF),
    ngx_string("* BYE No required SSL certificate" CRLF)
};


static ngx_command_t  ngx_mail_imap_commands[] = {

    { ngx_string("imap_client_buffer"),
      NGX_MAIL_MAIN_CONF|NGX_MAIL_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_MAIL_SRV_CONF_OFFSET,
      offsetof(ngx_mail_imap_srv_conf_t, client_buffer_size),
      NULL },

    { ngx_string("imap_capabilities"),
      NGX_MAIL_MAIN_CONF|NGX_MAIL_SRV_CONF|NGX_CONF_1MORE,
      ngx_mail_capabilities,
      NGX_MAIL_SRV_CONF_OFFSET,
      offsetof(ngx_mail_imap_srv_conf_t, capabilities),
      NULL },

    { ngx_string("imap_auth"),
      NGX_MAIL_MAIN_CONF|NGX_MAIL_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_MAIL_SRV_CONF_OFFSET,
      offsetof(ngx_mail_imap_srv_conf_t, auth_methods),
      &ngx_mail_imap_auth_methods },

      ngx_null_command
};


static ngx_mail_module_t  ngx_mail_imap_module_ctx = {
    &ngx_mail_imap_protocol,               /* protocol */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_mail_imap_create_srv_conf,         /* create server configuration */
    ngx_mail_imap_merge_srv_conf           /* merge server configuration */
};


ngx_module_t  ngx_mail_imap_module = {
    NGX_MODULE_V1,
    &ngx_mail_imap_module_ctx,             /* module context */
    ngx_mail_imap_commands,                /* module directives */
    NGX_MAIL_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static void *
ngx_mail_imap_create_srv_conf(ngx_conf_t *cf)
{
    ngx_mail_imap_srv_conf_t  *iscf;

    iscf = ngx_pcalloc(cf->pool, sizeof(ngx_mail_imap_srv_conf_t));
    if (iscf == NULL) {
        return NULL;
    }

    iscf->client_buffer_size = NGX_CONF_UNSET_SIZE;

    if (ngx_array_init(&iscf->capabilities, cf->pool, 4, sizeof(ngx_str_t))
        != NGX_OK)
    {
        return NULL;
    }

    return iscf;
}


static char *
ngx_mail_imap_merge_srv_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_mail_imap_srv_conf_t *prev = parent;
    ngx_mail_imap_srv_conf_t *conf = child;

    u_char      *p, *auth;
    size_t       size;
    ngx_str_t   *c, *d;
    ngx_uint_t   i, m;

    ngx_conf_merge_size_value(conf->client_buffer_size,
                              prev->client_buffer_size,
                              (size_t) ngx_pagesize);

    ngx_conf_merge_bitmask_value(conf->auth_methods,
                              prev->auth_methods,
                              (NGX_CONF_BITMASK_SET
                               |NGX_MAIL_AUTH_PLAIN_ENABLED));


    if (conf->capabilities.nelts == 0) {
        conf->capabilities = prev->capabilities;
    }

    if (conf->capabilities.nelts == 0) {

        for (d = ngx_mail_imap_default_capabilities; d->len; d++) {
            c = ngx_array_push(&conf->capabilities);
            if (c == NULL) {
                return NGX_CONF_ERROR;
            }

            *c = *d;
        }
    }

    size = sizeof("* CAPABILITY" CRLF) - 1;

    c = conf->capabilities.elts;
    for (i = 0; i < conf->capabilities.nelts; i++) {
        size += 1 + c[i].len;
    }

    for (m = NGX_MAIL_AUTH_PLAIN_ENABLED, i = 0;
         m <= NGX_MAIL_AUTH_EXTERNAL_ENABLED;
         m <<= 1, i++)
    {
        if (m & conf->auth_methods) {
            size += 1 + ngx_mail_imap_auth_methods_names[i].len;
        }
    }

    p = ngx_pnalloc(cf->pool, size);
    if (p == NULL) {
        return NGX_CONF_ERROR;
    }

    conf->capability.len = size;
    conf->capability.data = p;

    p = ngx_cpymem(p, "* CAPABILITY", sizeof("* CAPABILITY") - 1);

    for (i = 0; i < conf->capabilities.nelts; i++) {
        *p++ = ' ';
        p = ngx_cpymem(p, c[i].data, c[i].len);
    }

    auth = p;

    for (m = NGX_MAIL_AUTH_PLAIN_ENABLED, i = 0;
         m <= NGX_MAIL_AUTH_EXTERNAL_ENABLED;
         m <<= 1, i++)
    {
        if (m & conf->auth_methods) {
            *p++ = ' ';
            p = ngx_cpymem(p, ngx_mail_imap_auth_methods_names[i].data,
                           ngx_mail_imap_auth_methods_names[i].len);
        }
    }

    *p++ = CR; *p = LF;


    size += sizeof(" STARTTLS") - 1;

    p = ngx_pnalloc(cf->pool, size);
    if (p == NULL) {
        return NGX_CONF_ERROR;
    }

    conf->starttls_capability.len = size;
    conf->starttls_capability.data = p;

    p = ngx_cpymem(p, conf->capability.data,
                   conf->capability.len - (sizeof(CRLF) - 1));
    p = ngx_cpymem(p, " STARTTLS", sizeof(" STARTTLS") - 1);
    *p++ = CR; *p = LF;


    size = (auth - conf->capability.data) + sizeof(CRLF) - 1
            + sizeof(" STARTTLS LOGINDISABLED") - 1;

    p = ngx_pnalloc(cf->pool, size);
    if (p == NULL) {
        return NGX_CONF_ERROR;
    }

    conf->starttls_only_capability.len = size;
    conf->starttls_only_capability.data = p;

    p = ngx_cpymem(p, conf->capability.data,
                   auth - conf->capability.data);
    p = ngx_cpymem(p, " STARTTLS LOGINDISABLED",
                   sizeof(" STARTTLS LOGINDISABLED") - 1);
    *p++ = CR; *p = LF;

    return NGX_CONF_OK;
}
>start + s->tag.len;
            s->buffer->pos = s->arg_start;
            s->buffer->last = s->arg_start;

        } else {
            s->buffer->pos = s->buffer->start;
            s->buffer->last = s->buffer->start;
            s->tag.len = 0;
        }
    }

    ngx_mail_send(c->write);
}


static ngx_int_t
ngx_mail_imap_login(ngx_mail_session_t *s, ngx_connection_t *c)
{
    ngx_str_t  *arg;

#if (NGX_MAIL_SSL)
    if (ngx_mail_starttls_only(s, c)) {
        return NGX_MAIL_PARSE_INVALID_COMMAND;
    }
#endif

    arg = s->args.elts;

    if (s->args.nelts != 2 || arg[0].len == 0) {
        return NGX_MAIL_PARSE_INVALID_COMMAND;
    }

    s->login.len = arg[0].len;
    s->login.data = ngx_pnalloc(c->pool, s->login.len);
    if (s->login.data == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(s->login.data, arg[0].data, s->login.len);

    s->passwd.len = arg[1].len;
    s->passwd.data = ngx_pnalloc(c->pool, s->passwd.len);
    if (s->passwd.data == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(s->passwd.data, arg[1].data, s->passwd.len);

#if (NGX_DEBUG_MAIL_PASSWD)
    ngx_log_debug2(NGX_LOG_DEBUG_MAIL, c->log, 0,
                   "imap login:\"%V\" passwd:\"%V\"",
                   &s->login, &s->passwd);
#else
    ngx_log_debug1(NGX_LOG_DEBUG_MAIL, c->log, 0,
                   "imap login:\"%V\"", &s->login);
#endif

    return NGX_DONE;
}


static ngx_int_t
ngx_mail_imap_authenticate(ngx_mail_session_t *s, ngx_connection_t *c)
{
    ngx_int_t                  rc;
    ngx_mail_core_srv_conf_t  *cscf;
    ngx_mail_imap_srv_conf_t  *iscf;

#if (NGX_MAIL_SSL)
    if (ngx_mail_starttls_only(s, c)) {
        return NGX_MAIL_PARSE_INVALID_COMMAND;
    }
#endif

    iscf = ngx_mail_get_module_srv_conf(s, ngx_mail_imap_module);

    rc = ngx_mail_auth_parse(s, c);

    switch (rc) {

    case NGX_MAIL_AUTH_LOGIN:

        ngx_str_set(&s->out, imap_username);
        s->mail_state = ngx_imap_auth_login_username;

        return NGX_OK;

    case NGX_MAIL_AUTH_LOGIN_USERNAME:

        ngx_str_set(&s->out, imap_password);
        s->mail_state = ngx_imap_auth_login_password;

        return ngx_mail_auth_login_username(s, c, 1);

    case NGX_MAIL_AUTH_PLAIN:

        ngx_str_set(&s->out, imap_plain_next);
        s->mail_state = ngx_imap_auth_plain;

        return NGX_OK;

    case NGX_MAIL_AUTH_CRAM_MD5:

        if (!(iscf->auth_methods & NGX_MAIL_AUTH_CRAM_MD5_ENABLED)) {
            return NGX_MAIL_PARSE_INVALID_COMMAND;
        }

        if (s->salt.data == NULL) {
            cscf = ngx_mail_get_module_srv_conf(s, ngx_mail_core_module);

            if (ngx_mail_salt(s, c, cscf) != NGX_OK) {
                return NGX_ERROR;
            }
        }

        if (ngx_mail_auth_cram_md5_salt(s, c, "+ ", 2) == NGX_OK) {
            s->mail_state = ngx_imap_auth_cram_md5;
            return NGX_OK;
        }

        return NGX_ERROR;

    case NGX_MAIL_AUTH_EXTERNAL:

        if (!(iscf->auth_methods & NGX_MAIL_AUTH_EXTERNAL_ENABLED)) {
            return NGX_MAIL_PARSE_INVALID_COMMAND;
        }

        ngx_str_set(&s->out, imap_username);
        s->mail_state = ngx_imap_auth_external;

        return NGX_OK;
    }

    return rc;
}


static ngx_int_t
ngx_mail_imap_capability(ngx_mail_session_t *s, ngx_connection_t *c)
{
    ngx_mail_imap_srv_conf_t  *iscf;

    iscf = ngx_mail_get_module_srv_conf(s, ngx_mail_imap_module);

#if (NGX_MAIL_SSL)

    if (c->ssl == NULL) {
        ngx_mail_ssl_conf_t  *sslcf;

        sslcf = ngx_mail_get_module_srv_conf(s, ngx_mail_ssl_module);

        if (sslcf->starttls == NGX_MAIL_STARTTLS_ON) {
            s->text = iscf->starttls_capability;
            return NGX_OK;
        }

        if (sslcf->starttls == NGX_MAIL_STARTTLS_ONLY) {
            s->text = iscf->starttls_only_capability;
            return NGX_OK;
        }
    }
#endif

    s->text = iscf->capability;

    return NGX_OK;
}


static ngx_int_t
ngx_mail_imap_starttls(ngx_mail_session_t *s, ngx_connection_t *c)
{
#if (NGX_MAIL_SSL)
    ngx_mail_ssl_conf_t  *sslcf;

    if (c->ssl == NULL) {
        sslcf = ngx_mail_get_module_srv_conf(s, ngx_mail_ssl_module);
        if (sslcf->starttls) {
            c->read->handler = ngx_mail_starttls_handler;
            return NGX_OK;
        }
    }

#endif

    return NGX_MAIL_PARSE_INVALID_COMMAND;
}
