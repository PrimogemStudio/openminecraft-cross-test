#include "stdio.h"
#include "libmmd/file.h"
#include "stdlib.h"

int main()
{
	puts("test!");

    mmd_file_base fb;
	printf("%d\n", mmd_file_open("/home/coder2/extsources/libmmd/xmake.lua", &fb));
    printf("%d\n", fb.length);

	return 0;
}
