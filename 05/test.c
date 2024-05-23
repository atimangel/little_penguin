#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
	char buf[10];
	int fd;
	int ret;

	bzero(buf, 10);

	fd = open("/dev/fortytwo", O_RDWR);
	ret = read(fd, buf, 10);
	printf("read file: \"%s\", %d\n", buf, ret);
	bzero(buf, 10);
	ret = read(fd, buf, 10);
	printf("read file: \"%s\", %d\n", buf, ret);
	lseek(fd, 0, SEEK_SET);
	bzero(buf, 10);
	ret = read(fd, buf, 10);
	printf("read file: \"%s\", %d\n", buf, ret);
	ret = write(fd, "snpark", 6);
	printf("write file (snpark, 6): %d\n", ret);
	ret = write(fd, "aaa", 3);
	printf("write file (aaa, 3): %d\n", ret);
	ret = write(fd, "snpark42", 8);
	printf("write file (snpark42, 8): %d\n", ret);
	close(fd);
	return 0;
}
