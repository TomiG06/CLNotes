CC = gcc
CFLAGS = -Wall -o clnotesExec

SOURCE_FILES = main.c notes.c csv.c

all: main.c notes.c csv.c
	$(CC) $(CFLAGS) $(SOURCE_FILES)

debug:
	$(CC) $(CFLAGS) $(SOURCE_FILES) -DDEBUG
