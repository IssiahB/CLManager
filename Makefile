# Compiler to use
CC = gcc

# Compiler flags, e.g. -I<include> -L<link>
CFLAGS = -Iinclude

# Linker flags, e.g. -lm (math library)
LDFLAGS = -Lbin -lsqlite3

# Name of the executable
TARGET = bin/CLManager

# List of all .c files in the project
SRCS = $(wildcard src/*.c)

# List of all .o files that will be generated from .c files
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o $(TARGET)
