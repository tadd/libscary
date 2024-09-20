#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef POSIXLY_CORRUPT
#include <unistd.h>
#include <sys/types.h>
#endif

#include "scary.h"

#define NONNULL(p) do { \
        errno = 0; \
        void *q = (p); \
        if (q == NULL) { \
            perror("scary"); \
            abort(); \
        } \
    } while (0)

static inline void *xmalloc(size_t size)
{
    void *p;
    NONNULL(p = malloc(size));
    return p;
}

#ifndef POSIXLY_CORRUPT
static inline void *xrealloc(void *p0, size_t size)
{
    void *p;
    NONNULL(p = realloc(p0, size));
    return p;
}
#endif

typedef struct {
#ifdef POSIXLY_CORRUPT
    FILE *stream;
    size_t capacity, elem_size;
    void *space;
#else
    size_t capacity, length, elem_size;
    uint8_t space[];
#endif
} Scary;

enum {
    SCARY_OFFSET = offsetof(Scary, space),
    SCARY_INIT = 8,
    SCARY_INC_RATIO = 2,
};

static inline void *opaque(Scary *a)
{
    uint8_t *bp = (void *) a;
    return bp + SCARY_OFFSET;
}

static inline Scary *get(const void *p)
{
    const uint8_t *bp = p;
    return (Scary *) (bp - SCARY_OFFSET);
}

void *scary_new(size_t elem_size)
{
#ifdef POSIXLY_CORRUPT
    Scary *ary = xmalloc(sizeof(Scary) + sizeof(void *));
    ary->elem_size = elem_size;
    ary->stream = open_memstream((char **) &ary->space, &ary->capacity);
    return opaque(ary);
#else
    size_t cap = elem_size * SCARY_INIT;
    Scary *ary = xmalloc(sizeof(Scary) + cap);
    ary->capacity = cap;
    ary->length = 0;
    ary->elem_size = elem_size;
    return opaque(ary);
#endif
}

void scary_free(void *p)
{
    if (p != NULL)
        free(get(p));
}

static Scary *maybe_resize(void *p)
{
    const void **pp = p;
    Scary *ary = get(*pp);
#ifndef POSIXLY_CORRUPT
    if (ary->capacity <= ary->length * ary->elem_size) {
        ary->capacity *= SCARY_INC_RATIO;
        ary = xrealloc(ary, sizeof(Scary) + ary->capacity);
        *pp = opaque(ary);
    }
#endif
    return ary;
}

size_t scary_length(const void *p)
{
#ifdef POSIXLY_CORRUPT
    const Scary *ary = get(p);
    return ary->capacity / ary->elem_size;
#else
    return get(p)->length;
#endif
}

#define FOR_T(f, t) f(t##_t, t)
#define FOR_1(f, t) f(t, t)
#define FOR_DATA(f) \
    FOR_1(f, char) \
    FOR_T(f, int8) \
    FOR_T(f, int16) \
    FOR_T(f, int32) \
    FOR_T(f, int64) \
    FOR_T(f, uint8) \
    FOR_T(f, uint16) \
    FOR_T(f, uint32) \
    FOR_T(f, uint64)
#define FOR_PTRS(f) FOR_1(f, void) FOR_DATA(f)

#ifdef POSIXLY_CORRUPT
#define DEF_PUSH_VARIANT(type, suffix) \
    void scary_push_##suffix(type **p, type elem) \
    { \
        Scary *ary = maybe_resize(p); \
        fwrite(&elem, ary->elem_size, 1, ary->stream); \
        fflush(ary->stream); \
    }
#else
#define DEF_PUSH_VARIANT(type, suffix) \
    void scary_push_##suffix(type **p, type elem) \
    { \
        Scary *ary = maybe_resize(p); \
        type *sp = (type *) ary->space; \
        sp[ary->length++] = elem; \
    }
#endif
FOR_DATA(DEF_PUSH_VARIANT)

static void scary_push_ptr(void *p, const void *elem)
{
    Scary *ary = maybe_resize(p);
#ifdef POSIXLY_CORRUPT
    size_t n = fwrite(elem, ary->elem_size, 1, ary->stream);
    fprintf(stderr, "wrote: %zu\n", n);
    fflush(ary->stream);
#else
    const void **sp = (const void **) ary->space;
    sp[ary->length++] = elem;
#endif
}
#define DEF_PUSH_VARIANT_PTR(type, suffix) \
    void scary_push_##suffix##p(type ***p, const type *elem) \
    { \
        scary_push_ptr(p, elem); \
    }
FOR_PTRS(DEF_PUSH_VARIANT_PTR)

void scary_push_ccharp(const char ***p, const char *elem)
{
    scary_push_ptr(p, elem);
}

void scary_pop(void *p)
{
#ifdef POSIXLY_CORRUPT
    Scary *ary = get(p);
    fseek(ary->stream, -ary->elem_size, SEEK_CUR);
    ftruncate(fileno(ary->stream), ary->capacity - ary->elem_size);
    fflush(ary->stream);
#else
    get(p)->length--; // do not shrink for speed
#endif
}

void *scary_dup_(const void *p)
{
    Scary *ary = get(p);
    Scary *dup = xmalloc(sizeof(Scary) + ary->capacity);
    *dup = *ary;
    memcpy(dup->space, ary->space, ary->capacity);
    return opaque(dup);
}

#define DEF_DUP_VARIANT(type, suffix) \
    type *scary_dup_##suffix(const type *p) \
    { \
        return scary_dup_(p); \
    }
FOR_PTRS(DEF_DUP_VARIANT)
