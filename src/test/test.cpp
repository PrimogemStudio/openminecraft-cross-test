#include "stdio.h"
#include "libmmd/file.h"
#include "libmmd/memory.h"
#include "stdlib.h"

int main()
{
	puts("test!");

    mmd_file_base* fb = (mmd_file_base*) mmd_memory_allocate(sizeof(mmd_file_base));
	if (mmd_file_open(fb, "/home/coder2/mmd/lumine/lumine.pmx"))
    {
        printf("File open failed!");
        return 0;
    }

    printf("%d\n", fb->length);
    printf("%d\n", ((uint32_t*) fb->data)[0]);
    printf("%d\n", *((uint32_t*) ((const char*) "PMX ")));

	return 0;
}
