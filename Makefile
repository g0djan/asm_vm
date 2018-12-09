SRCD=.
TO_DELETE=asm vm check

CC=/usr/bin/clang
CXX=/usr/bin/clang++
LD=/usr/bin/clang++

GTEST=$(SRCD)/googletest/googletest

INCLUDES=-I$(GTEST) -I$(GTEST)/include
WARN_OPTS=-Wall -Werror -pedantic


CFLAGS=$(WARN_OPTS) $(INCLUDES) \
        -std=c11 -g

CXXFLAGS=$(WARN_OPTS) $(INCLUDES) \
        -std=c++17 -g

LDFLAGS=-lm -lpthread

LINK_EXECUTABLE=$(LD) -o $@ $^ $(LDFLAGS)

COMPILE_CXX_SRC=$(CXX) $(CXXFLAGS) -c -o $@ $^

COMPILE_C_SRC=$(CC) $(CFLAGS) -c -o $@ $^

all: check asm vm

vm: vm.bin;
asm: asm.bin;
check: check.bin;
clean:
	rm -f *.o $(TO_DELETE:=.exe) $(TO_DELETE:=.bin)

vm.bin: vm.o
	$(LINK_EXECUTABLE)

asm.bin: asm.o
	$(LINK_EXECUTABLE)

check.bin: check.o gtest-all.o gtest_main.o f.o
	$(LINK_EXECUTABLE)

gtest-all.o: $(GTEST)/src/gtest-all.cc
	$(COMPILE_CXX_SRC)

gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(COMPILE_CXX_SRC)

asm.o: $(SRCD)/asm/asm.c
	$(COMPILE_C_SRC)

vm.o: $(SRCD)/vm/vm.c
	$(COMPILE_C_SRC)

check.o: $(SRCD)/tests/check.cpp
	$(COMPILE_CXX_SRC)

f.o: $(SRCD)/asm/f.c
	$(COMPILE_C_SRC)
