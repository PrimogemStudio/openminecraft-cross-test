#ifndef PMX_FILE
#define PMX_FILE

#include "libmmd/file.h"
#include "stdlib.h"

extern "C" {

typedef struct {
    float version;
    uint8_t data_size;
    
    // 0 -> UTF-16
    // 1 -> UTF-8
    uint8_t encode;
    uint8_t add_uv_num;

    uint8_t vertex_index_size;
    uint8_t texture_index_size;
    uint8_t material_index_size;
    uint8_t bone_index_size;
    uint8_t morph_index_size;
    uint8_t rigid_body_index_size;
} mmd_pmx_file;

}

#endif
