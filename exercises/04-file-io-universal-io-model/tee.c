#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../book-src/lib/tlpi_hdr.h"

int printHelp() {
	printf("Usage ./a.out [OPT] fileToWrite\nOptions are:\n");
	printf("  -h: Show this message\n");
	printf("  -a: Append to existing file\n");
}

int main(int argc, char *argv[]) {
	int fd;
	char *buf[10];
	ssize_t numBytesRead, numBytesWritten;
	int flags;

	int shouldAppend = 0, opt;
	while ((opt = getopt(argc, argv, "ah")) != -1) {
		switch (opt) {
		case 'a':
			shouldAppend = 1;
			break;
		case 'h':
			printHelp();
			exit(EXIT_SUCCESS);
		default:
			exit(EXIT_FAILURE);
		}
	}

	if (argc <= optind) {
		printf("Expected fileToWrite argument\n");
		printHelp();
		exit(EXIT_FAILURE);
	}

	if (argc - optind > 1) {
		printf("Expected just ONE fileToWrite argument\n");
		printHelp();
		exit(EXIT_FAILURE);
	}

	flags = O_CREAT | O_WRONLY;
	if (shouldAppend) {
		flags |= O_APPEND;
	} else {
        flags |= O_TRUNC;
    }

	fd = open(argv[argc - 1], flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	while ((numBytesRead = read(STDIN_FILENO, buf, 10)) > 0) {
		if (numBytesRead < 0) {
			errExit("read");
		}

		if (write(fd, buf, numBytesRead) < 0) {
			errExit("write");
		}

		if (write(STDOUT_FILENO, buf, numBytesRead) < 0) {
			errExit("write");
		}
	}

	if (numBytesRead < 0) {
		errExit("read");
	}

	if (close(fd) < 0) {
		errExit("close");
	}

	return 0;
}