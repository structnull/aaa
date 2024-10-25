# Variables
CC = cc
CFLAGS = -Wall -Wextra -std=c99
TARGET = aaai
SRC = main.c

# Default target
all: $(TARGET)

# Compile the binary
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up build files
clean:
	rm -f $(TARGET)

# For remake (clean and then make)
remake: clean all

# Phony targets
.PHONY: all clean remake
