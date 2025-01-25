# Compiler and flags
CC = gcc
CFLAGS = -Wall -I$(IDIR) -g

# Directories
IDIR = ./include/
SRCDIR = ./src/

PROJECT_NAME = wsfs

SOURCES = $(wildcard ${SRCDIR}*.c)

# Main targets
all: clean $(PROJECT_NAME) run

# Rules
$(PROJECT_NAME):
	$(CC) $(SOURCES) $(CFLAGS) -o $@

run:
	./$(PROJECT_NAME)

clean:
	rm -f $(PROJECT_NAME)

doxygen:
	doxygen Doxyfile