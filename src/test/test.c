#include "stdio.h"
#include "libmmd/file.h"

int main()
{
	puts("test!");

    struct FileBase fb;
	printf("%d\n", mmd_file_open("/home/coder2/extsources/libmmd/xmake.lua", &fb));
    printf("%d", fb.length);

	return 0;
}
