CC=g++
LD=g++
GDWARF=-gdwarf-2
RM=rm
MKDIR=mkdir
RPATH=$(realpath ../../lib)
# TODO: create use pkg-config. create pc file for whats-acppi
CFLAGS= `pkg-config --cflags libcurl`
LDFLAGS= `pkg-config --libs libcurl`

.PHONY: all

.PRECIOUS: %.o

all: main.o
	g++ $(CFLAGS) $(LDFLAGS) main.o -L ../../lib/ -lwhats_acppi -o main -Wl,-rpath=$(RPATH)

clean:
	- $(RM) *.o main

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

