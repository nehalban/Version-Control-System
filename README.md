This project implements a simplified version control system (VCS) in C++.
The goal is to simulate how real systems like Git manage file versions, snapshots, and rollbacks, while focusing on tree structures, hashing, and linked lists.

A file can undergo multiple updates. Some updates can be marked as snapshots (like commits), which freeze the state of the file. The system allows rolling back to previous versions and retrieving the history of snapshots.

2. Data Structures Used
(a) Tree Structure (TreeNode)

Each version of a file is represented as a node in a tree.

Fields:

content: file data at this version

created_timestamp: creation time

snapshot_timestamp: set only if node is a snapshot

message: commit message (empty if not a snapshot)

version_id: unique identifier

parent and children: links in the version tree

Rationale:
The tree naturally represents how versions branch from snapshots.
Each path from root to a node corresponds to a sequence of edits.

(b) Hashmap with Separate Chaining

Implemented as an array of linked lists.

Maps version_id → TreeNode* for constant-time access.

Rationale:
Fast lookup of versions by ID is required for rollback.
A custom hashmap demonstrates understanding of hashing and collision resolution.

(c) Linked List

Each bucket of the hashmap uses a singly linked list.

Stores key-value pairs (version_id, TreeNode*).

Rationale:
Handles collisions in the hash table using chaining.

3. Classes and Methods
TreeNode

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

4. Algorithms

Update Algorithm

If the active version is a snapshot → create new child node.

Else → modify content directly.
Time Complexity: O(1)

Snapshot Algorithm

Assign a snapshot timestamp and message.

Store (version_id, node) in snapshot history.
Time Complexity: O(1)

Rollback Algorithm

If version ID is provided → lookup in hashmap.

If not → move to parent.
Time Complexity: O(1) (due to hashmap)

History Traversal

Iterate over vector of snapshotted nodes.
Time Complexity: O(k), where k = number of snapshots

5. Example Workflow

Update content

myfile.UPDATE("Hello");


Take snapshot

myfile.SNAPSHOT("Initial commit");


Modify further

myfile.INSERT(", world!");
myfile.SNAPSHOT("Added greeting");


Rollback

myfile.ROLLBACK(0); // go back to first snapshot


View history

myfile.HISTORY();

6. Complexity Analysis

Insert version: O(1)

Snapshot: O(1)

Rollback by ID: O(1) (due to hashmap)

Rollback by parent: O(1)

History retrieval: O(k), k = number of snapshots

Memory usage:

Tree nodes: O(n)

Hashmap: O(n)

Linked lists (chaining): O(n)

7. Limitations & Future Work

Current implementation supports only single files.

No branching/merging like in Git.

Hashmap has fixed size (N = 10); resizing not supported.

All data is in-memory; no persistence to disk.

Possible improvements:

Dynamic resizing of hashmap

Persistent storage of versions

Support for multiple files and branching

8. Conclusion

This assignment demonstrates how core data structures — trees, linked lists, and hashmaps — can be combined to implement a simplified version-control system.
The design balances efficient version lookup (via hashing) with logical organization of versions (via tree structure).
