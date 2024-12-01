#ifndef _FILE_H
#define _FILE_H

#include "stdint.h"

struct FileBase {
    uint64_t length;
    void* data;
};

int mmd_file_open(char* path, void* pResult);

#endif
