
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


/*
 * the single part format:
 *
 * "HTTP/1.0 206 Partial Content" CRLF
 * ... header ...
 * "Content-Type: image/jpeg" CRLF
 * "Content-Length: SIZE" CRLF
 * "Content-Range: bytes START-END/SIZE" CRLF
 * CRLF
 * ... data ...
 *
 *
 * the multipart format:
 *
 * "HTTP/1.0 206 Partial Content" CRLF
 * ... header ...
 * "Content-Type: multipart/byteranges; boundary=0123456789" CRLF
 * CRLF
 * CRLF
 * "--0123456789" CRLF
 * "Content-Type: image/jpeg" CRLF
 * "Content-Range: bytes START0-END0/SIZE" CRLF
 * CRLF
 * ... data ...
 * CRLF
 * "--0123456789" CRLF
 * "Content-Type: image/jpeg" CRLF
 * "Content-Range: bytes START1-END1/SIZE" CRLF
 * CRLF
 * ... data ...
 * CRLF
 * "--0123456789--" CRLF
 */


typedef struct {
    off_t        start;
    off_t        end;
    ngx_str_t    content_range;
} ngx_http_range_t;


typedef struct {
    off_t        offset;
    ngx_str_t    boundary_header;
    ngx_array_t  ranges;
} ngx_http_range_filter_ctx_t;


static ngx_int_t ngx_http_range_parse(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_uint_t ranges);
static ngx_int_t ngx_http_range_singlepart_header(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx);
static ngx_int_t ngx_http_range_multipart_header(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx);
static ngx_int_t ngx_http_range_not_satisfiable(ngx_http_request_t *r);
static ngx_int_t ngx_http_range_test_overlapped(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_chain_t *in);
static ngx_int_t ngx_http_range_singlepart_body(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_chain_t *in);
static ngx_int_t ngx_http_range_multipart_body(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_chain_t *in);

static ngx_int_t ngx_http_range_header_filter_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_range_body_filter_init(ngx_conf_t *cf);


static ngx_http_module_t  ngx_http_range_header_filter_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_range_header_filter_init,     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL,                                  /* merge location configuration */
};


ngx_module_t  ngx_http_range_header_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_range_header_filter_module_ctx, /* module context */
    NULL,                                  /* module directives */
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


static ngx_http_module_t  ngx_http_range_body_filter_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_range_body_filter_init,       /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL,                                  /* merge location configuration */
};


ngx_module_t  ngx_http_range_body_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_range_body_filter_module_ctx, /* module context */
    NULL,                                  /* module directives */
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


static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt    ngx_http_next_body_filter;


static ngx_int_t
ngx_http_range_header_filter(ngx_http_request_t *r)
{
    time_t                        if_range_time;
    ngx_str_t                    *if_range, *etag;
    ngx_uint_t                    ranges;
    ngx_http_core_loc_conf_t     *clcf;
    ngx_http_range_filter_ctx_t  *ctx;

    if (r->http_version < NGX_HTTP_VERSION_10
        || r->headers_out.status != NGX_HTTP_OK
        || (r != r->main && !r->subrequest_ranges)
        || r->headers_out.content_length_n == -1
        || !r->allow_ranges)
    {
        return ngx_http_next_header_filter(r);
    }

    clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);

    if (clcf->max_ranges == 0) {
        return ngx_http_next_header_filter(r);
    }

    if (r->headers_in.range == NULL
        || r->headers_in.range->value.len < 7
        || ngx_strncasecmp(r->headers_in.range->value.data,
                           (u_char *) "bytes=", 6)
           != 0)
    {
        goto next_filter;
    }

    if (r->headers_in.if_range) {

        if_range = &r->headers_in.if_range->value;

        if (if_range->len >= 2 && if_range->data[if_range->len - 1] == '"') {

            if (r->headers_out.etag == NULL) {
                goto next_filter;
            }

            etag = &r->headers_out.etag->value;

            ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "http ir:%V etag:%V", if_range, etag);

            if (if_range->len != etag->len
                || ngx_strncmp(if_range->data, etag->data, etag->len) != 0)
            {
                goto next_filter;
            }

            goto parse;
        }

        if (r->headers_out.last_modified_time == (time_t) -1) {
            goto next_filter;
        }

        if_range_time = ngx_parse_http_time(if_range->data, if_range->len);

        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "http ir:%T lm:%T",
                       if_range_time, r->headers_out.last_modified_time);

        if (if_range_time != r->headers_out.last_modified_time) {
            goto next_filter;
        }
    }

parse:

    ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_range_filter_ctx_t));
    if (ctx == NULL) {
        return NGX_ERROR;
    }

    ctx->offset = r->headers_out.content_offset;

    ranges = r->single_range ? 1 : clcf->max_ranges;

    switch (ngx_http_range_parse(r, ctx, ranges)) {

    case NGX_OK:
        ngx_http_set_ctx(r, ctx, ngx_http_range_body_filter_module);

        r->headers_out.status = NGX_HTTP_PARTIAL_CONTENT;
        r->headers_out.status_line.len = 0;

        if (ctx->ranges.nelts == 1) {
            return ngx_http_range_singlepart_header(r, ctx);
        }

        return ngx_http_range_multipart_header(r, ctx);

    case NGX_HTTP_RANGE_NOT_SATISFIABLE:
        return ngx_http_range_not_satisfiable(r);

    case NGX_ERROR:
        return NGX_ERROR;

    default: /* NGX_DECLINED */
        break;
    }

next_filter:

    r->headers_out.accept_ranges = ngx_list_push(&r->headers_out.headers);
    if (r->headers_out.accept_ranges == NULL) {
        return NGX_ERROR;
    }

    r->headers_out.accept_ranges->hash = 1;
    ngx_str_set(&r->headers_out.accept_ranges->key, "Accept-Ranges");
    ngx_str_set(&r->headers_out.accept_ranges->value, "bytes");

    return ngx_http_next_header_filter(r);
}


