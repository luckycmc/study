
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    u_char  next;
    u_char  emit;
    u_char  sym;
    u_char  ending;
} ngx_http_v2_huff_decode_code_t;


static ngx_inline ngx_int_t ngx_http_v2_huff_decode_bits(u_char *state,
    u_char *ending, ngx_uint_t bits, u_char **dst);


static ngx_http_v2_huff_decode_code_t  ngx_http_v2_huff_decode_codes[256][16] =
{
    /* 0 */
    {
        {0x04, 0x00, 0x00, 0x00}, {0x05, 0x00, 0x00, 0x00},
        {0x07, 0x00, 0x00, 0x00}, {0x08, 0x00, 0x00, 0x00},
        {0x0b, 0x00, 0x00, 0x00}, {0x0c, 0x00, 0x00, 0x00},
        {0x10, 0x00, 0x00, 0x00}, {0x13, 0x00, 0x00, 0x00},
        {0x19, 0x00, 0x00, 0x00}, {0x1c, 0x00, 0x00, 0x00},
        {0x20, 0x00, 0x00, 0x00}, {0x23, 0x00, 0x00, 0x00},
        {0x2a, 0x00, 0x00, 0x00}, {0x31, 0x00, 0x00, 0x00},
        {0x39, 0x00, 0x00, 0x00}, {0x40, 0x00, 0x00, 0x01}
    },
    {
        {0x00, 0x01, 0x30, 0x01}, {0x00, 0x01, 0x31, 0x01},
        {0x00, 0x01, 0x32, 0x01}, {0x00, 0x01, 0x61, 0x01},
        {0x00, 0x01, 0x63, 0x01}, {0x00, 0x01, 0x65, 0x01},
        {0x00, 0x01, 0x69, 0x01}, {0x00, 0x01, 0x6f, 0x01},
        {0x00, 0x01, 0x73, 0x01}, {0x00, 0x01, 0x74, 0x01},
        {0x0d, 0x00, 0x00, 0x00}, {0x0e, 0x00, 0x00, 0x00},
        {0x11, 0x00, 0x00, 0x00}, {0x12, 0x00, 0x00, 0x00},
        {0x14, 0x00, 0x00, 0x00}, {0x15, 0x00, 0x00, 0x00}
    },
    {
        {0x01, 0x01, 0x30, 0x00}, {0x16, 0x01, 0x30, 0x01},
        {0x01, 0x01, 0x31, 0x00}, {0x16, 0x01, 0x31, 0x01},
        {0x01, 0x01, 0x32, 0x00}, {0x16, 0x01, 0x32, 0x01},
        {0x01, 0x01, 0x61, 0x00}, {0x16, 0x01, 0x61, 0x01},
        {0x01, 0x01, 0x63, 0x00}, {0x16, 0x01, 0x63, 0x01},
        {0x01, 0x01, 0x65, 0x00}, {0x16, 0x01, 0x65, 0x01},
        {0x01, 0x01, 0x69, 0x00}, {0x16, 0x01, 0x69, 0x01},
        {0x01, 0x01, 0x6f, 0x00}, {0x16, 0x01, 0x6f, 0x01}
    },
    {
        {0x02, 0x01, 0x30, 0x00}, {0x09, 0x01, 0x30, 0x00},
        {0x17, 0x01, 0x30, 0x00}, {0x28, 0x01, 0x30, 0x01},
        {0x02, 0x01, 0x31, 0x00}, {0x09, 0x01, 0x31, 0x00},
        {0x17, 0x01, 0x31, 0x00}, {0x28, 0x01, 0x31, 0x01},
        {0x02, 0x01, 0x32, 0x00}, {0x09, 0x01, 0x32, 0x00},
        {0x17, 0x01, 0x32, 0x00}, {0x28, 0x01, 0x32, 0x01},
        {0x02, 0x01, 0x61, 0x00}, {0x09, 0x01, 0x61, 0x00},
        {0x17, 0x01, 0x61, 0x00}, {0x28, 0x01, 0x61, 0x01}
    },
    {
        {0x03, 0x01, 0x30, 0x00}, {0x06, 0x01, 0x30, 0x00},
        {0x0a, 0x01, 0x30, 0x00}, {0x0f, 0x01, 0x30, 0x00},
        {0x18, 0x01, 0x30, 0x00}, {0x1f, 0x01, 0x30, 0x00},
        {0x29, 0x01, 0x30, 0x00}, {0x38, 0x01, 0x30, 0x01},
        {0x03, 0x01, 0x31, 0x00}, {0x06, 0x01, 0x31, 0x00},
        {0x0a, 0x01, 0x31, 0x00}, {0x0f, 0x01, 0x31, 0x00},
        {0x18, 0x01, 0x31, 0x00}, {0x1f, 0x01, 0x31, 0x00},
        {0x29, 0x01, 0x31, 0x00}, {0x38, 0x01, 0x31, 0x01}
    },
    /* 5 */
    {
        {0x03, 0x01, 0x32, 0x00}, {0x06, 0x01, 0x32, 0x00},
        {0x0a, 0x01, 0x32, 0x00}, {0x0f, 0x01, 0x32, 0x00},
        {0x18, 0x01, 0x32, 0x00}, {0x1f, 0x01, 0x32, 0x00},
        {0x29, 0x01, 0x32, 0x00}, {0x38, 0x01, 0x32, 0x01},
        {0x03, 0x01, 0x61, 0x00}, {0x06, 0x01, 0x61, 0x00},
        {0x0a, 0x01, 0x61, 0x00}, {0x0f, 0x01, 0x61, 0x00},
        {0x18, 0x01, 0x61, 0x00}, {0x1f, 0x01, 0x61, 0x00},
        {0x29, 0x01, 0x61, 0x00}, {0x38, 0x01, 0x61, 0x01}
    },
    {
        {0x02, 0x01, 0x63, 0x00}, {0x09, 0x01, 0x63, 0x00},
        {0x17, 0x01, 0x63, 0x00}, {0x28, 0x01, 0x63, 0x01},
        {0x02, 0x01, 0x65, 0x00}, {0x09, 0x01, 0x65, 0x00},
        {0x17, 0x01, 0x65, 0x00}, {0x28, 0x01, 0x65, 0x01},
        {0x02, 0x01, 0x69, 0x00}, {0x09, 0x01, 0x69, 0x00},
        {0x17, 0x01, 0x69, 0x00}, {0x28, 0x01, 0x69, 0x01},
        {0x02, 0x01, 0x6f, 0x00}, {0x09, 0x01, 0x6f, 0x00},
        {0x17, 0x01, 0x6f, 0x00}, {0x28, 0x01, 0x6f, 0x01}
    },
    {
        {0x03, 0x01, 0x63, 0x00}, {0x06, 0x01, 0x63, 0x00},
        {0x0a, 0x01, 0x63, 0x00}, {0x0f, 0x01, 0x63, 0x00},
        {0x18, 0x01, 0x63, 0x00}, {0x1f, 0x01, 0x63, 0x00},
        {0x29, 0x01, 0x63, 0x00}, {0x38, 0x01, 0x63, 0x01},
        {0x03, 0x01, 0x65, 0x00}, {0x06, 0x01, 0x65, 0x00},
        {0x0a, 0x01, 0x65, 0x00}, {0x0f, 0x01, 0x65, 0x00},
        {0x18, 0x01, 0x65, 0x00}, {0x1f, 0x01, 0x65, 0x00},
        {0x29, 0x01, 0x65, 0x00}, {0x38, 0x01, 0x65, 0x01}
    },
    {
        {0x03, 0x01, 0x69, 0x00}, {0x06, 0x01, 0x69, 0x00},
        {0x0a, 0x01, 0x69, 0x00}, {0x0f, 0x01, 0x69, 0x00},
        {0x18, 0x01, 0x69, 0x00}, {0x1f, 0x01, 0x69, 0x00},
        {0x29, 0x01, 0x69, 0x00}, {0x38, 0x01, 0x69, 0x01},
        {0x03, 0x01, 0x6f, 0x00}, {0x06, 0x01, 0x6f, 0x00},
        {0x0a, 0x01, 0x6f, 0x00}, {0x0f, 0x01, 0x6f, 0x00},
        {0x18, 0x01, 0x6f, 0x00}, {0x1f, 0x01, 0x6f, 0x00},
        {0x29, 0x01, 0x6f, 0x00}, {0x38, 0x01, 0x6f, 0x01}
    },
    {
        {0x01, 0x01, 0x73, 0x00}, {0x16, 0x01, 0x73, 0x01},
        {0x01, 0x01, 0x74, 0x00}, {0x16, 0x01, 0x74, 0x01},
        {0x00, 0x01, 0x20, 0x01}, {0x00, 0x01, 0x25, 0x01},
        {0x00, 0x01, 0x2d, 0x01}, {0x00, 0x01, 0x2e, 0x01},
        {0x00, 0x01, 0x2f, 0x01}, {0x00, 0x01, 0x33, 0x01},
        {0x00, 0x01, 0x34, 0x01}, {0x00, 0x01, 0x35, 0x01},
        {0x00, 0x01, 0x36, 0x01}, {0x00, 0x01, 0x37, 0x01},
        {0x00, 0x01, 0x38, 0x01}, {0x00, 0x01, 0x39, 0x01}
    },
    /* 10 */
    {
        {0x02, 0x01, 0x73, 0x00}, {0x09, 0x01, 0x73, 0x00},
        {0x17, 0x01, 0x73, 0x00}, {0x28, 0x01, 0x73, 0x01},
        {0x02, 0x01, 0x74, 0x00}, {0x09, 0x01, 0x74, 0x00},
        {0x17, 0x01, 0x74, 0x00}, {0x28, 0x01, 0x74, 0x01},
        {0x01, 0x01, 0x20, 0x00}, {0x16, 0x01, 0x20, 0x01},
        {0x01, 0x01, 0x25, 0x00}, {0x16, 0x01, 0x25, 0x01},
        {0x01, 0x01, 0x2d, 0x00}, {0x16, 0x01, 0x2d, 0x01},
        {0x01, 0x01, 0x2e, 0x00}, {0x16, 0x01, 0x2e, 0x01}
    },
    {
        {0x03, 0x01, 0x73, 0x00}, {0x06, 0x01, 0x73, 0x00},
        {0x0a, 0x01, 0x73, 0x00}, {0x0f, 0x01, 0x73, 0x00},
        {0x18, 0x01, 0x73, 0x00}, {0x1f, 0x01, 0x73, 0x00},
        {0x29, 0x01, 0x73, 0x00}, {0x38, 0x01, 0x73, 0x01},
        {0x03, 0x01, 0x74, 0x00}, {0x06, 0x01, 0x74, 0x00},
        {0x0a, 0x01, 0x74, 0x00}, {0x0f, 0x01, 0x74, 0x00},
        {0x18, 0x01, 0x74, 0x00}, {0x1f, 0x01, 0x74, 0x00},
        {0x29, 0x01, 0x74, 0x00}, {0x38, 0x01, 0x74, 0x01}
    },
    {
        {0x02, 0x01, 0x20, 0x00}, {0x09, 0x01, 0x20, 0x00},
        {0x17, 0x01, 0x20, 0x00}, {0x28, 0x01, 0x20, 0x01},
        {0x02, 0x01, 0x25, 0x00}, {0x09, 0x01, 0x25, 0x00},
        {0x17, 0x01, 0x25, 0x00}, {0x28, 0x01, 0x25, 0x01},
        {0x02, 0x01, 0x2d, 0x00}, {0x09, 0x01, 0x2d, 0x00},
        {0x17, 0x01, 0x2d, 0x00}, {0x28, 0x01, 0x2d, 0x01},
        {0x02, 0x01, 0x2e, 0x00}, {0x09, 0x01, 0x2e, 0x00},
        {0x17, 0x01, 0x2e, 0x00}, {0x28, 0x01, 0x2e, 0x01}
    },
    {
        {0x03, 0x01, 0x20, 0x00}, {0x06, 0x01, 0x20, 0x00},
        {0x0a, 0x01, 0x20, 0x00}, {0x0f, 0x01, 0x20, 0x00},
        {0x18, 0x01, 0x20, 0x00}, {0x1f, 0x01, 0x20, 0x00},
        {0x29, 0x01, 0x20, 0x00}, {0x38, 0x01, 0x20, 0x01},
        {0x03, 0x01, 0x25, 0x00}, {0x06, 0x01, 0x25, 0x00},
        {0x0a, 0x01, 0x25, 0x00}, {0x0f, 0x01, 0x25, 0x00},
        {0x18, 0x01, 0x25, 0x00}, {0x1f, 0x01, 0x25, 0x00},
        {0x29, 0x01, 0x25, 0x00}, {0x38, 0x01, 0x25, 0x01}
    },
    {
        {0x03, 0x01, 0x2d, 0x00}, {0x06, 0x01, 0x2d, 0x00},
        {0x0a, 0x01, 0x2d, 0x00}, {0x0f, 0x01, 0x2d, 0x00},
        {0x18, 0x01, 0x2d, 0x00}, {0x1f, 0x01, 0x2d, 0x00},
        {0x29, 0x01, 0x2d, 0x00}, {0x38, 0x01, 0x2d, 0x01},
        {0x03, 0x01, 0x2e, 0x00}, {0x06, 0x01, 0x2e, 0x00},
        {0x0a, 0x01, 0x2e, 0x00}, {0x0f, 0x01, 0x2e, 0x00},
        {0x18, 0x01, 0x2e, 0x00}, {0x1f, 0x01, 0x2e, 0x00},
        {0x29, 0x01, 0x2e, 0x00}, {0x38, 0x01, 0x2e, 0x01}
    },
    /* 15 */
    {
        {0x01, 0x01, 0x2f, 0x00}, {0x16, 0x01, 0x2f, 0x01},
        {0x01, 0x01, 0x33, 0x00}, {0x16, 0x01, 0x33, 0x01},
        {0x01, 0x01, 0x34, 0x00}, {0x16, 0x01, 0x34, 0x01},
        {0x01, 0x01, 0x35, 0x00}, {0x16, 0x01, 0x35, 0x01},
        {0x01, 0x01, 0x36, 0x00}, {0x16, 0x01, 0x36, 0x01},
        {0x01, 0x01, 0x37, 0x00}, {0x16, 0x01, 0x37, 0x01},
        {0x01, 0x01, 0x38, 0x00}, {0x16, 0x01, 0x38, 0x01},
        {0x01, 0x01, 0x39, 0x00}, {0x16, 0x01, 0x39, 0x01}
    },
    {
        {0x02, 0x01, 0x2f, 0x00}, {0x09, 0x01, 0x2f, 0x00},
        {0x17, 0x01, 0x2f, 0x00}, {0x28, 0x01, 0x2f, 0x01},
        {0x02, 0x01, 0x33, 0x00}, {0x09, 0x01, 0x33, 0x00},
        {0x17, 0x01, 0x33, 0x00}, {0x28, 0x01, 0x33, 0x01},
        {0x02, 0x01, 0x34, 0x00}, {0x09, 0x01, 0x34, 0x00},
        {0x17, 0x01, 0x34, 0x00}, {0x28, 0x01, 0x34, 0x01},
        {0x02, 0x01, 0x35, 0x00}, {0x09, 0x01, 0x35, 0x00},
        {0x17, 0x01, 0x35, 0x00}, {0x28, 0x01, 0x35, 0x01}
    },
    {
        {0x03, 0x01, 0x2f, 0x00}, {0x06, 0x01, 0x2f, 0x00},
        {0x0a, 0x01, 0x2f, 0x00}, {0x0f, 0x01, 0x2f, 0x00},
        {0x18, 0x01, 0x2f, 0x00}, {0x1f, 0x01, 0x2f, 0x00},
        {0x29, 0x01, 0x2f, 0x00}, {0x38, 0x01, 0x2f, 0x01},
        {0x03, 0x01, 0x33, 0x00}, {0x06, 0x01, 0x33, 0x00},
        {0x0a, 0x01, 0x33, 0x00}, {0x0f, 0x01, 0x33, 0x00},
        {0x18, 0x01, 0x33, 0x00}, {0x1f, 0x01, 0x33, 0x00},
        {0x29, 0x01, 0x33, 0x00}, {0x38, 0x01, 0x33, 0x01}
    },
    {
        {0x03, 0x01, 0x34, 0x00}, {0x06, 0x01, 0x34, 0x00},
        {0x0a, 0x01, 0x34, 0x00}, {0x0f, 0x01, 0x34, 0x00},
        {0x18, 0x01, 0x34, 0x00}, {0x1f, 0x01, 0x34, 0x00},
        {0x29, 0x01, 0x34, 0x00}, {0x38, 0x01, 0x34, 0x01},
        {0x03, 0x01, 0x35, 0x00}, {0x06, 0x01, 0x35, 0x00},
        {0x0a, 0x01, 0x35, 0x00}, {0x0f, 0x01, 0x35, 0x00},
        {0x18, 0x01, 0x35, 0x00}, {0x1f, 0x01, 0x35, 0x00},
        {0x29, 0x01, 0x35, 0x00}, {0x38, 0x01, 0x35, 0x01}
    },
    {
        {0x02, 0x01, 0x36, 0x00}, {0x09, 0x01, 0x36, 0x00},
        {0x17, 0x01, 0x36, 0x00}, {0x28, 0x01, 0x36, 0x01},
        {0x02, 0x01, 0x37, 0x00}, {0x09, 0x01, 0x37, 0x00},
        {0x17, 0x01, 0x37, 0x00}, {0x28, 0x01, 0x37, 0x01},
        {0x02, 0x01, 0x38, 0x00}, {0x09, 0x01, 0x38, 0x00},
        {0x17, 0x01, 0x38, 0x00}, {0x28, 0x01, 0x38, 0x01},
        {0x02, 0x01, 0x39, 0x00}, {0x09, 0x01, 0x39, 0x00},
        {0x17, 0x01, 0x39, 0x00}, {0x28, 0x01, 0x39, 0x01}
    },
    /* 20 */
    {
        {0x03, 0x01, 0x36, 0x00}, {0x06, 0x01, 0x36, 0x00},
        {0x0a, 0x01, 0x36, 0x00}, {0x0f, 0x01, 0x36, 0x00},
        {0x18, 0x01, 0x36, 0x00}, {0x1f, 0x01, 0x36, 0x00},
        {0x29, 0x01, 0x36, 0x00}, {0x38, 0x01, 0x36, 0x01},
        {0x03, 0x01, 0x37, 0x00}, {0x06, 0x01, 0x37, 0x00},
        {0x0a, 0x01, 0x37, 0x00}, {0x0f, 0x01, 0x37, 0x00},
        {0x18, 0x01, 0x37, 0x00}, {0x1f, 0x01, 0x37, 0x00},
        {0x29, 0x01, 0x37, 0x00}, {0x38, 0x01, 0x37, 0x01}
    },
    {
        {0x03, 0x01, 0x38, 0x00}, {0x06, 0x01, 0x38, 0x00},
        {0x0a, 0x01, 0x38, 0x00}, {0x0f, 0x01, 0x38, 0x00},
        {0x18, 0x01, 0x38, 0x00}, {0x1f, 0x01, 0x38, 0x00},
        {0x29, 0x01, 0x38, 0x00}, {0x38, 0x01, 0x38, 0x01},
        {0x03, 0x01, 0x39, 0x00}, {0x06, 0x01, 0x39, 0x00},
        {0x0a, 0x01, 0x39, 0x00}, {0x0f, 0x01, 0x39, 0x00},
        {0x18, 0x01, 0x39, 0x00}, {0x1f, 0x01, 0x39, 0x00},
        {0x29, 0x01, 0x39, 0x00}, {0x38, 0x01, 0x39, 0x01}
    },
    {
        {0x1a, 0x00, 0x00, 0x00}, {0x1b, 0x00, 0x00, 0x00},
        {0x1d, 0x00, 0x00, 0x00}, {0x1e, 0x00, 0x00, 0x00},
        {0x21, 0x00, 0x00, 0x00}, {0x22, 0x00, 0x00, 0x00},
        {0x24, 0x00, 0x00, 0x00}, {0x25, 0x00, 0x00, 0x00},
        {0x2b, 0x00, 0x00, 0x00}, {0x2e, 0x00, 0x00, 0x00},
        {0x32, 0x00, 0x00, 0x00}, {0x35, 0x00, 0x00, 0x00},
        {0x3a, 0x00, 0x00, 0x00}, {0x3d, 0x00, 0x00, 0x00},
        {0x41, 0x00, 0x00, 0x00}, {0x44, 0x00, 0x00, 0x01}
    },
    {
        {0x00, 0x01, 0x3d, 0x01}, {0x00, 0x01, 0x41, 0x01},
        {0x00, 0x01, 0x5f, 0x01}, {0x00, 0x01, 0x62, 0x01},
        {0x00, 0x01, 0x64, 0x01}, {0x00, 0x01, 0x66, 0x01},
        {0x00, 0x01, 0x67, 0x01}, {0x00, 0x01, 0x68, 0x01},
        {0x00, 0x01, 0x6c, 0x01}, {0x00, 0x01, 0x6d, 0x01},
        {0x00, 0x01, 0x6e, 0x01}, {0x00, 0x01, 0x70, 0x01},
        {0x00, 0x01, 0x72, 0x01}, {0x00, 0x01, 0x75, 0x01},
        {0x26, 0x00, 0x00, 0x00}, {0x27, 0x00, 0x00, 0x00}
    },
    {
        {0x01, 0x01, 0x3d, 0x00}, {0x16, 0x01, 0x3d, 0x01},
        {0x01, 0x01, 0x41, 0x00}, {0x16, 0x01, 0x41, 0x01},
        {0x01, 0x01, 0x5f, 0x00}, {0x16, 0x01, 0x5f, 0x01},
        {0x01, 0x01, 0x62, 0x00}, {0x16, 0x01, 0x62, 0x01},
        {0x01, 0x01, 0x64, 0x00}, {0x16, 0x01, 0x64, 0x01},
        {0x01, 0x01, 0x66, 0x00}, {0x16, 0x01, 0x66, 0x01},
        {0x01, 0x01, 0x67, 0x00}, {0x16, 0x01, 0x67, 0x01},
        {0x01, 0x01, 0x68, 0x00}, {0x16, 0x01, 0x68, 0x01}
    },
    /* 25 */
    {
        {0x02, 0x01, 0x3d, 0x00}, {0x09, 0x01, 0x3d, 0x00},
        {0x17, 0x01, 0x3d, 0x00}, {0x28, 0x01, 0x3d, 0x01},
        {0x02, 0x01, 0x41, 0x00}, {0x09, 0x01, 0x41, 0x00},
        {0x17, 0x01, 0x41, 0x00}, {0x28, 0x01, 0x41, 0x01},
        {0x02, 0x01, 0x5f, 0x00}, {0x09, 0x01, 0x5f, 0x00},
        {0x17, 0x01, 0x5f, 0x00}, {0x28, 0x01, 0x5f, 0x01},
        {0x02, 0x01, 0x62, 0x00}, {0x09, 0x01, 0x62, 0x00},
        {0x17, 0x01, 0x62, 0x00}, {0x28, 0x01, 0x62, 0x01}
    },
    {
        {0x03, 0x01, 0x3d, 0x00}, {0x06, 0x01, 0x3d, 0x00},
        {0x0a, 0x01, 0x3d, 0x00}, {0x0f, 0x01, 0x3d, 0x00},
        {0x18, 0x01, 0x3d, 0x00}, {0x1f, 0x01, 0x3d, 0x00},
        {0x29, 0x01, 0x3d, 0x00}, {0x38, 0x01, 0x3d, 0x01},
        {0x03, 0x01, 0x41, 0x00}, {0x06, 0x01, 0x41, 0x00},
        {0x0a, 0x01, 0x41, 0x00}, {0x0f, 0x01, 0x41, 0x00},
        {0x18, 0x01, 0x41, 0x00}, {0x1f, 0x01, 0x41, 0x00},
        {0x29, 0x01, 0x41, 0x00}, {0x38, 0x01, 0x41, 0x01}
    },
    {
        {0x03, 0x01, 0x5f, 0x00}, {0x06, 0x01, 0x5f, 0x00},
        {0x0a, 0x01, 0x5f, 0x00}, {0x0f, 0x01, 0x5f, 0x00},
        {0x18, 0x01, 0x5f, 0x00}, {0x1f, 0x01, 0x5f, 0x00},
        {0x29, 0x01, 0x5f, 0x00}, {0x38, 0x01, 0x5f, 0x01},
        {0x03, 0x01, 0x62, 0x00}, {0x06, 0x01, 0x62, 0x00},
        {0x0a, 0x01, 0x62, 0x00}, {0x0f, 0x01, 0x62, 0x00},
        {0x18, 0x01, 0x62, 0x00}, {0x1f, 0x01, 0x62, 0x00},
        {0x29, 0x01, 0x62, 0x00}, {0x38, 0x01, 0x62, 0x01}
    },
    {
        {0x02, 0x01, 0x64, 0x00}, {0x09, 0x01, 0x64, 0x00},
        {0x17, 0x01, 0x64, 0x00}, {0x28, 0x01, 0x64, 0x01},
        {0x02, 0x01, 0x66, 0x00}, {0x09, 0x01, 0x66, 0x00},
        {0x17, 0x01, 0x66, 0x00}, {0x28, 0x01, 0x66, 0x01},
        {0x02, 0x01, 0x67, 0x00}, {0x09, 0x01, 0x67, 0x00},
        {0x17, 0x01, 0x67, 0x00}, {0x28, 0x01, 0x67, 0x01},
        {0x02, 0x01, 0x68, 0x00}, {0x09, 0x01, 0x68, 0x00},
        {0x17, 0x01, 0x68, 0x00}, {0x28, 0x01, 0x68, 0x01}
    },
    {
        {0x03, 0x01, 0x64, 0x00}, {0x06, 0x01, 0x64, 0x00},
        {0x0a, 0x01, 0x64, 0x00}, {0x0f, 0x01, 0x64, 0x00},
        {0x18, 0x01, 0x64, 0x00}, {0x1f, 0x01, 0x64, 0x00},
        {0x29, 0x01, 0x64, 0x00}, {0x38, 0x01, 0x64, 0x01},
        {0x03, 0x01, 0x66, 0x00}, {0x06, 0x01, 0x66, 0x00},
        {0x0a, 0x01, 0x66, 0x00}, {0x0f, 0x01, 0x66, 0x00},
        {0x18, 0x01, 0x66, 0x00}, {0x1f, 0x01, 0x66, 0x00},
        {0x29, 0x01, 0x66, 0x00}, {0x38, 0x01, 0x66, 0x01}
    },
    /* 30 */
    {
        {0x03, 0x01, 0x67, 0x00}, {0x06, 0x01, 0x67, 0x00},
        {0x0a, 0x01, 0x67, 0x00}, {0x0f, 0x01, 0x67, 0x00},
        {0x18, 0x01, 0x67, 0x00}, {0x1f, 0x01, 0x67, 0x00},
        {0x29, 0x01, 0x67, 0x00}, {0x38, 0x01, 0x67, 0x01},
        {0x03, 0x01, 0x68, 0x00}, {0x06, 0x01, 0x68, 0x00},
        {0x0a, 0x01, 0x68, 0x00}, {0x0f, 0x01, 0x68, 0x00},
        {0x18, 0x01, 0x68, 0x00}, {0x1f, 0x01, 0x68, 0x00},
        {0x29, 0x01, 0x68, 0x00}, {0x38, 0x01, 0x68, 0x01}
    },
    {
        {0x01, 0x01, 0x6c, 0x00}, {0x16, 0x01, 0x6c, 0x01},
        {0x01, 0x01, 0x6d, 0x00}, {0x16, 0x01, 0x6d, 0x01},
        {0x01, 0x01, 0x6e, 0x00}, {0x16, 0x01, 0x6e, 0x01},
        {0x01, 0x01, 0x70, 0x00}, {0x16, 0x01, 0x70, 0x01},
        {0x01, 0x01, 0x72, 0x00}, {0x16, 0x01, 0x72, 0x01},
        {0x01, 0x01, 0x75, 0x00}, {0x16, 0x01, 0x75, 0x01},
        {0x00, 0x01, 0x3a, 0x01}, {0x00, 0x01, 0x42, 0x01},
        {0x00, 0x01, 0x43, 0x01}, {0x00, 0x01, 0x44, 0x01}
    },
    {
        {0x02, 0x01, 0x6c, 0x00}, {0x09, 0x01, 0x6c, 0x00},
        {0x17, 0x01, 0x6c, 0x00}, {0x28, 0x01, 0x6c, 0x01},
        {0x02, 0x01, 0x6d, 0x00}, {0x09, 0x01, 0x6d, 0x00},
        {0x17, 0x01, 0x6d, 0x00}, {0x28, 0x01, 0x6d, 0x01},
        {0x02, 0x01, 0x6e, 0x00}, {0x09, 0x01, 0x6e, 0x00},
        {0x17, 0x01, 0x6e, 0x00}, {0x28, 0x01, 0x6e, 0x01},
        {0x02, 0x01, 0x70, 0x00}, {0x09, 0x01, 0x70, 0x00},
        {0x17, 0x01, 0x70, 0x00}, {0x28, 0x01, 0x70, 0x01}
    },
    {
        {0x03, 0x01, 0x6c, 0x00}, {0x06, 0x01, 0x6c, 0x00},
        {0x0a, 0x01, 0x6c, 0x00}, {0x0f, 0x01, 0x6c, 0x00},
        {0x18, 0x01, 0x6c, 0x00}, {0x1f, 0x01, 0x6c, 0x00},
        {0x29, 0x01, 0x6c, 0x00}, {0x38, 0x01, 0x6c, 0x01},
        {0x03, 0x01, 0x6d, 0x00}, {0x06, 0x01, 0x6d, 0x00},
        {0x0a, 0x01, 0x6d, 0x00}, {0x0f, 0x01, 0x6d, 0x00},
        {0x18, 0x01, 0x6d, 0x00}, {0x1f, 0x01, 0x6d, 0x00},
        {0x29, 0x01, 0x6d, 0x00}, {0x38, 0x01, 0x6d, 0x01}
    },
    {
        {0x03, 0x01, 0x6e, 0x00}, {0x06, 0x01, 0x6e, 0x00},
        {0x0a, 0x01, 0x6e, 0x00}, {0x0f, 0x01, 0x6e, 0x00},
        {0x18, 0x01, 0x6e, 0x00}, {0x1f, 0x01, 0x6e, 0x00},
        {0x29, 0x01, 0x6e, 0x00}, {0x38, 0x01, 0x6e, 0x01},
        {0x03, 0x01, 0x70, 0x00}, {0x06, 0x01, 0x70, 0x00},
        {0x0a, 0x01, 0x70, 0x00}, {0x0f, 0x01, 0x70, 0x00},
        {0x18, 0x01, 0x70, 0x00}, {0x1f, 0x01, 0x70, 0x00},
        {0x29, 0x01, 0x70, 0x00}, {0x38, 0x01, 0x70, 0x01}
    },
    /* 35 */
    {
        {0x02, 0x01, 0x72, 0x00}, {0x09, 0x01, 0x72, 0x00},
        {0x17, 0x01, 0x72, 0x00}, {0x28, 0x01, 0x72, 0x01},
        {0x02, 0x01, 0x75, 0x00}, {0x09, 0x01, 0x75, 0x00},
        {0x17, 0x01, 0x75, 0x00}, {0x28, 0x01, 0x75, 0x01},
        {0x01, 0x01, 0x3a, 0x00}, {0x16, 0x01, 0x3a, 0x01},
        {0x01, 0x01, 0x42, 0x00}, {0x16, 0x01, 0x42, 0x01},
        {0x01, 0x01, 0x43, 0x00}, {0x16, 0x01, 0x43, 0x01},
        {0x01, 0x01, 0x44, 0x00}, {0x16, 0x01, 0x44, 0x01}
    },
    {
        {0x03, 0x01, 0x72, 0x00}, {0x06, 0x01, 0x72, 0x00},
        {0x0a, 0x01, 0x72, 0x00}, {0x0f, 0x01, 0x72, 0x00},
        {0x18, 0x01, 0x72, 0x00}, {0x1f, 0x01, 0x72, 0x00},
        {0x29, 0x01, 0x72, 0x00}, {0x38, 0x01, 0x72, 0x01},
        {0x03, 0x01, 0x75, 0x00}, {0x06, 0x01, 0x75, 0x00},
        {0x0a, 0x01, 0x75, 0x00}, {0x0f, 0x01, 0x75, 0x00},
        {0x18, 0x01, 0x75, 0x00}, {0x1f, 0x01, 0x75, 0x00},
        {0x29, 0x01, 0x75, 0x00}, {0x38, 0x01, 0x75, 0x01}
    },
    {
        {0x02, 0x01, 0x3a, 0x00}, {0x09, 0x01, 0x3a, 0x00},
        {0x17, 0x01, 0x3a, 0x00}, {0x28, 0x01, 0x3a, 0x01},
        {0x02, 0x01, 0x42, 0x00}, {0x09, 0x01, 0x42, 0x00},
        {0x17, 0x01, 0x42, 0x00}, {0x28, 0x01, 0x42, 0x01},
        {0x02, 0x01, 0x43, 0x00}, {0x09, 0x01, 0x43, 0x00},
        {0x17, 0x01, 0x43, 0x00}, {0x28, 0x01, 0x43, 0x01},
        {0x02, 0x01, 0x44, 0x00}, {0x09, 0x01, 0x44, 0x00},
        {0x17, 0x01, 0x44, 0x00}, {0x28, 0x01, 0x44, 0x01}
    },
    {
        {0x03, 0x01, 0x3a, 0x00}, {0x06, 0x01, 0x3a, 0x00},
        {0x0a, 0x01, 0x3a, 0x00}, {0x0f, 0x01, 0x3a, 0x00},
        {0x18, 0x01, 0x3a, 0x00}, {0x1f, 0x01, 0x3a, 0x00},
        {0x29, 0x01, 0x3a, 0x00}, {0x38, 0x01, 0x3a, 0x01},
        {0x03, 0x01, 0x42, 0x00}, {0x06, 0x01, 0x42, 0x00},
        {0x0a, 0x01, 0x42, 0x00}, {0x0f, 0x01, 0x42, 0x00},
        {0x18, 0x01, 0x42, 0x00}, {0x1f, 0x01, 0x42, 0x00},
        {0x29, 0x01, 0x42, 0x00}, {0x38, 0x01, 0x42, 0x01}
    },
    {
        {0x03, 0x01, 0x43, 0x00}, {0x06, 0x01, 0x43, 0x00},
        {0x0a, 0x01, 0x43, 0x00}, {0x0f, 0x01, 0x43, 0x00},
        {0x18, 0x01, 0x43, 0x00}, {0x1f, 0x01, 0x43, 0x00},
        {0x29, 0x01, 0x43, 0x00}, {0x38, 0x01, 0x43, 0x01},
        {0x03, 0x01, 0x44, 0x00}, {0x06, 0x01, 0x44, 0x00},
        {0x0a, 0x01, 0x44, 0x00}, {0x0f, 0x01, 0x44, 0x00},
        {0x18, 0x01, 0x44, 0x00}, {0x1f, 0x01, 0x44, 0x00},
        {0x29, 0x01, 0x44, 0x00}, {0x38, 0x01, 0x44, 0x01}
    },
    /* 40 */
    {
        {0x2c, 0x00, 0x00, 0x00}, {0x2d, 0x00, 0x00, 0x00},
        {0x2f, 0x00, 0x00, 0x00}, {0x30, 0x00, 0x00, 0x00},
        {0x33, 0x00, 0x00, 0x00}, {0x34, 0x00, 0x00, 0x00},
        {0x36, 0x00, 0x00, 0x00}, {0x37, 0x00, 0x00, 0x00},
        {0x3b, 0x00, 0x00, 0x00}, {0x3c, 0x00, 0x00, 0x00},
        {0x3e, 0x00, 0x00, 0x00}, {0x3f, 0x00, 0x00, 0x00},
        {0x42, 0x00, 0x00, 0x00}, {0x43, 0x00, 0x00, 0x00},
        {0x45, 0x00, 0x00, 0x00}, {0x48, 0x00, 0x00, 0x01}
    },
    {
        {0x00, 0x01, 0x45, 0x01}, {0x00, 0x01, 0x46, 0x01},
        {0x00, 0x01, 0x47, 0x01}, {0x00, 0x01, 0x48, 0x01},
        {0x00, 0x01, 0x49, 0x01}, {0x00, 0x01, 0x4a, 0x01},
        {0x00, 0x01, 0x4b, 0x01}, {0x00, 0x01, 0x4c, 0x01},
        {0x00, 0x01, 0x4d, 0x01}, {0x00, 0x01, 0x4e, 0x01},
        {0x00, 0x01, 0x4f, 0x01}, {0x00, 0x01, 0x50, 0x01},
        {0x00, 0x01, 0x51, 0x01}, {0x00, 0x01, 0x52, 0x01},
        {0x00, 0x01, 0x53, 0x01}, {0x00, 0x01, 0x54, 0x01}
    },
    {
        {0x01, 0x01, 0x45, 0x00}, {0x16, 0x01, 0x45, 0x01},
        {0x01, 0x01, 0x46, 0x00}, {0x16, 0x01, 0x46, 0x01},
        {0x01, 0x01, 0x47, 0x00}, {0x16, 0x01, 0x47, 0x01},
        {0x01, 0x01, 0x48, 0x00}, {0x16, 0x01, 0x48, 0x01},
        {0x01, 0x01, 0x49, 0x00}, {0x16, 0x01, 0x49, 0x01},
        {0x01, 0x01, 0x4a, 0x00}, {0x16, 0x01, 0x4a, 0x01},
        {0x01, 0x01, 0x4b, 0x00}, {0x16, 0x01, 0x4b, 0x01},
        {0x01, 0x01, 0x4c, 0x00}, {0x16, 0x01, 0x4c, 0x01}
    },
    {
        {0x02, 0x01, 0x45, 0x00}, {0x09, 0x01, 0x45, 0x00},
        {0x17, 0x01, 0x45, 0x00}, {0x28, 0x01, 0x45, 0x01},
        {0x02, 0x01, 0x46, 0x00}, {0x09, 0x01, 0x46, 0x00},
        {0x17, 0x01, 0x46, 0x00}, {0x28, 0x01, 0x46, 0x01},
        {0x02, 0x01, 0x47, 0x00}, {0x09, 0x01, 0x47, 0x00},
        {0x17, 0x01, 0x47, 0x00}, {0x28, 0x01, 0x47, 0x01},
        {0x02, 0x01, 0x48, 0x00}, {0x09, 0x01, 0x48, 0x00},
        {0x17, 0x01, 0x48, 0x00}, {0x28, 0x01, 0x48, 0x01}
    },
    {
        {0x03, 0x01, 0x45, 0x00}, {0x06, 0x01, 0x45, 0x00},
        {0x0a, 0x01, 0x45, 0x00}, {0x0f, 0x01, 0x45, 0x00},
        {0x18, 0x01, 0x45, 0x00}, {0x1f, 0x01, 0x45, 0x00},
        {0x29, 0x01, 0x45, 0x00}, {0x38, 0x01, 0x45, 0x01},
        {0x03, 0x01, 0x46, 0x00}, {0x06, 0x01, 0x46, 0x00},
        {0x0a, 0x01, 0x46, 0x00}, {0x0f, 0x01, 0x46, 0x00},
        {0x18, 0x01, 0x46, 0x00}, {0x1f, 0x01, 0x46, 0x00},
        {0x29, 0x01, 0x46, 0x00}, {0x38, 0x01, 0x46, 0x01}
    },
    /* 45 */
    {
        {0x03, 0x01, 0x47, 0x00}, {0x06, 0x01, 0x47, 0x00},
        {0x0a, 0x01, 0x47, 0x00}, {0x0f, 0x01, 0x47, 0x00},
        {0x18, 0x01, 0x47, 0x00}, {0x1f, 0x01, 0x47, 0x00},
        {0x29, 0x01, 0x47, 0x00}, {0x38, 0x01, 0x47, 0x01},
        {0x03, 0x01, 0x48, 0x00}, {0x06, 0x01, 0x48, 0x00},
        {0x0a, 0x01, 0x48, 0x00}, {0x0f, 0x01, 0x48, 0x00},
        {0x18, 0x01, 0x48, 0x00}, {0x1f, 0x01, 0x48, 0x00},
        {0x29, 0x01, 0x48, 0x00}, {0x38, 0x01, 0x48, 0x01}
    },
    {
        {0x02, 0x01, 0x49, 0x00}, {0x09, 0x01, 0x49, 0x00},
        {0x17, 0x01, 0x49, 0x00}, {0x28, 0x01, 0x49, 0x01},
        {0x02, 0x01, 0x4a, 0x00}, {0x09, 0x01, 0x4a, 0x00},
        {0x17, 0x01, 0x4a, 0x00}, {0x28, 0x01, 0x4a, 0x01},
        {0x02, 0x01, 0x4b, 0x00}, {0x09, 0x01, 0x4b, 0x00},
        {0x17, 0x01, 0x4b, 0x00}, {0x28, 0x01, 0x4b, 0x01},
        {0x02, 0x01, 0x4c, 0x00}, {0x09, 0x01, 0x4c, 0x00},
        {0x17, 0x01, 0x4c, 0x00}, {0x28, 0x01, 0x4c, 0x01}
    },
    {
        {0x03, 0x01, 0x49, 0x00}, {0x06, 0x01, 0x49, 0x00},
        {0x0a, 0x01, 0x49, 0x00}, {0x0f, 0x01, 0x49, 0x00},
        {0x18, 0x01, 0x49, 0x00}, {0x1f, 0x01, 0x49, 0x00},
        {0x29, 0x01, 0x49, 0x00}, {0x38, 0x01, 0x49, 0x01},
        {0x03, 0x01, 0x4a, 0x00}, {0x06, 0x01, 0x4a, 0x00},
        {0x0a, 0x01, 0x4a, 0x00}, {0x0f, 0x01, 0x4a, 0x00},
        {0x18, 0x01, 0x4a, 0x00}, {0x1f, 0x01, 0x4a, 0x00},
        {0x29, 0x01, 0x4a, 0x00}, {0x38, 0x01, 0x4a, 0x01}
    },
    {
        {0x03, 0x01, 0x4b, 0x00}, {0x06, 0x01, 0x4b, 0x00},
        {0x0a, 0x01, 0x4b, 0x00}, {0x0f, 0x01, 0x4b, 0x00},
        {0x18, 0x01, 0x4b, 0x00}, {0x1f, 0x01, 0x4b, 0x00},
        {0x29, 0x01, 0x4b, 0x00}, {0x38, 0x01, 0x4b, 0x01},
        {0x03, 0x01, 0x4c, 0x00}, {0x06, 0x01, 0x4c, 0x00},
        {0x0a, 0x01, 0x4c, 0x00}, {0x0f, 0x01, 0x4c, 0x00},
        {0x18, 0x01, 0x4c, 0x00}, {0x1f, 0x01, 0x4c, 0x00},
        {0x29, 0x01, 0x4c, 0x00}, {0x38, 0x01, 0x4c, 0x01}
    },
    {
        {0x01, 0x01, 0x4d, 0x00}, {0x16, 0x01, 0x4d, 0x01},
        {0x01, 0x01, 0x4e, 0x00}, {0x16, 0x01, 0x4e, 0x01},
        {0x01, 0x01, 0x4f, 0x00}, {0x16, 0x01, 0x4f, 0x01},
        {0x01, 0x01, 0x50, 0x00}, {0x16, 0x01, 0x50, 0x01},
        {0x01, 0x01, 0x51, 0x00}, {0x16, 0x01, 0x51, 0x01},
        {0x01, 0x01, 0x52, 0x00}, {0x16, 0x01, 0x52, 0x01},
        {0x01, 0x01, 0x53, 0x00}, {0x16, 0x01, 0x53, 0x01},
        {0x01, 0x01, 0x54, 0x00}, {0x16, 0x01, 0x54, 0x01}
    },
    /* 50 */
    {
        {0x02, 0x01, 0x4d, 0x00}, {0x09, 0x01, 0x4d, 0x00},
        {0x17, 0x01, 0x4d, 0x00}, {0x28, 0x01, 0x4d, 0x01},
        {0x02, 0x01, 0x4e, 0x00}, {0x09, 0x01, 0x4e, 0x00},
        {0x17, 0x01, 0x4e, 0x00}, {0x28, 0x01, 0x4e, 0x01},
        {0x02, 0x01, 0x4f, 0x00}, {0x09, 0x01, 0x4f, 0x00},
        {0x17, 0x01, 0x4f, 0x00}, {0x28, 0x01, 0x4f, 0x01},
        {0x02, 0x01, 0x50, 0x00}, {0x09, 0x01, 0x50, 0x00},
        {0x17, 0x01, 0x50, 0x00}, {0x28, 0x01, 0x50, 0x01}
    },
    {
        {0x03, 0x01, 0x4d, 0x00}, {0x06, 0x01, 0x4d, 0x00},
        {0x0a, 0x01, 0x4d, 0x00}, {0x0f, 0x01, 0x4d, 0x00},
        {0x18, 0x01, 0x4d, 0x00}, {0x1f, 0x01, 0x4d, 0x00},
        {0x29, 0x01, 0x4d, 0x00}, {0x38, 0x01, 0x4d, 0x01},
        {0x03, 0x01, 0x4e, 0x00}, {0x06, 0x01, 0x4e, 0x00},
        {0x0a, 0x01, 0x4e, 0x00}, {0x0f, 0x01, 0x4e, 0x00},
        {0x18, 0x01, 0x4e, 0x00}, {0x1f, 0x01, 0x4e, 0x00},
        {0x29, 0x01, 0x4e, 0x00}, {0x38, 0x01, 0x4e, 0x01}
    },
    {
        {0x03, 0x01, 0x4f, 0x00}, {0x06, 0x01, 0x4f, 0x00},
        {0x0a, 0x01, 0x4f, 0x00}, {0x0f, 0x01, 0x4f, 0x00},
        {0x18, 0x01, 0x4f, 0x00}, {0x1f, 0x01, 0x4f, 0x00},
        {0x29, 0x01, 0x4f, 0x00}, {0x38, 0x01, 0x4f, 0x01},
        {0x03, 0x01, 0x50, 0x00}, {0x06, 0x01, 0x50, 0x00},
        {0x0a, 0x01, 0x50, 0x00}, {0x0f, 0x01, 0x50, 0x00},
        {0x18, 0x01, 0x50, 0x00}, {0x1f, 0x01, 0x50, 0x00},
        {0x29, 0x01, 0x50, 0x00}, {0x38, 0x01, 0x50, 0x01}
    },
    {
        {0x02, 0x01, 0x51, 0x00}, {0x09, 0x01, 0x51, 0x00},
        {0x17, 0x01, 0x51, 0x00}, {0x28, 0x01, 0x51, 0x01},
        {0x02, 0x01, 0x52, 0x00}, {0x09, 0x01, 0x52, 0x00},
        {0x17, 0x01, 0x52, 0x00}, {0x28, 0x01, 0x52, 0x01},
        {0x02, 0x01, 0x53, 0x00}, {0x09, 0x01, 0x53, 0x00},
        {0x17, 0x01, 0x53, 0x00}, {0x28, 0x01, 0x53, 0x01},
        {0x02, 0x01, 0x54, 0x00}, {0x09, 0x01, 0x54, 0x00},
        {0x17, 0x01, 0x54, 0x00}, {0x28, 0x01, 0x54, 0x01}
    },
    {
        {0x03, 0x01, 0x51, 0x00}, {0x06, 0x01, 0x51, 0x00},
        {0x0a, 0x01, 0x51, 0x00}, {0x0f, 0x01, 0x51, 0x00},
        {0x18, 0x01, 0x51, 0x00}, {0x1f, 0x01, 0x51, 0x00},
        {0x29, 0x01, 0x51, 0x00}, {0x38, 0x01, 0x51, 0x01},
        {0x03, 0x01, 0x52, 0x00}, {0x06, 0x01, 0x52, 0x00},
        {0x0a, 0x01, 0x52, 0x00}, {0x0f, 0x01, 0x52, 0x00},
        {0x18, 0x01, 0x52, 0x00}, {0x1f, 0x01, 0x52, 0x00},
        {0x29, 0x01, 0x52, 0x00}, {0x38, 0x01, 0x52, 0x01}
    },
    /* 55 */
    {
        {0x03, 0x01, 0x53, 0x00}, {0x06, 0x01, 0x53, 0x00},
        {0x0a, 0x01, 0x53, 0x00}, {0x0f, 0x01, 0x53, 0x00},
        {0x18, 0x01, 0x53, 0x00}, {0x1f, 0x01, 0x53, 0x00},
        {0x29, 0x01, 0x53, 0x00}, {0x38, 0x01, 0x53, 0x01},
        {0x03, 0x01, 0x54, 0x00}, {0x06, 0x01, 0x54, 0x00},
        {0x0a, 0x01, 0x54, 0x00}, {0x0f, 0x01, 0x54, 0x00},
        {0x18, 0x01, 0x54, 0x00}, {0x1f, 0x01, 0x54, 0x00},
        {0x29, 0x01, 0x54, 0x00}, {0x38, 0x01, 0x54, 0x01}
    },
    {
        {0x00, 0x01, 0x55, 0x01}, {0x00, 0x01, 0x56, 0x01},
        {0x00, 0x01, 0x57, 0x01}, {0x00, 0x01, 0x59, 0x01},
        {0x00, 0x01, 0x6a, 0x01}, {0x00, 0x01, 0x6b, 0x01},
        {0x00, 0x01, 0x71, 0x01}, {0x00, 0x01, 0x76, 0x01},
        {0x00, 0x01, 0x77, 0x01}, {0x00, 0x01, 0x78, 0x01},
        {0x00, 0x01, 0x79, 0x01}, {0x00, 0x01, 0x7a, 0x01},
        {0x46, 0x00, 0x00, 0x00}, {0x47, 0x00, 0x00, 0x00},
        {0x49, 0x00, 0x00, 0x00}, {0x4a, 0x00, 0x00, 0x01}
    },
    {
        {0x01, 0x01, 0x55, 0x00}, {0x16, 0x01, 0x55, 0x01},
        {0x01, 0x01, 0x56, 0x00}, {0x16, 0x01, 0x56, 0x01},
        {0x01, 0x01, 0x57, 0x00}, {0x16, 0x01, 0x57, 0x01},
        {0x01, 0x01, 0x59, 0x00}, {0x16, 0x01, 0x59, 0x01},
        {0x01, 0x01, 0x6a, 0x00}, {0x16, 0x01, 0x6a, 0x01},
        {0x01, 0x01, 0x6b, 0x00}, {0x16, 0x01, 0x6b, 0x01},
        {0x01, 0x01, 0x71, 0x00}, {0x16, 0x01, 0x71, 0x01},
        {0x01, 0x01, 0x76, 0x00}, {0x16, 0x01, 0x76, 0x01}
    },
    {
        {0x02, 0x01, 0x55, 0x00}, {0x09, 0x01, 0x55, 0x00},
        {0x17, 0x01, 0x55, 0x00}, {0x28, 0x01, 0x55, 0x01},
        {0x02, 0x01, 0x56, 0x00}, {0x09, 0x01, 0x56, 0x00},
        {0x17, 0x01, 0x56, 0x00}, {0x28, 0x01, 0x56, 0x01},
        {0x02, 0x01, 0x57, 0x00}, {0x09, 0x01, 0x57, 0x00},
        {0x17, 0x01, 0x57, 0x00}, {0x28, 0x01, 0x57, 0x01},
        {0x02, 0x01, 0x59, 0x00}, {0x09, 0x01, 0x59, 0x00},
        {0x17, 0x01, 0x59, 0x00}, {0x28, 0x01, 0x59, 0x01}
    },
    {
        {0x03, 0x01, 0x55, 0x00}, {0x06, 0x01, 0x55, 0x00},
        {0x0a, 0x01, 0x55, 0x00}, {0x0f, 0x01, 0x55, 0x00},
        {0x18, 0x01, 0x55, 0x00}, {0x1f, 0x01, 0x55, 0x00},
        {0x29, 0x01, 0x55, 0x00}, {0x38, 0x01, 0x55, 0x01},
        {0x03, 0x01, 0x56, 0x00}, {0x06, 0x01, 0x56, 0x00},
        {0x0a, 0x01, 0x56, 0x00}, {0x0f, 0x01, 0x56, 0x00},
        {0x18, 0x01, 0x56, 0x00}, {0x1f, 0x01, 0x56, 0x00},
        {0x29, 0x01, 0x56, 0x00}, {0x38, 0x01, 0x56, 0x01}
    },
    /* 60 */
    {
        {0x03, 0x01, 0x57, 0x00}, {0x06, 0x01, 0x57, 0x00},
        {0x0a, 0x01, 0x57, 0x00}, {0x0f, 0x01, 0x57, 0x00},
        {0x18, 0x01, 0x57, 0x00}, {0x1f, 0x01, 0x57, 0x00},
        {0x29, 0x01, 0x57, 0x00}, {0x38, 0x01, 0x57, 0x01},
        {0x03, 0x01, 0x59, 0x00}, {0x06, 0x01, 0x59, 0x00},
        {0x0a, 0x01, 0x59, 0x00}, {0x0f, 0x01, 0x59, 0x00},
        {0x18, 0x01, 0x59, 0x00}, {0x1f, 0x01, 0x59, 0x00},
        {0x29, 0x01, 0x59, 0x00}, {0x38, 0x01, 0x59, 0x01}
    },
    {
        {0x02, 0x01, 0x6a, 0x00}, {0x09, 0x01, 0x6a, 0x00},
        {0x17, 0x01, 0x6a, 0x00}, {0x28, 0x01, 0x6a, 0x01},
        {0x02, 0x01, 0x6b, 0x00}, {0x09, 0x01, 0x6b, 0x00},
        {0x17, 0x01, 0x6b, 0x00}, {0x28, 0x01, 0x6b, 0x01},
        {0x02, 0x01, 0x71, 0x00}, {0x09, 0x01, 0x71, 0x00},
        {0x17, 0x01, 0x71, 0x00}, {0x28, 0x01, 0x71, 0x01},
        {0x02, 0x01, 0x76, 0x00}, {0x09, 0x01, 0x76, 0x00},
        {0x17, 0x01, 0x76, 0x00}, {0x28, 0x01, 0x76, 0x01}
    },
    {
        {0x03, 0x01, 0x6a, 0x00}, {0x06, 0x01, 0x6a, 0x00},
        {0x0a, 0x01, 0x6a, 0x00}, {0x0f, 0x01, 0x6a, 0x00},
        {0x18, 0x01, 0x6a, 0x00}, {0x1f, 0x01, 0x6a, 0x00},
        {0x29, 0x01, 0x6a, 0x00}, {0x38, 0x01, 0x6a, 0x01},
        {0x03, 0x01, 0x6b, 0x00}, {0x06, 0x01, 0x6b, 0x00},
        {0x0a, 0x01, 0x6b, 0x00}, {0x0f, 0x01, 0x6b, 0x00},
        {0x18, 0x01, 0x6b, 0x00}, {0x1f, 0x01, 0x6b, 0x00},
        {0x29, 0x01, 0x6b, 0x00}, {0x38, 0x01, 0x6b, 0x01}
    },
    {
        {0x03, 0x01, 0x71, 0x00}, {0x06, 0x01, 0x71, 0x00},
        {0x0a, 0x01, 0x71, 0x00}, {0x0f, 0x01, 0x71, 0x00},
        {0x18, 0x01, 0x71, 0x00}, {0x1f, 0x01, 0x71, 0x00},
        {0x29, 0x01, 0x71, 0x00}, {0x38, 0x01, 0x71, 0x01},
        {0x03, 0x01, 0x76, 0x00}, {0x06, 0x01, 0x76, 0x00},
        {0x0a, 0x01, 0x76, 0x00}, {0x0f, 0x01, 0x76, 0x00},
        {0x18, 0x01, 0x76, 0x00}, {0x1f, 0x01, 0x76, 0x00},
        {0x29, 0x01, 0x76, 0x00}, {0x38, 0x01, 0x76, 0x01}
    },
    {
        {0x01, 0x01, 0x77, 0x00}, {0x16, 0x01, 0x77, 0x01},
        {0x01, 0x01, 0x78, 0x00}, {0x16, 0x01, 0x78, 0x01},
        {0x01, 0x01, 0x79, 0x00}, {0x16, 0x01, 0x79, 0x01},
        {0x01, 0x01, 0x7a, 0x00}, {0x16, 0x01, 0x7a, 0x01},
        {0x00, 0x01, 0x26, 0x01}, {0x00, 0x01, 0x2a, 0x01},
        {0x00, 0x01, 0x2c, 0x01}, {0x00, 0x01, 0x3b, 0x01},
        {0x00, 0x01, 0x58, 0x01}, {0x00, 0x01, 0x5a, 0x01},
        {0x4b, 0x00, 0x00, 0x00}, {0x4e, 0x00, 0x00, 0x01}
    },
    /* 65 */
    {
        {0x02, 0x01, 0x77, 0x00}, {0x09, 0x01, 0x77, 0x00},
        {0x17, 0x01, 0x77, 0x00}, {0x28, 0x01, 0x77, 0x01},
        {0x02, 0x01, 0x78, 0x00}, {0x09, 0x01, 0x78, 0x00},
        {0x17, 0x01, 0x78, 0x00}, {0x28, 0x01, 0x78, 0x01},
        {0x02, 0x01, 0x79, 0x00}, {0x09, 0x01, 0x79, 0x00},
        {0x17, 0x01, 0x79, 0x00}, {0x28, 0x01, 0x79, 0x01},
        {0x02, 0x01, 0x7a, 0x00}, {0x09, 0x01, 0x7a, 0x00},
        {0x17, 0x01, 0x7a, 0x00}, {0x28, 0x01, 0x7a, 0x01}
    },
    {
        {0x03, 0x01, 0x77, 0x00}, {0x06, 0x01, 0x77, 0x00},
        {0x0a, 0x01, 0x77, 0x00}, {0x0f, 0x01, 0x77, 0x00},
        {0x18, 0x01, 0x77, 0x00}, {0x1f, 0x01, 0x77, 0x00},
        {0x29, 0x01, 0x77, 0x00}, {0x38, 0x01, 0x77, 0x01},
        {0x03, 0x01, 0x78, 0x00}, {0x06, 0x01, 0x78, 0x00},
        {0x0a, 0x01, 0x78, 0x00}, {0x0f, 0x01, 0x78, 0x00},
        {0x18, 0x01, 0x78, 0x00}, {0x1f, 0x01, 0x78, 0x00},
        {0x29, 0x01, 0x78, 0x00}, {0x38, 0x01, 0x78, 0x01}
    },
    {
        {0x03, 0x01, 0x79, 0x00}, {0x06, 0x01, 0x79, 0x00},
        {0x0a, 0x01, 0x79, 0x00}, {0x0f, 0x01, 0x79, 0x00},
        {0x18, 0x01, 0x79, 0x00}, {0x1f, 0x01, 0x79, 0x00},
        {0x29, 0x01, 0x79, 0x00}, {0x38, 0x01, 0x79, 0x01},
        {0x03, 0x01, 0x7a, 0x00}, {0x06, 0x01, 0x7a, 0x00},
        {0x0a, 0x01, 0x7a, 0x00}, {0x0f, 0x01, 0x7a, 0x00},
        {0x18, 0x01, 0x7a, 0x00}, {0x1f, 0x01, 0x7a, 0x00},
        {0x29, 0x01, 0x7a, 0x00}, {0x38, 0x01, 0x7a, 0x01}
    },
    {
        {0x01, 0x01, 0x26, 0x00}, {0x16, 0x01, 0x26, 0x01},
        {0x01, 0x01, 0x2a, 0x00}, {0x16, 0x01, 0x2a, 0x01},
        {0x01, 0x01, 0x2c, 0x00}, {0x16, 0x01, 0x2c, 0x01},
        {0x01, 0x01, 0x3b, 0x00}, {0x16, 0x01, 0x3b, 0x01},
        {0x01, 0x01, 0x58, 0x00}, {0x16, 0x01, 0x58, 0x01},
        {0x01, 0x01, 0x5a, 0x00}, {0x16, 0x01, 0x5a, 0x01},
        {0x4c, 0x00, 0x00, 0x00}, {0x4d, 0x00, 0x00, 0x00},
        {0x4f, 0x00, 0x00, 0x00}, {0x51, 0x00, 0x00, 0x01}
    },
    {
        {0x02, 0x01, 0x26, 0x00}, {0x09, 0x01, 0x26, 0x00},
        {0x17, 0x01, 0x26, 0x00}, {0x28, 0x01, 0x26, 0x01},
        {0x02, 0x01, 0x2a, 0x00}, {0x09, 0x01, 0x2a, 0x00},
        {0x17, 0x01, 0x2a, 0x00}, {0x28, 0x01, 0x2a, 0x01},
        {0x02, 0x01, 0x2c, 0x00}, {0x09, 0x01, 0x2c, 0x00},
        {0x17, 0x01, 0x2c, 0x00}, {0x28, 0x01, 0x2c, 0x01},
        {0x02, 0x01, 0x3b, 0x00}, {0x09, 0x01, 0x3b, 0x00},
        {0x17, 0x01, 0x3b, 0x00}, {0x28, 0x01, 0x3b, 0x01}
    },
    /* 70 */
    {
        {0x03, 0x01, 0x26, 0x00}, {0x06, 0x01, 0x26, 0x00},
        {0x0a, 0x01, 0x26, 0x00}, {0x0f, 0x01, 0x26, 0x00},
        {0x18, 0x01, 0x26, 0x00}, {0x1f, 0x01, 0x26, 0x00},
        {0x29, 0x01, 0x26, 0x00}, {0x38, 0x01, 0x26, 0x01},
        {0x03, 0x01, 0x2a, 0x00}, {0x06, 0x01, 0x2a, 0x00},
        {0x0a, 0x01, 0x2a, 0x00}, {0x0f, 0x01, 0x2a, 0x00},
        {0x18, 0x01, 0x2a, 0x00}, {0x1f, 0x01, 0x2a, 0x00},
        {0x29, 0x01, 0x2a, 0x00}, {0x38, 0x01, 0x2a, 0x01}
    },
    {
        {0x03, 0x01, 0x2c, 0x00}, {0x06, 0x01, 0x2c, 0x00},
        {0x0a, 0x01, 0x2c, 0x00}, {0x0f, 0x01, 0x2c, 0x00},
        {0x18, 0x01, 0x2c, 0x00}, {0x1f, 0x01, 0x2c, 0x00},
        {0x29, 0x01, 0x2c, 0x00}, {0x38, 0x01, 0x2c, 0x01},
        {0x03, 0x01, 0x3b, 0x00}, {0x06, 0x01, 0x3b, 0x00},
        {0x0a, 0x01, 0x3b, 0x00}, {0x0f, 0x01, 0x3b, 0x00},
        {0x18, 0x01, 0x3b, 0x00}, {0x1f, 0x01, 0x3b, 0x00},
        {0x29, 0x01, 0x3b, 0x00}, {0x38, 0x01, 0x3b, 0x01}
    },
    {
        {0x02, 0x01, 0x58, 0x00}, {0x09, 0x01, 0x58, 0x00},
        {0x17, 0x01, 0x58, 0x00}, {0x28, 0x01, 0x58, 0x01},
        {0x02, 0x01, 0x5a, 0x00}, {0x09, 0x01, 0x5a, 0x00},
        {0x17, 0x01, 0x5a, 0x00}, {0x28, 0x01, 0x5a, 0x01},
        {0x00, 0x01, 0x21, 0x01}, {0x00, 0x01, 0x22, 0x01},
        {0x00, 0x01, 0x28, 0x01}, {0x00, 0x01, 0x29, 0x01},
        {0x00, 0x01, 0x3f, 0x01}, {0x50, 0x00, 0x00, 0x00},
        {0x52, 0x00, 0x00, 0x00}, {0x54, 0x00, 0x00, 0x01}
    },
    {
        {0x03, 0x01, 0x58, 0x00}, {0x06, 0x01, 0x58, 0x00},
        {0x0a, 0x01, 0x58, 0x00}, {0x0f, 0x01, 0x58, 0x00},
        {0x18, 0x01, 0x58, 0x00}, {0x1f, 0x01, 0x58, 0x00},
        {0x29, 0x01, 0x58, 0x00}, {0x38, 0x01, 0x58, 0x01},
        {0x03, 0x01, 0x5a, 0x00}, {0x06, 0x01, 0x5a, 0x00},
        {0x0a, 0x01, 0x5a, 0x00}, {0x0f, 0x01, 0x5a, 0x00},
        {0x18, 0x01, 0x5a, 0x00}, {0x1f, 0x01, 0x5a, 0x00},
        {0x29, 0x01, 0x5a, 0x00}, {0x38, 0x01, 0x5a, 0x01}
    },
    {
        {0x01, 0x01, 0x21, 0x00}, {0x16, 0x01, 0x21, 0x01},
        {0x01, 0x01, 0x22, 0x00}, {0x16, 0x01, 0x22, 0x01},
        {0x01, 0x01, 0x28, 0x00}, {0x16, 0x01, 0x28, 0x01},
        {0x01, 0x01, 0x29, 0x00}, {0x16, 0x01, 0x29, 0x01},
        {0x01, 0x01, 0x3f, 0x00}, {0x16, 0x01, 0x3f, 0x01},
        {0x00, 0x01, 0x27, 0x01}, {0x00, 0x01, 0x2b, 0x01},
        {0x00, 0x01, 0x7c, 0x01}, {0x53, 0x00, 0x00, 0x00},
        {0x55, 0x00, 0x00, 0x00}, {0x58, 0x00, 0x00, 0x01}
    },
    /* 75 */
    {
        {0x02, 0x01, 0x21, 0x00}, {0x09, 0x01, 0x21, 0x00},
        {0x17, 0x01, 0x21, 0x00}, {0x28, 0x01, 0x21, 0x01},
        {0x02, 0x01, 0x22, 0x00}, {0x09, 0x01, 0x22, 0x00},
        {0x17, 0x01, 0x22, 0x00}, {0x28, 0x01, 0x22, 0x01},
        {0x02, 0x01, 0x28, 0x00}, {0x09, 0x01, 0x28, 0x00},
        {0x17, 0x01, 0x28, 0x00}, {