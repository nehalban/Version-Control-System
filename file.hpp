#include "hashmap.hpp"
#include <iostream> //for READ
// File Structure

struct File{
    std::string filename;
    TreeNode* root; // Your implementation of the tree
    TreeNode* active_version;
    Hashmap version_map; // Your implementation of the HashMap
    int total_versions=0;
    std::vector<TreeNode*> snapshotted;
    //std::unordered_map<string, int> commit_messages; // commit message to version number
    File(std::string filename_): root(new_version()), filename(filename_){
    }
    TreeNode* new_version(std::string content="", TreeNode* parent=nullptr){
        active_version = version_map.insert(new TreeNode(total_versions, content, parent));
        total_versions++;
        return active_version;
    }


    void READ(){
        std::cout<<active_version->content<<std::endl;
    }
    void UPDATE(std::string content){
        if(active_version->snapshot_timestamp){
            active_version->children.push_back(new_version(content, active_version));
        }
        else{
            active_version->content=content;
        }
    }
    void INSERT(std::string content){
        std::string actual_content = active_version->content + content;
        UPDATE(actual_content);
    }
    void SNAPSHOT(std::string message){
        time_t snap = active_version->snapshot_timestamp;
        if(snap){
            std::cout<<"This version is already snapshot at "<<snap<<std::endl;
        }
        else{
            active_version->snapshot(message);
            snapshotted.push_back(active_version);
        }
    }
    void ROLLBACK(int version_id=NULL){
        if(version_id==NULL) active_version=active_version->parent;
        else active_version = version_map.find(version_id);
    }
    void HISTORY(){
        for (TreeNode* node : snapshotted){
            std::cout<<node->version_id<<' '<<node->snapshot_timestamp<<' '<<node->message<<std::endl;
        }
    }
};
