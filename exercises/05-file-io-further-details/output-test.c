#include "../../book-src/lib/tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int fd1, fd2, fd3;

	fd1 = open("tests.tmp", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd1 == -1) errExit("open");

	fd2 = dup(fd1);
	if (fd2 == -1) errExit("open");

	fd3 = open("tests.tmp", O_RDWR);
	if (fd3 == -1) errExit("open");

	sleep(10);
	if (write(fd1, "Hello,", 6) == -1) errExit("write");
	printf("Write\n");
	sleep(10);

	if (write(fd2, "world", 5) == -1) errExit("write");
	printf("Write\n");
	sleep(10);

	if (lseek(fd2, 0, SEEK_SET) == -1) errExit("lseek");

	if (write(fd1, "HELLO,", 6) == -1) errExit("write");
	printf("Write\n");
	sleep(10);

	if (write(fd3, "Gidday", 6) == -1) errExit("write");
	printf("Write\n");
	sleep(10);
}