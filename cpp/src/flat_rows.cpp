#include <yajl/yajl_parse.h>
#include <stdio.h>
#include <iostream>
#include "flat_rows.h"


void 
init_context(Context* ctx) {
    // allocating items map
    ITEMS_TYPE(int)* items = new ITEMS_TYPE(int)();
    ctx->items = items;
    
    // initializing state
    ctx->state = wait_start;
}


static int
parse_start_array(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_start) {
        ctx->state = wait_map;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_start_map(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_map) {
        ctx->state = wait_key;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_map_key(void* ctx_, const unsigned char* key_, size_t size) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_key) {
        ctx->state = wait_value;
        std::string key((const char*)key_, (const char*)key_ + size);
        ctx->key = key;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_integer(void* ctx_, long long value) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_value) {
        ctx->state = wait_key;

        if (ctx->items->count(ctx->key) == 0) {
            std::vector<int>* vec = new std::vector<int>();
            vec->push_back(value);
            (*ctx->items)[ctx->key] = vec;
        } else {
            (*ctx->items)[ctx->key]->push_back(value);
        }

        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_end_map(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_key) {
        ctx->state = wait_map;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_end_array(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_map) {
        ctx->state = complete;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static yajl_callbacks callbacks = {
    NULL,               // yajl_null
    NULL,               // yajl_boolean
    parse_integer,      // yajl_integer
    NULL,               // yajl_double
    NULL,               // yajl_number
    NULL,               // yajl_string
    parse_start_map,    // yajl_start_map
    parse_map_key,      // yajl_map_key
    parse_end_map,      // yajl_end_map
    parse_start_array,  // yajl_start_array
    parse_end_array,    // yajl_end_array
};


static void dealloc_items(ITEMS_TYPE(int)* items) {
    for (ITEMS_TYPE(int)::iterator it = items->begin();
            it != items->end();
            it++) {
        delete it->second;
    }
}


ITEMS_TYPE(int)*
parse(unsigned char* buffer, size_t size) {
    Context ctx;
    init_context(&ctx);

    yajl_handle hand = yajl_alloc(&callbacks, NULL, (void*) &ctx);
    yajl_status stat;

    stat = yajl_parse(hand, buffer, size);
    std::cerr << "parse status: " << stat << std::endl;
    yajl_free(hand);

    if (stat != yajl_status_ok) {
        dealloc_items(ctx.items);
        return NULL;
    }

    return ctx.items;
}

