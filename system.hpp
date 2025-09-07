#include "file.hpp"

class Heap {
private:
    std::vector<const File*> elements;
    
    // Comparator function pointer:
    // For a MIN-heap, this should return true if (a > b)
    // For a MAX-heap, this should return true if (a < b)
    bool (*comparator)(const File*, const File*);

    // Helper functions to get parent and child indices
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

public:
    // Constructor accepts the comparator function
    Heap(bool (*comp)(const File*, const File*)) : comparator(comp) {}

    // Inserts an element into the heap
    void push(const File* value) {
        elements.push_back(value);
        int i = elements.size()-1;
        while (i > 0 && comparator(elements[parent(i)], elements[i])) {
            // Swap element with its parent
            const File* temp = elements[parent(i)];
            elements[parent(i)] = elements[i];
            elements[i] = temp;
            i = parent(i);
        }
    }

    // Removes the top element from the heap and returns it
    const File* pop() {
        if (elements.empty()) return nullptr;
        const File* temp_ = elements[0];
        elements[0] = elements.back();
        elements.pop_back();
        int ind = 0, priorityIndex;
        int l = leftChild(ind);
        int size_=elements.size();
        while(l<size_){
            if (comparator(elements[ind], elements[l])) {
                priorityIndex = l;
            }
            int r = rightChild(ind);
            if (r < size_ && comparator(elements[priorityIndex], elements[r])) {
                priorityIndex = r;
            }
            if (ind != priorityIndex) {
                // Swap with the child that has higher priority
                const File* temp = elements[ind];
                elements[ind] = elements[priorityIndex];
                elements[priorityIndex] = temp;
            }
            ind = priorityIndex;
            l=leftChild(ind);
        }
        return temp_;
    }

    const File* top() {
        return elements[0];
    }
    
    int size() const {
        return elements.size();
    }

    // Checks if the heap is empty
    bool empty() const {
        return elements.empty();
    }
};

class Dict{
    static constexpr int N=10;
    std::vector<std::vector<File*>> hashtable{N};
public:
    int hash(std::string key){
        int hash=0;
        for(char x:key) hash+=(int)x;
        return hash%N;
    }
    void insert(File& A){
        hashtable[hash(A.filename)].push_back(&A);
    }
    File* find(std::string filename){
        int ind = hash(filename);
        for(File* file:hashtable[ind]){
            if(file->filename == filename) return file;
        }
        return nullptr;
    }
};

File* active_file;
std::vector<File*> Allfiles;
Dict filenameMap;
void CREATE(std::string filename){
    active_file = new File(filename);
    Allfiles.push_back(active_file);
    filenameMap.insert(*active_file);
}

bool modified_after(const File* A, const File* B){
    return A->last_modified>B->last_modified;
};
bool has_more_versions(const File* A, const File* B){
    return A->total_versions>B->total_versions;
};

void syswide(bool (*comp)(const File*A, const File* B), int num){
    Heap H(comp);
    if(num<=Allfiles.size()){
        std::sort(Allfiles.begin(), Allfiles.end(), comp);
        for(File* file:Allfiles) std::cout<<file->filename<<' ';
        std::cout<<std::endl;
    }
    else{
        for (int i = 0; i < num; i++){
            H.push(Allfiles[i]);
        }
        for (int i = num; i < Allfiles.size(); i++){
            if(comp(Allfiles[i], H.top())){
                H.pop();
                H.push(Allfiles[i]);
            }
        }
        for (int i = 0; i < num; i++)
        {
            std::cout<<H.pop()->filename<<' ';
        }
        std::cout<<std::endl;
        
    }
}

