.PHONY: all create_key clean

CC = cc
CFLAGS = -std=c11 -Wall -Wextra

SRC = main.c key.c
OBJ = main.o key.o
EXEC = main

all: $(EXEC)

create_key:
	@echo "Creating key.c"
	@echo '#include "key.h"' > key.c
	@echo '' >> key.c
	@echo 'const char* get_key() {' >> key.c
	@echo '    return "'$(shell whoami)'";' >> key.c
	@echo '}' >> key.c

$(EXEC): $(OBJ)
	@echo "Linking main"
	$(CC) $(OBJ) -o $(EXEC)

main.o: main.c
	@echo "Compiling main.c"
	$(CC) $(CFLAGS) -c main.c

key.o: main.o create_key key.c
	@echo "Compiling key.c"
	$(CC) $(CFLAGS) -c key.c

clean:
	@rm -f $(OBJ) $(EXEC) key.c