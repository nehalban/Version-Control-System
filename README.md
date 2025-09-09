# COL106 Long Assignment - Version Control System

Author: Nehal Bansal
Entry Number: 2024MT10788

## Project Overview
This project is an in-memory, Git-inspired version control system. It allows for the creation and manipulation of files, each with a complete version history represented as a tree. Users can create new versions, save immutable snapshots(like commits), and navigate through the history of a file. It also supports system-wide analytics to identify recently modified files and files with the most extensive version histories.
## Data structures and Classes
I implemented all core data structures from scratch. The following custom structures were used:
### Tree (TreeNode):
Manages the branching version history for each file. Each version of a file is represented as a node in tree.
### HashMap (Hashmap, Dict):
Provides O(1) average-time lookup for versions by their ID and for files by their filename.
* Heap (Heap): Efficiently finds the top 'k' files for system-wide analytics commands without needing to sort the entire dataset.

## How to Compile and Run
Prerequisites: A C++ compiler (g++ recommended).
Compile: A shell script is provided for easy compilation. Run the following command in your terminal:
```sh compile.sh```

Execute: After successful compilation, an executable named filesystem will be created. Run it with:
```./filesystem```

Interact: The program will now wait for input. You can start typing commands as specified below. To exit, use Ctrl+D.
Command Reference

## Core File Operations
CREATE <filename>
Initializes a new file with the given name. Creates a root version (ID 0) with empty content. This initial version is automatically marked as a snapshot.
READ <filename>
Prints the content of the currently active version of the specified file to the console.
UPDATE <filename> <content>
Replaces the entire content of the active version with <content>.
Versioning Logic: If the active version is a snapshot (immutable), a new child version is created to store the new content. If the active version is not a snapshot, its content is modified in place.
INSERT <filename> <content>
Appends <content> to the end of the active version's existing content. It follows the same versioning logic as UPDATE.
SNAPSHOT <filename> <message>
Marks the currently active version as a snapshot, making its content immutable. The provided <message> is stored with the version for historical reference.
ROLLBACK <filename> [version_id]
Changes the active version pointer.
With version_id: The active version is set to the version corresponding to the given ID.
Without version_id: The active version is set to the parent of the current active version.
HISTORY <filename>
Displays a chronological list of all snapshotted versions on the direct path from the currently active version back to the root. Each entry shows the Version ID, Snapshot Timestamp, and Snapshot Message.
System-Wide Analytics
RECENT_FILES [num]
Lists the num files that have been modified most recently. The output is a space-separated list of filenames, sorted in descending order of their last modification timestamp.
BIGGEST_TREES [num]
Lists the num files that have the largest number of versions. The output is a space-separated list of filenames, sorted in descending order of their total version count.

## Handling of Edge Cases and Tie-Breaking
The system is designed to handle various edge cases as follows:
Invalid Command: If an unrecognized command is entered, the system prints an "Unknown command" message and waits for the next input.
Operations on Non-existent Files: If a command (e.g., READ, UPDATE) is used with a filename that does not exist, the program may encounter a segmentation fault as the lookup will return a nullptr. It is expected that files are created with CREATE before being used.
ROLLBACK to Invalid Version: If ROLLBACK is called with a version_id that does not exist for the file, the active version will be set to nullptr. Subsequent operations on this file will lead to a crash.
ROLLBACK from Root: If ROLLBACK is called without a version_id while the root version is active, the active version will become nullptr (as the root has no parent).
SNAPSHOT on an Existing Snapshot: If a version is already a snapshot, calling SNAPSHOT on it again will print a message indicating it has already been snapshotted and no changes will be made.
Tie-Breaking for Analytics: For the RECENT_FILES and BIGGEST_TREES commands, if multiple files have the exact same modification time or version count, their relative order in the output is not guaranteed. The current heap implementation does not use a secondary sorting criterion for tie-breaking.









content: file data at this version

created_timestamp: creation time

snapshot_timestamp: set only if node is a snapshot

message: commit message (empty if not a snapshot)

version_id: unique identifier

parent and children: links in the version tree

Rationale: The tree naturally represents how versions branch from snapshots. Each path from root to a node corresponds to a sequence of edits.

(b) Hashmap with Separate Chaining

Implemented as an array of linked lists.

Maps version_id → TreeNode* for constant-time access.

Rationale: Fast lookup of versions by ID is required for rollback. A custom hashmap demonstrates understanding of hashing and collision resolution.

(c) Linked List

Each bucket of the hashmap uses a singly linked list.

Stores key-value pairs (version_id, TreeNode*).

Rationale: Handles collisions in the hash table using chaining.

Classes and Methods TreeNode
Represents a version of the file. Supports marking itself as a snapshot.

LinkedList & Hashmap

Provide custom implementation for efficient storage and lookup of versions.

file

Encapsulates all version-control logic.

new_version(content, parent) → create a new version

READ() → print current version content

UPDATE(content) → modify file; creates child node if current version is a snapshot

INSERT(content) → append new content

SNAPSHOT(message) → mark current version as a snapshot

ROLLBACK(version_id) → switch to a previous version

HISTORY() → print all snapshots with version IDs and timestamps

Algorithms
Update Algorithm

If the active version is a snapshot → create new child node.

Else → modify content directly. Time Complexity: O(1)

Snapshot Algorithm

Assign a snapshot timestamp and message.

Store (version_id, node) in snapshot history. Time Complexity: O(1)

Rollback Algorithm

If version ID is provided → lookup in hashmap.

If not → move to parent. Time Complexity: O(1) (due to hashmap)

History Traversal

Iterate over vector of snapshotted nodes. Time Complexity: O(k), where k = number of snapshots

Example Workflow
Update content

myfile.UPDATE("Hello");

Take snapshot

myfile.SNAPSHOT("Initial commit");

Modify further

myfile.INSERT(", world!"); myfile.SNAPSHOT("Added greeting");

Rollback

myfile.ROLLBACK(0); // go back to first snapshot

View history

myfile.HISTORY();

Complexity Analysis
Insert version: O(1)

Snapshot: O(1)

Rollback by ID: O(1) (due to hashmap)

Rollback by parent: O(1)

History retrieval: O(k), k = number of snapshots

Memory usage:

Tree nodes: O(n)

Hashmap: O(n)

Linked lists (chaining): O(n)

Limitations & Future Work
Current implementation supports only single files.

No branching/merging like in Git.

Hashmap has fixed size (N = 10); resizing not supported.

All data is in-memory; no persistence to disk.

Possible improvements:

Dynamic resizing of hashmap

Persistent storage of versions

Support for multiple files and branching
