#include "libmmd/file.h"
#include "libmmd/memory.h"
#include "libmmd/return_codes.h"
#include "libmmd/encoding.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include <cstring>

int mmd_file_open(mmd_file_base* pResult, const char* path) {
    if (!path) return MMD_NULL_PTR;
    
    FILE* fp = fopen(path, "rb");
    if (!fp) return MMD_FILE_NOT_FOUND;

    fseek(fp, 0, SEEK_END);
    uint64_t l = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t* data = (uint8_t*) mmd_memory_allocate(l);
    if (!fgets((char*) data, l, fp)) return MMD_FILE_NOT_FOUND;

    fclose(fp);

    pResult->length = l;
    pResult->data = (uint8_t*) data;
    pResult->pointer = 0;

    return MMD_NO_ERROR;
}

int mmd_file_wrap(mmd_file_base* pResult, uint64_t data_length, void* data)
{
    if (!data) return MMD_NULL_PTR;

    pResult->length = data_length;
    pResult->data = (uint8_t*) data;
    pResult->pointer = 0;

    return MMD_NO_ERROR;
}

int mmd_file_check(mmd_file_base* file)
{
    if (!file) return MMD_NULL_PTR;
    if (file->pointer >= file->length) return MMD_FILE_BUFFER_OVERFLOW;

    return MMD_NO_ERROR;
}

int mmd_file_reset(mmd_file_base* file)
{
    if (!file) return MMD_NULL_PTR;
    file->pointer = 0;
    
    return MMD_NO_ERROR;
}

int mmd_file_read_nbytes(mmd_file_base* file, uint64_t length, void* buf)
{
    if (!file || !file->data) return MMD_NULL_PTR;
    if (file->pointer + 4 > file->length) return MMD_FILE_BUFFER_OVERFLOW;
    memcpy(buf, file->data + file->pointer, length);
    file->pointer += length;

    return MMD_NO_ERROR;
}

int mmd_file_read_1byte(mmd_file_base* file, void* buf)
{
    return mmd_file_read_nbytes(file, 1, buf);
}

int mmd_file_read_2bytes(mmd_file_base* file, void* buf)
{
    return mmd_file_read_nbytes(file, 2, buf);
}

int mmd_file_read_4bytes(mmd_file_base* file, void* buf)
{
    return mmd_file_read_nbytes(file, 4, buf);
}

int mmd_file_read_8bytes(mmd_file_base* file, void* buf)
{
    return mmd_file_read_nbytes(file, 8, buf);
}

int mmd_file_read_12bytes(mmd_file_base* file, void* buf)
{
    return mmd_file_read_nbytes(file, 12, buf);
}

int mmd_file_read_16bytes(mmd_file_base* file, void* buf)
{
    return mmd_file_read_nbytes(file, 16, buf); 
}

int mmd_file_read_36bytes(mmd_file_base* file, void* buf)
{
    return mmd_file_read_nbytes(file, 36, buf);
}

int mmd_file_read_64bytes(mmd_file_base* file, void* buf)
{
    return mmd_file_read_nbytes(file, 64, buf);
}

int mmd_file_read_lengthed_string(mmd_file_base* file, bool is_utf16, char** str)
{
    int length;
    int result = 0;
    result = mmd_file_read_4bytes(file, &length);
    if (result) return result;

    char* strc = (char*) mmd_memory_allocate(length + 1);
    result = mmd_file_read_nbytes(file, length, strc);
    strc[length] = '\0';
    if (result) return result;

    char* conv = is_utf16 ? mmd_encoding_utf16_to_utf8(strc, (uint64_t) length) : strc;
    if (is_utf16) mmd_memory_deallocate(strc);
 
    *str = conv;
    return MMD_NO_ERROR;
}

int mmd_file_close(mmd_file_base* file)
{
    if (!file || !file->data) return MMD_NULL_PTR;
    mmd_memory_deallocate(file->data);
    return MMD_NO_ERROR;
}
