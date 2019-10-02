#include <yajl/yajl_parse.h>
#include <vector>

#define CLIENT_SUCCESS 1
#define CLIENT_FAILURE 0


typedef struct {
    std::vector<int>* items;
} Context;

void init_context(Context*);

std::vector<int>* 
parse(const unsigned char* buffer, int length);
