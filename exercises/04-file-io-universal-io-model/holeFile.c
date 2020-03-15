#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../book-src/lib/tlpi_hdr.h"

int printHelp() {
	printf("Usage ./a.out [OPT] <numGaps> <gapSize> <fileToWrite>\nOptions are:\n");
	printf("  -h: Show this message\n");
}

static char *text = "olar tiago";

int main(int argc, char *argv[]) {
	int fd;
	char *buf[10];
	ssize_t numBytesRead, numBytesWritten;
	int numGaps, gapSize;

	if (argc < 3) {
		printf("Expected 3 arguments");
		printHelp();
		exit(EXIT_FAILURE);
	}

	numGaps = getInt(argv[1], GN_GT_0, "numGaps");
	gapSize = getInt(argv[2], GN_GT_0, "gapSize");

	fd = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH);
	if (fd < 0) {
		errExit("open");
	}

	for (int i = 0; i < numGaps; i++) {
		if (write(fd, text, 10) < 0) {
			errExit("write");
		}

		if (lseek(fd, gapSize, SEEK_CUR) < 0) {
			errExit("lseek");
		}
	}

	if (close(fd) < 0) {
		errExit("close");
	}

	return 0;
}