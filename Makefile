CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I./src
TARGET = dragontongue

# Find all source files
SRCS = src/main.c \
       src/arena.c \
       src/lexer/source.c \
       src/lexer/token.c \
       src/lexer/scanner.c \
       src/parser/ast.c \
       src/parser/parser.c

# Convert .c to .o
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile C files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET) tests/test_arena tests/test_source tests/test_scanner

# Test targets
test-arena:
	$(CC) $(CFLAGS) tests/test_arena.c src/arena.c -o tests/test_arena
	./tests/test_arena

test-source:
	$(CC) $(CFLAGS) tests/test_source.c src/arena.c src/lexer/source.c -o tests/test_source
	./tests/test_source

test-scanner:
	$(CC) $(CFLAGS) tests/test_scanner.c src/arena.c src/lexer/source.c src/lexer/token.c src/lexer/scanner.c -o tests/test_scanner
	./tests/test_scanner

# Run all tests
test: test-arena test-source test-scanner

# Run the compiler on a test file
run: $(TARGET)
	./$(TARGET) examples/test.dt

.PHONY: all test clean run