CC=g++
LD=g++
GDWARF=-gdwarf-2
RM=rm
MKDIR=mkdir

SRC_PATH=./src/
LIB_PATH=./lib/

HEADERS=$(SRC_PATH)/phone.h $(SRC_PATH)/register.h
OBJECTS=\
	$(SRC_PATH)/phone.o \
	$(SRC_PATH)/register.o \
	$(SRC_PATH)/util/base64.o \
	$(SRC_PATH)/util/sha1.o \
	$(SRC_PATH)/util/request.o \
	$(SRC_PATH)/util/util.o \
	$(SRC_PATH)/util/json.o \
	$(SRC_PATH)/util/hmac_sha1.o \
	$(SRC_PATH)/util/rc4.o \
	$(SRC_PATH)/util/pbkdf2.o \
	$(SRC_PATH)/util/socket.o \
	$(SRC_PATH)/util/log.o \
	$(SRC_PATH)/protocol/wa.o \
	$(SRC_PATH)/protocol/bin_tree_node_writer.o \
	$(SRC_PATH)/protocol/node.o \
	$(SRC_PATH)/protocol/key_stream.o \
	$(SRC_PATH)/protocol/bin_tree_node_reader.o \
	$(SRC_PATH)/protocol/token_map.o

OUT=$(LIB_PATH)/libwhats_acppi.so

# TODO: only put curl if compiling with curl
CFLAGS= '-DGIT_REV="$(shell git rev-parse HEAD)"' '-DTIME_COMPILED="$(shell date '+%F %T')"' -Wall -Wextra -g -std=c++11 -D__USE_OPENSSL__ -D__USE_CURL__ `pkg-config --cflags libcurl`
LDFLAGS= -shared `pkg-config --libs libcurl`

.PHONY: all

.PRECIOUS: %.o

all: $(LIB_PATH) $(OUT)

$(LIB_PATH):
	$(MKDIR) $(LIB_PATH)

$(OUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

clean:
	- $(RM) $(SRC_PATH)/*.o $(SRC_PATH)/*/*.o
	- $(RM) -r $(LIB_PATH)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"
