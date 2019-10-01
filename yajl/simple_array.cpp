#include <iostream>
#include "simple_array.h"


void init_context(Context* ctx) {
    ctx->items = new std::vector<int>();
}


static int parse_integer(void* ctx, long long val) {
    Context* ctx_ = (Context*) ctx;
    ctx_->items->push_back((int) val);
    return CLIENT_SUCCESS;
}


static yajl_callbacks callbacks = {
    NULL,
    NULL,
    parse_integer,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL 
};


std::vector<int>*
parse(unsigned char* buffer, int length) {
    Context ctx;
    init_context(&ctx);

    yajl_handle hand = yajl_alloc(&callbacks, NULL, (void*) &ctx);
    yajl_status stat = yajl_parse(hand, buffer, length);

    std::cerr << "parse status: " << stat << std::endl;
    yajl_free(hand);

    if (stat != yajl_status_ok) {
        delete ctx.items;
        return NULL;
    }

    return ctx.items;
}

