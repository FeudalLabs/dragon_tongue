CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I./src
DEBUG_FLAGS = -DDEBUG
TARGET = dragontongue

SRCS = src/main.c src/arena.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test:
	$(CC) $(CFLAGS) tests/test_arena.c src/arena.c -o tests/test_arena
	./tests/test_arena

clean:
	rm -f $(OBJS) $(TARGET) tests/test_arena

.PHONY: all debug test clean

test: test-arena test-source

test-arena:
	$(CC) $(CFLAGS) tests/test_arena.c src/arena.c -o tests/test_arena
	./tests/test_arena

test-source:
	$(CC) $(CFLAGS) tests/test_source.c src/arena.c src/lexer/source.c -o tests/test_source
	./tests/test_source

test-all: test-arena test-source

SRCS = src/main.c src/arena.c src/lexer/source.c