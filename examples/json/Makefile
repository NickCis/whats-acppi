CC=g++
LD=g++
GDWARF=-gdwarf-2
RM=rm
MKDIR=mkdir
RPATH=$(realpath ../../lib)
# TODO: create use pkg-config. create pc file for whats-acppi
CFLAGS= -Wall -Wextra -g -std=c++11
LDFLAGS=  -L $(RPATH) -lwhats_acppi -Wl,-rpath=$(RPATH)

.PHONY: all

.PRECIOUS: %.o

all: main.o
	g++ $(CFLAGS) $(LDFLAGS) main.o  -o main 

clean:
	- $(RM) *.o main

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

