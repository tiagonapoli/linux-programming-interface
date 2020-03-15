#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../../book-src/lib/tlpi_hdr.h"

void printUsage() {
	printf("Copy file on source to dest, including holes\n");
	printf("Usage ./a.out [OPT] source dest\nOptions are:\n");
	printf("  -h: Show this message\n");
}

int main(int argc, char* argv[]) {
	char* buf[10];
	ssize_t bytesRead;
	int fdA, fdB;

	if (argc < 2) {
		printf("Missing file arguments\n");
		printUsage();
		exit(EXIT_FAILURE);
	}

	fdA = open(argv[1], O_RDONLY);
	if (fdA < 0) {
		errExit("open");
	}

	fdB = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRGRP | S_IROTH | S_IRUSR | S_IWGRP | S_IWOTH | S_IWUSR);
	if (fdB < 0) {
		errExit("open");
	}

	while ((bytesRead = read(fdA, buf, 10)) > 0) {
		if (bytesRead < 0) {
			errExit("read");
		}

		if (write(fdB, buf, bytesRead) < 0) {
			errExit("write");
		}
	}

	if (bytesRead < 0) {
		errExit("read");
	}

    if(close(fdA)) {
        errExit("close");
    }

    if(close(fdB)) {
        errExit("close");
    }

    return 0;
}