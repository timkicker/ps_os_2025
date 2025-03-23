CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = signal_handler

all: $(TARGET)

$(TARGET): signal_handler.o
	$(CC) $(CFLAGS) -o $(TARGET) signal_handler.o

signal_handler.o: signal_handler.c
	$(CC) $(CFLAGS) -c signal_handler.c

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean