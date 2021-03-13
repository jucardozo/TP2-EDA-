SHELL := /bin/bash
CC = gcc
FLAGS = -Wall -std=c11 -pedantic
FLAGS_VERBOSE = -DVERBOSE
TEST_NAME = libparse_test

obj = libparse.o
obj_test = test.o test_table.o ${obj}

.PHONY: all force test libonly clean

ifdef VERBOSE
FLAGS += ${FLAGS_VERBOSE}
endif

all: ${obj}
	${CC} ${FLAGS} -c libparse.c -o libparse.o

force: clean all

test: ${obj_test}
	${CC} ${FLAGS_TEST} ${obj_test} -o ${TEST_NAME}

clean:
	rm --force ${obj} ${obj_test}

libparse.o: libparse.h libparse.c
	${CC} ${FLAGS} -c libparse.c -o libparse.o

test.o: test.c test_table.h test_table.c libparse.h libparse.c
	${CC} ${FLAGS} -c test.c -o test.o
	
test_table.o: test_table.h test_table.c
	${CC} ${FLAGS} -c test_table.c -o test_table.o

