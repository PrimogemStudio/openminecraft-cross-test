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
int mmd_file_read_nbytes(mmd_file_base* file, uint64_t length, void* buf);
int mmd_file_read_lengthed_string(mmd_file_base* file, bool is_utf16, char** str);

}

#endif
