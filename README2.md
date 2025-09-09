# Simplified Version Control System in C++

This project implements a simplified version control system (VCS) in C++. The goal is to simulate how real systems like Git manage file versions, snapshots, and rollbacks, while focusing on tree structures, hashing, and linked lists. It serves as a practical demonstration of data structure and algorithm concepts.

A file can undergo multiple updates. Some updates can be marked as snapshots (like commits), which freeze the state of the file. The system allows rolling back to previous versions and retrieving the history of snapshots.

## Data Structures Used

### (a) Tree Structure (TreeNode)

Each version of a file is represented as a node in a tree. This tree structure allows us to track the lineage of each version and efficiently navigate between different states of the file.

**Fields:**

*   `content`: File data at this version (e.g., a string representing the file's contents).
*   `created_timestamp`: Timestamp indicating when this version was created.
*   `snapshot_timestamp`: Timestamp indicating when this version was marked as a snapshot (set only if the node is a snapshot).
*   `message`: Commit message (empty if not a snapshot). This provides context for snapshot versions.
*   `version_id`: Unique identifier for this version.
*   `parent`: Pointer to the parent TreeNode (the previous version).
*   `children`: Vector of pointers to child TreeNodes (subsequent versions).

**Rationale:**

The tree naturally represents how versions branch from snapshots. Each path from the root to a node corresponds to a sequence of edits. The parent-child relationship clearly defines the history and evolution of the file.

### (b) Hashmap with Separate Chaining

Implemented as an array of linked lists. This allows for efficient storage and retrieval of TreeNode pointers based on their unique version IDs.

Maps `version_id` → `TreeNode*` for constant-time (average case) access.

**Rationale:**

Fast lookup of versions by ID is required for rollback operations. A custom hashmap demonstrates understanding of hashing and collision resolution techniques. Using a hashmap ensures that we can quickly access any version of the file given its ID, which is crucial for the rollback functionality.

### (c) Linked List

Each bucket of the hashmap uses a singly linked list. This handles collisions when different version IDs hash to the same index in the hashmap.

Stores key-value pairs (`version_id`, `TreeNode*`).

**Rationale:**

Handles collisions in the hash table using chaining. When multiple keys map to the same index, they are added to the linked list at that index. This prevents data loss and allows us to retrieve the correct TreeNode even when collisions occur.

## Classes and Methods

### TreeNode

Represents a version of the file. Supports marking itself as a snapshot. The TreeNode stores the file content, timestamps, commit messages, and relationships to other versions.

### LinkedList & Hashmap

Provide custom implementations for efficient storage and lookup of versions. These classes manage the underlying data structures used to store and retrieve file versions.

### `file` Class

Encapsulates all version-control logic. This class provides the main interface for interacting with the version control system.

**Key Methods:**

*   `new_version(content, parent)`: Creates a new version of the file as a child of the specified parent node. Returns a pointer to the new TreeNode.
*   `READ()`: Prints the content of the current version of the file.
*   `UPDATE(content)`: Modifies the content of the current version. If the current version is a snapshot, it creates a new child node with the updated content; otherwise, it directly modifies the content of the current node.
*   `INSERT(content)`: Appends new content to the current version.  Similar to `UPDATE`, it creates a new node if the current version is a snapshot.
*   `SNAPSHOT(message)`: Marks the current version as a snapshot, assigning it a snapshot timestamp and a commit message.
*   `ROLLBACK(version_id)`: Switches to a previous version with the given version ID.
*   `HISTORY()`: Prints all snapshots with their version IDs, timestamps, and commit messages.

## Algorithms

### Update Algorithm

*   If the active version is a snapshot → create a new child node.
*   Else → modify content directly in the current node.

**Time Complexity:** O(1) - Creating a new node or modifying existing content takes constant time.

### Snapshot Algorithm

*   Assign a snapshot timestamp and message to the current node.
*   Store (`version_id`, `node`) in the snapshot history (e.g., in a vector for easy iteration).

**Time Complexity:** O(1) - Assigning values and storing the node pointer are constant-time operations.

### Rollback Algorithm

*   If a version ID is provided → lookup the corresponding TreeNode in the hashmap.
*   If the version ID is not found or invalid, handle the error gracefully (e.g., return an error message or throw an exception).
*   Update the current version pointer to the retrieved TreeNode.

**Time Complexity:** O(1) on average, due to the hashmap lookup. In the worst-case scenario (collision in the hashmap), it could be O(n), where n is the number of elements in the linked list at the collided index, but with a good hash function, this is unlikely.

### History Traversal

*   Iterate over the collection of snapshotted nodes (e.g., a vector).
*   Print the version ID, timestamp, and message for each snapshot.

**Time Complexity:** O(k), where k is the number of snapshots. The algorithm iterates through each snapshot in the history.

## Example Workflow

This section demonstrates how to use the version control system with code snippets (assuming you have a `file` object named `myfile`).



**Explanation:**

1.  We start by creating a `file` object.
2.  The `UPDATE` method is used to set the initial content to "Hello".
3.  `SNAPSHOT` creates a snapshot with the message "Initial commit".
4.  `INSERT` appends ", world!" to the current content.
5.  Another `SNAPSHOT` is taken with the message "Added greeting".
6.  `ROLLBACK(0)` reverts the file to the version with ID 0 (the initial commit).
7.  `HISTORY` displays the history of snapshots, including their IDs, timestamps, and messages.

## Complexity Analysis

*   **Insert Version:** O(1) - Creating a new TreeNode and updating pointers.
*   **Snapshot:** O(1) - Assigning values and storing the node pointer.
*   **Rollback by ID:** O(1) (average case, due to hashmap) - Looking up the TreeNode in the hashmap.
*   **History Retrieval:** O(k) - where k is the number of snapshots.

**Memory Usage:**

*   Tree nodes: O(n) - where n is the total number of versions.
*   Hashmap: O(n) - Storing pointers to all TreeNodes.
*   Linked lists (chaining in hashmap): O(n) - In the worst-case scenario, all keys collide to the same index.

## Limitations & Future Work

This implementation has several limitations and areas for potential improvement:

*   **Single File Support:** The current implementation supports only a single file.  Extending it to handle multiple files would require managing a collection of `file` objects.
*   **No Branching/Merging:**  The system lacks branching and merging capabilities, which are essential features of modern VCS like Git.
*   **Fixed-Size Hashmap:** The hashmap has a fixed size (e.g., N = 10); dynamic resizing is not supported. This can lead to performance degradation if the number of versions exceeds the hashmap's capacity.
*   **In-Memory Storage:** All data is stored in memory; there is no persistence to disk.  This means that all changes are lost when the program exits.
*   **Limited Error Handling:**  The implementation lacks robust error handling for cases such as invalid version IDs or memory allocation failures.

**Possible Improvements:**

*   **Dynamic Resizing of Hashmap:** Implement dynamic resizing to maintain O(1) average lookup time as the number of versions grows.
*   **Persistent Storage of Versions:** Store versions on disk using file serialization or a database to preserve data across sessions.
*   **Support for Multiple Files and Branching:** Extend the system to manage multiple files and implement branching and merging functionalities.
*   **Improved Error Handling:** Add error handling to gracefully manage invalid input and potential failures.
*   **Implement a more sophisticated diff algorithm:** Currently, the entire file content is stored in each version. A diff algorithm could store only the changes between versions, reducing memory usage.

## Conclusion

This project demonstrates how core data structures—trees, linked lists, and hashmaps—can be combined to implement a simplified version control system. The design balances efficient version lookup (via hashing) with the logical organization of versions (via a tree structure). While the current implementation has limitations, it provides a solid foundation for understanding the fundamental principles behind version control systems.
