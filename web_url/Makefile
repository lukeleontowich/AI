CC=g++
CFLAGS=-std=c++11 -g

default_target: web_url
.PHONY : default_target

missionaries_cannibals: web_url.o
	$(CC) $(CFLAGS) -o $@ $^

missionaries_cannibals.o: web_url.cpp
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f web_rul *~ *.o
