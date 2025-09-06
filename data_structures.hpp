#include <vector>
#include <string>
#include <ctime>

struct TreeNode {
    int version_id;
    std::string content;
    std::string message; // Empty if not a snapshot
    time_t created_timestamp;
    time_t snapshot_timestamp; // Null if not a snapshot
    TreeNode* parent;
    std::vector<TreeNode*> children;
    TreeNode(int version_id_, std::string content_="", TreeNode* parent_=nullptr): version_id(version_id_), content(content_), parent(parent_), created_timestamp(time(0)), snapshot_timestamp(0){
    }
    void snapshot(std::string message_){
        message= message_;
        snapshot_timestamp = time(0);
    }
};

class LinkedList{
protected:
    struct Node{
        TreeNode* data;
        Node *next;
        Node(TreeNode* data_=nullptr, Node* next_=nullptr): data(data_), next(next_){};
    };
    Node* head = nullptr;
public:
    void push(TreeNode* node_){
        Node* temp = new Node(node_, head);
        head = temp;
    }
    TreeNode* find(int key_){
        Node* temp = head;
        if(!head) return NULL;
        while(temp->data->version_id!=key_&& temp->next) temp = temp->next;
        if(temp->data->version_id == key_) return temp->data;
        else return NULL;
    }
    ~LinkedList(){
        while(head){
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};


class Hashmap{
    static constexpr int N=10;
    std::vector<LinkedList*> hashtable{N};
    
public:
    Hashmap(){
        for (int i = 0; i < N; i++) {
            hashtable[i] = new LinkedList();
        }
    };
    int hash(int key){
        return key%N;
    }
    TreeNode* insert(TreeNode* node_){
        hashtable[hash(node_->version_id)]->push(node_);
        return node_;
    };
    TreeNode* find(int key){
        return hashtable[hash(key)]->find(key);
    }
    ~Hashmap(){
        for (auto i:hashtable) delete i;
    }
};

class Heap {
private:
    std::vector<File> elements;
    
    // Comparator function pointer:
    // For a MIN-heap, this should return true if (a > b)
    // For a MAX-heap, this should return true if (a < b)
    bool (*comparator)(const File&, const File&);

    // Helper functions to get parent and child indices
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

public:
    // Constructor accepts the comparator function
    Heap(bool (*comp)(const File&, const File&)) : comparator(comp) {}

    // Inserts an element into the heap
    void push(const File& value) {
        elements.push_back(value);
        int i = elements.size()-1;
        while (i > 0 && comparator(elements[parent(i)], elements[i])) {
            // Swap element with its parent
            File temp = elements[parent(i)];
            elements[parent(i)] = elements[i];
            elements[i] = temp;
            i = parent(i);
        }
    }

    // Removes the top element from the heap and returns it
    File pop() {
        if (elements.empty()) return;
        File temp_ = elements[0];
        elements[0] = elements.back();
        elements.pop_back();
        int ind = 0, priorityIndex;
        int l = leftChild(ind);
        int size_=elements.size();
        while(l<size_){
            if (comparator(elements[ind], elements[l])) {
                priorityIndex = l;
            }
            int r = rightChild(0);
            if (r < size_ && comparator(elements[priorityIndex], elements[r])) {
                priorityIndex = r;
            }
            if (ind != priorityIndex) {
                // Swap with the child that has higher priority
                File temp = elements[ind];
                elements[ind] = elements[priorityIndex];
                elements[priorityIndex] = temp;
            }
            ind = priorityIndex;
            l=leftChild(ind);
        }
        return temp_;
    }

    File& top() {
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
