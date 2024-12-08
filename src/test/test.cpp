#include "stdio.h"
#include "libmmd/file.h"
#include "libmmd/memory.h"
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

    mmd_pmx_file_header pmx_header;

    printf("%lu\n", fb.length);
    mmd_pmx_file_read_header(&pmx_header, &fb);
    printf("%lu\n", fb.pointer);


	return 0;
}
