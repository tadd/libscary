libscary: An (scary) scalable array library in C
================================================

This is one of those rotten dynamic array libraries, but wait, just take a look.

```c
int *a = scary_new(sizeof(int));
```
Look at this LHS; it looks like an ordinary C array, but

```c
printf("length: %zu\n", scary_length(a)); //=> 0
```
they know their length by themselves 😱.

```c
scary_push(&a, -1);
scary_push(&a, 42);
printf("new length: %zu\n", scary_length(a)); //=> 2
```
You can push elements with automatic memory extension,
as much as you want.

Moreover, the `scary_push` function is _generic_ 😱. If you put this code,

```c
scary_push(&a, 0UL); // Pushing `unsigned long` into an array of `int`!
```

it will produce a warning by default with modern compilers like GCC 12.

<pre><code><b>warning:</b> passing argument 1 of 'scary_push_uint64' from incompatible pointer type [<b>-Wincompatible-pointer-types</b>]
    scary_push(<b>&a</b>, 0UL);
               <b>^~</b>
               <b>|</b>
               <b>int **</b>
</code></pre>

You can of course opt-in an option `-Werror` to prevent such typing mistakes.

And you'll see **magic** here:

```c
int i = a[1];
```
You can read/write them as ordinary C arrays with **zero**-overhead 😱😱.

```c
printf("content: %d\n", i);
```
Then it prints `42`. Happy ending. 🤔🤔

## Usage

1. `make` and get `libscary.so`.
2. Write your code with `#include <scary.h>`.
3. Compile with `-lscary` and proper `-I`/`-L`.
4. Enjoy your looseness.

## Development

We use [Criterion](https://github.com/Snaipe/Criterion) for tests so
you'll need to install that before `make test`.

Dear Debian/Ubuntu users: You can install its package via
<code>apt install [libcriterion-dev](https://packages.debian.org/stable/libcriterion-dev)</code>.

## License

[CC0](./CC0.md).
