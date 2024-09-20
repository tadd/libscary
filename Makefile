CC=gcc
CFLAGS=-Wall -Wextra -O3 $(XCFLAGS)
#CFLAGS=-Wall -Wextra -ggdb3 -O $(XCFLAGS)
ANALYZE=-fanalyzer
LIB=libscary.so

all: $(LIB)

$(LIB): scary.o
scary.o: scary.h

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.so:
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@

test: testlib
	env LD_LIBRARY_PATH=. ./$<

testlib: testlib.c $(LIB)
	$(CC) $(CFLAGS) -L. -lscary -lcriterion $^ -o $@

clean:
	rm -f *.o *.so

%.analyze.o: %.c
	$(CC) $(CFLAGS) $(ANALYZE) -c $< -o /dev/null

analyze: scary.analyze.o

.PHONY: clean analyze test
