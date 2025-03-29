CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: cat_cut cat_grep_cut

cat_cut: cat_cut.c
	$(CC) $(CFLAGS) -o cat_cut cat_cut.c

cat_grep_cut: cat_grep_cut.c
	$(CC) $(CFLAGS) -o cat_grep_cut cat_grep_cut.c

clean:
	rm -f cat_cut cat_grep_cut
