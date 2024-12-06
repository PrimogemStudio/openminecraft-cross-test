#ifndef _FILE_H
#define _FILE_H

#include "stdint.h"

extern "C" {

typedef struct {
    uint64_t length;
    uint64_t pointer;
    void* data;
} mmd_file_base;

int mmd_file_open(const char* path, mmd_file_base* pResult);
int mmd_file_wrap(uint64_t data_length, void* data, mmd_file_base* pResult);

}

#endif
