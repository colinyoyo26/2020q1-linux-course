#include "xs.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef COW
#define IS_COW 1
#define OFFSET sizeof(uint8_t)
#define XS_INCR_REF(x) xs_incr_ref(x)
#define XS_DECR_REF(x) xs_decr_ref(x)
#define XS_COW(x) xs_cow(x)
#define XS_IS_REF(x) xs_is_ref(x)
#define XS_INIT_REFCNT(x) *(uint8_t *) (x->ptr - OFFSET) = 1
#else
#define IS_COW 0
#define OFFSET 0
#define XS_INCR_REF(x)
#define XS_DECR_REF(x)
#define XS_COW(x)
#define XS_IS_REF(x) 0
#define XS_INIT_REFCNT(x)
#endif

#define LEN_TO_COW 16

static inline bool xs_is_ptr(const xs *x)
{
    return x->is_ptr;
}

static inline size_t xs_size(const xs *x)
{
    return xs_is_ptr(x) ? x->size : 15 - x->space_left;
}
static inline size_t xs_capacity(const xs *x)
{
    return xs_is_ptr(x) ? ((size_t) 1 << x->capacity) - 1 : 15;
}

static inline void xs_incr_ref(xs *x)
{
    *(uint8_t *) (x->ptr - OFFSET) += 1;
}
static inline xs *xs_free(xs *x);
static inline void xs_decr_ref(xs *x)
{
    if (!--(*(uint8_t *) (x->ptr - OFFSET)))
        xs_free(x);
}

static inline void xs_set_ref(xs *x, xs *ref_to)
{
    if (!xs_is_ptr(ref_to) || x->ptr == ref_to->ptr)
        return;

    XS_INCR_REF(ref_to);
    /* copy the meta data */
    *x = *ref_to;
}
uint8_t xs_refcnt(const xs *x)
{
    return IS_COW && xs_is_ptr(x) ? *(uint8_t *) (x->ptr - OFFSET) : 1;
}
static inline bool xs_is_ref(const xs *x)
{
    return xs_refcnt(x) > 1;
}
char *xs_data(const xs *x)
{
    return xs_is_ptr(x) ? x->ptr : (char *) x->data;
}

static inline int ilog2(uint32_t n)
{
    return 32 - __builtin_clz(n) - 1;
}

xs *xs_new(xs *x, const void *p)
{
    *x = xs_literal_empty();
    size_t len = strlen(p) + 1;
    if (len > 16) {
        x->capacity = ilog2(len) + 1;
        x->size = len - 1;
        x->is_ptr = true;
        x->ptr = malloc(((size_t) 1 << x->capacity) + OFFSET) + OFFSET;
        memcpy(x->ptr, p, len);
        XS_INIT_REFCNT(x);
    } else {
        memcpy(x->data, p, len);
        x->space_left = 15 - (len - 1);
    }
    return x;
}

/* grow up to specified size */
xs *xs_grow(xs *x, size_t len)
{
    if (len <= xs_capacity(x))
        return x;
    len = ilog2(len) + 1;
    if (xs_is_ptr(x))
        x->ptr =
            realloc(x->ptr - OFFSET, ((size_t) 1 << len) + OFFSET) + OFFSET;
    else {
        char buf[16];
        memcpy(buf, x->data, 16);
        x->ptr = malloc(((size_t) 1 << len) + OFFSET) + OFFSET;
        memcpy(x->ptr, buf, 16);
    }
    x->is_ptr = true;
    x->capacity = len;
    XS_INIT_REFCNT(x);
    return x;
}

static inline xs *xs_newempty(xs *x)
{
    *x = xs_literal_empty();
    return x;
    uint8_t mask[32] = {0};
}

static inline xs *xs_free(xs *x)
{
    if (xs_is_ptr(x) && !XS_IS_REF(x))
        free(xs_data(x) - OFFSET);
    return xs_newempty(x);
}