static ngx_int_t
ngx_http_range_parse(ngx_http_request_t *r, ngx_http_range_filter_ctx_t *ctx,
    ngx_uint_t ranges)
{
    u_char                       *p;
    off_t                         start, end, size, content_length, cutoff,
                                  cutlim;
    ngx_uint_t                    suffix;
    ngx_http_range_t             *range;
    ngx_http_range_filter_ctx_t  *mctx;

    if (r != r->main) {
        mctx = ngx_http_get_module_ctx(r->main,
                                       ngx_http_range_body_filter_module);
        if (mctx) {
            ctx->ranges = mctx->ranges;
            return NGX_OK;
        }
    }

    if (ngx_array_init(&ctx->ranges, r->pool, 1, sizeof(ngx_http_range_t))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    p = r->headers_in.range->value.data + 6;
    size = 0;
    content_length = r->headers_out.content_length_n;

    cutoff = NGX_MAX_OFF_T_VALUE / 10;
    cutlim = NGX_MAX_OFF_T_VALUE % 10;

    for ( ;; ) {
        start = 0;
        end = 0;
        suffix = 0;

        while (*p == ' ') { p++; }

        if (*p != '-') {
            if (*p < '0' || *p > '9') {
                return NGX_HTTP_RANGE_NOT_SATISFIABLE;
            }

            while (*p >= '0' && *p <= '9') {
                if (start >= cutoff && (start > cutoff || *p - '0' > cutlim)) {
                    return NGX_HTTP_RANGE_NOT_SATISFIABLE;
                }

                start = start * 10 + *p++ - '0';
            }

            while (*p == ' ') { p++; }

            if (*p++ != '-') {
                return NGX_HTTP_RANGE_NOT_SATISFIABLE;
            }

            while (*p == ' ') { p++; }

            if (*p == ',' || *p == '\0') {
                end = content_length;
                goto found;
            }

        } else {
            suffix = 1;
            p++;
        }

        if (*p < '0' || *p > '9') {
            return NGX_HTTP_RANGE_NOT_SATISFIABLE;
        }

        while (*p >= '0' && *p <= '9') {
            if (end >= cutoff && (end > cutoff || *p - '0' > cutlim)) {
                return NGX_HTTP_RANGE_NOT_SATISFIABLE;
            }

            end = end * 10 + *p++ - '0';
        }

        while (*p == ' ') { p++; }

        if (*p != ',' && *p != '\0') {
            return NGX_HTTP_RANGE_NOT_SATISFIABLE;
        }

        if (suffix) {
            start = content_length - end;
            end = content_length - 1;
        }

        if (end >= content_length) {
            end = content_length;

        } else {
            end++;
        }

    found:

        if (start < end) {
            range = ngx_array_push(&ctx->ranges);
            if (range == NULL) {
                return NGX_ERROR;
            }

            range->start = start;
            range->end = end;

            size += end - start;

            if (ranges-- == 0) {
                return NGX_DECLINED;
            }
        }

        if (*p++ != ',') {
            break;
        }
    }

    if (ctx->ranges.nelts == 0) {
        return NGX_HTTP_RANGE_NOT_SATISFIABLE;
    }

    if (size > content_length) {
        return NGX_DECLINED;
    }

    return NGX_OK;
}


static ngx_int_t
ngx_http_range_singlepart_header(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx)
{
    ngx_table_elt_t   *content_range;
    ngx_http_range_t  *range;

    if (r != r->main) {
        return ngx_http_next_header_filter(r);
    }

    content_range = ngx_list_push(&r->headers_out.headers);
    if (content_range == NULL) {
        return NGX_ERROR;
    }

    r->headers_out.content_range = content_range;

    content_range->hash = 1;
    ngx_str_set(&content_range->key, "Content-Range");

    content_range->value.data = ngx_pnalloc(r->pool,
                                    sizeof("bytes -/") - 1 + 3 * NGX_OFF_T_LEN);
    if (content_range->value.data == NULL) {
        content_range->hash = 0;
        r->headers_out.content_range = NULL;
        return NGX_ERROR;
    }

    /* "Content-Range: bytes SSSS-EEEE/TTTT" header */

    range = ctx->ranges.elts;

    content_range->value.len = ngx_sprintf(content_range->value.data,
                                           "bytes %O-%O/%O",
                                           range->start, range->end - 1,
                                           r->headers_out.content_length_n)
                               - content_range->value.data;

    r->headers_out.content_length_n = range->end - range->start;
    r->headers_out.content_offset = range->start;

    if (r->headers_out.content_length) {
        r->headers_out.content_length->hash = 0;
        r->headers_out.content_length = NULL;
    }

    return ngx_http_next_header_filter(r);
}


static ngx_int_t
ngx_http_range_multipart_header(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx)
{
    size_t              len;
    ngx_uint_t          i;
    ngx_http_range_t   *range;
    ngx_atomic_uint_t   boundary;

    len = sizeof(CRLF "--") - 1 + NGX_ATOMIC_T_LEN
          + sizeof(CRLF "Content-Type: ") - 1
          + r->headers_out.content_type.len
          + sizeof(CRLF "Content-Range: bytes ") - 1;

    if (r->headers_out.content_type_len == r->headers_out.content_type.len
        && r->headers_out.charset.len)
    {
        len += sizeof("; charset=") - 1 + r->headers_out.charset.len;
    }

    ctx->boundary_header.data = ngx_pnalloc(r->pool, len);
    if (ctx->boundary_header.data == NULL) {
        return NGX_ERROR;
    }

    boundary = ngx_next_temp_number(0);

    /*
     * The boundary header of the range:
     * CRLF
     * "--0123456789" CRLF
     * "Content-Type: image/jpeg" CRLF
     * "Content-Range: bytes "
     */

    if (r->headers_out.content_type_len == r->headers_out.content_type.len
        && r->headers_out.charset.len)
    {
        ctx->boundary_header.len = ngx_sprintf(ctx->boundary_header.data,
                                           CRLF "--%0muA" CRLF
                                           "Content-Type: %V; charset=%V" CRLF
                                           "Content-Range: bytes ",
                                           boundary,
                                           &r->headers_out.content_type,
                                           &r->headers_out.charset)
                                   - ctx->boundary_header.data;

    } else if (r->headers_out.content_type.len) {
        ctx->boundary_header.len = ngx_sprintf(ctx->boundary_header.data,
                                           CRLF "--%0muA" CRLF
                                           "Content-Type: %V" CRLF
                                           "Content-Range: bytes ",
                                           boundary,
                                           &r->headers_out.content_type)
                                   - ctx->boundary_header.data;

    } else {
        ctx->boundary_header.len = ngx_sprintf(ctx->boundary_header.data,
                                           CRLF "--%0muA" CRLF
                                           "Content-Range: bytes ",
                                           boundary)
                                   - ctx->boundary_header.data;
    }

    r->headers_out.content_type.data =
        ngx_pnalloc(r->pool,
                    sizeof("Content-Type: multipart/byteranges; boundary=") - 1
                    + NGX_ATOMIC_T_LEN);

    if (r->headers_out.content_type.data == NULL) {
        return NGX_ERROR;
    }

    r->headers_out.content_type_lowcase = NULL;

    /* "Content-Type: multipart/byteranges; boundary=0123456789" */

    r->headers_out.content_type.len =
                           ngx_sprintf(r->headers_out.content_type.data,
                                       "multipart/byteranges; boundary=%0muA",
                                       boundary)
                           - r->headers_out.content_type.data;

    r->headers_out.content_type_len = r->headers_out.content_type.len;

    r->headers_out.charset.len = 0;

    /* the size of the last boundary CRLF "--0123456789--" CRLF */

    len = sizeof(CRLF "--") - 1 + NGX_ATOMIC_T_LEN + sizeof("--" CRLF) - 1;

    range = ctx->ranges.elts;
    for (i = 0; i < ctx->ranges.nelts; i++) {

        /* the size of the range: "SSSS-EEEE/TTTT" CRLF CRLF */

        range[i].content_range.data =
                               ngx_pnalloc(r->pool, 3 * NGX_OFF_T_LEN + 2 + 4);

        if (range[i].content_range.data == NULL) {
            return NGX_ERROR;
        }

        range[i].content_range.len = ngx_sprintf(range[i].content_range.data,
                                               "%O-%O/%O" CRLF CRLF,
                                               range[i].start, range[i].end - 1,
                                               r->headers_out.content_length_n)
                                     - range[i].content_range.data;

        len += ctx->boundary_header.len + range[i].content_range.len
                                    + (size_t) (range[i].end - range[i].start);
    }

    r->headers_out.content_length_n = len;

    if (r->headers_out.content_length) {
        r->headers_out.content_length->hash = 0;
        r->headers_out.content_length = NULL;
    }

    return ngx_http_next_header_filter(r);
}


static ngx_int_t
ngx_http_range_not_satisfiable(ngx_http_request_t *r)
{
    ngx_table_elt_t  *content_range;

    r->headers_out.status = NGX_HTTP_RANGE_NOT_SATISFIABLE;

    content_range = ngx_list_push(&r->headers_out.headers);
    if (content_range == NULL) {
        return NGX_ERROR;
    }

    r->headers_out.content_range = content_range;

    content_range->hash = 1;
    ngx_str_set(&content_range->key, "Content-Range");

    content_range->value.data = ngx_pnalloc(r->pool,
                                       sizeof("bytes */") - 1 + NGX_OFF_T_LEN);
    if (content_range->value.data == NULL) {
        content_range->hash = 0;
        r->headers_out.content_range = NULL;
        return NGX_ERROR;
    }

    content_range->value.len = ngx_sprintf(content_range->value.data,
                                           "bytes */%O",
                                           r->headers_out.content_length_n)
                               - content_range->value.data;

    ngx_http_clear_content_length(r);

    return NGX_HTTP_RANGE_NOT_SATISFIABLE;
}


static ngx_int_t
ngx_http_range_body_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
    ngx_http_range_filter_ctx_t  *ctx;

    if (in == NULL) {
        return ngx_http_next_body_filter(r, in);
    }

    ctx = ngx_http_get_module_ctx(r, ngx_http_range_body_filter_module);

    if (ctx == NULL) {
        return ngx_http_next_body_filter(r, in);
    }

    if (ctx->ranges.nelts == 1) {
        return ngx_http_range_singlepart_body(r, ctx, in);
    }

    /*
     * multipart ranges are supported only if whole body is in a single buffer
     */

    if (ngx_buf_special(in->buf)) {
        return ngx_http_next_body_filter(r, in);
    }

    if (ngx_http_range_test_overlapped(r, ctx, in) != NGX_OK) {
        return NGX_ERROR;
    }

    return ngx_http_range_multipart_body(r, ctx, in);
}


static ngx_int_t
ngx_http_range_test_overlapped(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_chain_t *in)
{
    off_t              start, last;
    ngx_buf_t         *buf;
    ngx_uint_t         i;
    ngx_http_range_t  *range;

    if (ctx->offset) {
        goto overlapped;
    }

    buf = in->buf;

    if (!buf->last_buf) {
        start = ctx->offset;
        last = ctx->offset + ngx_buf_size(buf);

        range = ctx->ranges.elts;
        for (i = 0; i < ctx->ranges.nelts; i++) {
            if (start > range[i].start || last < range[i].end) {
                goto overlapped;
            }
        }
    }

    ctx->offset = ngx_buf_size(buf);

    return NGX_OK;

overlapped:

    ngx_log_error(NGX_LOG_ALERT, r->connection->log, 0,
                  "range in overlapped buffers");

    return NGX_ERROR;
}


