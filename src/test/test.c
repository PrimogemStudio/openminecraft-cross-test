#include "stdio.h"
#include "libmmd/file.h"

int main()
{
	puts("test!");

	struct FileBase fp;
	fp.length = 1;
	printf("%lu", fp.length);
	printf("%d\n", mmd_file_open("t", NULL));

	return 0;
}
