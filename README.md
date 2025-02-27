# WSFS - Without eyeS's File System

## Overview

WSFS is a RAM-based file system written in C. It has writing/reading files, directories, and symbolic links using a hierarchical node-based system.
This project also has simple CLI program to test file system's functions. 

## Features

- System is a tree that has a single-linked list as a child.
- Path resolution to retrieve full paths of files.
- Recursive file search.

## Example diagram

![Schematic diagram](diagram.png?)

## Installation

```sh
# Clone the repository
git clone https://github.com/without-eyes/wsfs.git
cd wsfs

# Generate documentation (located in ./docs/)
sudo apt-get install doxygen doxygen-gui graphviz
make doxygen

# Compile shared library
make

# Or if you want to use CLI program
make ui
```

## Usage

```sh
# Use WSFS library
gcc $(PROG_SOURCES) -L$(LIBDIR) -Wl,-rpath=$(LIBDIR) -lwsfs $(CFLAGS) $(MACROS) -o $(PROG_NAME)
```
where:
- PROG_SOURCES - your program source files
- LIBDIR - location of libwsfs.so file
- CFLAGS - gcc environment variables
- MACROS - set macros(MAX_MEMORY_SIZE, MAX_FILE_COUNT, PERMISSION_MASK, MAX_NAME_SIZE,
  BUFFER_SIZE, END_OF_FILE_LINE)
- PROG_NAME - name of executable

### Example:
```
gcc ./src/main.c -L . -Wl,-rpath=. -lwsfs -Wall -I ./CLIIDIR/ -DMAX_FILE_COUNT=100 -DPERMISSION_MASK=4 -o test
```

### Commands:

- `q` - Quit
- `f` - Create a file
- `d` - Create a directory
- `s` - Create a symbolic link
- `x` - Chnage node's permissions
- `c` - Change node's name
- `e` - Delete (erase) file node
- `w` - Write into file
- `r` - Read from file
- `g` - Go into child directory
- `m` - Move file node to new location
- `p` - Get the path of the file node
- `b` - Go back into the parent directory

## Code Structure

```
wsfs/
│── library/
│   │── src/
│   │   ├── file_node_funcs.c     # File system structs and enums
│   |   ├── file_node_structs.c   # File system functions
│   |   ├── wsfs.c                # File system functions
|   │
|   │── include/
|   │   ├── file_structs.h        # File node structures and functions
|   |   ├── wsfs.h                # File system functions
│   |
|   │── test/
|   │   ├── file_structs_test.h   # Unit tests for 
|   |   ├── wsfs_test.h           # User interface functions
│
│── cli/
│   │── src/
│   │   ├── main.c                # Entry point
│   |   ├── ui.c                  # User interface functions
|   │
|   │── include/
|   |   ├── ui.h                  # User interface functions
|
│── docs/                     # Documentation location
|
│── makefile                  # Build system
│── README.md                 # Project overview
```

## Example

```sh
drw-     50 2025-02-27 17:33 \
Input command('h' for help): f
Enter file name: fi

drw-    101 2025-02-27 17:33 \
frw-     51 2025-02-27 17:33 fi
Input command('h' for help): q
```

## Debugging

Use `gdb` for debugging:

```sh
gdb ./wsfs
```

Set breakpoints and run step-by-step debugging.

## Contributing

Contributions are welcome! Follow these steps to contribute:

1. **Read [Contribution guide](CONTRIBUTING.md)** before contributting.
2. **Fork the repository** on GitHub.
3. **Clone your fork** locally:
   ```sh
   git clone https://github.com/without-eyes/wsfs.git
   cd wsfs
   ```
4. **Create a new branch** for your changes:
   ```sh
   git checkout -b my-feature-branch
   ```
5. **Make your changes** and commit:
   ```sh
   git add .
   git commit -m "Description of changes"
   ```
6. **Push your changes** to GitHub:
   ```sh
   git push origin my-feature-branch
   ```
7. **Submit a Pull Request** via GitHub.

Make sure to follow the project's coding style and include necessary documentation for your changes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

