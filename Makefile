CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = example
SRC_DIR = src/memory_leak.c src/interface.c example.c
INC_DIR = src

all: $(TARGET)

$(TARGET): $(SRC_DIR)
	$(CC) $(CFLAGS) -o $@ -I$(INC_DIR) $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
