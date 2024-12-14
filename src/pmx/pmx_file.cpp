#include "libmmd/pmx/pmx_file.h"
#include "libmmd/file.h"
#include "libmmd/memory.h"
#include "libmmd/return_codes.h"

int mmd_pmx_file_create(mmd_pmx_file* pResult, mmd_file_base* file)
{
    pResult->header = mmd_memory_allocate_struct(mmd_pmx_file_header);
    if (!mmd_pmx_file_read_header(pResult->header, file)) return MMD_PMX_FILE_INVAILD_HEADER;
    pResult->info = mmd_memory_allocate_struct(mmd_pmx_file_info);
    if (!mmd_pmx_file_read_info(pResult->info, pResult->header, file)) return MMD_PMX_FILE_INVAILD_INFO;
    pResult->vertices = mmd_memory_allocate_struct(mmd_pmx_file_vertices);
    if (!mmd_pmx_file_read_vertices(pResult->vertices, pResult->header, file)) return MMD_PMX_FILE_INVAILD_VERTICES;

    return MMD_NO_ERROR;
}

int mmd_pmx_file_read_header(mmd_pmx_file_header* pResult, mmd_file_base* file)
{
    uint32_t pmx_header;
    mmd_file_read_4bytes(file, &pmx_header);

    if (pmx_header != *((uint32_t*) ((const char*) "PMX "))) return MMD_PMX_FILE_INVAILD_HEADER;
    
    mmd_file_read_4bytes(file, &pResult->version);
    mmd_file_read_1byte(file, &pResult->data_size);
    mmd_file_read_1byte(file, &pResult->encode);
    mmd_file_read_1byte(file, &pResult->add_uv_num);
    mmd_file_read_1byte(file, &pResult->vertex_index_size);
    mmd_file_read_1byte(file, &pResult->texture_index_size);
    mmd_file_read_1byte(file, &pResult->material_index_size);
    mmd_file_read_1byte(file, &pResult->bone_index_size);
    mmd_file_read_1byte(file, &pResult->morph_index_size);
    mmd_file_read_1byte(file, &pResult->rigid_body_index_size);

    return mmd_file_check(file) ? MMD_NO_ERROR : MMD_FILE_BUFFER_OVERFLOW;
}

int mmd_pmx_file_read_info(mmd_pmx_file_info* pResult, mmd_pmx_file_header* header, mmd_file_base* file)
{
    mmd_file_read_lengthed_string(file, !header->encode, &pResult->model_name);
    mmd_file_read_lengthed_string(file, !header->encode, &pResult->model_comment);
    mmd_file_read_lengthed_string(file, !header->encode, &pResult->english_name);
    mmd_file_read_lengthed_string(file, !header->encode, &pResult->english_comment);
    return mmd_file_check(file) ? MMD_NO_ERROR : MMD_FILE_BUFFER_OVERFLOW;
}

int mmd_pmx_file_read_vertices(mmd_pmx_file_vertices* pResult, mmd_pmx_file_header* header, mmd_file_base* file)
{
    mmd_file_read_4bytes(file, &pResult->length);
    mmd_pmx_file_vertex* vtx = mmd_memory_allocate_struct(mmd_pmx_file_vertex);
    mmd_file_read_12bytes(file, &vtx->position);
    mmd_file_read_12bytes(file, &vtx->normal);
    mmd_file_read_8bytes(file, &vtx->uv);

    vtx->addition_uv = mmd_memory_allocate_struct(mmd_pmx_file_vertex_additional_uv);
    uint8_t uvnum = header->add_uv_num;
    vtx->addition_uv->length = uvnum;
    vtx->addition_uv->data = (glm::vec4*) mmd_memory_allocate(16 * uvnum);
    mmd_file_read_nbytes(file, 4 * uvnum, vtx->addition_uv->data);

    
    return mmd_file_check(file) ? MMD_NO_ERROR : MMD_FILE_BUFFER_OVERFLOW; 
}