static ngx_int_t
ngx_http_range_singlepart_body(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_chain_t *in)
{
    off_t              start, last;
    ngx_buf_t         *buf;
    ngx_chain_t       *out, *cl, **ll;
    ngx_http_range_t  *range;

    out = NULL;
    ll = &out;
    range = ctx->ranges.elts;

    for (cl = in; cl; cl = cl->next) {

        buf = cl->buf;

        start = ctx->offset;
        last = ctx->offset + ngx_buf_size(buf);

        ctx->offset = last;

        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "http range body buf: %O-%O", start, last);

        if (ngx_buf_special(buf)) {
            *ll = cl;
            ll = &cl->next;
            continue;
        }

        if (range->end <= start || range->start >= last) {

            ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "http range body skip");

            if (buf->in_file) {
                buf->file_pos = buf->file_last;
            }

            buf->pos = buf->last;
            buf->sync = 1;

            continue;
        }

        if (range->start > start) {

            if (buf->in_file) {
                buf->file_pos += range->start - start;
            }

            if (ngx_buf_in_memory(buf)) {
                buf->pos += (size_t) (range->start - start);
            }
        }

        if (range->end <= last) {

            if (buf->in_file) {
                buf->file_last -= last - range->end;
            }

            if (ngx_buf_in_memory(buf)) {
                buf->last -= (size_t) (last - range->end);
            }

            buf->last_buf = (r == r->main) ? 1 : 0;
            buf->last_in_chain = 1;
            *ll = cl;
            cl->next = NULL;

            break;
        }

        *ll = cl;
        ll = &cl->next;
    }

    if (out == NULL) {
        return NGX_OK;
    }

    return ngx_http_next_body_filter(r, out);
}


static ngx_int_t
ngx_http_range_multipart_body(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_chain_t *in)
{
    ngx_buf_t         *b, *buf;
    ngx_uint_t         i;
    ngx_chain_t       *out, *hcl, *rcl, *dcl, **ll;
    ngx_http_range_t  *range;

    ll = &out;
    buf = in->buf;
    range = ctx->ranges.elts;

    for (i = 0; i < ctx->ranges.nelts; i++) {

        /*
         * The boundary header of the range:
         * CRLF
         * "--0123456789" CRLF
         * "Content-Type: image/jpeg" CRLF
         * "Content-Range: bytes "
         */

        b = ngx_calloc_buf(r->pool);
        if (b == NULL) {
            return NGX_ERROR;
        }

        b->memory = 1;
        b->pos = ctx->boundary_header.data;
        b->last = ctx->boundary_header.data + ctx->boundary_header.len;

        hcl = ngx_alloc_chain_link(r->pool);
        if (hcl == NULL) {
            return NGX_ERROR;
        }

        hcl->buf = b;


        /* "SSSS-EEEE/TTTT" CRLF CRLF */

        b = ngx_calloc_buf(r->pool);
        if (b == NULL) {
            return NGX_ERROR;
        }

        b->temporary = 1;
        b->pos = range[i].content_range.data;
        b->last = range[i].content_range.data + range[i].content_range.len;

        rcl = ngx_alloc_chain_link(r->pool);
        if (rcl == NULL) {
            return NGX_ERROR;
        }

        rcl->buf = b;


        /* the range data */

        b = ngx_calloc_buf(r->pool);
        if (b == NULL) {
            return NGX_ERROR;
        }

        b->in_file = buf->in_file;
        b->temporary = buf->temporary;
        b->memory = buf->memory;
        b->mmap = buf->mmap;
        b->file = buf->file;

        if (buf->in_file) {
            b->file_pos = buf->file_pos + range[i].start;
            b->file_last = buf->file_pos + range[i].end;
        }

        if (ngx_buf_in_memory(buf)) {
            b->pos = buf->pos + (size_t) range[i].start;
            b->last = buf->pos + (size_t) range[i].end;
        }

        dcl = ngx_alloc_chain_link(r->pool);
        if (dcl == NULL) {
            return NGX_ERROR;
        }

        dcl->buf = b;

        *ll = hcl;
        hcl->next = rcl;
        rcl->next = dcl;
        ll = &dcl->next;
    }

    /* the last boundary CRLF "--0123456789--" CRLF  */

    b = ngx_calloc_buf(r->pool);
    if (b == NULL) {
        return NGX_ERROR;
    }

    b->temporary = 1;
    b->last_buf = 1;

    b->pos = ngx_pnalloc(r->pool, sizeof(CRLF "--") - 1 + NGX_ATOMIC_T_LEN
                                  + sizeof("--" CRLF) - 1);
    if (b->pos == NULL) {
        return NGX_ERROR;
    }

    b->last = ngx_cpymem(b->pos, ctx->boundary_header.data,
                         sizeof(CRLF "--") - 1 + NGX_ATOMIC_T_LEN);
    *b->last++ = '-'; *b->last++ = '-';
    *b->last++ = CR; *b->last++ = LF;

    hcl = ngx_alloc_chain_link(r->pool);
    if (hcl == NULL) {
        return NGX_ERROR;
    }

    hcl->buf = b;
    hcl->next = NULL;

    *ll = hcl;

    return ngx_http_next_body_filter(r, out);
}


static ngx_int_t
ngx_http_range_header_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_range_header_filter;

    return NGX_OK;
}


static ngx_int_t
ngx_http_range_body_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_range_body_filter;

    return NGX_OK;
}
GX_CONF_TAKE1,
      ngx_http_proxy_ssl_password_file,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

#endif

      ngx_null_command
};


static ngx_http_module_t  ngx_http_proxy_module_ctx = {
    ngx_http_proxy_add_variables,          /* preconfiguration */
    NULL,                                  /* postconfiguration */

    ngx_http_proxy_create_main_conf,       /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_proxy_create_loc_conf,        /* create location configuration */
    ngx_http_proxy_merge_loc_conf          /* merge location configuration */
};


