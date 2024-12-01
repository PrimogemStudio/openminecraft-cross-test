#ifndef _FILE_H
#define _FILE_H

#include "stdint.h"

typedef struct {
    uint64_t length;
    void* data;
} FileBase;

int mmd_file_open(char* path, void* pResult);

#endif
