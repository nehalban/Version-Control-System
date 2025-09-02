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
};

