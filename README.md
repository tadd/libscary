libscary: An (scary) scalable array library in C
================================================

This is one of those rotten dynamic array libraries, but wait, just take a look.

```c
int *a = scary_new(sizeof(int));
```
Look at this LHS; it looks like an ordinary C array, but

```c
printf("length: %zu\n", scary_length(a));
```
they knows their length by themselves 😱.

```c
scary_push(a, -1);
scary_push(a, 42);
printf("new length: %zu\n", scary_length(a));
```
You can push elements with automatic memory extension,
as much as you want.

And you'll see **magic** here:

```c
int i = a[1];
```
You can read/write them as an ordinary C array without any overhead 😱😱.

```c
printf("content: %d\n", i);
```
Then it prints `42`. Happy ending. 🤔🤔

## Development

We use [Criterion](https://github.com/Snaipe/Criterion) for tests so
you'll need to install that before `make test`.

Dear Debian/Ubuntu users: You can install [its
package](https://packages.debian.org/bookworm/libcriterion-dev) via
`apt install libcriterion-dev`.

## License

[CC0](./CC0.md).
