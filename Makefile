# Compiler to use
CC = gcc

# Compiler flags, e.g. -I<include> -L<link>
CFLAGS = -Iinclude -Iinclude/curl

# Linker flags
LDFLAGS = -Llib -lsqlite3 -l:libcurl.so.4.8.0
# Name of the executable
TARGET = bin/CLManager

# List of all .c files in the project
SRCS = $(wildcard src/*.c)

# List of all .o files that will be generated from .c files
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# Check the operating system
ifeq ($(OS),Windows_NT)
	TARGET := $(TARGET).exe
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o $(TARGET)
