#ifndef PMX_FILE
#define PMX_FILE

#include "libmmd/file.h"
#include "stdlib.h"

typedef struct {
    float version;
    uint8_t data_size;
    
    // 0 -> UTF-16
    // 1 -> UTF-8
    uint8_t encode;
} mmd_pmx_file;

#endif
