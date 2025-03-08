In-Memory File System (C++) – Project Overview
📌 Project Description
This project involves designing and implementing a basic in-memory file system in C++, simulating Unix-like commands such as mkdir, cd, ls, pwd, cp, rm, and touch. The file system will be structured as a hierarchical tree, where each node represents either a file or a directory.

The goal is to provide core file system functionalities while reinforcing recursion, tree data structures, and dynamic memory management.

📂 Project Requirements
The program must support the following features:

Directory & File Creation

Users can create directories (mkdir) and files (touch) within the current working directory.
Navigation & Display

cd allows moving between directories.
pwd prints the absolute path of the current directory.
ls lists files and subdirectories in the current directory.
File & Directory Manipulation

rm removes files or directories, recursively deleting subdirectories if needed.
cp copies files or directories to a new location, handling deep copying for nested directories.
Recursive Operations

The system should support recursive directory copying and deletion while maintaining proper parent-child relationships.
Error Handling

The program should prevent invalid operations (e.g., trying to cd into a file, overwriting existing files in cp).
🔧 Implementation Details
The file system will be tree-based, where:
Each directory is a node that can have multiple children.
Each file is a leaf node with no children.
The system will always start with a root (/) directory.
Operations such as cp and rm will use recursion to handle nested directories.
The program will be memory-based and will not interact with the actual disk.
🛠 Concepts Covered
Tree Data Structures → Hierarchical file system representation.
Recursion → Implementing rm and cp for deep deletion and copying.
Pointers & Memory Management → Dynamically managing files and directories.
Object-Oriented Design → Encapsulating file system logic in a class structure.
📌 Expected Outcome
By the end of this project, the program will function as a mini Unix file system simulation, capable of creating, managing, and manipulating files and directories efficiently.
