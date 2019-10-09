CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LFLAGS = -lpthread -lrt

SOURCES = river.c program.c
OBJECTS = $(SOURCES:.c=.o)

all: $(SOURCES:.c=.o) compile delete

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@ 

compile:
	$(CC) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o proj2 

delete:
	rm $(OBJECTS)

