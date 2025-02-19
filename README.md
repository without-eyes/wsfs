# WSFS - Without eyeS's File System

## Overview

WSFS is a RAM-based file system written in C. It has writing/reading files, directories, and symbolic links using a hierarchical node-based system.

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

# Build the project
make
```

## Usage

```sh
# Run the WSFS application
./wsfs
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
│── src/
│   ├── file_node_funcs.c     # File system structs and enums
|   ├── file_node_structs.c   # File system functions
│   ├── main.c                # Entry point
|   ├── ui.c                  # User interface functions
│   ├── utils.c               # Utility functions
|   ├── wsfs.c                # File system functions
│
│── include/
│   ├── file_structs.h        # File node structures and functions
|   ├── ui.h                  # User interface functions
│   ├── utils.h               # Utility functions
|   ├── wsfs.h                # File system functions
│
│── docs/                     # Documentation location
|
│── makefile                  # Build system
│── README.md                 # Project overview
```

## Example

```sh
d 19:45 \
Input command('h' for help): f
Enter file name: test_file

d 19:45 \
f 19:46 test_file
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

