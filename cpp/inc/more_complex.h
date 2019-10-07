#include <vector>
#include <string>
#include <map>

#define CLIENT_FAILURE 0
#define CLIENT_SUCCESS 1
#define ITEMS_TYPE(t) std::map<std::string,std::vector<t> >


typedef enum {
    wait_start,
    wait_map0,
    wait_key0,
    wait_value0,
    wait_map1,
    wait_key1,
    wait_value1,
    wait_key2,
    wait_value2,
    complete
} ParseState;


typedef struct {
    // state holds parser state
    ParseState state;

    // row_index holds row index for each row. with each new row this should 
    // be increased by 1, thus, initialized with -1.
    int row_index;

    // holds recently encountered key
    std::string key;

    // index map records row_index of each entry in items. this gives 
    // flexibility of handling variable number of entries per row
    ITEMS_TYPE(int) index;

    // saves actual data
    ITEMS_TYPE(float) items;
} Context;


// convenience structure for return type
typedef struct {
    ITEMS_TYPE(int) index;
    ITEMS_TYPE(float) items;
} ComplexReturnType;


// initializing routine
void init_context(Context*);

ComplexReturnType* parse(const unsigned char*, size_t);

