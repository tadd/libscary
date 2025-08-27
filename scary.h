#ifndef SCARY_H
#define SCARY_H

#include <stddef.h>
#include <stdint.h>

#define SCARY_NONE()
#define SCARY_COMMA() ,
#define SCARY_FOR_T(f, t) f(t##_t, t)
#define SCARY_FOR_1(f, t) f(t, t)
#define SCARY_FOR_DATA_DELIMITED(f, d) \
    SCARY_FOR_1(f, char) d() \
    SCARY_FOR_T(f, int8) d() \
    SCARY_FOR_T(f, int16) d() \
    SCARY_FOR_T(f, int32) d() \
    SCARY_FOR_T(f, int64) d() \
    SCARY_FOR_T(f, uint8) d() \
    SCARY_FOR_T(f, uint16) d() \
    SCARY_FOR_T(f, uint32) d() \
    SCARY_FOR_T(f, uint64)
#define SCARY_FOR_DATA_COMMA(f) SCARY_FOR_DATA_DELIMITED(f, SCARY_COMMA)
#define SCARY_FOR_DATA(f) SCARY_FOR_DATA_DELIMITED(f, SCARY_NONE)
#define SCARY_FOR_PTRS_DELIMITED(f, d) SCARY_FOR_1(f, void) d() SCARY_FOR_DATA_DELIMITED(f, d)
#define SCARY_FOR_PTRS_COMMA(f) SCARY_FOR_1(f, void), SCARY_FOR_DATA_COMMA(f)
#define SCARY_FOR_PTRS(f) SCARY_FOR_PTRS_DELIMITED(f, SCARY_NONE)

void *scary_new(size_t size);
void *scary_new_sized(size_t nmemb, size_t size);
void scary_free(void *ary);
size_t scary_length(const void *ary);
void scary_pop(void *ary);

#ifdef __APPLE__
#define SCARY_PUSH_ARCHDEP_PATTERN() unsigned long: scary_push_uint64,
#else
#define SCARY_PUSH_ARCHDEP_PATTERN() //empty
#endif
#define SCARY_PUSH_PATTERN(ty, sf) ty: scary_push_##sf,
#define SCARY_PUSH_PATTERN_PTR(ty, sf) ty *: scary_push_##sf##p,

#define scary_push(pary, elem) \
    _Generic(elem, \
        SCARY_PUSH_ARCHDEP_PATTERN() \
        SCARY_FOR_DATA(SCARY_PUSH_PATTERN) \
        SCARY_FOR_PTRS(SCARY_PUSH_PATTERN_PTR) \
        const char *: scary_push_ccharp)(pary, elem)

#define SCARY_PUSH_DATA_DECL(ty, sf) void scary_push_##sf(ty **, ty);
#define SCARY_PUSH_PTR_DECL(ty, sf) void scary_push_##sf##p(ty ***, const ty *);
SCARY_FOR_DATA(SCARY_PUSH_DATA_DECL)
SCARY_FOR_PTRS(SCARY_PUSH_PTR_DECL)
void scary_push_ccharp(const char ***, const char *);

#define SCARY_DUP_PATTERN(ty, sf) ty *: scary_dup_##sf
#define scary_dup(p) _Generic(p, SCARY_FOR_PTRS_COMMA(SCARY_DUP_PATTERN))(p)
#define SCARY_DUP_DECL(ty, sf) ty *scary_dup_##sf(const ty *);
SCARY_FOR_PTRS(SCARY_DUP_DECL)

#endif
