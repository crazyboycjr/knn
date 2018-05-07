
.PHONY: clean run

CFLAGS  := -Wall -g -std=c++14
INCLUDE = -I./
LIB     = -L./
LD      := g++
LDLIBS  := ${LDLIBS}

HFILES := $(wildcard *.h)
OBJECT := $(patsubst %.cc,%.o,$(wildcard *.cc))

DEST = ../bin/

APPS    := main gen

%.o: %.cc ${HFILES}
	g++ ${CFLAGS} ${INCLUDE} -c -o $@ $< 

all: ${APPS}

main: main.o
	${LD} ${CFLAGS} ${LIB} -o $@ $^ ${LDLIBS}

gen: gen.o
	${LD} ${CFLAGS} ${LIB} -o $@ $^ ${LDLIBS} 

clean:
	rm -f *.o ${APPS}

run: all
	./gen | ./main
