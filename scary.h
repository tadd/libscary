#ifndef SCARY_H
#define SCARY_H

#include <stddef.h>
#include <stdint.h>

void *scary_new(size_t size);
void scary_free(void *ary);
size_t scary_length(const void *ary);
void scary_push_ref(void *ary, const void *elem);
#ifdef __APPLE__
#define scary_push_archdep_pattern() unsigned long: scary_push_uint64,
#else
#define scary_push_archdep_pattern() //empty
#endif
#define scary_push(pary, elem) \
    _Generic(elem, \
        scary_push_archdep_pattern() \
        int8_t: scary_push_int8, \
        int16_t: scary_push_int16, \
        int32_t: scary_push_int32, \
        int64_t: scary_push_int64, \
        char: scary_push_char, \
        uint8_t: scary_push_uint8, \
        uint16_t: scary_push_uint16, \
        uint32_t: scary_push_uint32, \
        uint64_t: scary_push_uint64, \
        char *: scary_push_ptr, \
        const char *: scary_push_ptr, \
        const char **: scary_push_ptr, \
        int8_t *: scary_push_ptr, \
        int16_t *: scary_push_ptr, \
        int32_t *: scary_push_ptr, \
        int64_t *: scary_push_ptr, \
        uint8_t *: scary_push_ptr, \
        uint16_t *: scary_push_ptr, \
        uint32_t *: scary_push_ptr, \
        uint64_t *: scary_push_ptr, \
        void *: scary_push_ptr)(pary, elem)
void scary_push_char(char **, char);
void scary_push_int8(int8_t **, int8_t);
void scary_push_int16(int16_t **, int16_t);
void scary_push_int32(int32_t **, int32_t);
void scary_push_int64(int64_t **, int64_t);
void scary_push_uint8(uint8_t **, uint8_t);
void scary_push_uint16(uint16_t **, uint16_t);
void scary_push_uint32(uint32_t **, uint32_t);
void scary_push_uint64(uint64_t **, uint64_t);
void scary_push_ptr(void *, const void *);
void scary_pop(void *ary);
void *scary_dup(void *ary);

#endif
