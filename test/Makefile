CC=g++
LD=g++
GDWARF=-gdwarf-2
RM=rm -f
MKDIR=mkdir
RPATH=$(realpath ../lib)
IPATH=$(realpath ../src)
# TODO: create use pkg-config. create pc file for whats-acppi
CFLAGS=  -Wall -Wextra -g -std=c++11 -I$(IPATH)
LDFLAGS=  -L $(RPATH) -lwhats_acppi -Wl,-rpath=$(RPATH) #`pkg-config --libs libcurl`

TESTS_CPP ?= $(wildcard ./*/*.cpp ./*.cpp)
TESTS_O=$(patsubst %.cpp, %.o, $(TESTS_CPP))
TESTS=$(patsubst %.cpp, %, $(TESTS_CPP))

.PHONY: all test

.PRECIOUS: %.o

all: $(TESTS)

test: all
	@for t in $(TESTS) ; do \
		echo -n $$t;\
		OUTPUT=`./$$t 2>&1` ;\
		if [ $$? -eq 0 ]  ; then \
			echo " OK" ; \
		else \
			echo " ERROR" ; \
			echo $$OUTPUT ; \
		fi ; \
	done

clean:
	- $(RM) *.o */*.o $(TESTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

$(TESTS): $(TESTS_O)
	$(CC) $(CFLAGS) -o "$@" "$@.o" $(LDFLAGS)

