CC := gcc
TARGET := reaction_diffusion
CFLAGS := -std=c11 -Wall -O2 $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs) -lm

all:
	$(CC) $(CFLAGS) src/main.c -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)