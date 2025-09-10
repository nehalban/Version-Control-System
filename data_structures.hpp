#include <vector>
#include <string>
#include <ctime>

struct TreeNode {
    int version_id;
    std::string content;
    std::string message; // Empty if not a snapshot
    time_t created_timestamp;
    time_t snapshot_timestamp; // 0 if not a snapshot
    TreeNode* parent;
    std::vector<TreeNode*> children;
    
    TreeNode(int version_id_, std::string content_="", TreeNode* parent_=nullptr)
        : version_id(version_id_), content(content_), parent(parent_), 
          created_timestamp(time(0)), snapshot_timestamp(0), message("") {
        if (parent) {
            parent->children.push_back(this);
        }
    }
    
    void snapshot(std::string message_) {
        message = message_;
        snapshot_timestamp = time(0);
    }
    
    bool is_snapshot() const {
        return snapshot_timestamp != 0;
    }
    
    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child; // Recursive deletion
        }
    }
};

class LinkedList {
protected:
    struct Node {
        TreeNode* data;
        Node* next;
        Node(TreeNode* data_=nullptr, Node* next_=nullptr): data(data_), next(next_) {}
    };
    Node* head = nullptr;
    
public:
    void push(TreeNode* node_) {
        Node* temp = new Node(node_, head);
        head = temp;
    }
    
    TreeNode* find(int key_) {
        Node* temp = head;
        while (temp) {
            if (temp->data && temp->data->version_id == key_) {
                return temp->data;
            }
            temp = temp->next;
        }
        return nullptr;
    }
    
    ~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class Hashmap {
    static constexpr int N = 17; // Prime number for better distribution
    std::vector<LinkedList*> hashtable;
    
public:
    Hashmap() : hashtable(N) {
        for (int i = 0; i < N; i++) {
            hashtable[i] = new LinkedList();
        }
    }
    
    int hash(int key) {
        return key % N;
    }
    
    TreeNode* insert(TreeNode* node_) {
        if (node_) {
            hashtable[hash(node_->version_id)]->push(node_);
        }
        return node_;
    }
    
    TreeNode* find(int key) {
        return hashtable[hash(key)]->find(key);
    }
    
    ~Hashmap() {
        for (auto list : hashtable) {
            delete list;
        }
    }
};