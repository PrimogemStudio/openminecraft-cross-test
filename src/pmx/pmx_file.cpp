#include "libmmd/pmx/pmx_file.h"
#include "libmmd/file.h"
#include "libmmd/memory.h"
#include "libmmd/return_codes.h"
#include "stdio.h"

int mmd_pmx_file_create(mmd_pmx_file* pResult, mmd_file_base* file)
{
    pResult->header = mmd_memory_allocate_struct(mmd_pmx_file_header);
    if (!mmd_pmx_file_read_header(pResult->header, file)) return MMD_PMX_FILE_INVALID_HEADER;
    pResult->info = mmd_memory_allocate_struct(mmd_pmx_file_info);
    if (!mmd_pmx_file_read_info(pResult->info, pResult->header, file)) return MMD_PMX_FILE_INVALID_INFO;
    pResult->vertices = mmd_memory_allocate_struct(mmd_pmx_file_vertices);
    if (!mmd_pmx_file_read_vertices(pResult->vertices, pResult->header, file)) return MMD_PMX_FILE_INVALID_VERTICES;
    pResult->faces = mmd_memory_allocate_struct(mmd_pmx_file_faces);
    if (!mmd_pmx_file_read_faces(pResult->faces, pResult->header, file)) return MMD_PMX_FILE_INVALID_FACES;
    pResult->textures = mmd_memory_allocate_struct(mmd_pmx_file_textures);
    if (!mmd_pmx_file_read_textures(pResult->textures, pResult->header, file)) return MMD_PMX_FILE_INVALID_TEXTURES;
    pResult->materials = mmd_memory_allocate_struct(mmd_pmx_file_materials);
    if (!mmd_pmx_file_read_materials(pResult->materials, pResult->header, file)) return MMD_PMX_FILE_INVALID_MATERIALS;

    return MMD_NO_ERROR;
}

int mmd_pmx_file_read_header(mmd_pmx_file_header* pResult, mmd_file_base* file)
{
    uint32_t pmx_header;
    mmd_file_read(file, uint32_t, &pmx_header);

    if (pmx_header != *((uint32_t*) ((const char*) "PMX "))) return MMD_PMX_FILE_INVALID_HEADER;
    
    mmd_file_read(file, float, &pResult->version);
    mmd_file_read(file, uint8_t, &pResult->data_size);
    mmd_file_read(file, uint8_t, &pResult->encode);
    mmd_file_read(file, uint8_t, &pResult->add_uv_num);
    mmd_file_read(file, uint8_t, &pResult->vertex_index_size);
    mmd_file_read(file, uint8_t, &pResult->texture_index_size);
    mmd_file_read(file, uint8_t, &pResult->material_index_size);
    mmd_file_read(file, uint8_t, &pResult->bone_index_size);
    mmd_file_read(file, uint8_t, &pResult->morph_index_size);
    mmd_file_read(file, uint8_t, &pResult->rigid_body_index_size);

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
    pResult->data = mmd_memory_allocate_array(mmd_pmx_file_vertex, pResult->length); 
    
    for (int i = 0; i < pResult->length; i++)
    { 
        mmd_pmx_file_vertex* vtx = &pResult->data[i];
        mmd_file_read(file, glm::vec3, &vtx->position);
        mmd_file_read(file, glm::vec3, &vtx->normal);
        mmd_file_read(file, glm::vec2, &vtx->uv);

        vtx->addition_uv = mmd_memory_allocate_struct(mmd_pmx_file_vertex_additional_uv);
        uint8_t uvnum = header->add_uv_num;
        vtx->addition_uv->length = uvnum;
        vtx->addition_uv->data = mmd_memory_allocate_array(glm::vec4, uvnum);
        mmd_file_read_array(file, uvnum, glm::vec4, vtx->addition_uv->data); 

        mmd_file_read(file, mmd_pmx_file_vertex_bonetype, &vtx->bone_type);
        
        if (vtx->bone_type == bdef1)
        {
            vtx->bone_data = mmd_memory_allocate_struct(mmd_pmx_file_vertex_bdef1_bone);
            mmd_pmx_file_vertex_bdef1_bone* bone = (mmd_pmx_file_vertex_bdef1_bone*) vtx->bone_data;
            mmd_file_read_nbytes(file, header->bone_index_size, &bone->bone_index);
        }
        else if (vtx->bone_type == bdef2)
        {
            vtx->bone_data = mmd_memory_allocate_struct(mmd_pmx_file_vertex_bdef2_bone);
            mmd_pmx_file_vertex_bdef2_bone* bone = (mmd_pmx_file_vertex_bdef2_bone*) vtx->bone_data;
            mmd_file_read_nbytes(file, header->bone_index_size, &bone->bone_index1);
            mmd_file_read_nbytes(file, header->bone_index_size, &bone->bone_index2);
            mmd_file_read(file, float, &bone->bone_weight);
        }
        else if (vtx->bone_type == sdef)
        {
            vtx->bone_data = mmd_memory_allocate_struct(mmd_pmx_file_vertex_sdef_bone);
            mmd_pmx_file_vertex_sdef_bone* bone = (mmd_pmx_file_vertex_sdef_bone*) vtx->bone_data;
            mmd_file_read_nbytes(file, header->bone_index_size, &bone->bone_index1);
            mmd_file_read_nbytes(file, header->bone_index_size, &bone->bone_index2);
            mmd_file_read(file, float, &bone->bone_weight);
            mmd_file_read(file, glm::vec3, &bone->sdefc);
            mmd_file_read(file, glm::vec3, &bone->sdefr0);
            mmd_file_read(file, glm::vec3, &bone->sdefr1);
        }
        else if (vtx->bone_type == bdef4 || vtx->bone_type == qdef)
        {
            vtx->bone_data = mmd_memory_allocate_struct(mmd_pmx_file_vertex_bdef4_qdef_bone);
            mmd_pmx_file_vertex_bdef4_qdef_bone* bone = (mmd_pmx_file_vertex_bdef4_qdef_bone*) vtx->bone_data;
            mmd_file_read_nbytes(file, header->bone_index_size, &bone->bone_index1);
            mmd_file_read_nbytes(file, header->bone_index_size, &bone->bone_index2);
            mmd_file_read_nbytes(file, header->bone_index_size, &bone->bone_index3);
            mmd_file_read_nbytes(file, header->bone_index_size, &bone->bone_index4);
            mmd_file_read(file, float, &bone->bone_weight1);
            mmd_file_read(file, float, &bone->bone_weight2);
            mmd_file_read(file, float, &bone->bone_weight3);
            mmd_file_read(file, float, &bone->bone_weight4);
        }
        else
        {
            return MMD_PMX_FILE_VERTEX_INVALID_BONETYPE;
        }

        mmd_file_read(file, float, &vtx->edge_margin); 
        if (mmd_file_check(file)) return MMD_FILE_BUFFER_OVERFLOW;
    }
    
    return mmd_file_check(file) ? MMD_NO_ERROR : MMD_FILE_BUFFER_OVERFLOW;
}

