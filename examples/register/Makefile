CC=g++
LD=g++
GDWARF=-gdwarf-2
RM=rm
MKDIR=mkdir
RPATH=$(realpath ../../lib)

.PHONY: all

.PRECIOUS: %.o

all: main.o
	g++ main.o -L ../../lib/ -lwhats_acppi -o main -Wl,-rpath=$(RPATH)

clean:
	- $(RM) *.o main

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

