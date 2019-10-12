# Testing cURL
# MAKEFILE

CC = gcc
CFLAGS = -Wall -Werror

# Librarys
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib 
LDLIBS = -lcurl

# Details
SOURCES = src/test.c
OUT = test

.PHONY: all

all: build

$(OUT): $(patsubst %.c,%.o,$(SOURCES))

build: $(SOURCES)
	$(CC) -o $(OUT) $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(SOURCES)
