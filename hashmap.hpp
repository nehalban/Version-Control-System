#pragma once

#include <vector>
#include <string>

template <typename K, typename V>
class HashMap {
private:
    // A key-value pair entry
    struct Entry {
        const K key;
        V value;
    };

    // A node in our custom singly linked list for collision resolution
    struct Node {
        Entry data;
        Node* next;

        Node(const K& key, const V& value) : data{key, value}, next(nullptr) {}
    };

    // The hash table is a vector of pointers to linked list heads
    std::vector<Node*> table;
    
    // The hash function provided by the user (as a function pointer)
    size_t (*hasher)(const K&);
    
    size_t currentSize;

public:
    // Constructor: initializes the table and stores the hash function pointer
    explicit HashMap(size_t tableSize, size_t (*hashFunc)(const K&))
        : table(tableSize, nullptr), hasher(hashFunc), currentSize(0) {}

    // Destructor: must clean up all dynamically allocated nodes
    ~HashMap() {
        for (size_t i = 0; i < table.size(); ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
    }

    // Inserts a key-value pair. If the key already exists, it updates the value.
    void insert(const K& key, const V& value) {
        size_t index = hasher(key) % table.size();
        Node* head = table[index];

        // Search for the key in the chain
        Node* current = head;
        while (current != nullptr) {
            if (current->data.key == key) {
                current->data.value = value; // Update existing key
                return;
            }
            current = current->next;
        }

        // Key not found, add a new node to the front of the list
        Node* newNode = new Node(key, value);
        newNode->next = head;
        table[index] = newNode;
        currentSize++;
    }

    // Accesses a value by key. Returns a pointer to the value,
    // or nullptr if the key is not found.
    V* get(const K& key) {
        size_t index = hasher(key) % table.size();
        Node* current = table[index];

        while (current != nullptr) {
            if (current->data.key == key) {
                return &(current->data.value);
            }
            current = current->next;
        }
        return nullptr; // Key not found
    }

    // Removes a key-value pair. Returns true if successful, false otherwise.
    bool remove(const K& key) {
        size_t index = hasher(key) % table.size();
        Node* current = table[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->data.key == key) {
                if (prev == nullptr) { // It's the head of the list
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                currentSize--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false; // Key not found
    }

    // Returns the number of elements in the map
    size_t size() const {
        return currentSize;
    }
};







//------------------------------------------------------------------------------//

#pragma once

#include <vector>

template <typename T>
class Heap {
private:
    std::vector<T> elements;
    
    // Comparator function pointer:
    // For a MIN-heap, this should return true if `a` has lower priority than `b` (a > b)
    // For a MAX-heap, this should return true if `a` has lower priority than `b` (a < b)
    bool (*comparator)(const T&, const T&);

    // Helper functions to get parent and child indices
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    // Moves an element up the heap to maintain the heap property
    void siftUp(int i) {
        while (i > 0 && comparator(elements[parent(i)], elements[i])) {
            // Swap element with its parent
            T temp = elements[parent(i)];
            elements[parent(i)] = elements[i];
            elements[i] = temp;
            i = parent(i);
        }
    }

    // Moves an element down the heap to maintain the heap property
    void siftDown(int i) {
        int priorityIndex = i;
        int l = leftChild(i);
        if (l < size() && comparator(elements[priorityIndex], elements[l])) {
            priorityIndex = l;
        }
        int r = rightChild(i);
        if (r < size() && comparator(elements[priorityIndex], elements[r])) {
            priorityIndex = r;
        }
        if (i != priorityIndex) {
            // Swap with the child that has higher priority
            T temp = elements[i];
            elements[i] = elements[priorityIndex];
            elements[priorityIndex] = temp;
            siftDown(priorityIndex);
        }
    }

public:
    // Constructor accepts the comparator function
    explicit Heap(bool (*comp)(const T&, const T&)) : comparator(comp) {}

    // Inserts an element into the heap
    void push(const T& value) {
        elements.push_back(value);
        siftUp(elements.size() - 1);
    }

    // Removes the top element from the heap
    void pop() {
        if (elements.empty()) return;
        elements[0] = elements.back();
        elements.pop_back();
        if (!elements.empty()) {
            siftDown(0);
        }
    }

    // Returns the top element
    T& top() {
        return elements[0];
    }
    
    const T& top() const {
        return elements[0];
    }

    // Returns the number of elements
    int size() const {
        return elements.size();
    }

    // Checks if the heap is empty
    bool empty() const {
        return elements.empty();
    }
};
//===================================================================================================//
#include <iostream>
#include <string>
#include <ctime>

// ---- Define your custom classes ----
struct File {
    std::string filename;
    time_t lastModified;
    int numVersions;
};

struct LinkedList {
    int id;
};

// ---- Define Hashing Functions ----
// A simple hash function for strings (djb2 algorithm)
size_t hashString(const std::string& s) {
    size_t hash = 5381;
    for (size_t i = 0; i < s.length(); ++i) {
        hash = ((hash << 5) + hash) + s[i]; // hash * 33 + c
    }
    return hash;
}

// A simple identity hash for integers
size_t hashInt(const int& i) {
    return i;
}

// ---- Define Heap Comparator Functions ----
// Comparator for a MIN-heap based on timestamp (to find k-most-recent)
// Returns true if `a` is OLDER than `b`
bool isOlderFile(const File& a, const File& b) {
    return a.lastModified > b.lastModified;
}

// Comparator for a MIN-heap based on versions (to find k-most-versions)
// Returns true if `a` has FEWER versions than `b`
bool hasFewerVersions(const File& a, const File& b) {
    return a.numVersions > b.numVersions;
}

// ---- Main Application ----
int main() {
    // ==========================================================
    // HashMap Example
    // ==========================================================
    std::cout << "--- HashMap Example ---" << std::endl;
    HashMap<std::string, File*> fileMap(100, &hashString);

    File* fileA = new File{"report.docx", time(0), 5};
    fileMap.insert("report.docx", fileA);

    // Retrieve a file using get()
    File** pFile = fileMap.get("report.docx");
    if (pFile != nullptr) {
        std::cout << "Found file: " << (*pFile)->filename << std::endl;
    } else {
        std::cout << "File not found." << std::endl;
    }

    // ==========================================================
    // Heap Example
    // ==========================================================
    std::cout << "\n--- Heap Example ---" << std::endl;
    std::vector<File> allFiles;
    allFiles.push_back(File{"a.txt", 1672531200, 5});
    allFiles.push_back(File{"b.log", 1672876800, 2});
    allFiles.push_back(File{"c.json", 1672617600, 8});
    allFiles.push_back(File{"d.xml", 1673136000, 1});
    allFiles.push_back(File{"e.dat", 1672704000, 15});
    
    const int k = 3;

    // --- Scenario 1: K most recently modified files ---
    std::cout << "Finding the " << k << " most recently modified files:" << std::endl;
    Heap<File> recentFilesHeap(&isOlderFile); // Min-heap on timestamp

    for (size_t i = 0; i < allFiles.size(); ++i) {
        if (recentFilesHeap.size() < k) {
            recentFilesHeap.push(allFiles[i]);
        } else if (allFiles[i].lastModified > recentFilesHeap.top().lastModified) {
            recentFilesHeap.pop();
            recentFilesHeap.push(allFiles[i]);
        }
    }
    
    while (!recentFilesHeap.empty()) {
        std::cout << "  - " << recentFilesHeap.top().filename 
                  << " (Timestamp: " << recentFilesHeap.top().lastModified << ")" << std::endl;
        recentFilesHeap.pop();
    }
    
    // --- Scenario 2: K files with most versions ---
    std::cout << "\nFinding the " << k << " files with the most versions:" << std::endl;
    Heap<File> mostVersionsHeap(&hasFewerVersions); // Min-heap on versions

    for (size_t i = 0; i < allFiles.size(); ++i) {
        if (mostVersionsHeap.size() < k) {
            mostVersionsHeap.push(allFiles[i]);
        } else if (allFiles[i].numVersions > mostVersionsHeap.top().numVersions) {
            mostVersionsHeap.pop();
            mostVersionsHeap.push(allFiles[i]);
        }
    }
    
    while (!mostVersionsHeap.empty()) {
        std::cout << "  - " << mostVersionsHeap.top().filename 
                  << " (Versions: " << mostVersionsHeap.top().numVersions << ")" << std::endl;
        mostVersionsHeap.pop();
    }

    // Clean up dynamically allocated memory
    delete fileA;

    return 0;
}
