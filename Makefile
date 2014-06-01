CC=g++
CPPFLAGS=-std=gnu++11 -Wall -Werror -O2 -march=native -ffast-math

.PHONY: all clean distclean

all: vm

vm: vm.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) vm
