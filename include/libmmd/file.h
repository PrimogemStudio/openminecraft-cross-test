#ifndef _FILE_H
#define _FILE_H

#include "stdint.h"

extern "C" {

typedef struct {
    uint64_t length;
    uint64_t pointer;
    uint8_t* data;
} mmd_file_base;

int mmd_file_open(mmd_file_base* pResult, const char* path);
int mmd_file_wrap(mmd_file_base* pResult, uint64_t data_length, void* data);

int mmd_file_check(mmd_file_base* file);
int mmd_file_reset(mmd_file_base* file);

int mmd_file_read_1byte(mmd_file_base* file, void* buf);
int mmd_file_read_2bytes(mmd_file_base* file, void* buf);
int mmd_file_read_4bytes(mmd_file_base* file, void* buf);
int mmd_file_read_8bytes(mmd_file_base* file, void* buf);
int mmd_file_read_12bytes(mmd_file_base* file, void* buf);
int mmd_file_read_16bytes(mmd_file_base* file, void* buf);
int mmd_file_read_36bytes(mmd_file_base* file, void* buf);
int mmd_file_read_64bytes(mmd_file_base* file, void* buf);
int mmd_file_read_nbytes(mmd_file_base* file, uint64_t length, void* buf);
int mmd_file_read_lengthed_string(mmd_file_base* file, bool is_utf16, char** str);

#define mmd_file_read_array(file, n, type, buf) mmd_file_read_nbytes(file, n * sizeof(type), buf)
#define mmd_file_read(file, type, buf) mmd_file_read_nbytes(file, sizeof(type), buf)

int mmd_file_close(mmd_file_base* file);

}

#endif
