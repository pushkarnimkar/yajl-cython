#include <yajl/yajl_parse.h>
#include "complex_object.h"
#include <iostream>


void
init_context(Context* ctx) {
    // allocating index map
    ctx->index = new ITEMS_TYPE(int)();
    // allocating items map
    ctx->items = new ITEMS_TYPE(float)();

    // ready to receive next row
    ctx->row_index = -1;
    ctx->state = wait_start;
}


/** Convenience method for updating context with value and index
 * 
 * If items and index fields do not exist for current key, creates them
 * and update items and index fields with new value.
 *
 * WARNING: does not check parsing state.
 */
static void 
update_context_value(Context* ctx, float value) {
    if (ctx->items->count(ctx->key) == 0) {
        (*ctx->items)[ctx->key] = new std::vector<float>();
        (*ctx->index)[ctx->key] = new std::vector<int>();
    }

    (*ctx->items)[ctx->key]->push_back(value);
    (*ctx->index)[ctx->key]->push_back(ctx->row_index);
}


static int
parse_integer(void* ctx_, long long val) {
     Context* ctx = (Context*) ctx_;

     if (ctx->state == wait_value) {
         ctx->state = wait_key;
         update_context_value(ctx, (float) val);
         return CLIENT_SUCCESS;
     }

     if (ctx->state == wait_array_value) {
         update_context_value(ctx, (float) val);
         return CLIENT_SUCCESS;
     }

     return CLIENT_FAILURE;
}


static int
parse_double(void* ctx_, double val) {
     Context* ctx = (Context*) ctx_;

     if (ctx->state == wait_value) {
         ctx->state = wait_key;
         update_context_value(ctx, (float) val);
         return CLIENT_SUCCESS;
     }

     if (ctx->state == wait_array_value) {
         update_context_value(ctx, (float) val);
         return CLIENT_SUCCESS;
     }

     return CLIENT_FAILURE;
}


static int
parse_start_map(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_map) {
        ctx->state = wait_key;
        ctx->row_index++;
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
parse_end_map(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_key) {
        ctx->state = wait_map;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_start_array(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_start) {
        ctx->state = wait_map;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_value) {
        ctx->state = wait_array_value;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_end_array(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_array_value) {
        ctx->state = wait_key;
        return CLIENT_SUCCESS;
    }

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
    parse_double,       // yajl_double
    NULL,               // yajl_number
    NULL,               // yajl_string
    parse_start_map,    // yajl_start_map
    parse_map_key,      // yajl_map_key
    parse_end_map,      // yajl_end_map
    parse_start_array,  // yajl_start_array
    parse_end_array,    // yajl_end_array
};


template<typename T>
static void dealloc_items(ITEMS_TYPE(T)* items) {
    // https://stackoverflow.com/questions/610245
    for (typename ITEMS_TYPE(T)::iterator it = items->begin();
            it != items->end();
            it++) {
        delete it->second;
    }
}



ComplexReturnType
parse(unsigned char* buffer, size_t size) {
    Context ctx;
    init_context(&ctx);

    yajl_handle hand = yajl_alloc(&callbacks, NULL, (void*) &ctx);
    yajl_status stat;

    stat = yajl_parse(hand, buffer, size);
    std::cerr << "parse status: " << stat << std::endl;
    yajl_free(hand);

    ComplexReturnType res;

    if (stat != yajl_status_ok) {
        dealloc_items(ctx.items);
        res = {NULL, NULL};
    } else {
        res = {ctx.index, ctx.items};
    }

    return res;
}