ngx_module_t  ngx_http_proxy_module = {
    NGX_MODULE_V1,
    &ngx_http_proxy_module_ctx,            /* module context */
    ngx_http_proxy_commands,               /* module directives */
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


static char  ngx_http_proxy_version[] = " HTTP/1.0" CRLF;
static char  ngx_http_proxy_version_11[] = " HTTP/1.1" CRLF;


static ngx_keyval_t  ngx_http_proxy_headers[] = {
    { ngx_string("Host"), ngx_string("$proxy_host") },
    { ngx_string("Connection"), ngx_string("close") },
    { ngx_string("Content-Length"), ngx_string("$proxy_internal_body_length") },
    { ngx_string("Transfer-Encoding"), ngx_string("$proxy_internal_chunked") },
    { ngx_string("TE"), ngx_string("") },
    { ngx_string("Keep-Alive"), ngx_string("") },
    { ngx_string("Expect"), ngx_string("") },
    { ngx_string("Upgrade"), ngx_string("") },
    { ngx_null_string, ngx_null_string }
};


static ngx_str_t  ngx_http_proxy_hide_headers[] = {
    ngx_string("Date"),
    ngx_string("Server"),
    ngx_string("X-Pad"),
    ngx_string("X-Accel-Expires"),
    ngx_string("X-Accel-Redirect"),
    ngx_string("X-Accel-Limit-Rate"),
    ngx_string("X-Accel-Buffering"),
    ngx_string("X-Accel-Charset"),
    ngx_null_string
};


#if (NGX_HTTP_CACHE)

static ngx_keyval_t  ngx_http_proxy_cache_headers[] = {
    { ngx_string("Host"), ngx_string("$proxy_host") },
    { ngx_string("Connection"), ngx_string("close") },
    { ngx_string("Content-Length"), ngx_string("$proxy_internal_body_length") },
    { ngx_string("Transfer-Encoding"), ngx_string("$proxy_internal_chunked") },
    { ngx_string("TE"), ngx_string("") },
    { ngx_string("Keep-Alive"), ngx_string("") },
    { ngx_string("Expect"), ngx_string("") },
    { ngx_string("Upgrade"), ngx_string("") },
    { ngx_string("If-Modified-Since"),
      ngx_string("$upstream_cache_last_modified") },
    { ngx_string("If-Unmodified-Since"), ngx_string("") },
    { ngx_string("If-None-Match"), ngx_string("$upstream_cache_etag") },
    { ngx_string("If-Match"), ngx_string("") },
    { ngx_string("Range"), ngx_string("") },
    { ngx_string("If-Range"), ngx_string("") },
    { ngx_null_string, ngx_null_string }
};

#endif


static ngx_http_variable_t  ngx_http_proxy_vars[] = {

    { ngx_string("proxy_host"), NULL, ngx_http_proxy_host_variable, 0,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE|NGX_HTTP_VAR_NOHASH, 0 },

    { ngx_string("proxy_port"), NULL, ngx_http_proxy_port_variable, 0,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE|NGX_HTTP_VAR_NOHASH, 0 },

    { ngx_string("proxy_add_x_forwarded_for"), NULL,
      ngx_http_proxy_add_x_forwarded_for_variable, 0, NGX_HTTP_VAR_NOHASH, 0 },

#if 0
    { ngx_string("proxy_add_via"), NULL, NULL, 0, NGX_HTTP_VAR_NOHASH, 0 },
#endif

    { ngx_string("proxy_internal_body_length"), NULL,
      ngx_http_proxy_internal_body_length_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE|NGX_HTTP_VAR_NOHASH, 0 },

    { ngx_string("proxy_internal_chunked"), NULL,
      ngx_http_proxy_internal_chunked_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE|NGX_HTTP_VAR_NOHASH, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_path_init_t  ngx_http_proxy_temp_path = {
    ngx_string(NGX_HTTP_PROXY_TEMP_PATH), { 1, 2, 0 }
};


static ngx_int_t
ngx_http_proxy_handler(ngx_http_request_t *r)
{
    ngx_int_t                    rc;
    ngx_http_upstream_t         *u;
    ngx_http_proxy_ctx_t        *ctx;
    ngx_http_proxy_loc_conf_t   *plcf;
#if (NGX_HTTP_CACHE)
    ngx_http_proxy_main_conf_t  *pmcf;
#endif

    if (ngx_http_upstream_create(r) != NGX_OK) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_proxy_ctx_t));
    if (ctx == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_http_set_ctx(r, ctx, ngx_http_proxy_module);

    plcf = ngx_http_get_module_loc_conf(r, ngx_http_proxy_module);

    u = r->upstream;

    if (plcf->proxy_lengths == NULL) {
        ctx->vars = plcf->vars;
        u->schema = plcf->vars.schema;
#if (NGX_HTTP_SSL)
        u->ssl = (plcf->upstream.ssl != NULL);
#endif

    } else {
        if (ngx_http_proxy_eval(r, ctx, plcf) != NGX_OK) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
    }

    u->output.tag = (ngx_buf_tag_t) &ngx_http_proxy_module;

    u->conf = &plcf->upstream;

#if (NGX_HTTP_CACHE)
    pmcf = ngx_http_get_module_main_conf(r, ngx_http_proxy_module);

    u->caches = &pmcf->caches;
    u->create_key = ngx_http_proxy_create_key;
#endif

    u->create_request = ngx_http_proxy_create_request;
    u->reinit_request = ngx_http_proxy_reinit_request;
    u->process_header = ngx_http_proxy_process_status_line;
    u->abort_request = ngx_http_proxy_abort_request;
    u->finalize_request = ngx_http_proxy_finalize_request;
    r->state = 0;

    if (plcf->redirects) {
        u->rewrite_redirect = ngx_http_proxy_rewrite_redirect;
    }

    if (plcf->cookie_domains || plcf->cookie_paths) {
        u->rewrite_cookie = ngx_http_proxy_rewrite_cookie;
    }

    u->buffering = plcf->upstream.buffering;

    u->pipe = ngx_pcalloc(r->pool, sizeof(ngx_event_pipe_t));
    if (u->pipe == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    u->pipe->input_filter = ngx_http_proxy_copy_filter;
    u->pipe->input_ctx = r;

    u->input_filter_init = ngx_http_proxy_input_filter_init;
    u->input_filter = ngx_http_proxy_non_buffered_copy_filter;
    u->input_filter_ctx = r;

    u->accel = 1;

    if (!plcf->upstream.request_buffering
        && plcf->body_values == NULL && plcf->upstream.pass_request_body
        && (!r->headers_in.chunked
            || plcf->http_version == NGX_HTTP_VERSION_11))
    {
        r->request_body_no_buffering = 1;
    }

    rc = ngx_http_read_client_request_body(r, ngx_http_upstream_init);

    if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
        return rc;
    }

    return NGX_DONE;
}


static ngx_int_t
ngx_http_proxy_eval(ngx_http_request_t *r, ngx_http_proxy_ctx_t *ctx,
    ngx_http_proxy_loc_conf_t *plcf)
{
    u_char               *p;
    size_t                add;
    u_short               port;
    ngx_str_t             proxy;
    ngx_url_t             url;
    ngx_http_upstream_t  *u;

    if (ngx_http_script_run(r, &proxy, plcf->proxy_lengths->elts, 0,
                            plcf->proxy_values->elts)
        == NULL)
    {
        return NGX_ERROR;
    }

    if (proxy.len > 7
        && ngx_strncasecmp(proxy.data, (u_char *) "http://", 7) == 0)
    {
        add = 7;
        port = 80;

#if (NGX_HTTP_SSL)

    } else if (proxy.len > 8
               && ngx_strncasecmp(proxy.data, (u_char *) "https://", 8) == 0)
    {
        add = 8;
        port = 443;
        r->upstream->ssl = 1;

#endif

    } else {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "invalid URL prefix in \"%V\"", &proxy);
        return NGX_ERROR;
    }

    u = r->upstream;

    u->schema.len = add;
    u->schema.data = proxy.data;

    ngx_memzero(&url, sizeof(ngx_url_t));

    url.url.len = proxy.len - add;
    url.url.data = proxy.data + add;
    url.default_port = port;
    url.uri_part = 1;
    url.no_resolve = 1;

    if (ngx_parse_url(r->pool, &url) != NGX_OK) {
        if (url.err) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "%s in upstream \"%V\"", url.err, &url.url);
        }

        return NGX_ERROR;
    }

    if (url.uri.len) {
        if (url.uri.data[0] == '?') {
            p = ngx_pnalloc(r->pool, url.uri.len + 1);
            if (p == NULL) {
                return NGX_ERROR;
            }

            *p++ = '/';
            ngx_memcpy(p, url.uri.data, url.uri.len);

            url.uri.len++;
            url.uri.data = p - 1;
        }
    }

    ctx->vars.key_start = u->schema;

    ngx_http_proxy_set_vars(&url, &ctx->vars);

    u->resolved = ngx_pcalloc(r->pool, sizeof(ngx_http_upstream_resolved_t));
    if (u->resolved == NULL) {
        return NGX_ERROR;
    }

    if (url.addrs) {
        u->resolved->sockaddr = url.addrs[0].sockaddr;
        u->resolved->socklen = url.addrs[0].socklen;
        u->resolved->name = url.addrs[0].name;
        u->resolved->naddrs = 1;
    }

    u->resolved->host = url.host;
    u->resolved->port = (in_port_t) (url.no_port ? port : url.port);
    u->resolved->no_port = url.no_port;

    return NGX_OK;
}


#if (NGX_HTTP_CACHE)

static ngx_int_t
ngx_http_proxy_create_key(ngx_http_request_t *r)
{
    size_t                      len, loc_len;
    u_char                     *p;
    uintptr_t                   escape;
    ngx_str_t                  *key;
    ngx_http_upstream_t        *u;
    ngx_http_proxy_ctx_t       *ctx;
    ngx_http_proxy_loc_conf_t  *plcf;

    u = r->upstream;

    plcf = ngx_http_get_module_loc_conf(r, ngx_http_proxy_module);

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    key = ngx_array_push(&r->cache->keys);
    if (key == NULL) {
        return NGX_ERROR;
    }

    if (plcf->cache_key.value.data) {

        if (ngx_http_complex_value(r, &plcf->cache_key, key) != NGX_OK) {
            return NGX_ERROR;
        }

        return NGX_OK;
    }

    *key = ctx->vars.key_start;

    key = ngx_array_push(&r->cache->keys);
    if (key == NULL) {
        return NGX_ERROR;
    }

    if (plcf->proxy_lengths && ctx->vars.uri.len) {

        *key = ctx->vars.uri;
        u->uri = ctx->vars.uri;

        return NGX_OK;

    } else if (ctx->vars.uri.len == 0 && r->valid_unparsed_uri && r == r->main)
    {
        *key = r->unparsed_uri;
        u->uri = r->unparsed_uri;

        return NGX_OK;
    }

    loc_len = (r->valid_location && ctx->vars.uri.len) ? plcf->location.len : 0;

    if (r->quoted_uri || r->internal) {
        escape = 2 * ngx_escape_uri(NULL, r->uri.data + loc_len,
                                    r->uri.len - loc_len, NGX_ESCAPE_URI);
    } else {
        escape = 0;
    }

    len = ctx->vars.uri.len + r->uri.len - loc_len + escape
          + sizeof("?") - 1 + r->args.len;

    p = ngx_pnalloc(r->pool, len);
    if (p == NULL) {
        return NGX_ERROR;
    }

    key->data = p;

    if (r->valid_location) {
        p = ngx_copy(p, ctx->vars.uri.data, ctx->vars.uri.len);
    }

    if (escape) {
        ngx_escape_uri(p, r->uri.data + loc_len,
                       r->uri.len - loc_len, NGX_ESCAPE_URI);
        p += r->uri.len - loc_len + escape;

    } else {
        p = ngx_copy(p, r->uri.data + loc_len, r->uri.len - loc_len);
    }

    if (r->args.len > 0) {
        *p++ = '?';
        p = ngx_copy(p, r->args.data, r->args.len);
    }

    key->len = p - key->data;
    u->uri = *key;

    return NGX_OK;
}

#endif


