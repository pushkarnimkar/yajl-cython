#include <yajl/yajl_parse.h>
#include <iostream>
#include <fstream>

#if defined FLAT_ROWS
#  include "flat_rows.h"
#elif defined SIMPLE_ARRAY
#  include "simple_array.h"
#elif defined COMPLEX_OBJECT
#  include "complex_object.h"
#elif defined MORE_COMPLEX
#  include "more_complex.h"
#endif


#if defined SIMPLE_ARRAY
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

#elif defined FLAT_ROWS
void flat_rows(unsigned char* buffer, int length) {
    ITEMS_TYPE(int)* items = parse(buffer, length);

    if (items == NULL) {
        std::cout << "parsing failed" << std::endl;
        return;
    }
    
    for (ITEMS_TYPE(int)::iterator it = items->begin();
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

#elif defined COMPLEX_OBJECT
void complex_object(unsigned char* buffer, int length) {
    ComplexReturnType* res = parse(buffer, length);

    if (res == NULL) {
        std::cout << "parsing failed" << std::endl;
        return;
    }

    ITEMS_TYPE(float) items = res->items;
    
    for (ITEMS_TYPE(float)::iterator it = items.begin();
            it != items.end();
            it++) {
        float sum = 0;
        for (std::vector<float>::iterator itv = it->second.begin();
                itv != it->second.end();
                itv++) {
            sum += *itv;
        }
        std::cout << it->first << " => " << sum << std::endl;
    }
}

#elif defined MORE_COMPLEX
void more_complex(unsigned char* buffer, int length) {
    ComplexReturnType* res = parse(buffer, length);

    if (res == NULL) {
        std::cout << "parsing failed" << std::endl;
        return;
    }

    ITEMS_TYPE(float) items = res->items;
    
    for (ITEMS_TYPE(float)::iterator it = items.begin();
            it != items.end();
            it++) {
        float sum = 0;
        for (std::vector<float>::iterator itv = it->second.begin();
                itv != it->second.end();
                itv++) {
            sum += *itv;
        }
        std::cout << it->first << " => " << sum << std::endl;
    }
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

#if defined SIMPLE_ARRAY
    std::cout << "parsing simple array format ..." << std::endl;
    simple_array(buffer, length);

#elif defined FLAT_ROWS
    std::cout << "parsing flat rows format ..." << std::endl;
    flat_rows(buffer, length);

#elif defined COMPLEX_OBJECT
    std::cout << "parsing complex object format ..." << std::endl;
    complex_object(buffer, length);

#elif defined MORE_COMPLEX
    std::cout << "parsing more complex format ..." << std::endl;
    more_complex(buffer, length);

#else
    std::cout << "no parse format built!" << std::endl;

#endif

    std::cout << "process exiting ..." << std::endl;
    stream_json.close();
    delete [] buffer;
    return 0;
}

