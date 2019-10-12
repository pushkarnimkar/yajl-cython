#include <yajl/yajl_parse.h>
#include "more_complex.h"
#include <iostream>


void
init_context(Context* ctx) {
    ITEMS_TYPE(int)* index = new ITEMS_TYPE(int);
    ctx->index = *index;

    ITEMS_TYPE(ITEMS_DTYPE)* items = new ITEMS_TYPE(ITEMS_DTYPE);
    ctx->items = *items;

    // ready to receive next row
    ctx->row_index = -1;

    // initialiazing state as waiting for parse
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
update_context_value(Context* ctx, ITEMS_DTYPE value) {
    if (ctx->items.count(ctx->key) == 0) {
        ctx->items[ctx->key] = *(new std::vector<ITEMS_DTYPE>());
        ctx->index[ctx->key] = *(new std::vector<int>());
    }

    ctx->items[ctx->key].push_back(value);
    ctx->index[ctx->key].push_back(ctx->row_index);
}


static int
parse_null(void* ctx_) {
    Context* ctx = (Context*) ctx_;
    update_context_value(ctx, -1);

    if (ctx->state == wait_value0) {
        ctx->state = wait_key0;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_value1) {
        ctx->state = wait_key1;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_value2) {
        ctx->state = wait_key2;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_integer(void* ctx_, long long val) {
     Context* ctx = (Context*) ctx_;

     if (ctx->state == wait_value2) {
         ctx->state = wait_key2;
         update_context_value(ctx, (ITEMS_DTYPE) val);
         return CLIENT_SUCCESS;
     }

     if (ctx->state == wait_value1) {
         ctx->state = wait_key1;
         update_context_value(ctx, (ITEMS_DTYPE) val);
         return CLIENT_SUCCESS;
     }

     if (ctx->state == wait_value3) {
         update_context_value(ctx, (ITEMS_DTYPE) val);
         return CLIENT_SUCCESS;
     }

     if (ctx->state == wait_value0) {
         ctx->state = wait_key0;
         update_context_value(ctx, (ITEMS_DTYPE) val);
         return CLIENT_SUCCESS;
     }

     return CLIENT_FAILURE;
}


static int
parse_double(void* ctx_, double val) {
     Context* ctx = (Context*) ctx_;

     if (ctx->state == wait_value2) {
         ctx->state = wait_key2;
         update_context_value(ctx, (ITEMS_DTYPE) val);
         return CLIENT_SUCCESS;
     }

     if (ctx->state == wait_value1) {
         ctx->state = wait_key1;
         update_context_value(ctx, (ITEMS_DTYPE) val);
         return CLIENT_SUCCESS;
     }

     if (ctx->state == wait_value3) {
         update_context_value(ctx, (ITEMS_DTYPE) val);
         return CLIENT_SUCCESS;
     }

     if (ctx->state == wait_value0) {
         ctx->state = wait_key0;
         update_context_value(ctx, (ITEMS_DTYPE) val);
         return CLIENT_SUCCESS;
     }

     return CLIENT_FAILURE;
}


static int
parse_string(void* ctx_, const unsigned char* str, size_t size) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_value0) {
        ctx->state = wait_key0;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_array3) {
        ctx->state = wait_key3;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_start_map(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_value1) {
        ctx->state = wait_key2;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_map1) {
        ctx->state = wait_key1;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_value0) {
        ctx->state = wait_key3;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_map0) {
        ctx->state = wait_key0;
        ctx->row_index++;
        return CLIENT_SUCCESS;
    }
    
    return CLIENT_FAILURE;
}


static int
parse_map_key(void* ctx_, const unsigned char* key, size_t size) {
    Context* ctx = (Context*) ctx_;
    ctx->key = std::string((const char*) key, (const char*) key + size);

    if (ctx->state == wait_key2) {
        ctx->state = wait_value2;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_key1) {
        ctx->state = wait_value1;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_key0) {
        ctx->state = wait_value0;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_key3) {
        ctx->state = wait_array3;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_end_map(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_key2) {
        ctx->state = wait_key1;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_key1) {
        ctx->state = wait_map1;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_key3) {
        ctx->state = wait_key0;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_key0) {
        ctx->state = wait_map0;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_start_array(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_value0) {
        ctx->state = wait_map1;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_array3) {
        ctx->state = wait_value3;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_start) {
        ctx->state = wait_map0;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static int
parse_end_array(void* ctx_) {
    Context* ctx = (Context*) ctx_;

    if (ctx->state == wait_map1) {
        ctx->state = wait_key0;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_value3) {
        ctx->state = wait_key3;
        return CLIENT_SUCCESS;
    }

    if (ctx->state == wait_map0) {
        ctx->state = complete;
        return CLIENT_SUCCESS;
    }

    return CLIENT_FAILURE;
}


static yajl_callbacks callbacks = {
    parse_null,         // yajl_null
    NULL,               // yajl_boolean
    parse_integer,      // yajl_integer
    parse_double,       // yajl_double
    NULL,               // yajl_number
    parse_string,       // yajl_string
    parse_start_map,    // yajl_start_map
    parse_map_key,      // yajl_map_key
    parse_end_map,      // yajl_end_map
    parse_start_array,  // yajl_start_array
    parse_end_array,    // yajl_end_array
};


template<typename T>
static void dealloc_items(ITEMS_TYPE(T) items) {
    for (typename ITEMS_TYPE(T)::iterator it = items.begin();
            it != items.end();
            it++) {
        // we hope that this deletes the vector this is holding although, 
        // review by an experienced c++ developer will be good
        items.erase(it->first);
    }
}



ComplexReturnType*
parse(const unsigned char* buffer, size_t size) {
    Context ctx;
    init_context(&ctx);

    yajl_handle hand = yajl_alloc(&callbacks, NULL, (void*) &ctx);
    yajl_status stat;

    stat = yajl_parse(hand, buffer, size);
    std::cerr << "parse status: " << stat << std::endl;
    yajl_free(hand);

    if (stat != yajl_status_ok) {
        dealloc_items(ctx.index);
        dealloc_items(ctx.items);
        return NULL;
    } 

    ComplexReturnType *res = new ComplexReturnType;
    res->index = ctx.index;
    res->items = ctx.items;

    return res;
}

