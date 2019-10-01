#include <vector>
#include <string>
#include <map>

#define CLIENT_FAILURE 0
#define CLIENT_SUCCESS 1
#define ITEMS_TYPE std::map<std::string,std::vector<int>*>


typedef enum {
    wait_start,
    wait_key,
    wait_map,
    wait_value,
    complete
} ParseState;


typedef struct {
    ParseState state;
    std::string key;
    std::map<std::string,std::vector<int>* >* items;
} Context;


void init_context(Context*);

std::map<std::string,std::vector<int>* >* parse(unsigned char*, size_t);

