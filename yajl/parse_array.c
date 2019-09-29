#include <yajl/yajl_parse.h>
#include <stdio.h>


typedef struct {
    int length;
    int array[256];
} Context;


void init_context(Context* ctx) {
    ctx->length = 0;
}


static int parse_integer(void* ctx, long long val) {
    printf("encountered value %lld\n", val);
    Context* ctx_ = (Context*) ctx;
    ctx_->array[(ctx_->length)++] = (int) val;
    return 1;
}


static int parse_start_array(void* ctx) {
    printf("encountered start of the array\n");
    return 1;
}


static int parse_end_array(void* ctx) {
    printf("encountered end of the array\n");
    return 1;
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
    parse_start_array,
    parse_end_array
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
    yajl_free(hand);
    fclose(fp);

    int sum = 0;
    for (int i = 0; i < ctx.length; i++) {
        sum += ctx.array[i];
    }

    printf("sum of %d items of array is %d\n", ctx.length, sum);
    return 0;
}

