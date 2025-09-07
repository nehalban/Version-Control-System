#include "data_structures.hpp"
#include <iostream>
// File Structure

struct File{
    std::string filename;
    time_t last_modified;
    TreeNode* root; // Your implementation of the tree
    TreeNode* active_version;
    Hashmap version_map; // Your implementation of the HashMap
    int total_versions=0;
    //std::unordered_map<string, int> commit_messages; // commit message to version number
    File(std::string filename_): root(new_version()), filename(filename_), last_modified(time(0)){
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
        last_modified = time(0);
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
        }
    }
    void ROLLBACK(){
        active_version=active_version->parent;
    }
    void ROLLBACK(int version_id){
        active_version = version_map.find(version_id);
    }

    void HISTORY(){
        std::vector<TreeNode*> V;
        TreeNode* temp = active_version;
        if(temp->snapshot_timestamp) V.push_back(temp);
        while(temp->parent) {
            temp = temp->parent;
            V.push_back(temp);
        }
        for(int i=V.size()-1; i>=0; i--){
            temp = V[i];
            std::cout<<temp->version_id<<' '<<temp->snapshot_timestamp<<' '<<temp->message<<std::endl;
        }
    }
    ~File() {
    delete root; // This starts the chain reaction to delete all TreeNodes
}
};
