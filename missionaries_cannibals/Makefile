CC=g++
CFLAGS=-std=c++11 -g

default_target: missionaries_cannibals
.PHONY : default_target

missionaries_cannibals: missionaries_cannibals.o
	$(CC) $(CFLAGS) -o $@ $^

missionaries_cannibals.o: missionaries_cannibals.cpp
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f missionaries_cannibals *~ *.o

