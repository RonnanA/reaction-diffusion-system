CC := gcc
TARGET := reaction_diffusion
CFLAGS := -std=c11 -Wall -O2 $(shell pkg-config --cflags sdl3)
LDFLAGS := $(shell pkg-config --libs sdl3) -lm

all:
	$(CC) $(CFLAGS) src/main.c -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)