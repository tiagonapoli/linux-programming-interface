
#include "../../book-src/lib/tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int printHelp() { printf("Usage ./a.out [OPT] <filePath>\n"); }

int main(int argc, char *argv[]) {
	int fd, byteCount;

	if (argc < 2) {
		printf("Expected 1 argument");
		printHelp();
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_RDWR | O_APPEND);
	if (fd == -1) errExit("open");

	byteCount = write(fd, "firstAppendwololo", 17);
	if (byteCount == -1) errExit("write");

	if (lseek(fd, 0, SEEK_SET) == -1) errExit("lseek");

	byteCount = write(fd, "secondAppend", 12);
	if (byteCount == -1) errExit("write");

	if(close(fd) == -1) errExit("close");
}