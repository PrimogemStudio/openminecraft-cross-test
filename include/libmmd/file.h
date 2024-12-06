#ifndef _FILE_H
#define _FILE_H

#include "stdint.h"

typedef struct {
    uint64_t length;
    uint64_t pointer;
    void* data;
} mmd_file_base;

int mmd_file_open(char* path, void* pResult);
int mmd_file_wrap(uint64_t data_length, void* data, void* pResult);

#endif
