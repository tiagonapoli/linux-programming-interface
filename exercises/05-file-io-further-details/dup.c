#include "../../book-src/lib/tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

int duplicate(int fd) {
	if (fcntl(fd, F_GETFD) == -1) {
		errno = EBADF;
		return -1;
	}

	int newFd = fcntl(fd, F_DUPFD, 0);
	if (newFd == -1) errExit("fcntl");
	return newFd;
}

int duplicate2(int fd, int fdNumber) {
	if (fcntl(fd, F_GETFD) == -1) {
		errno = EBADF;
		return -1;
	}

	close(fdNumber);
	int newFd = fcntl(fd, F_DUPFD, fdNumber);
	if (newFd != fdNumber) {
		printf("ERROR: newFd is not the required");
		exit(1);
	}

	return newFd;
}

int main() {
	int fd, fd1, fd2;

	fd = open("test.tmp", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1) errExit("open");

	fd1 = duplicate(fd);

	if (write(fd, "aaaaa", 5) != 5) errExit("write");
	if (write(fd1, "bbbbb", 5) != 5) errExit("write");

	if (close(STDERR_FILENO) == -1) errExit("close");
	fd2 = duplicate2(fd, STDERR_FILENO);
	if(fd2 == -1) {
		printf("duplicate2 error: %d\n", errno);
		exit(1);
	}

	if (write(fd2, "ccccc", 5) != 5) errExit("write");

	close(fd);
	close(fd1);
	close(fd2);
}