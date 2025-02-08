# Compiler and flags
CC = gcc
CFLAGS = -Wall -I$(IDIR) -g
VFLAGS = -s --leak-check=full --show-leak-kinds=all

# Directories
IDIR = ./include/
SRCDIR = ./src/

PROJECT_NAME = wsfs

SOURCES = $(wildcard ${SRCDIR}*.c)

# Main targets
all: clean $(PROJECT_NAME)

# Rules
$(PROJECT_NAME):
	$(CC) $(SOURCES) $(CFLAGS) -o $@

run:
	./$(PROJECT_NAME)

doxygen:
	doxygen Doxyfile

valgrind:
	valgrind $(VFLAGS) ./$(PROJECT_NAME)

clean:
	rm -f $(PROJECT_NAME)