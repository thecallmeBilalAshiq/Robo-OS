#include <iostream>
#include <fstream>
#include <dirent.h>
#include <cstdlib>
#include <cstring>
#include<unistd.h>
using namespace std;
int main() {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir("/proc");
    if (!dir) {
        std::cerr << "Failed to open /proc directory" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "List of running processes:" << std::endl;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char *name = entry->d_name;
            if (std::atoi(name) != 0) {
                char path[50];
                std::snprintf(path, sizeof(path), "/proc/%s/status", name);

                std::ifstream file(path);
                if (file.is_open()) {
                    std::string line;
                    while (std::getline(file, line)) {
                        if (std::strncmp(line.c_str(), "Name:", 5) == 0) {
                            std::cout << line.substr(6) << std::endl;
                            count++;
                            break;
                        }
                    }
                    file.close();
					sleep(2);
                }
            }
        }
    }

    closedir(dir);

    std::cout << "Total " << count << " processes running" << std::endl;

    return 0;
}