int mmd_pmx_file_read_faces(mmd_pmx_file_faces* pResult, mmd_pmx_file_header* header, mmd_file_base* file)
{
    mmd_file_read(file, uint32_t, &pResult->length);
    if (pResult->length % 3 != 0) return MMD_PMX_FILE_INVALID_FACES;
    pResult->data = mmd_memory_allocate_array(mmd_pmx_file_face, pResult->length / 3);

    for (int i = 0; i < pResult->length / 3; i++)
    {
        pResult->data[i] = *mmd_memory_allocate_struct(mmd_pmx_file_face);
        mmd_file_read_nbytes(file, header->vertex_index_size, &pResult->data[i].face_index1);
        mmd_file_read_nbytes(file, header->vertex_index_size, &pResult->data[i].face_index2);
        mmd_file_read_nbytes(file, header->vertex_index_size, &pResult->data[i].face_index3);
    }

    return mmd_file_check(file) ? MMD_NO_ERROR : MMD_FILE_BUFFER_OVERFLOW;
}

int mmd_pmx_file_read_textures(mmd_pmx_file_textures* pResult, mmd_pmx_file_header* header, mmd_file_base* file)
{
    mmd_file_read(file, uint32_t, &pResult->length);
    pResult->data = mmd_memory_allocate_array(char*, pResult->length);

    for (int i = 0; i < pResult->length; i++)
    {
        mmd_file_read_lengthed_string(file, !header->encode, &pResult->data[i]);
    }

    return mmd_file_check(file) ? MMD_NO_ERROR : MMD_FILE_BUFFER_OVERFLOW;
}

int mmd_pmx_file_read_materials(mmd_pmx_file_materials* pResult, mmd_pmx_file_header* header, mmd_file_base* file)
{
    mmd_file_read(file, uint32_t, &pResult->length);
    pResult->data = mmd_memory_allocate_array(mmd_pmx_file_material, pResult->length);

    for (int i = 0; i < 1; i++)
    {
        mmd_file_read_lengthed_string(file, !header->encode, &pResult->data[i].name);
        mmd_file_read_lengthed_string(file, !header->encode, &pResult->data[i].english_name);
        mmd_file_read(file, glm::vec4, &pResult->data[i].diffuse);
        mmd_file_read(file, glm::vec3, &pResult->data[i].specular);
        mmd_file_read(file, float, &pResult->data[i].specular_power);
        mmd_file_read(file, glm::vec3, &pResult->data[i].ambient);
        mmd_file_read(file, mmd_pmx_file_material_drawmode_flags, &pResult->data[i].draw_mode);
    }

    return mmd_file_check(file) ? MMD_NO_ERROR : MMD_FILE_BUFFER_OVERFLOW;
}