# WSFS - Without eyeS's File System

## Overview
WSFS is a RAM-based file system written in C. It has writing/reading files, directories, symbolic links using a hierarchical node-based system.

## Features
- System is tree that has single-linked list as child.
- Path resolution to retrieve full paths of files.
- Recursive file search.

## Installation
```sh
# Clone the repository
git clone https://github.com/without-eyes/wsfs.git
cd wsfs

# Generate documentation(located in ./docs/)
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
- `e` - Delete(erase) file node 
- `w` - Write into file
- `r` - Read from file
- `g` - Go into child directory
- `p` - Get path of file node
- `b` - Go back into parent directory

## Code Structure
```
wsfs/
│── src/
│   ├── file_structs.c      # File system logic
│   ├── main.c              # Entry point
|   ├── ui.c                # User interface functions
│   ├── utils.c             # Utility functions
|   ├── wsfs.c              # File system functions
│
│── include/
│   ├── file_structs.h      # File node structures and functions
|   ├── ui.h                # User interface functions
│   ├── utils.h             # Utility functions
|   ├── wsfs.h              # File system functions
│
│── docs/                   # Documentation location
|
│── makefile                # Build system
│── README.md               # Project overview
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

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
