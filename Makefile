CC=g++
LD=g++
GDWARF=-gdwarf-2
RM=rm
MKDIR=mkdir

SRC_PATH=./src/
LIB_PATH=./lib/

HEADERS=phone.h register.h
OBJECTS=$(SRC_PATH)/phone.o
OUT=$(LIB_PATH)/libwhats_acppi.so

CFLAGS= '-DGIT_REV="$(shell git rev-parse HEAD)"' '-DTIME_COMPILED="$(shell date '+%F %T')"' -Wall -Wextra -g -std=c++11
LDFLAGS= -shared

.PHONY: all

.PRECIOUS: %.o

all: $(LIB_PATH) $(OUT)

$(LIB_PATH):
	$(MKDIR) $(LIB_PATH)

$(OUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

clean:
	- $(RM) $(SRC_PATH)/*.o
	- $(RM) -r $(LIB_PATH)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"
