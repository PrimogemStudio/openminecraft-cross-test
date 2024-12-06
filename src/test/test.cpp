#include "stdio.h"
#include "libmmd/file.h"
#include "stdlib.h"
#include "glm/vec2.hpp"

int main()
{
	puts("test!");

    mmd_file_base fb;
	if (mmd_file_open("/home/coder2/mmd/lumine/lumine.pmx", &fb))
    {
        printf("File open failed!");
        return 0;
    }

    printf("%d\n", fb.length);

	return 0;
}
