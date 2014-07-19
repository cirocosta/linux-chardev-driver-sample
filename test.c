#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char buf[100];
	char i = 0;
	int fp;

	assert(argc > 1);

	memset(buf, 0, 100);
	printf("Input: %s\n", argv[1]);

	fp = open("./myDev", O_RDWR);
	write(fp, argv[1], strlen(argv[1]));

	while (read(fp, &buf[i++], 1))
		printf("Reversed by the driver: %s\n", buf);

	return 0;
}
