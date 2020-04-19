#include "../../book-src/lib/tlpi_hdr.h"
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>

int printHelp() {
	printf("Usage ./a.out [OPT] fileToWrite\nOptions are:\n");
	printf("  -n: Number of bytes to write\n");
	printf("  -b: Buffer chunk size\n");
	printf("  -a: Use O_APPEND flag\n");
	printf("  -h: Show this message\n");
}

int readOpts(int argc, char *argv[], char **filename, long long *bytes, int *append, int *bufferSize) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	*filename = argv[1];
	*append = 0;
	*bytes = 10;
	*bufferSize = 1;
	while ((opt = getopt(argc, argv, "an:b:h")) != -1) {
		switch (opt) {
		case 'n':
			*bytes = atol(optarg);
			break;
		case 'a':
			*append = 1;
			break;
		case 'b':
			*bufferSize = atoi(optarg);
			break;
		case 'h':
			printHelp();
			exit(EXIT_SUCCESS);
		default:
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[]) {
	int fd, append, openFlags, i, cnt;
	long long bytes;
	int bufferSize;
	char *buffer;
	char *filename;
	char character;

	readOpts(argc, argv, &filename, &bytes, &append, &bufferSize);
	
	character = 'a';
	if(append) {
		character = 'b';
	}

	buffer = (char *)malloc(bufferSize * sizeof(char));
	for (i = 0; i < bufferSize; i++) {
		buffer[i] = character;
	}

	printf("Output: %s\nBytes: %lld\nAppend: %d\nBuffer size: %d\n\n", filename, bytes, append, bufferSize);

	openFlags = O_CREAT | O_RDWR;
	if (append) openFlags |= O_APPEND;

	fd = open(filename, openFlags, S_IRUSR | S_IWUSR | S_IXUSR);
	if (fd == -1) errExit("open");

	for (i = 0; i < bytes;) {
		if (!append && lseek(fd, 0, SEEK_END) == -1) errExit("lseek");
		cnt = write(fd, buffer, min(bufferSize, bytes - i));
		if (cnt == -1) errExit("write");
		i += cnt;
	}

	if(close(fd) == -1) errExit("close");
	
}