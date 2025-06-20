CC=gcc
TARGET=bin/subtitler
INCLUDE=include
SRC=src/subtitler.c

rm=rm
uname := $(shell uname)

# https://github.com/stpettersens/uname-windows
ifeq ($(uname),Windows)
	TARGET=bin\subtitler.exe
	rm=del
endif

all: run

run: make
	./$(TARGET)

make:
	$(CC) $(SRC) -I$(INCLUDE) -o$(TARGET)

clean:
	$(rm) $(TARGET)
