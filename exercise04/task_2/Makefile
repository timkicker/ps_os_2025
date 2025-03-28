CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -pthread
LDFLAGS = -pthread

TARGETS = cat_cut cat_grep_cut

all: $(TARGETS)

cat_cut: cat_cut.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

cat_grep_cut: cat_grep_cut.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean