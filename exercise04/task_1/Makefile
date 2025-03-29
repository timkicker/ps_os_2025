CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = arg_counter
SRC = arg_counter.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET) "JULy@15xxX" "DecembeR*10" "IV-20Mg"

test:
	OFFSET=5 ./$(TARGET) "JULy@15xxX" "DecembeR*10" "IV-2099Mg"
