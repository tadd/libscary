CC = gcc
warnflags = -Wcast-align -Wfloat-equal -Wpointer-arith -Wshadow -Wstrict-prototypes \
	    -Wswitch-enum -Wundef -Wunreachable-code -Wwrite-strings -Wformat=2
CFLAGS = -Wall -Wextra $(warnflags) -fPIC -ggdb3 $(XCFLAGS)
# XCFLAGS = -g0 -O3
ANALYZE = -fanalyzer
LIB = libscary.so

all: $(LIB)

$(LIB): scary.o
testlib.o scary.o: scary.h

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.s: %.c
	$(CC) $(CFLAGS) -S -c $<

%.so:
	$(CC) $(CFLAGS) -shared $^ -o $@

test: testlib
	env LD_LIBRARY_PATH=. ./$<

testlib: testlib.c $(LIB)
	$(CC) $(CFLAGS) -L. $< -lscary -lcriterion -o $@

clean:
	rm -f *.o *.so testlib

%.analyze.o: %.c
	$(CC) $(CFLAGS) $(ANALYZE) -c $< -o /dev/null

analyze: scary.analyze.o

.PHONY: clean analyze test
