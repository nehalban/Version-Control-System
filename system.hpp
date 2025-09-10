#include "file.hpp"

class Heap {
private:
    std::vector<const File*> elements;
    bool (*comparator)(const File*, const File*);
    
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
    
    void heapifyUp(int i) {
        while (i > 0 && comparator(elements[parent(i)], elements[i])) {
            std::swap(elements[i], elements[parent(i)]);
            i = parent(i);
        }
    }
    
    void heapifyDown(int i) {
        int size = elements.size();
        while (true) {
            int largest = i;
            int left = leftChild(i);
            int right = rightChild(i);
            
            if (left < size && comparator(elements[largest], elements[left])) {
                largest = left;
            }
            if (right < size && comparator(elements[largest], elements[right])) {
                largest = right;
            }
            
            if (largest != i) {
                std::swap(elements[i], elements[largest]);
                i = largest;
            } else {
                break;
            }
        }
    }
    
public:
    Heap(bool (*comp)(const File*, const File*)) : comparator(comp) {}
    
    void push(const File* value) {
        elements.push_back(value);
        heapifyUp(elements.size() - 1);
    }
    
    const File* pop() {
        if (elements.empty()) return nullptr;
        
        const File* result = elements[0];
        elements[0] = elements.back();
        elements.pop_back();
        
        if (!elements.empty()) {
            heapifyDown(0);
        }
        
        return result;
    }
    
    const File* top() const {
        return elements.empty() ? nullptr : elements[0];
    }
    
    int size() const {
        return elements.size();
    }
    
    bool empty() const {
        return elements.empty();
    }
};

class Dict {
    static constexpr int N = 53; //Prime
    std::vector<std::vector<File*>> hashtable;
    
public:
    Dict() : hashtable(N) {}
    
    int hash(std::string key) {
        int hash_value = 0;
        for (char c : key) {
            hash_value = (hash_value * 31 + (int)c) % N;
        }
        return hash_value;
    }
    
    void insert(File* file) {
        if (file) {
            hashtable[hash(file->filename)].push_back(file);
        }
    }
    
    File* find(std::string filename) {
        int index = hash(filename);
        for (File* file : hashtable[index]) {
            if (file && file->filename == filename) {
                return file;
            }
        }
        return nullptr;
    }
};

// Global variables
File* active_file = nullptr;
std::vector<File*> Allfiles;
Dict filenameMap;

void CREATE(std::string filename) {
    // Check if file already exists
    if (filenameMap.find(filename)) {
        std::cout << "Error: File '" << filename << "' already exists" << std::endl;
        return;
    }
    
    active_file = new File(filename);
    Allfiles.push_back(active_file);
    filenameMap.insert(active_file);
    std::cout << "File '" << filename << "' created successfully" << std::endl;
}

// Comparator for RECENT_FILES (MIN heap - older files have lower priority)
bool modified_after(const File* A, const File* B) {
    return A->last_modified < B->last_modified;
}

// Comparator for BIGGEST_TREES (MIN heap - fewer versions have lower priority)
bool has_more_versions(const File* A, const File* B) {
    return A->total_versions < B->total_versions;
}

void syswide(bool (*comp)(const File* A, const File* B), int num) {
    if (num <= 0) {
        std::cout << "Error: Invalid number specified" << std::endl;
        return;
    }
    
    Heap H(comp);
    
    // Build a min-heap of size 'num' with the first 'num' files
    for (size_t i = 0; i < Allfiles.size() && i < (size_t)num; i++) {
        H.push(Allfiles[i]);
    }
    
    // For remaining files, if better than min, replace min
    for (size_t i = num; i < Allfiles.size(); i++) {
        if (H.top() && !comp(Allfiles[i], H.top())) {
            H.pop();
            H.push(Allfiles[i]);
        }
    }
    
    // Extract all elements and store in vector for reverse printing
    std::vector<const File*> result;
    while (!H.empty()) {
        result.push_back(H.pop());
    }
    
    // Print in descending order (reverse of min-heap extraction)
    for (int i = result.size() - 1; i >= 0; i--) {
        std::cout << result[i]->filename;
        if (i > 0) std::cout << " ";
    }
    std::cout << std::endl;
}