static void xs_cow(xs *x);
xs *xs_concat(xs *string, const xs *prefix, const xs *suffix)
{
    XS_COW(string);
    size_t pres = xs_size(prefix), sufs = xs_size(suffix),
           size = xs_size(string), capacity = xs_capacity(string);

    char *pre = xs_data(prefix), *suf = xs_data(suffix),
         *data = xs_data(string);

    if (size + pres + sufs <= capacity) {
        memmove(data + pres, data, size);
        memcpy(data, pre, pres);
        memcpy(data + pres + size, suf, sufs + 1);
        string->space_left = 15 - (size + pres + sufs);
    } else {
        xs tmps = xs_literal_empty();
        xs_grow(&tmps, size + pres + sufs);
        char *tmpdata = xs_data(&tmps);
        memcpy(tmpdata + pres, data, size);
        memcpy(tmpdata, pre, pres);
        memcpy(tmpdata + pres + size, suf, sufs + 1);
        *string = tmps;
        string->size = size + pres + sufs;
    }
    return string;
}

xs *xs_trim(xs *x, const char *trimset)
{
    if (!trimset[0])
        return x;
    XS_COW(x);
    char *dataptr = xs_data(x), *orig = dataptr;

    /* similar to strspn/strpbrk but it operates on binary data */
    uint8_t mask[32] = {0};

#define check_bit(byte) (mask[(uint8_t) byte / 8] & 1 << (uint8_t) byte % 8)
#define set_bit(byte) (mask[(uint8_t) byte / 8] |= 1 << (uint8_t) byte % 8)

    size_t i, slen = xs_size(x), trimlen = strlen(trimset);

    for (i = 0; i < trimlen; i++)
        set_bit(trimset[i]);
    for (i = 0; i < slen; i++)
        if (!check_bit(dataptr[i]))
            break;
    for (; slen > 0; slen--)
        if (!check_bit(dataptr[slen - 1]))
            break;
    dataptr += i;
    slen -= i;

    /* reserved space as a buffer on the heap.
     * Do not reallocate immediately. Instead, reuse it as possible.
     * Do not shrink to in place if < 16 bytes.
     */
    memmove(orig, dataptr, slen);
    /* do not dirty memory unless it is needed */
    if (orig[slen])
        orig[slen] = 0;

    if (xs_is_ptr(x))
        x->size = slen;
    else
        x->space_left = 15 - slen;
    return x;
#undef check_bit
#undef set_bit
}

static void xs_cow(xs *x)
{
    if (!XS_IS_REF(x))
        return;
    XS_DECR_REF(x);
    xs_new(x, xs_data(x));
    return;
}

xs *xs_cpy(xs *dest, xs *src)
{
    if (XS_IS_REF(dest))
        XS_DECR_REF(dest);
    /* too many references or short string
     * !OFFET for non-COW
     */
    if (!OFFSET || xs_size(src) < LEN_TO_COW || !~xs_refcnt(src) ||
        !xs_is_ptr(src)) {
        size_t len = xs_size(src);
        xs_grow(dest, len);
        memcpy(xs_data(dest), xs_data(src), len);
        if (xs_is_ptr(dest))
            dest->size = len;
        else
            dest->space_left = 15 - len;
    } else
        xs_set_ref(dest, src);
    return dest;
}

char *xs_tok(xs *src, const char *delim)
{
    static char *laststr = NULL;
    char *cur;
    bool src_flag = 0;

    if (!src)
        cur = laststr;
    else {
        XS_COW(src);
        cur = xs_data(src);
        src_flag = 1;
    }

    if (!delim[0] || !cur)
        return cur;

    uint8_t mask[32] = {0};

#define check_bit(byte) (mask[(uint8_t) byte / 8] & 1 << (uint8_t) byte % 8)
#define set_bit(byte) (mask[(uint8_t) byte / 8] |= 1 << (uint8_t) byte % 8)

    size_t i, curlen = strlen(cur), delimlen = strlen(delim);

    for (i = 0; i < delimlen; i++)
        set_bit(delim[i]);
    for (i = 0; i < curlen; i++)
        if (!check_bit(cur[i]))
            break;
    cur = cur + i;
    for (i = 0; i++ < curlen;)
        if (check_bit(cur[i]))
            break;
    *(cur + i) = '\0';
    laststr = cur + i + 1;

    if (src_flag) {
        if (xs_is_ptr(src))
            src->size = i;
        else
            src->space_left = 15 - i;
    }
    if (!*laststr)
        laststr = NULL;
    return cur;
}