static ngx_int_t
ngx_http_proxy_create_request(ngx_http_request_t *r)
{
    size_t                        len, uri_len, loc_len, body_len;
    uintptr_t                     escape;
    ngx_buf_t                    *b;
    ngx_str_t                     method;
    ngx_uint_t                    i, unparsed_uri;
    ngx_chain_t                  *cl, *body;
    ngx_list_part_t              *part;
    ngx_table_elt_t              *header;
    ngx_http_upstream_t          *u;
    ngx_http_proxy_ctx_t         *ctx;
    ngx_http_script_code_pt       code;
    ngx_http_proxy_headers_t     *headers;
    ngx_http_script_engine_t      e, le;
    ngx_http_proxy_loc_conf_t    *plcf;
    ngx_http_script_len_code_pt   lcode;

    u = r->upstream;

    plcf = ngx_http_get_module_loc_conf(r, ngx_http_proxy_module);

#if (NGX_HTTP_CACHE)
    headers = u->cacheable ? &plcf->headers_cache : &plcf->headers;
#else
    headers = &plcf->headers;
#endif

    if (u->method.len) {
        /* HEAD was changed to GET to cache response */
        method = u->method;

    } else if (plcf->method) {
        if (ngx_http_complex_value(r, plcf->method, &method) != NGX_OK) {
            return NGX_ERROR;
        }

    } else {
        method = r->method_name;
    }

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (method.len == 4
        && ngx_strncasecmp(method.data, (u_char *) "HEAD", 4) == 0)
    {
        ctx->head = 1;
    }

    len = method.len + 1 + sizeof(ngx_http_proxy_version) - 1
          + sizeof(CRLF) - 1;

    escape = 0;
    loc_len = 0;
    unparsed_uri = 0;

    if (plcf->proxy_lengths && ctx->vars.uri.len) {
        uri_len = ctx->vars.uri.len;

    } else if (ctx->vars.uri.len == 0 && r->valid_unparsed_uri && r == r->main)
    {
        unparsed_uri = 1;
        uri_len = r->unparsed_uri.len;

    } else {
        loc_len = (r->valid_location && ctx->vars.uri.len) ?
                      plcf->location.len : 0;

        if (r->quoted_uri || r->space_in_uri || r->internal) {
            escape = 2 * ngx_escape_uri(NULL, r->uri.data + loc_len,
                                        r->uri.len - loc_len, NGX_ESCAPE_URI);
        }

        uri_len = ctx->vars.uri.len + r->uri.len - loc_len + escape
                  + sizeof("?") - 1 + r->args.len;
    }

    if (uri_len == 0) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "zero length URI to proxy");
        return NGX_ERROR;
    }

    len += uri_len;

    ngx_memzero(&le, sizeof(ngx_http_script_engine_t));

    ngx_http_script_flush_no_cacheable_variables(r, plcf->body_flushes);
    ngx_http_script_flush_no_cacheable_variables(r, headers->flushes);

    if (plcf->body_lengths) {
        le.ip = plcf->body_lengths->elts;
        le.request = r;
        le.flushed = 1;
        body_len = 0;

        while (*(uintptr_t *) le.ip) {
            lcode = *(ngx_http_script_len_code_pt *) le.ip;
            body_len += lcode(&le);
        }

        ctx->internal_body_length = body_len;
        len += body_len;

    } else if (r->headers_in.chunked && r->reading_body) {
        ctx->internal_body_length = -1;
        ctx->internal_chunked = 1;

    } else {
        ctx->internal_body_length = r->headers_in.content_length_n;
    }

    le.ip = headers->lengths->elts;
    le.request = r;
    le.flushed = 1;

    while (*(uintptr_t *) le.ip) {
        while (*(uintptr_t *) le.ip) {
            lcode = *(ngx_http_script_len_code_pt *) le.ip;
            len += lcode(&le);
        }
        le.ip += sizeof(uintptr_t);
    }


    if (plcf->upstream.pass_request_headers) {
        part = &r->headers_in.headers.part;
        header = part->elts;

        for (i = 0; /* void */; i++) {

            if (i >= part->nelts) {
                if (part->next == NULL) {
                    break;
                }

                part = part->next;
                header = part->elts;
                i = 0;
            }

            if (ngx_hash_find(&headers->hash, header[i].hash,
                              header[i].lowcase_key, header[i].key.len))
            {
                continue;
            }

            len += header[i].key.len + sizeof(": ") - 1
                + header[i].value.len + sizeof(CRLF) - 1;
        }
    }


    b = ngx_create_temp_buf(r->pool, len);
    if (b == NULL) {
        return NGX_ERROR;
    }

    cl = ngx_alloc_chain_link(r->pool);
    if (cl == NULL) {
        return NGX_ERROR;
    }

    cl->buf = b;


    /* the request line */

    b->last = ngx_copy(b->last, method.data, method.len);
    *b->last++ = ' ';

    u->uri.data = b->last;

    if (plcf->proxy_lengths && ctx->vars.uri.len) {
        b->last = ngx_copy(b->last, ctx->vars.uri.data, ctx->vars.uri.len);

    } else if (unparsed_uri) {
        b->last = ngx_copy(b->last, r->unparsed_uri.data, r->unparsed_uri.len);

    } else {
        if (r->valid_location) {
            b->last = ngx_copy(b->last, ctx->vars.uri.data, ctx->vars.uri.len);
        }

        if (escape) {
            ngx_escape_uri(b->last, r->uri.data + loc_len,
                           r->uri.len - loc_len, NGX_ESCAPE_URI);
            b->last += r->uri.len - loc_len + escape;

        } else {
            b->last = ngx_copy(b->last, r->uri.data + loc_len,
                               r->uri.len - loc_len);
        }

        if (r->args.len > 0) {
            *b->last++ = '?';
            b->last = ngx_copy(b->last, r->args.data, r->args.len);
        }
    }

    u->uri.len = b->last - u->uri.data;

    if (plcf->http_version == NGX_HTTP_VERSION_11) {
        b->last = ngx_cpymem(b->last, ngx_http_proxy_version_11,
                             sizeof(ngx_http_proxy_version_11) - 1);

    } else {
        b->last = ngx_cpymem(b->last, ngx_http_proxy_version,
                             sizeof(ngx_http_proxy_version) - 1);
    }

    ngx_memzero(&e, sizeof(ngx_http_script_engine_t));

    e.ip = headers->values->elts;
    e.pos = b->last;
    e.request = r;
    e.flushed = 1;

    le.ip = headers->lengths->elts;

    while (*(uintptr_t *) le.ip) {
        lcode = *(ngx_http_script_len_code_pt *) le.ip;

        /* skip the header line name length */
        (void) lcode(&le);

        if (*(ngx_http_script_len_code_pt *) le.ip) {

            for (len = 0; *(uintptr_t *) le.ip; len += lcode(&le)) {
                lcode = *(ngx_http_script_len_code_pt *) le.ip;
            }

            e.skip = (len == sizeof(CRLF) - 1) ? 1 : 0;

        } else {
            e.skip = 0;
        }

        le.ip += sizeof(uintptr_t);

        while (*(uintptr_t *) e.ip) {
            code = *(ngx_http_script_code_pt *) e.ip;
            code((ngx_http_script_engine_t *) &e);
        }
        e.ip += sizeof(uintptr_t);
    }

    b->last = e.pos;


    if (plcf->upstream.pass_request_headers) {
        part = &r->headers_in.headers.part;
        header = part->elts;

        for (i = 0; /* void */; i++) {

            if (i >= part->nelts) {
                if (part->next == NULL) {
                    break;
                }

                part = part->next;
                header = part->elts;
                i = 0;
            }

            if (ngx_hash_find(&headers->hash, header[i].hash,
                              header[i].lowcase_key, header[i].key.len))
            {
                continue;
            }

            b->last = ngx_copy(b->last, header[i].key.data, header[i].key.len);

            *b->last++ = ':'; *b->last++ = ' ';

            b->last = ngx_copy(b->last, header[i].value.data,
                               header[i].value.len);

            *b->last++ = CR; *b->last++ = LF;

            ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "http proxy header: \"%V: %V\"",
                           &header[i].key, &header[i].value);
        }
    }


    /* add "\r\n" at the header end */
    *b->last++ = CR; *b->last++ = LF;

    if (plcf->body_values) {
        e.ip = plcf->body_values->elts;
        e.pos = b->last;
        e.skip = 0;

        while (*(uintptr_t *) e.ip) {
            code = *(ngx_http_script_code_pt *) e.ip;
            code((ngx_http_script_engine_t *) &e);
        }

        b->last = e.pos;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http proxy header:%N\"%*s\"",
                   (size_t) (b->last - b->pos), b->pos);

    if (r->request_body_no_buffering) {

        u->request_bufs = cl;

        if (ctx->internal_chunked) {
            u->output.output_filter = ngx_http_proxy_body_output_filter;
            u->output.filter_ctx = r;
        }

    } else if (plcf->body_values == NULL && plcf->upstream.pass_request_body) {

        body = u->request_bufs;
        u->request_bufs = cl;

        while (body) {
            b = ngx_alloc_buf(r->pool);
            if (b == NULL) {
                return NGX_ERROR;
            }

            ngx_memcpy(b, body->buf, sizeof(ngx_buf_t));

            cl->next = ngx_alloc_chain_link(r->pool);
            if (cl->next == NULL) {
                return NGX_ERROR;
            }

            cl = cl->next;
            cl->buf = b;

            body = body->next;
        }

    } else {
        u->request_bufs = cl;
    }

    b->flush = 1;
    cl->next = NULL;

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_reinit_request(ngx_http_request_t *r)
{
    ngx_http_proxy_ctx_t  *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (ctx == NULL) {
        return NGX_OK;
    }

    ctx->status.code = 0;
    ctx->status.count = 0;
    ctx->status.start = NULL;
    ctx->status.end = NULL;
    ctx->chunked.state = 0;

    r->upstream->process_header = ngx_http_proxy_process_status_line;
    r->upstream->pipe->input_filter = ngx_http_proxy_copy_filter;
    r->upstream->input_filter = ngx_http_proxy_non_buffered_copy_filter;
    r->state = 0;

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_body_output_filter(void *data, ngx_chain_t *in)
{
    ngx_http_request_t  *r = data;

    off_t                  size;
    u_char                *chunk;
    ngx_int_t              rc;
    ngx_buf_t             *b;
    ngx_chain_t           *out, *cl, *tl, **ll, **fl;
    ngx_http_proxy_ctx_t  *ctx;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "proxy output filter");

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (in == NULL) {
        out = in;
        goto out;
    }

    out = NULL;
    ll = &out;

    if (!ctx->header_sent) {
        /* first buffer contains headers, pass it unmodified */

        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "proxy output header");

        ctx->header_sent = 1;

        tl = ngx_alloc_chain_link(r->pool);
        if (tl == NULL) {
            return NGX_ERROR;
        }

        tl->buf = in->buf;
        *ll = tl;
        ll = &tl->next;

        in = in->next;

        if (in == NULL) {
            tl->next = NULL;
            goto out;
        }
    }

    size = 0;
    cl = in;
    fl = ll;

    for ( ;; ) {
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "proxy output chunk: %O", ngx_buf_size(cl->buf));

        size += ngx_buf_size(cl->buf);

        if (cl->buf->flush
            || cl->buf->sync
            || ngx_buf_in_memory(cl->buf)
            || cl->buf->in_file)
        {
            tl = ngx_alloc_chain_link(r->pool);
            if (tl == NULL) {
                return NGX_ERROR;
            }

            tl->buf = cl->buf;
            *ll = tl;
            ll = &tl->next;
        }

        if (cl->next == NULL) {
            break;
        }

        cl = cl->next;
    }

    if (size) {
        tl = ngx_chain_get_free_buf(r->pool, &ctx->free);
        if (tl == NULL) {
            return NGX_ERROR;
        }

        b = tl->buf;
        chunk = b->start;

        if (chunk == NULL) {
            /* the "0000000000000000" is 64-bit hexadecimal string */

            chunk = ngx_palloc(r->pool, sizeof("0000000000000000" CRLF) - 1);
            if (chunk == NULL) {
                return NGX_ERROR;
            }

            b->start = chunk;
            b->end = chunk + sizeof("0000000000000000" CRLF) - 1;
        }

        b->tag = (ngx_buf_tag_t) &ngx_http_proxy_body_output_filter;
        b->memory = 0;
        b->temporary = 1;
        b->pos = chunk;
        b->last = ngx_sprintf(chunk, "%xO" CRLF, size);

        tl->next = *fl;
        *fl = tl;
    }

    if (cl->buf->last_buf) {
        tl = ngx_chain_get_free_buf(r->pool, &ctx->free);
        if (tl == NULL) {
            return NGX_ERROR;
        }

        b = tl->buf;

        b->tag = (ngx_buf_tag_t) &ngx_http_proxy_body_output_filter;
        b->temporary = 0;
        b->memory = 1;
        b->last_buf = 1;
        b->pos = (u_char *) CRLF "0" CRLF CRLF;
        b->last = b->pos + 7;

        cl->buf->last_buf = 0;

        *ll = tl;

        if (size == 0) {
            b->pos += 2;
        }

    } else if (size > 0) {
        tl = ngx_chain_get_free_buf(r->pool, &ctx->free);
        if (tl == NULL) {
            return NGX_ERROR;
        }

        b = tl->buf;

        b->tag = (ngx_buf_tag_t) &ngx_http_proxy_body_output_filter;
        b->temporary = 0;
        b->memory = 1;
        b->pos = (u_char *) CRLF;
        b->last = b->pos + 2;

        *ll = tl;

    } else {
        *ll = NULL;
    }

