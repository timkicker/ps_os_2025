CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = arg_counter

all: $(TARGET)

$(TARGET): arg_counter.o
	$(CC) $(CFLAGS) -o $(TARGET) arg_counter.o

arg_counter.o: arg_counter.c
	$(CC) $(CFLAGS) -c arg_counter.c

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean