#include "libmmd/file.h"
#include "libmmd/memory.h"
#include "libmmd/return_codes.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

int mmd_file_open(const char* path, mmd_file_base* pResult) {
    if (!path) return MMD_NULL_PTR;
    
    FILE* fp = fopen(path, "rb");
    if (!fp) return MMD_FILE_NOT_FOUND;

    fseek(fp, 0, SEEK_END);
    uint64_t l = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t* data = (uint8_t*) mmd_memory_allocate(l);
    fgets((char*) data, l, fp);

    fclose(fp);

    pResult->length = l;
    pResult->data = data;
    pResult->pointer = 0;

    return MMD_NO_ERROR;
}

int mmd_file_wrap(uint64_t data_length, void* data, mmd_file_base* pResult)
{
    if (!data) return MMD_NULL_PTR;

    pResult->length = data_length;
    pResult->data = data;
    pResult->pointer = 0;

    return MMD_NO_ERROR;
}

int mmd_file_check(mmd_file_base* file)
{
    if (!file) return MMD_NULL_PTR;
    if (file->pointer >= file->length) return MMD_FILE_BUFFER_OVERFLOW;

    return MMD_NO_ERROR;
}
