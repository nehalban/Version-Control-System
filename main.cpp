#include "system.hpp"
#include <sstream>

int main() {
    std::string line;
    std::cout << "Version Control System" << std::endl;
    std::cout << "Enter commands (Ctrl+Z to exit):" << std::endl;
    
    while (std::getline(std::cin, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        
        if (command == "RECENT_FILES") {
            int num = 1; // Default value is 1
            ss >> num;
            syswide(modified_before, num);
            continue;
        }
        else if (command == "BIGGEST_TREES") {
            int num = 1; // Default value is 1
            ss >> num;
            syswide(has_less_versions, num);
            continue;
        }
        
        std::string filename;
        ss >> filename;
        
        if (command == "CREATE") {
            CREATE(filename);
        }
        else {
            active_file = filenameMap.find(filename);
            
            if (!active_file) {
                std::cout << "Error: File '" << filename << "' not found" << std::endl;
                continue;
            }
            
            if (command == "READ") {
                active_file->READ();
            }
            else if (command == "INSERT") {
                std::string content;
                std::getline(ss, content);
                if (!content.empty() && content[0] == ' ') {
                    content = content.substr(1);
                }
                active_file->INSERT(content);
            }
            else if (command == "UPDATE") {
                std::string content;
                std::getline(ss, content);
                if (!content.empty() && content[0] == ' ') {
                    content = content.substr(1);
                }
                active_file->UPDATE(content);
            }
            else if (command == "SNAPSHOT") {
                std::string message;
                std::getline(ss, message);
                if (!message.empty() && message[0] == ' ') {
                    message = message.substr(1);
                }
                active_file->SNAPSHOT(message);
            }
            else if (command == "ROLLBACK") {
                int version_id;
                if (ss >> version_id) {
                    active_file->ROLLBACK(version_id);
                } else {
                    active_file->ROLLBACK();
                }
            }
            else if (command == "HISTORY") {
                active_file->HISTORY();
            }
            else {
                std::cout << "Unknown command: " << command << std::endl;
            }
        }
    }

    // Deallocate memory
    for (File* file_ptr : Allfiles) {
        delete file_ptr;
    }
    
    return 0;
}
