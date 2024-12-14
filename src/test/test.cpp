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

    printf("%lu\n", fb.length);
    printf("%d\n", mmd_pmx_file_create(&pmx_file, &fb));
    printf("+0x%x\n", fb.pointer);

    mmd_file_close(&fb);

	return 0;
}
