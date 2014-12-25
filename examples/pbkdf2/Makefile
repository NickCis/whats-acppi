CC=g++
LD=g++
GDWARF=-gdwarf-2
RM=rm
MKDIR=mkdir
RPATH=$(realpath ../../lib)
# TODO: create use pkg-config. create pc file for whats-acppi
CFLAGS=  -Wall -Wextra -g -std=c++11 #`pkg-config --cflags libcurl`
LDFLAGS=  -L $(RPATH) -lwhats_acppi -Wl,-rpath=$(RPATH) #`pkg-config --libs libcurl`

.PHONY: all

.PRECIOUS: %.o

all: main.o
	g++ $(CFLAGS) $(LDFLAGS) main.o  -o pbkdf2

clean:
	- $(RM) *.o pbkdf2

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

