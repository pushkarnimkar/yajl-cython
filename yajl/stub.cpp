#include <yajl/yajl_parse.h>
#include <iostream>
#include <fstream>

#if defined FLAT_ROWS
#  include "flat_rows.h"
#elif defined SIMPLE_ARRAY
#  include "simple_array.h"
#endif


#if defined FLAT_ROWS
void flat_rows(unsigned char* buffer, int length) {
    ITEMS_TYPE* items = parse(buffer, length);

    if (items == NULL) {
        std::cout << "parsing failed" << std::endl;
        return;
    }
    
    for (ITEMS_TYPE::iterator it = items->begin();
            it != items->end();
            it++) {
        int sum = 0;
        for (std::vector<int>::iterator itv = it->second->begin();
                itv != it->second->end();
                itv++) {
            sum += *itv;
        }
        std::cout << it->first << " => " << sum << std::endl;
    }
}
#elif defined SIMPLE_ARRAY
void simple_array(unsigned char* buffer, int length) {
    std::vector<int>* items = parse(buffer, length);
    if (items == NULL) {
        std::cout << "parsing failed" << std::endl;
        return;
    }

    int sum = 0;
    for (std::vector<int>::iterator it = items->begin();
            it != items->end();
            it++) {
        sum += *it;
    }

    std::cout << "items sum up to " << sum << std::endl;
}
#endif


int
main(int argc, char** argv) {
    std::ifstream stream_json;
    stream_json.open(argv[1]);

    stream_json.seekg(0, stream_json.end);
    int length = stream_json.tellg();
    stream_json.seekg(0, stream_json.beg);

    unsigned char* buffer = new unsigned char[length];
    stream_json.read((char*) buffer, length);

#if defined FLAT_ROWS
    std::cout << "parsing flat rows format ..." << std::endl;
    flat_rows(buffer, length);
#elif defined SIMPLE_ARRAY
    std::cout << "parsing simple array format ..." << std::endl;
    simple_array(buffer, length);
#else
    std::cout << "no parse format built!" << std::endl;
#endif

    std::cout << "process exiting ..." << std::endl;
    stream_json.close();
    delete [] buffer;
    return 0;
}

