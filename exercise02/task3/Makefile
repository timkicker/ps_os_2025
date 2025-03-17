CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
LDFLAGS = -lrt


TARGET = task3


SRCS = task3.c


OBJS = $(SRCS:.c=.o)


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: all clean


clean:
	rm -f $(OBJS) $(TARGET)