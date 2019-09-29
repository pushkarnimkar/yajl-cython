#include <vector>
#include <iostream>


int
main(int argc, char** argv) {
    std::vector<int> vec;
    std::vector<int>::iterator it = vec.begin();
    while (it != vec.end()) {
        std::cout << *(it++) << std::endl;
    }
    
    return 0;
}

