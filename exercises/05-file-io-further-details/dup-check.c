#include "../../book-src/lib/tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

off_t fdPos(int fd) {
	off_t off;
	off = lseek(fd, 0, SEEK_CUR);
	if (off == -1L) errExit("lseek");
	return off;
}

int flags(int fd) {
	int flags = fcntl(fd, F_GETFL);
	if (flags == -1) errExit("fcntl");
	return flags;
}

int main() {
	int fd, fd0;
	off_t fd0Off, fdOff;

	fd0 = open("test.tmp", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd0 == -1) errExit("open");

	fd = dup(fd0);
	if (fd == -1) errExit("dup");

	printf("off: %ld vs %ld\n", fdPos(fd), fdPos(fd0));
	printf("flags: %d vs %d\n", flags(fd), flags(fd0));
	if (write(fd, "teset", 5) == -1) errExit("write");
	printf("off: %ld vs %ld\n", fdPos(fd), fdPos(fd0));
	printf("flags: %d vs %d\n", flags(fd), flags(fd0));

	if (close(fd) == -1) errExit("close");
	if (write(fd0, "teset", 5) == -1) errExit("write");
	printf("off: %ld\n", fdPos(fd0));
	printf("flags: %d\n", flags(fd0));
}