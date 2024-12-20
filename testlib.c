#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "scary.h"

Test(libscary, push_and_length) {
    int32_t *a = scary_new(sizeof(int32_t));
    cr_expect(eq(sz, 0, scary_length(a)));

    int32_t l[] = { 1, -2, 3, -5, 7, -11 }, *p = l;
    const size_t n = sizeof(l)/sizeof(*l);
    for (size_t i = 0; i < n; i++)
        scary_push_int32(&a, *p++);

    cr_expect(eq(sz, n, scary_length(a)));
    cr_expect(eq(i32, 1, a[0]));
    cr_expect(eq(i32[n], l, a));

    scary_free(a);
}

Test(libscary, push) {
    int *a = scary_new(sizeof(int));
    scary_push(&a, 42);
    cr_expect(eq(sz, 1, scary_length(a)));
    scary_free(a);

    char *b = scary_new(sizeof(char));
    char lb[] = { 'a', 'b' };
    scary_push(&b, lb[0]);
    scary_push(&b, lb[1]);
    cr_expect(eq(sz, 2, scary_length(b)));
    cr_expect(eq(chr, 'a', b[0]));
    cr_expect(eq(chr, 'b', b[1]));
    scary_free(b);
}

Test(libscary, push_ptr) {
    int64_t **a = scary_new(sizeof(int64_t *));
    int64_t la[] = { 1, -2 };
    scary_push(&a, &la[0]);
    scary_push(&a, &la[1]);
    cr_expect(eq(sz, 2, scary_length(a)));
    cr_expect(eq(ptr, &la[0], a[0]));
    cr_expect(eq(ptr, &la[1], a[1]));
    scary_free(a);

    char **b = scary_new(sizeof(char *));
    char *l[] = { "foo", "bar" };
    scary_push(&b, l[0]);
    scary_push(&b, l[1]);
    cr_expect(eq(sz, 2, scary_length(b)));
    cr_expect(eq(str, "foo", b[0]));
    cr_expect(eq(str, "bar", b[1]));
    scary_free(b);
}

Test(libscary, pop) {
    int *a = scary_new(sizeof(int));
    scary_push(&a, 42);
    scary_push(&a, -42);
    scary_pop(a);
    cr_expect(eq(sz, 1, scary_length(a)));
    scary_free(a);
}

Test(libscary, dup) {
    int *a = scary_new(sizeof(int));
    scary_push(&a, 42);
    scary_push(&a, -42);
    int *b = scary_dup(a);
    cr_expect(ne(ptr, a, b));
    cr_expect(eq(sz, scary_length(a), scary_length(b)));
    cr_expect(eq(sz, 2, scary_length(b)));
    cr_expect(eq(int[2], a, b));
    scary_free(b);
    scary_free(a);
}

Test(libscary, free_null) {
    scary_free(NULL);
    int *a = scary_new(sizeof(int));
    scary_push(&a, 42);
    scary_push(&a, -42);
    scary_free(a);
    scary_free(NULL);
}
