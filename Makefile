CC = clang # clang also works.
TARGET = bin/subtitler
INCLUDE = include
FLAGS=-Wall -Wextra -std=c11
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

RM = rm -f obj/*.o bin/*

uname := $(shell uname)

# https://github.com/stpettersens/uname-windows
ifeq ($(uname),Windows)
	TARGET=bin\subtitler.exe
	RM=del obj\*.o bin\*.exe
endif

run: all
	$(TARGET)

all: clean default

default: $(TARGET)

clean:
	$(RM)

$(TARGET): $(OBJ)
	$(CC) -o $@ $? -lm

obj/%.o: src/%.c
	$(CC)$(FLAGS) -c $< -o $@ -I$(INCLUDE)
