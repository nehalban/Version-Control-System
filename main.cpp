#include <sstream>
#include "file.hpp"

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        if(command=="RECENT_FILES"){
            int num;
            ss>>num;
            syswide(modified_after, num);
            continue;
        }
        else if (command == "BIGGEST_TREES"){
            int num;
            ss>>num;
            syswide(has_more_versions, num);
            continue;
        }

        std::string filename;
        ss >> filename;

        if (command == "CREATE") CREATE(filename);
        else{
            active_file = filenameMap.find(filename);//lookup file* in hashmap
            if(command == "READ"){
                active_file->READ();
            }
            else if (command == "INSERT") {
                std::string content;
                getline(ss, content);
                while (!content.empty() && content[0] == ' ') content = content.substr(1);
                active_file->INSERT(content);
            }
            else if(command == "UPDATE"){
                std::string content;
                getline(ss, content);
                while (!content.empty() && content[0] == ' ') content = content.substr(1);
                active_file->UPDATE(content);
            }
            else if (command == "SNAPSHOT") {
                std::string message;
                std::getline(ss, message);
                if (!message.empty() && message[0] == ' ')
                    message = message.substr(1);
                active_file->SNAPSHOT(message);
            }
            else if (command == "ROLLBACK"){
                int version_id_;
                if(ss>>version_id_) active_file->ROLLBACK(version_id_);
                else active_file->ROLLBACK();
            }
            else if(command == "HISTORY"){
                active_file->HISTORY();
            }
            else {
                std::cout << "Unknown command: " << command << std::endl;
            }
        } 
    }
}