out:

    rc = ngx_chain_writer(&r->upstream->writer, out);

    ngx_chain_update_chains(r->pool, &ctx->free, &ctx->busy, &out,
                            (ngx_buf_tag_t) &ngx_http_proxy_body_output_filter);

    return rc;
}


static ngx_int_t
ngx_http_proxy_process_status_line(ngx_http_request_t *r)
{
    size_t                 len;
    ngx_int_t              rc;
    ngx_http_upstream_t   *u;
    ngx_http_proxy_ctx_t  *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (ctx == NULL) {
        return NGX_ERROR;
    }

    u = r->upstream;

    rc = ngx_http_parse_status_line(r, &u->buffer, &ctx->status);

    if (rc == NGX_AGAIN) {
        return rc;
    }

    if (rc == NGX_ERROR) {

#if (NGX_HTTP_CACHE)

        if (r->cache) {
            r->http_version = NGX_HTTP_VERSION_9;
            return NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "upstream sent no valid HTTP/1.0 header");

#if 0
        if (u->accel) {
            return NGX_HTTP_UPSTREAM_INVALID_HEADER;
        }
#endif

        r->http_version = NGX_HTTP_VERSION_9;
        u->state->status = NGX_HTTP_OK;
        u->headers_in.connection_close = 1;

        return NGX_OK;
    }

    if (u->state && u->state->status == 0) {
        u->state->status = ctx->status.code;
    }

    u->headers_in.status_n = ctx->status.code;

    len = ctx->status.end - ctx->status.start;
    u->headers_in.status_line.len = len;

    u->headers_in.status_line.data = ngx_pnalloc(r->pool, len);
    if (u->headers_in.status_line.data == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(u->headers_in.status_line.data, ctx->status.start, len);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http proxy status %ui \"%V\"",
                   u->headers_in.status_n, &u->headers_in.status_line);

    if (ctx->status.http_version < NGX_HTTP_VERSION_11) {
        u->headers_in.connection_close = 1;
    }

    u->process_header = ngx_http_proxy_process_header;

    return ngx_http_proxy_process_header(r);
}


static ngx_int_t
ngx_http_proxy_process_header(ngx_http_request_t *r)
{
    ngx_int_t                       rc;
    ngx_table_elt_t                *h;
    ngx_http_upstream_t            *u;
    ngx_http_proxy_ctx_t           *ctx;
    ngx_http_upstream_header_t     *hh;
    ngx_http_upstream_main_conf_t  *umcf;

    umcf = ngx_http_get_module_main_conf(r, ngx_http_upstream_module);

    for ( ;; ) {

        rc = ngx_http_parse_header_line(r, &r->upstream->buffer, 1);

        if (rc == NGX_OK) {

            /* a header line has been parsed successfully */

            h = ngx_list_push(&r->upstream->headers_in.headers);
            if (h == NULL) {
                return NGX_ERROR;
            }

            h->hash = r->header_hash;

            h->key.len = r->header_name_end - r->header_name_start;
            h->value.len = r->header_end - r->header_start;

            h->key.data = ngx_pnalloc(r->pool,
                               h->key.len + 1 + h->value.len + 1 + h->key.len);
            if (h->key.data == NULL) {
                h->hash = 0;
                return NGX_ERROR;
            }

            h->value.data = h->key.data + h->key.len + 1;
            h->lowcase_key = h->key.data + h->key.len + 1 + h->value.len + 1;

            ngx_memcpy(h->key.data, r->header_name_start, h->key.len);
            h->key.data[h->key.len] = '\0';
            ngx_memcpy(h->value.data, r->header_start, h->value.len);
            h->value.data[h->value.len] = '\0';

            if (h->key.len == r->lowcase_index) {
                ngx_memcpy(h->lowcase_key, r->lowcase_header, h->key.len);

            } else {
                ngx_strlow(h->lowcase_key, h->key.data, h->key.len);
            }

            hh = ngx_hash_find(&umcf->headers_in_hash, h->hash,
                               h->lowcase_key, h->key.len);

            if (hh && hh->handler(r, h, hh->offset) != NGX_OK) {
                return NGX_ERROR;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "http proxy header: \"%V: %V\"",
                           &h->key, &h->value);

            continue;
        }

        if (rc == NGX_HTTP_PARSE_HEADER_DONE) {

            /* a whole header has been parsed successfully */

            ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "http proxy header done");

            /*
             * if no "Server" and "Date" in header line,
             * then add the special empty headers
             */

            if (r->upstream->headers_in.server == NULL) {
                h = ngx_list_push(&r->upstream->headers_in.headers);
                if (h == NULL) {
                    return NGX_ERROR;
                }

                h->hash = ngx_hash(ngx_hash(ngx_hash(ngx_hash(
                                    ngx_hash('s', 'e'), 'r'), 'v'), 'e'), 'r');

                ngx_str_set(&h->key, "Server");
                ngx_str_null(&h->value);
                h->lowcase_key = (u_char *) "server";
            }

            if (r->upstream->headers_in.date == NULL) {
                h = ngx_list_push(&r->upstream->headers_in.headers);
                if (h == NULL) {
                    return NGX_ERROR;
                }

                h->hash = ngx_hash(ngx_hash(ngx_hash('d', 'a'), 't'), 'e');

                ngx_str_set(&h->key, "Date");
                ngx_str_null(&h->value);
                h->lowcase_key = (u_char *) "date";
            }

            /* clear content length if response is chunked */

            u = r->upstream;

            if (u->headers_in.chunked) {
                u->headers_in.content_length_n = -1;
            }

            /*
             * set u->keepalive if response has no body; this allows to keep
             * connections alive in case of r->header_only or X-Accel-Redirect
             */

            ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

            if (u->headers_in.status_n == NGX_HTTP_NO_CONTENT
                || u->headers_in.status_n == NGX_HTTP_NOT_MODIFIED
                || ctx->head
                || (!u->headers_in.chunked
                    && u->headers_in.content_length_n == 0))
            {
                u->keepalive = !u->headers_in.connection_close;
            }

            if (u->headers_in.status_n == NGX_HTTP_SWITCHING_PROTOCOLS) {
                u->keepalive = 0;

                if (r->headers_in.upgrade) {
                    u->upgrade = 1;
                }
            }

            return NGX_OK;
        }

        if (rc == NGX_AGAIN) {
            return NGX_AGAIN;
        }

        /* there was error while a header line parsing */

        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "upstream sent invalid header");

        return NGX_HTTP_UPSTREAM_INVALID_HEADER;
    }
}


