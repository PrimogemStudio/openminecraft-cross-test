#include "stdio.h"
#include "libmmd/file.h"
#include "libmmd/memory.h"
#include "libmmd/encoding.h"
#include "libmmd/pmx/pmx_file.h"
#include "stdlib.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

int main()
{
	puts("test!");

    btDbvtBroadphase* t = new btDbvtBroadphase();
    printf("%lu\n", ((uint64_t) t));
     
    mmd_file_base fb;
	if (mmd_file_open(&fb, "/home/coder2/mmd/lumine/lumine.pmx"))
    {
        printf("File open failed!");
        return 0;
    }

    mmd_pmx_file pmx_file;

    printf("%lu\n", fb.length);
    printf("%d\n", mmd_pmx_file_create(&pmx_file, &fb));
    printf("%lu\n", fb.pointer);

    char* test;
    mmd_file_read_lengthed_string(&fb, true, &test);
    printf("%s\n", test);

	return 0;
}
