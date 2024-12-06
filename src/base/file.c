#include "libmmd/file.h"
#include "libmmd/memory.h"
#include "libmmd/return_codes.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

int mmd_file_open(char* path, void* pResult) {
    if (!path) return MMD_NULL_PTR;
    
    FILE* fp = fopen(path, "r");
    if (!fp) return MMD_FILE_NOT_FOUND;

    fseek(fp, 0, SEEK_END);
    uint64_t l = ftell(fp);

    uint8_t* data = mmd_memory_allocate(l);
    fgets(data, l, fp);

    fclose(fp);

    mmd_file_base* fb = (mmd_file_base*) pResult;
    fb->length = l;
    fb->data = data;
    fb->pointer = 0;

    return MMD_NO_ERROR;
}

int mmd_file_wrap(uint64_t data_length, void* data, void* pResult)
{
    if (!data) return MMD_NULL_PTR;

    mmd_file_base* fb = (mmd_file_base*) pResult;
    fb->length = data_length;
    fb->data = data;
    fb->pointer = 0;

    return MMD_NO_ERROR;
}
