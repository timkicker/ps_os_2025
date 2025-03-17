USERNAME := $(shell whoami)
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
OBJFILES = main.o key.o


.PHONY: all clean create_key build_objects

all: create_key build_objects main

create_key:
	echo '#include "key.h"\n\nconst char* get_key() {\n    return "$(USERNAME)";\n}' > key.c


build_objects: key.c
	echo "Compiling main.c"
	$(CC) $(CFLAGS) -c main.c -o main.o
	echo "Compiling key.c"
	$(CC) $(CFLAGS) -c key.c -o key.o

# final target to executable
main: $(OBJFILES)
	echo "Linking main"
	$(CC) $(OBJFILES) -o main


clean:
	rm -f $(OBJFILES) main key.c
