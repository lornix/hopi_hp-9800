#
# Generic compiles-it-all Makefile
#  Loni Nix <lornix@lornix.com>
#    Last modified **********
#
# adhere to a higher standard
CSTDCFLAGS=-std=gnu99
CXXSTDCFLAGS=-std=gnu++11
#
# save any options preset on cmd line
EXTFLAGS:=$(CFLAGS) $(CXXFLAGS)
CFLAGS=
#
# lots of debugging stuff included
CFLAGS+=-ggdb3
#
# but no optimization by default
CFLAGS+=-O0
#
# warn about lots of things
CFLAGS+=-Wall -Wextra -Wunused
#
# but only include libs as needed
CFLAGS+=-Wl,--as-needed
#
# sometimes we want to see it all
#CFLAGS+=-save-temps -fverbose-asm
#
# and in glorious color!
CFLAGS+=-fdiagnostics-color=always
#
# don't really use LDFLAGS much
#LDFLAGS+=
#
# but need to list libraries needed
#LIBS+=-lm
#LIBS+=-lgd
LIBS+=-lmodbus
#
ASFLAGS=-ggdb3
#
AS=gas
CC:=gcc
CXX:=g++
#
.SUFFIXES:
.SUFFIXES: .c .cpp .s .o
#
.phony: all clean
#
SRC=$(wildcard *.c) $(wildcard *.cpp) $(wildcard *.s)
EXECS=$(basename $(SRC))
#
all: $(EXECS)

%: %.c
	$(CC) $(CSTDCFLAGS) $(CFLAGS) $(EXTFLAGS) $(LDFLAGS) -c $< -o $@.o $(LIBS)
	$(CC) $(CSTDCFLAGS) $(CFLAGS) $(EXTFLAGS) $(LDFLAGS) $@.o -o $@ $(LIBS)

%: %.cpp
	$(CXX) $(CXXSTDCFLAGS) $(CFLAGS) $(EXTFLAGS) $(LDFLAGS) -c $< -o $@.o $(LIBS)
	$(CXX) $(CXXSTDCFLAGS) $(CFLAGS) $(EXTFLAGS) $(LDFLAGS) $@.o -o $@ $(LIBS)

% :%.s
	$(AS) $(ASFLAGS) $< -o $@.o
	$(LD) $@.o -o $@

clean:
	rm -f *.o $(EXECS)
