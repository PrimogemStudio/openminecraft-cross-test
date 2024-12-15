#include "stdio.h"
#include "libmmd/file.h"
#include "libmmd/memory.h"
#include "libmmd/encoding.h"
#include "libmmd/pmx/pmx_file.h"
#include "stdlib.h"

int main()
{
	puts("test!");
     
    mmd_file_base fb;
	if (mmd_file_open(&fb, "/home/coder2/mmd/lumine/lumine.pmx"))
    {
        printf("File open failed!");
        return 0;
    }

    mmd_pmx_file pmx_file;
    
    mmd_pmx_file_create(&pmx_file, &fb);
    printf("+0x%lx total: 0x%x\n", fb.pointer, fb.length);
 
    mmd_file_close(&fb);

	return 0;
}
