#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "scary.h"

Test(libscary, push_ref_and_length) {
    int *a = scary_new(sizeof(int));
    cr_expect(eq(sz, 0, scary_length(a)));

    int l[] = { 1, -2, 3, -5, 7, -11 }, *p = l;
    const size_t n = sizeof(l)/sizeof(*l);
    for (size_t i = 0; i < n; i++)
        scary_push_ref(&a, p++);

    cr_expect(eq(sz, n, scary_length(a)));
    cr_expect(eq(int, 1, a[0]));
    cr_expect(eq(int[n], l, a));

    scary_free(a);
}
