#include "stdio.h"
#include "libmmd/file.h"
#include "stdlib.h"

int main()
{
	puts("test!");

    mmd_file_base fb;
	printf("%d\n", mmd_file_open("/home/coder2/extsources/libmmd/xmake.lua", &fb));
    printf("%d\n", fb.length);

    char* temp = malloc(2);
    int length = snprintf(temp, 2, "%s %d", "test", 114514);
    char* d = malloc(length+1);
    snprintf(d, length+1, "%s %d", "test", 114514);
    printf(d);

	return 0;
}
