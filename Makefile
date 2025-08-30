CC = gcc
CFLAGS = -std=gnu2x -Wall -Wextra -O3 -fPIC $(XCFLAGS)
#CFLAGS = -Wall -Wextra -ggdb3 -O $(XCFLAGS)
ANALYZE = -fanalyzer
LIB = libscary.so

all: test

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
