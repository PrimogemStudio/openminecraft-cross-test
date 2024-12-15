#ifndef PMX_FILE
#define PMX_FILE

#include "libmmd/file.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "stdlib.h"
#include "stdint.h"

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
} mmd_pmx_file_header;

typedef struct {
    char* model_name;
    char* model_comment;
    char* english_name;
    char* english_comment;
} mmd_pmx_file_info;

typedef struct {
    uint8_t length;
    glm::vec4* data;
} mmd_pmx_file_vertex_additional_uv;

typedef enum : uint8_t {
    bdef1 = 0, 
    bdef2 = 1, 
    bdef4 = 2, 
    sdef = 3, 
    qdef = 4
} mmd_pmx_file_vertex_bonetype;

typedef struct {
    uint32_t bone_index;
} mmd_pmx_file_vertex_bdef1_bone;

typedef struct {
    uint32_t bone_index1;
    uint32_t bone_index2;
    float bone_weight;
} mmd_pmx_file_vertex_bdef2_bone;

typedef struct {
    uint32_t bone_index1;
    uint32_t bone_index2;
    float bone_weight;
    glm::vec3 sdefc;
    glm::vec3 sdefr0;
    glm::vec3 sdefr1;
} mmd_pmx_file_vertex_sdef_bone;

typedef struct {
    uint32_t bone_index1;
    uint32_t bone_index2;
    uint32_t bone_index3;
    uint32_t bone_index4;
    float bone_weight1;
    float bone_weight2;
    float bone_weight3;
    float bone_weight4;
} mmd_pmx_file_vertex_bdef4_qdef_bone;

typedef struct {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    mmd_pmx_file_vertex_additional_uv* addition_uv;
    mmd_pmx_file_vertex_bonetype bone_type;
    void* bone_data;
    float edge_margin;
} mmd_pmx_file_vertex;

typedef struct {
    uint32_t length;
    mmd_pmx_file_vertex* data;
} mmd_pmx_file_vertices;

typedef struct {
    uint32_t face_index1;
    uint32_t face_index2;
    uint32_t face_index3;
} mmd_pmx_file_face;

typedef struct {
    uint32_t length;
    mmd_pmx_file_face* data;
} mmd_pmx_file_faces;

typedef struct {
    mmd_pmx_file_header* header;
    mmd_pmx_file_info* info;
    mmd_pmx_file_vertices* vertices;
} mmd_pmx_file;

int mmd_pmx_file_create(mmd_pmx_file* pResult, mmd_file_base* file);
int mmd_pmx_file_read_header(mmd_pmx_file_header* pResult, mmd_file_base* file);
int mmd_pmx_file_read_info(mmd_pmx_file_info* pResult, mmd_pmx_file_header* header, mmd_file_base* file);
int mmd_pmx_file_read_vertices(mmd_pmx_file_vertices* pResult, mmd_pmx_file_header* header, mmd_file_base* file);

}

#endif
