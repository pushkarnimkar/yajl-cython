#include <map>
#include <vector>
#include <string>


typedef enum {
    wait_start,
    wait_key,
    wait_value,
    complete
} ParseState;


typedef struct {
    ParseState state;
    std::string key;
    std::map<std::string,std::vector<int>* > items;
} Context;


void init_context(Context*);

