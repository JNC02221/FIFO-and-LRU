#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>







int main(int argc, char *argv[]) {
    std::string f, type, print, line;
    int n, per;

    if (argc == 5) {
        f = argv[1];
        n = atoi(argv[2]);
        type = argv[3];
        print = argv[4];
        std::cout << "Lru or Fifo";
    }
    else if (argc == 6) {
        f = argv[1];
        n = atoi(argv[2]);
        type = argv[3];
        per = atoi(argv[4]);
        print = argv[5];
        std::cout << "Vms";
    }

    int numLines = 0;
    std::ifstream my_file(argv[1]);

    if (my_file.is_open()) {
        std::cout << "File open" << std::endl;
        while (std::getline(my_file, line)) {
            ++numLines;
        }
        std::cout << "Number of Lines: " << numLines << std::endl;
        my_file.close();
    }
    else {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }
	std::cout << "Number of Lines: " << numLines << std::endl;

    return 0;
}
