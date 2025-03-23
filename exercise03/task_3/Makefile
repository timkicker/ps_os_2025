CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -pthread
LDFLAGS = -pthread

TARGETS = task_3a task_3b

all: $(TARGETS)

task_3a: task_3a.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

task_3b: task_3b.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean