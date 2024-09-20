#ifndef SCARY_H
#define SCARY_H

#include <stddef.h>

void *scary_new(size_t size);
void scary_free(void *ary);
size_t scary_length(const void *ary);
void scary_push_ref(void *pary, const void *elem);

#endif
