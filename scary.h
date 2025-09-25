#ifndef SCARY_H
#define SCARY_H

#include <stddef.h>
#include <stdint.h>

void *scary_new(size_t size);
void *scary_new_sized(size_t nmemb, size_t size);
void scary_free(void *ary);
size_t scary_length(const void *ary);
void scary_pop(void *ary);

#define scary_push(pary, elem) \
    _Generic(elem, \
        char: scary_push_char, \
        short: scary_push_short, \
        int: scary_push_int, \
        long: scary_push_long, \
        long long: scary_push_longlong, \
        unsigned char: scary_push_uchar, \
        unsigned short: scary_push_ushort, \
        unsigned int: scary_push_uint, \
        unsigned long: scary_push_ulong, \
        unsigned long long: scary_push_ulonglong, \
        char*: scary_push_charp, \
        const char *: scary_push_ccharp, \
        short*: scary_push_shortp, \
        int*: scary_push_intp, \
        long*: scary_push_longp, \
        long long*: scary_push_longlongp, \
        unsigned char*: scary_push_ucharp, \
        unsigned short*: scary_push_ushortp, \
        unsigned int*: scary_push_uintp, \
        unsigned long*: scary_push_ulongp, \
        unsigned long long*: scary_push_ulonglongp, \
        void *: scary_push_voidp)(pary, elem)

void scary_push_char(char **, char);
void scary_push_short(short **, short);
void scary_push_int(int **, int);
void scary_push_long(long **, long);
void scary_push_longlong(long long **, long long);
void scary_push_uchar(unsigned char **, unsigned char);
void scary_push_ushort(unsigned short **, unsigned short);
void scary_push_uint(unsigned int **, unsigned int);
void scary_push_ulong(unsigned long **, unsigned long);
void scary_push_ulonglong(unsigned long long **, unsigned long long);
void scary_push_charp(char ***, const char *);
void scary_push_ccharp(const char ***, const char *);
void scary_push_voidp(void ***, const void *);
void scary_push_shortp(short ***, const short *);
void scary_push_intp(int ***, const int *);
void scary_push_longp(long ***, const long *);
void scary_push_longlongp(long long ***, const long long *);
void scary_push_ucharp(unsigned char ***, const unsigned char *);
void scary_push_ushortp(unsigned short ***, const unsigned short *);
void scary_push_uintp(unsigned int ***, const unsigned int *);
void scary_push_ulongp(unsigned long ***, const unsigned long *);
void scary_push_ulonglongp(unsigned long long ***, const unsigned long long *);

#define scary_dup(p) \
    _Generic(p, \
        char *: scary_dup_char, \
        short *: scary_dup_short, \
        int *: scary_dup_int, \
        long *: scary_dup_long, \
        long long *: scary_dup_longlong, \
        unsigned char *: scary_dup_uchar, \
        unsigned short *: scary_dup_ushort, \
        unsigned int *: scary_dup_uint, \
        unsigned long *: scary_dup_ulong, \
        unsigned long long *: scary_dup_ulonglong, \
        void *: scary_dup_void)(p)

void *scary_dup_void(const void *);
char *scary_dup_char(const char *);
short *scary_dup_short(const short *);
int *scary_dup_int(const int *);
long *scary_dup_long(const long *);
long long *scary_dup_longlong(const long long *);
unsigned char *scary_dup_uchar(const unsigned char *);
unsigned short *scary_dup_ushort(const unsigned short *);
unsigned int *scary_dup_uint(const unsigned int *);
unsigned long *scary_dup_ulong(const unsigned long *);
unsigned long long *scary_dup_ulonglong(const unsigned long long *);

#endif
