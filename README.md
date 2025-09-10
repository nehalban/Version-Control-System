# COL106 Long Assignment - Version Control System

Author: Nehal Bansal  
Entry Number: 2024MT10788

## Project Overview
This project is an in-memory, Git-inspired version control system. It allows for the creation and manipulation of multiple files, each with a complete version history represented as a tree. Users can create new versions, save immutable snapshots(like commits), and navigate through the history of a file. It also supports system-wide commands to identify recently modified files and files with the most extensive version histories.
## Data structures and Classes
I implemented all core data structures from scratch. The following custom structures were used:
### Tree (TreeNode)
Manages the branching version history for each file. Each version of a file is represented as a node in tree.

**Fields:**

*   `content`: File data at this version (string representing the file's contents).
*   `created_timestamp`: Timestamp indicating when this version was created.
*   `snapshot_timestamp`: Timestamp indicating when this version was marked as a snapshot (set only if the node is a snapshot).
*   `message`: Commit message (empty if not a snapshot). This provides context for snapshot versions.
*   `version_id`: Unique identifier for this version.
*   `parent`: Pointer to the parent TreeNode (the previous version).
*   `children`: Vector of pointers to child TreeNodes (subsequent versions).

### Linked List (LinkedList)
Each bucket of the hashmap uses a singly linked list. This handles collisions when different version IDs hash to the same index in the hashmap.

### HashMaps (Hashmap, Dict)
Allow efficient storage and retrieval of data in key-value pairs. For collision resolution, they contain custom hashing functions.
* `Hashmap` maps `version_id` → `TreeNode*`. It is implemented as a vector of linked lists storing TreeNode*. It is crucial for fast ROLLBACK.
* `Dict` maps `filename` → `File*`. It is implemented as a vector of vectors storing File*.

### Heap (Heap)
Efficiently finds the top 'k' files for system-wide analytics commands without needing to sort the entire dataset.

## How to Compile and Run
You will need a C++ compiler like g++.

* **Compile:** A shell script is provided for easy compilation. Run the following command in your terminal:  
`sh compile.sh`

* **Execute:** After successful compilation, an executable named filesystem will be created. Run it with:  
`./filesystem`

* **Interact:** The program will now wait for input. You can start typing commands as specified below.  
To exit, use Ctrl+D.

# Command Reference

## Core File Operations
These are methods of `File` class:
#### `CREATE <filename>`
Initializes a new file with the given name. Creates a root version (ID 0) with empty content. This initial version is automatically marked as a snapshot.
#### `READ <filename>`
Prints the content of the active version of the specified file to the console.
#### `UPDATE <filename> <content>`
Replaces the entire content of the active version with <content>.  
Versioning Logic: If the active version is a snapshot (immutable), a new child version is created to store the new content. If the active version is not a snapshot, its content is modified in place.
#### `INSERT <filename> <content>`
Appends <content> to the end of the active version's existing content. It follows the same versioning logic as UPDATE.
#### `SNAPSHOT <filename> <message>`
Marks the currently active version as a snapshot, making its content immutable. It freezes the state of the file. The provided <message> is stored with the version for historical reference.
#### ` ROLLBACK <filename> [version_id]`
Changes the active version pointer.  
**With version_id:** The active version is set to the version corresponding to the given ID.  
**Without version_id:** The active version is set to the parent of the current active version.
#### `HISTORY <filename>`
Displays a chronological list of all snapshotted versions on the direct path from the currently active version back to the root.  
Each entry shows the Version ID, Snapshot Timestamp, and Snapshot Message.
## System-Wide Analytics
#### `RECENT_FILES [num]`
Lists the num files that have been modified most recently. The output is a space-separated list of filenames, sorted in descending order of their last modification timestamp.
#### `BIGGEST_TREES [num]`
Lists the num files that have the largest number of versions. The output is a space-separated list of filenames, sorted in descending order of their total version count.

## Handling of Edge Cases and Tie-Breaking
The system is designed to handle various edge cases as follows:
Invalid Command: If an unrecognized command is entered, the system prints an "Unknown command" message and waits for the next input.
Operations on Non-existent Files: If a command (e.g., READ, UPDATE) is used with a filename that does not exist, the program may encounter a segmentation fault as the lookup will return a nullptr. It is expected that files are created with CREATE before being used.
ROLLBACK to Invalid Version: If ROLLBACK is called with a version_id that does not exist for the file, the active version will be set to nullptr. Subsequent operations on this file will lead to a crash.
ROLLBACK from Root: If ROLLBACK is called without a version_id while the root version is active, the active version will become nullptr (as the root has no parent).
SNAPSHOT on an Existing Snapshot: If a version is already a snapshot, calling SNAPSHOT on it again will print a message indicating it has already been snapshotted and no changes will be made.
Tie-Breaking for Analytics: For the RECENT_FILES and BIGGEST_TREES commands, if multiple files have the exact same modification time or version count, their relative order in the output is not guaranteed. The current heap implementation does not use a secondary sorting criterion for tie-breaking.
