# Compiler and flags
CC = gcc
CFLAGS = -Wall -I$(CLIIDIR)
LFLAGS = -fPIC -shared -I$(LIBIDIR)
VFLAGS = -s --leak-check=full --show-leak-kinds=all
TFLAGS = -lcriterion --coverage -g -O3

# Directories
LIBIDIR = ./library/include/
LIBSRCDIR = ./library/src/
LIBTESTDIR = ./library/test/
CLIIDIR = ./cli/include/
CLISRCDIR = ./cli/src/
LIBDIR = .

PROJECT_NAME = wsfs
TESTS_NAME = tests_bin
LIB_NAME = libwsfs.so

LIB_SOURCES = ${LIBSRCDIR}file_node_funcs.c ${LIBSRCDIR}wsfs.c
PROG_SOURCES = ${CLISRCDIR}main.c ${CLISRCDIR}ui.c

TESTS = $(LIB_SOURCES) \
		$(wildcard ${LIBTESTDIR}*.c)

# Main targets
all: clean  $(LIB_NAME)
ui: clean  $(LIB_NAME) $(PROJECT_NAME)
test: clean criterion run_test

# Rules
# Build shared library
$(LIB_NAME): $(LIB_SOURCES)
	$(CC) $(LFLAGS) $^ -o $@

# Build main executable linked with shared library
$(PROJECT_NAME): $(LIB_NAME)
	$(CC) $(PROG_SOURCES) -L$(LIBDIR) -Wl,-rpath=$(LIBDIR) -lwsfs $(CFLAGS) -o $@

# Run tests
run_test:
	./$(TESTS_NAME)

# Documentation generation
doxygen:
	doxygen Doxyfile

# Memory leak check with Valgrind
valgrind:
	valgrind $(VFLAGS) ./$(PROJECT_NAME)

# Compile tests using Criterion framework
criterion:
	$(CC) $(TESTS) $(CFLAGS) $(TFLAGS) -o $(TESTS_NAME)

# Clean build files
clean:
	rm -f $(PROJECT_NAME) $(TESTS_NAME) $(LIBDIR)$(LIB_NAME) ./*.gcda ./*.gcno