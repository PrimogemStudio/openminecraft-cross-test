#include "libmmd/pmx/pmx_file.h"
#include "libmmd/file.h"
#include "libmmd/return_codes.h"
#include "stdio.h"

int mmd_pmx_file_read_header(mmd_pmx_file_header* pResult, mmd_file_base* file)
{
    uint32_t pmx_header;
    mmd_file_read_4bytes(file, &pmx_header);

    if (pmx_header != *((uint32_t*) ((const char*) "PMX "))) return MMD_PMX_FILE_INVAILD_HEADER;
    
    mmd_file_read_4bytes(file, &pResult->version);
    mmd_file_read_1byte(file, &pResult->data_size);
    mmd_file_read_1byte(file, &pResult->encode);
    mmd_file_read_1byte(file, &pResult->vertex_index_size);
    mmd_file_read_1byte(file, &pResult->texture_index_size);
    mmd_file_read_1byte(file, &pResult->material_index_size);
    mmd_file_read_1byte(file, &pResult->bone_index_size);
    mmd_file_read_1byte(file, &pResult->morph_index_size);
    mmd_file_read_1byte(file, &pResult->rigid_body_index_size);

    return 0;
}
