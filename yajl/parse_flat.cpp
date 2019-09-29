#include <yajl/yajl_parse.h>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include "parse_flat.h"

#define CLIENT_FAILURE 0
#define CLIENT_SUCCESS 1


void 
init_context(Context* ctx) {
    ctx->state = wait_start;
}


static int
parse_start_map(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_start) {
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

        if (ctx->items.count(ctx->key) == 0) {
            std::vector<int>* vec = new std::vector<int>();
            vec->push_back(value);
            ctx->items[ctx->key] = vec;
        } else {
            ctx->items[ctx->key]->push_back(value);
        }

        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_end_map(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_key) {
        ctx->state = complete;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static yajl_callbacks callbacks = {
    NULL,
    NULL,
    parse_integer,
    NULL,
    NULL,
    NULL,
    parse_start_map,
    parse_map_key,
    parse_end_map,
    NULL,
    NULL,
};


int
main(int argc, char** argv) {
    static unsigned char file_data[65536];
    Context ctx;
    init_context(&ctx);
    yajl_handle hand = yajl_alloc(&callbacks, NULL, (void*) &ctx);
    yajl_status stat;

    FILE* fp = fopen(argv[1], "r");
    int rd;
    
    for (;;) {
        rd = fread((void*) file_data, 1, sizeof(file_data), fp);
        if (rd == 0) {
            break;
        }

        file_data[rd] = 0;
        stat = yajl_parse(hand, file_data, rd);

        if (stat == yajl_status_ok) break;
    }

    stat = yajl_complete_parse(hand);
    printf("parse status: %d\n", stat);
    yajl_free(hand);
    fclose(fp);

    std::map<std::string,std::vector<int>* >::iterator it;
    for (it = ctx.items.begin(); it != ctx.items.end(); it++) {
        std::vector<int>* vec = it->second;
        int sum = 0;

        for (std::vector<int>::iterator it_ = vec->begin(); 
                it_ != vec->end(); 
                it_++) {
            sum += *it_;
        }

        std::cout << "sum of " << it->first << " items is " 
            << sum << std::endl;
    }

    return 0;
}

