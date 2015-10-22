CC = gcc-5
CFLAGS = -std=c11
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Wno-parentheses
CFLAGS += -Wno-int-conversion
CFLAGS += -Wno-unused-parameter
CFLAGS += -g3

all: libs.a test

clean:
	rm libs.a
	./test

test: test.c libs.a
	$(CC) $(CFLAGS) -o $@ $^ -lre

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

libs.a: s.o re.o
	ar -csr $@ $^

install:
	cp s.h /usr/local/include/s.h
	cp libs.a /usr/local/lib/libs.a