static ngx_int_t
ngx_http_proxy_input_filter_init(void *data)
{
    ngx_http_request_t    *r = data;
    ngx_http_upstream_t   *u;
    ngx_http_proxy_ctx_t  *ctx;

    u = r->upstream;
    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (ctx == NULL) {
        return NGX_ERROR;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http proxy filter init s:%ui h:%d c:%d l:%O",
                   u->headers_in.status_n, ctx->head, u->headers_in.chunked,
                   u->headers_in.content_length_n);

    /* as per RFC2616, 4.4 Message Length */

    if (u->headers_in.status_n == NGX_HTTP_NO_CONTENT
        || u->headers_in.status_n == NGX_HTTP_NOT_MODIFIED
        || ctx->head)
    {
        /* 1xx, 204, and 304 and replies to HEAD requests */
        /* no 1xx since we don't send Expect and Upgrade */

        u->pipe->length = 0;
        u->length = 0;
        u->keepalive = !u->headers_in.connection_close;

    } else if (u->headers_in.chunked) {
        /* chunked */

        u->pipe->input_filter = ngx_http_proxy_chunked_filter;
        u->pipe->length = 3; /* "0" LF LF */

        u->input_filter = ngx_http_proxy_non_buffered_chunked_filter;
        u->length = 1;

    } else if (u->headers_in.content_length_n == 0) {
        /* empty body: special case as filter won't be called */

        u->pipe->length = 0;
        u->length = 0;
        u->keepalive = !u->headers_in.connection_close;

    } else {
        /* content length or connection close */

        u->pipe->length = u->headers_in.content_length_n;
        u->length = u->headers_in.content_length_n;
    }

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_copy_filter(ngx_event_pipe_t *p, ngx_buf_t *buf)
{
    ngx_buf_t           *b;
    ngx_chain_t         *cl;
    ngx_http_request_t  *r;

    if (buf->pos == buf->last) {
        return NGX_OK;
    }

    cl = ngx_chain_get_free_buf(p->pool, &p->free);
    if (cl == NULL) {
        return NGX_ERROR;
    }

    b = cl->buf;

    ngx_memcpy(b, buf, sizeof(ngx_buf_t));
    b->shadow = buf;
    b->tag = p->tag;
    b->last_shadow = 1;
    b->recycled = 1;
    buf->shadow = b;

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, p->log, 0, "input buf #%d", b->num);

    if (p->in) {
        *p->last_in = cl;
    } else {
        p->in = cl;
    }
    p->last_in = &cl->next;

    if (p->length == -1) {
        return NGX_OK;
    }

    p->length -= b->last - b->pos;

    if (p->length == 0) {
        r = p->input_ctx;
        p->upstream_done = 1;
        r->upstream->keepalive = !r->upstream->headers_in.connection_close;

    } else if (p->length < 0) {
        r = p->input_ctx;
        p->upstream_done = 1;

        ngx_log_error(NGX_LOG_WARN, r->connection->log, 0,
                      "upstream sent more data than specified in "
                      "\"Content-Length\" header");
    }

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_chunked_filter(ngx_event_pipe_t *p, ngx_buf_t *buf)
{
    ngx_int_t              rc;
    ngx_buf_t             *b, **prev;
    ngx_chain_t           *cl;
    ngx_http_request_t    *r;
    ngx_http_proxy_ctx_t  *ctx;

    if (buf->pos == buf->last) {
        return NGX_OK;
    }

    r = p->input_ctx;
    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (ctx == NULL) {
        return NGX_ERROR;
    }

    b = NULL;
    prev = &buf->shadow;

    for ( ;; ) {

        rc = ngx_http_parse_chunked(r, buf, &ctx->chunked);

        if (rc == NGX_OK) {

            /* a chunk has been parsed successfully */

            cl = ngx_chain_get_free_buf(p->pool, &p->free);
            if (cl == NULL) {
                return NGX_ERROR;
            }

            b = cl->buf;

            ngx_memzero(b, sizeof(ngx_buf_t));

            b->pos = buf->pos;
            b->start = buf->start;
            b->end = buf->end;
            b->tag = p->tag;
            b->temporary = 1;
            b->recycled = 1;

            *prev = b;
            prev = &b->shadow;

            if (p->in) {
                *p->last_in = cl;
            } else {
                p->in = cl;
            }
            p->last_in = &cl->next;

            /* STUB */ b->num = buf->num;

            ngx_log_debug2(NGX_LOG_DEBUG_EVENT, p->log, 0,
                           "input buf #%d %p", b->num, b->pos);

            if (buf->last - buf->pos >= ctx->chunked.size) {

                buf->pos += (size_t) ctx->chunked.size;
                b->last = buf->pos;
                ctx->chunked.size = 0;

                continue;
            }

            ctx->chunked.size -= buf->last - buf->pos;
            buf->pos = buf->last;
            b->last = buf->last;

            continue;
        }

        if (rc == NGX_DONE) {

            /* a whole response has been parsed successfully */

            p->upstream_done = 1;
            r->upstream->keepalive = !r->upstream->headers_in.connection_close;

            break;
        }

        if (rc == NGX_AGAIN) {

            /* set p->length, minimal amount of data we want to see */

            p->length = ctx->chunked.length;

            break;
        }

        /* invalid response */

        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "upstream sent invalid chunked response");

        return NGX_ERROR;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http proxy chunked state %ui, length %O",
                   ctx->chunked.state, p->length);

    if (b) {
        b->shadow = buf;
        b->last_shadow = 1;

        ngx_log_debug2(NGX_LOG_DEBUG_EVENT, p->log, 0,
                       "input buf %p %z", b->pos, b->last - b->pos);

        return NGX_OK;
    }

    /* there is no data record in the buf, add it to free chain */

    if (ngx_event_pipe_add_free_buf(p, buf) != NGX_OK) {
        return NGX_ERROR;
    }

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_non_buffered_copy_filter(void *data, ssize_t bytes)
{
    ngx_http_request_t   *r = data;

    ngx_buf_t            *b;
    ngx_chain_t          *cl, **ll;
    ngx_http_upstream_t  *u;

    u = r->upstream;

    for (cl = u->out_bufs, ll = &u->out_bufs; cl; cl = cl->next) {
        ll = &cl->next;
    }

    cl = ngx_chain_get_free_buf(r->pool, &u->free_bufs);
    if (cl == NULL) {
        return NGX_ERROR;
    }

    *ll = cl;

    cl->buf->flush = 1;
    cl->buf->memory = 1;

    b = &u->buffer;

    cl->buf->pos = b->last;
    b->last += bytes;
    cl->buf->last = b->last;
    cl->buf->tag = u->output.tag;

    if (u->length == -1) {
        return NGX_OK;
    }

    u->length -= bytes;

    if (u->length == 0) {
        u->keepalive = !u->headers_in.connection_close;
    }

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_non_buffered_chunked_filter(void *data, ssize_t bytes)
{
    ngx_http_request_t   *r = data;

    ngx_int_t              rc;
    ngx_buf_t             *b, *buf;
    ngx_chain_t           *cl, **ll;
    ngx_http_upstream_t   *u;
    ngx_http_proxy_ctx_t  *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (ctx == NULL) {
        return NGX_ERROR;
    }

    u = r->upstream;
    buf = &u->buffer;

    buf->pos = buf->last;
    buf->last += bytes;

    for (cl = u->out_bufs, ll = &u->out_bufs; cl; cl = cl->next) {
        ll = &cl->next;
    }

    for ( ;; ) {

        rc = ngx_http_parse_chunked(r, buf, &ctx->chunked);

        if (rc == NGX_OK) {

            /* a chunk has been parsed successfully */

            cl = ngx_chain_get_free_buf(r->pool, &u->free_bufs);
            if (cl == NULL) {
                return NGX_ERROR;
            }

            *ll = cl;
            ll = &cl->next;

            b = cl->buf;

            b->flush = 1;
            b->memory = 1;

            b->pos = buf->pos;
            b->tag = u->output.tag;

            if (buf->last - buf->pos >= ctx->chunked.size) {
                buf->pos += (size_t) ctx->chunked.size;
                b->last = buf->pos;
                ctx->chunked.size = 0;

            } else {
                ctx->chunked.size -= buf->last - buf->pos;
                buf->pos = buf->last;
                b->last = buf->last;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "http proxy out buf %p %z",
                           b->pos, b->last - b->pos);

            continue;
        }

        if (rc == NGX_DONE) {

            /* a whole response has been parsed successfully */

            u->keepalive = !u->headers_in.connection_close;
            u->length = 0;

            break;
        }

        if (rc == NGX_AGAIN) {
            break;
        }

        /* invalid response */

        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "upstream sent invalid chunked response");

        return NGX_ERROR;
    }

    /* provide continuous buffer for subrequests in memory */

    if (r->subrequest_in_memory) {

        cl = u->out_bufs;

        if (cl) {
            buf->pos = cl->buf->pos;
        }

        buf->last = buf->pos;

        for (cl = u->out_bufs; cl; cl = cl->next) {
            ngx_log_debug3(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "http proxy in memory %p-%p %O",
                           cl->buf->pos, cl->buf->last, ngx_buf_size(cl->buf));

            if (buf->last == cl->buf->pos) {
                buf->last = cl->buf->last;
                continue;
            }

            buf->last = ngx_movemem(buf->last, cl->buf->pos,
                                    cl->buf->last - cl->buf->pos);

            cl->buf->pos = buf->last - (cl->buf->last - cl->buf->pos);
            cl->buf->last = buf->last;
        }
    }

    return NGX_OK;
}


static void
ngx_http_proxy_abort_request(ngx_http_request_t *r)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "abort http proxy request");

    return;
}


