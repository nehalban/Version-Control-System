#include "data_structures.hpp"
#include <iostream>

struct File {
    std::string filename;
    time_t last_modified;
    TreeNode* root;
    TreeNode* active_version;
    Hashmap version_map;
    int total_versions = 0;
    
    File(std::string filename_) : filename(filename_), last_modified(time(0)) {
        // Create root version with ID 0
        root = new TreeNode(0, "", nullptr);
        total_versions = 1;
        version_map.insert(root);
        active_version = root;
        
        // Root should be automatically snapshotted according to spec
        root->snapshot("Initial version");
    }
    
    TreeNode* new_version(std::string content = "", TreeNode* parent = nullptr) {
        TreeNode* new_node = new TreeNode(total_versions, content, parent);
        version_map.insert(new_node);
        total_versions++;
        active_version = new_node;
        last_modified = time(0);
        return new_node;
    }
    
    void READ() {
        if (active_version) {
            std::cout << active_version->content << std::endl;
        } else {
            std::cout << "Error: No active version" << std::endl;
        }
    }
    
    void UPDATE(std::string content) {
        if (!active_version) {
            std::cout << "Error: No active version" << std::endl;
            return;
        }
        
        if (active_version->is_snapshot()) {
            // Create new version as child of current
            new_version(content, active_version);
        } else {
            // Modify in place
            active_version->content = content;
            last_modified = time(0);
        }
    }
    
    void INSERT(std::string content) {
        if (!active_version) {
            std::cout << "Error: No active version" << std::endl;
            return;
        }
        std::string actual_content = active_version->content + content;
        UPDATE(actual_content);
    }
    
    void SNAPSHOT(std::string message) {
        if (!active_version) {
            std::cout << "Error: No active version" << std::endl;
            return;
        }
        
        if (active_version->is_snapshot()) {
            std::cout << "This version is already a snapshot (ID: " 
                     << active_version->version_id << ")" << std::endl;
        } else {
            active_version->snapshot(message);
        }
    }
    
    void ROLLBACK() {
        if (!active_version) {
            std::cout << "Error: No active version" << std::endl;
            return;
        }
        
        if (active_version->parent) {
            active_version = active_version->parent;
        } else {
            std::cout << "Error: Cannot rollback from root version" << std::endl;
        }
    }
    
    void ROLLBACK(int version_id) {
        TreeNode* target = version_map.find(version_id);
        if (target) {
            active_version = target;
        } else {
            std::cout << "Error: Version " << version_id << " not found" << std::endl;
        }
    }
    
    void HISTORY() {
        if (!active_version) {
            std::cout << "Error: No active version" << std::endl;
            return;
        }
        
        std::vector<TreeNode*> path;
        TreeNode* temp = active_version;
        
        // Traverse from active to root, collecting snapshots
        while (temp) {
            if (temp->is_snapshot()) {
                path.push_back(temp);
            }
            temp = temp->parent;
        }
        
        // Print in chronological order (root to active)
        for (int i = path.size() - 1; i >= 0; i--) {
            std::cout << "Version " << path[i]->version_id 
                     << " | Time: " << path[i]->snapshot_timestamp 
                     << " | Message: " << path[i]->message << std::endl;
        }
    }
    
    ~File() {
        delete root; // This triggers recursive deletion of all nodes
    }
};