# Makefile for compiling css.c with Raylib on macOS

# Compiler and compiler flags
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 $(shell pkg-config --cflags raylib)
LDFLAGS := $(shell pkg-config --libs raylib)

# Source and output files
SRC := source/main.c
OUT := love

# Targets
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

.PHONY: all 

