# Compiler and flags
CC = gcc
CFLAGS = -Wall -I$(IDIR) -g
VFLAGS = -s --leak-check=full --show-leak-kinds=all
TFLAGS = -lcriterion --coverage -g -O0

# Directories
IDIR = ./include/
SRCDIR = ./src/
TESTDIR = ./test/

PROJECT_NAME = wsfs
TESTS_NAME = tests_bin

SOURCES = $(wildcard ${SRCDIR}*.c)
TESTS = $(filter-out ${SRCDIR}main.c, \
		$(SOURCES) \
		$(wildcard ${TESTDIR}*.c))

# Main targets
all: clean $(PROJECT_NAME)
test: clean criterion run_test

# Rules
$(PROJECT_NAME):
	$(CC) $(SOURCES) $(CFLAGS) -o $@

run:
	./$(PROJECT_NAME)

run_test:
	./$(TESTS_NAME)

doxygen:
	doxygen Doxyfile

valgrind:
	valgrind $(VFLAGS) ./$(PROJECT_NAME)

criterion:
	$(CC) $(TESTS) $(CFLAGS) $(TFLAGS) -o $(TESTS_NAME)

clean:
	rm -f $(PROJECT_NAME) $(TESTS_NAME) ./*.gcda ./*.gcno