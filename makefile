# Compiler and flags
CC = gcc
CFLAGS = -Wall -lmenu -lncurses -I$(IDIR) -g

# Directories
IDIR = ./include/
SRCDIR = ./src/

PROJECT_NAME = ramfs

SOURCES = $(wildcard ${SRCDIR}*.c)

# Targets
.PHONY: all memory fullmemory test coverage allclean clean cleantest cleancoverage
.SILENT: all memory fullmemory test coverage allclean

# Main targets
all: $(PROJECT_NAME) run

# Rules
$(PROJECT_NAME):
	$(CC) $(SOURCES) $(CFLAGS) -o $@

run:
	./$(PROJECT_NAME)

clean:
	rm -f $(PROJECT_NAME)