static void
ngx_http_proxy_finalize_request(ngx_http_request_t *r, ngx_int_t rc)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "finalize http proxy request");

    return;
}


static ngx_int_t
ngx_http_proxy_host_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_http_proxy_ctx_t  *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (ctx == NULL) {
        v->not_found = 1;
        return NGX_OK;
    }

    v->len = ctx->vars.host_header.len;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = ctx->vars.host_header.data;

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_port_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_http_proxy_ctx_t  *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (ctx == NULL) {
        v->not_found = 1;
        return NGX_OK;
    }

    v->len = ctx->vars.port.len;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = ctx->vars.port.data;

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_add_x_forwarded_for_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    size_t             len;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_table_elt_t  **h;

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;

    n = r->headers_in.x_forwarded_for.nelts;
    h = r->headers_in.x_forwarded_for.elts;

    len = 0;

    for (i = 0; i < n; i++) {
        len += h[i]->value.len + sizeof(", ") - 1;
    }

    if (len == 0) {
        v->len = r->connection->addr_text.len;
        v->data = r->connection->addr_text.data;
        return NGX_OK;
    }

    len += r->connection->addr_text.len;

    p = ngx_pnalloc(r->pool, len);
    if (p == NULL) {
        return NGX_ERROR;
    }

    v->len = len;
    v->data = p;

    for (i = 0; i < n; i++) {
        p = ngx_copy(p, h[i]->value.data, h[i]->value.len);
        *p++ = ','; *p++ = ' ';
    }

    ngx_memcpy(p, r->connection->addr_text.data, r->connection->addr_text.len);

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_internal_body_length_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_http_proxy_ctx_t  *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (ctx == NULL || ctx->internal_body_length < 0) {
        v->not_found = 1;
        return NGX_OK;
    }

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;

    v->data = ngx_pnalloc(r->pool, NGX_OFF_T_LEN);

    if (v->data == NULL) {
        return NGX_ERROR;
    }

    v->len = ngx_sprintf(v->data, "%O", ctx->internal_body_length) - v->data;

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_internal_chunked_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_http_proxy_ctx_t  *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_proxy_module);

    if (ctx == NULL || !ctx->internal_chunked) {
        v->not_found = 1;
        return NGX_OK;
    }

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;

    v->data = (u_char *) "chunked";
    v->len = sizeof("chunked") - 1;

    return NGX_OK;
}


static ngx_int_t
ngx_http_proxy_rewrite_redirect(ngx_http_request_t *r, ngx_table_elt_t *h,
    size_t prefix)
{
    size_t                      len;
    ngx_int_t                   rc;
    ngx_uint_t                  i;
    ngx_http_proxy_rewrite_t   *pr;
    ngx_http_proxy_loc_conf_t  *plcf;

    plcf = ngx_http_get_module_loc_conf(r, ngx_http_proxy_module);

    pr = plcf->redirects->elts;

    if (pr == NULL) {
        return NGX_DECLINED;
    }

    len = h->value.len - prefix;

    for (i = 0; i < plcf->redirects->nelts; i++) {
        rc = pr[i].handler(r, h, prefix, len, &pr[i]);

        if (rc != NGX_DECLINED) {
            return rc;
        }
    }

    return NGX_DECLINED;
}


static ngx_int_t
ngx_http_proxy_rewrite_cookie(ngx_http_request_t *r, ngx_table_elt_t *h)
{
    size_t                      prefix;
    u_char                     *p;
    ngx_int_t                   rc, rv;
    ngx_http_proxy_loc_conf_t  *plcf;

    p = (u_char *) ngx_strchr(h->value.data, ';');
    if (p == NULL) {
        return NGX_DECLINED;
    }

    prefix = p + 1 - h->value.data;

    rv = NGX_DECLINED;

    plcf = ngx_http_get_module_loc_conf(r, ngx_http_proxy_module);

    if (plcf->cookie_domains) {
        p = ngx_strcasestrn(h->value.data + prefix, "domain=", 7 - 1);

        if (p) {
            rc = ngx_http_proxy_rewrite_cookie_value(r, h, p + 7,
                                                     plcf->cookie_domains);
            if (rc == NGX_ERROR) {
                return NGX_ERROR;
            }

            if (rc != NGX_DECLINED) {
                rv = rc;
            }
        }
    }

    if (plcf->cookie_paths) {
        p = ngx_strcasestrn(h->value.data + prefix, "path=", 5 - 1);

        if (p) {
            rc = ngx_http_proxy_rewrite_cookie_value(r, h, p + 5,
                                                     plcf->cookie_paths);
            if (rc == NGX_ERROR) {
                return NGX_ERROR;
            }

            if (rc != NGX_DECLINED) {
                rv = rc;
            }
        }
    }

    return rv;
}


static ngx_int_t
ngx_http_proxy_rewrite_cookie_value(ngx_http_request_t *r, ngx_table_elt_t *h,
    u_char *value, ngx_array_t *rewrites)
{
    size_t                     len, prefix;
    u_char                    *p;
    ngx_int_t                  rc;
    ngx_uint_t                 i;
    ngx_http_proxy_rewrite_t  *pr;

    prefix = value - h->value.data;

    p = (u_char *) ngx_strchr(value, ';');

    len = p ? (size_t) (p - value) : (h->value.len - prefix);

    pr = rewrites->elts;

    for (i = 0; i < rewrites->nelts; i++) {
        rc = pr[i].handler(r, h, prefix, len, &pr[i]);

        if (rc != NGX_DECLINED) {
            return rc;
        }
    }

    return NGX_DECLINED;
}


static ngx_int_t
ngx_http_proxy_rewrite_complex_handler(ngx_http_request_t *r,
    ngx_table_elt_t *h, size_t prefix, size_t len, ngx_http_proxy_rewrite_t *pr)
{
    ngx_str_t  pattern, replacement;

    if (ngx_http_complex_value(r, &pr->pattern.complex, &pattern) != NGX_OK) {
        return NGX_ERROR;
    }

    if (pattern.len > len
        || ngx_rstrncmp(h->value.data + prefix, pattern.data,
                        pattern.len) != 0)
    {
        return NGX_DECLINED;
    }

    if (ngx_http_complex_value(r, &pr->replacement, &replacement) != NGX_OK) {
        return NGX_ERROR;
    }

    return ngx_http_proxy_rewrite(r, h, prefix, pattern.len, &replacement);
}


#if (NGX_PCRE)

static ngx_int_t
ngx_http_proxy_rewrite_regex_handler(ngx_http_request_t *r, ngx_table_elt_t *h,
    size_t prefix, size_t len, ngx_http_proxy_rewrite_t *pr)
{
    ngx_str_t  pattern, replacement;

    pattern.len = len;
    pattern.data = h->value.data + prefix;

    if (ngx_http_regex_exec(r, pr->pattern.regex, &pattern) != N