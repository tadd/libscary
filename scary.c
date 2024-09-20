#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "scary.h"

#define NONNULL(p) do { \
        errno = 0; \
        void *q = (p); \
        if (q == NULL) \
            perror("scary"); \
    } while (0)

#ifndef POSIXLY_CORRUPT
static inline void *xmalloc(size_t size)
{
    void *p;
    NONNULL(p = malloc(size));
    return p;
}

static inline void *xrealloc(void *p0, size_t size)
{
    void *p;
    NONNULL(p = realloc(p0, size));
    return p;
}
#endif

#ifdef POSIXLY_CORRUPT
// [0] = FILE *stream
// [1] = size_t elem_size
typedef uintptr_t Scary;
#else
typedef struct {
    size_t capacity, length, elem_size;
    uint8_t space[];
} Scary;
#endif

enum {
#ifdef POSIXLY_CORRUPT
    SCARY_DISTANCE = sizeof(uintptr_t) * 2,
#else
    SCARY_DISTANCE = offsetof(Scary, space),
#endif
    SCARY_INIT = 8,
};

static inline void *opaque(Scary *a)
{
    uint8_t *bp = (void *) a;
    return bp + SCARY_DISTANCE;
}

static inline Scary *get(void *p)
{
    uint8_t *bp = p;
    return (Scary *) (bp - SCARY_DISTANCE);
}

void *scary_new(size_t elem_size)
{
#ifdef POSIXLY_CORRUPT
    Scary **ary = malloc(sizeof(void *));
    size_t *sz = malloc(sizeof(size_t));
    FILE *stream = open_memstream((char **) ary, sz); 
    FILE *val = stream;
    fwrite(val, sizeof(void *), 1, stream);
    fwrite(&elem_size, sizeof(void *), 1, stream);
    return opaque(*ary);
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
    free(get(p));
}

#ifndef POSIXLY_CORRUPT
static void maybe_resize(Scary **pary)
{
    Scary *ary = *pary;
    if (ary->capacity > ary->length * ary->elem_size)
        return;
    ary->capacity *= 2;
    *pary = xrealloc(ary, sizeof(Scary) + ary->capacity);
}
#endif

size_t scary_length(const void *p)
{
    const Scary *ary = get((void *) p);
#ifdef POSIXLY_CORRUPT
    struct stat st[1];
    
    fstat(fileno((void *) ary[0]), st);
    return st->st_size;
#else
    return ary->length;
#endif
}

void scary_push_ref(void *p, const void *elem)
{
    Scary *ary = get(*(void **) p);
#ifdef POSIXLY_CORRUPT
    size_t elem_size = ary[1];
    fwrite(elem, elem_size, 1, (void *) ary[0]);
#else
    maybe_resize(&ary);
    uint8_t *sp = ary->space + ary->elem_size * ary->length;
    memcpy(sp, elem, ary->elem_size);
    ary->length++;
#endif
    *(void **) p = opaque(ary);
}

#define DEF_PUSH_VARIANT2(type, suffix) \
    void scary_push_##suffix(void *p, type elem) \
    { \
        type tmp = elem; \
        scary_push_ref(p, &tmp); \
    }
#define DEF_PUSH_VARIANT(type) DEF_PUSH_VARIANT2(type##_t, type)

DEF_PUSH_VARIANT(int8)
DEF_PUSH_VARIANT(int16)
DEF_PUSH_VARIANT(int32)
DEF_PUSH_VARIANT(int64)
DEF_PUSH_VARIANT(uint8)
DEF_PUSH_VARIANT(uint16)
DEF_PUSH_VARIANT(uint32)
DEF_PUSH_VARIANT(uint64)
DEF_PUSH_VARIANT2(const void *, ptr)
