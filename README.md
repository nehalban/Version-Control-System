# COL106 Long Assignment - Version Control System

Author: Nehal Bansal  
Entry Number: 2024MT10788

## Project Overview
This project is an in-memory, Git-inspired version control system. It allows for the creation and manipulation of multiple files, each with a complete version history represented as a tree. Users can create new versions, save immutable snapshots(like commits), and navigate through the history of a file. It also supports system-wide commands to identify recently modified files and files with the most extensive version histories.

## How to Compile and Run
### Using the Provided Script
An automated script `run.sh` is provided to compile and run the project.

Make the script executable and run. Open a terminal, navigate to the project directory and execute:
   ```bash
   chmod +x run.sh
   ./run.sh
   ```
   This starts the program. You can now enter commands interactively and use Ctrl+Z to exit.

### Manual method
If you prefer to compile manually, run the following in bash terminal:
```bash
g++ -Wall -Wextra -O2 -o vcs main.cpp
```
To run interactively:
```bash
./vcs
```
To take input from a text file {Replace input.txt with your filename} :
```bash
./vcs < input.txt
```

## Data structures and Classes
All core data structures are implemented from scratch. The following custom structures were used:
### Tree (TreeNode)
Manages the branching version history for each file. Each version of a file is represented as a node in tree.

**Fields:**

*   `content`: File data at this version (string representing the file's contents).
*   `created_timestamp`: Timestamp indicating when this version was created.
*   `snapshot_timestamp`: Timestamp indicating when this version was marked as a snapshot (set only if the node is a snapshot).
*   `message`: Commit message (empty if not a snapshot). This provides context for snapshot.
*   `version_id`: Unique identifier for this version.
*   `parent`: Pointer to the parent TreeNode (the previous version).
*   `children`: Vector of pointers to child TreeNodes (subsequent versions).

### Linked List (LinkedList)
Each bucket of the hashmap uses a singly linked list. This handles collisions when different version IDs hash to the same index in the hashmap.

### HashMaps (Hashmap, Dict)
Allow efficient storage and retrieval of data in key-value pairs. For collision resolution, they contain custom hashing functions.
* `Hashmap` maps `version_id` → `TreeNode*`. It is implemented as a vector of linked lists storing TreeNode*. It is crucial for fast ROLLBACK.
* `Dict` maps `filename` → `File*`. It is implemented as a vector of vectors storing File*.
In `Dict`, number of buckets (17) and hashing value (31) are prime numbers to minimize collisions. In `Hashmap`, it does not make a difference, since version ids are sequentially assigned.

### Heap (Heap)
Efficiently finds the top 'k' files for system-wide analytics commands without needing to sort the entire dataset.

## Files in the Repository

*   **`data_structures.hpp` :** Implements the `TreeNode`, `LinkedList` and `Hashmap` classes.

*   **`file.hpp` :** Define the `File` class containing all the version control logic and methods for file operations.

*   **`system.hpp` :** Implements the `Heap` and `Dict` classes ; contains global variables and functions required for system-wide analytics.

*   **`main.cpp` :** Contains the main function that reads commands from standard input and executes them.

*   **`compile_and_run.sh` :** Automated compilation and execution script.

# Command Reference

#### `EXIT`
Terminates the program.

## Core File Operations
These are methods of `File` class:

#### `CREATE <filename>`
Initializes a new file with the given name. Creates a root version (ID 0) with empty content. This initial version is automatically marked as a snapshot.

#### `READ <filename>`
Prints the content of the active version of the specified file.

#### `UPDATE <filename> <content>`
Replaces the entire content of the active version with `<content>`.  
***Versioning Logic:** If the active version is a snapshot (immutable), a new child version is created to store the new content. If the active version is not a snapshot, its content is modified in place.*

#### `INSERT <filename> <content>`
Appends `<content>` to the end of the active version's existing content. It follows the same versioning logic as UPDATE.

#### `SNAPSHOT <filename> <message>`
Marks the currently active version as a snapshot, making its content immutable. It freezes the state of the file. The provided `<message>` is stored with the version for future reference.

#### `ROLLBACK <filename> [version_id]`
Changes the active version pointer.  
**With version_id:** The active version is set to the version corresponding to the given ID.  
**Without version_id:** The active version is set to the parent of the current active version.

#### `HISTORY <filename>`
Displays a chronological list of all snapshotted versions on the direct path from the currently active version back to the root.  
Each entry shows the Version ID, Snapshot Timestamp, and Snapshot Message.

## System-Wide Analytics

#### `RECENT_FILES [num]`
Lists the num files that have been modified most recently; Outputs all the files if num exceeds total number of files. The output is a list of filenames, sorted in descending order of their last modification timestamp.

#### `BIGGEST_TREES [num]`
Lists the num files that have the largest number of versions; Outputs all the files if num exceeds total number of files. The output is a list of filenames, sorted in descending order of their total version count.

## Error Handling, Edge Cases and Tie-Breaking

- Empty input lines are skipped without error.
- Displays `"Unknown command: <command>"` for unrecognized command names.
- Explicitly defined destructor methods prevent any memory leaks.

### CREATE Command
- If a file with the same name already exists, displays `"Error: File '<filename>' already exists"` and does not create a new file.

### File-Specific Commands
- If the specified filename doesn't exist, displays `"Error: File '<filename>' not found"` and continues execution.
- If the active version pointer is null (corrupted state), displays `"Error: No active version"` for all operations.
- Content and Message parameters trim leading spaces from parsed input. Empty content and messages are valid (empty strings).

### SNAPSHOT Command
- If attempting to snapshot an already snapshotted version, displays `"This version is already a snapshot (ID: <version_id>)"` instead of creating a duplicate.

### ROLLBACK Command
- If the specified version_id doesn't exist, displays `"Error: Version <version_id> not found"`.
- If attempting to rollback from the root version (version 0) without specifying a version_id, displays `"Error: Cannot rollback from root version"`.

### RECENT_FILES and BIGGEST_TREES Commands
- If `num` is <= 0, displays `"Error: Invalid number specified"`.
- If no files exist in the system, returns empty output.
- If timestamps/total_versions are identical, then first created appears first.
- Default value of num (if not provided) is 1.

