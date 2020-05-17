#include "../../book-src/lib/tlpi_hdr.h"
#include <getopt.h>
#include <sys/statvfs.h>
#include <fcntl.h>

void printHelp() {
	printf("Usage ./a.out PATH [OPT]\nOptions are:\n");
	printf("  -n [NUM]: output the last NUM lines\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], char *path, int *n) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	strcpy(path, argv[1]);
	*n = 10;
	while ((opt = getopt(argc, argv, "hn:")) != -1) {
		switch (opt) {
		case 'n':
			*n = atoi(optarg);
			break;
		case 'h':
			printHelp();
			exit(EXIT_SUCCESS);
		default:
			printHelp();
			exit(EXIT_FAILURE);
		}
	}
}

#define MAX_BUFFERS_CNT 1000
struct fileBuffer {
	char *buffer;
	size_t size;
};

struct fileBuffer buffers[MAX_BUFFERS_CNT];

off_t readBuf(int fd, off_t filePos, size_t bufSize, struct fileBuffer *fBuffer) {
	off_t newPos = max(0LL, filePos - (off_t)bufSize);
	fBuffer->size = filePos - newPos;
	fBuffer->buffer = (char *)malloc(fBuffer->size);

	if (fBuffer->buffer == NULL) {
		errExit("malloc");
	}

	if (lseek(fd, newPos, SEEK_SET) < 0) {
		errExit("lseek");
	}

	if (read(fd, fBuffer->buffer, fBuffer->size) != fBuffer->size) {
		errExit("read");
	}

	if (lseek(fd, newPos, SEEK_SET) < 0) {
		errExit("lseek");
	}

	return newPos;
}

void tail(int n, char *path) {
	int fd, bufferSize;
	int bufCount = 0;
	off_t filePos;
	struct statvfs fsInfo;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		errExit("fopen");
	}

	if (fstatvfs(fd, &fsInfo) < 0) {
		errExit("fstatvfs");
	}

	bufferSize = fsInfo.f_bsize;
	filePos = lseek(fd, 0, SEEK_END);
	if (filePos < 0) {
		errExit("lseek");
	}

	int linesCount = 1;
	int initBuf = 0;
	int initBufPos = 0;

	while (1) {
		filePos = readBuf(fd, filePos, bufferSize, &buffers[bufCount]);
		for (int i = buffers[bufCount].size - 1; i >= 0; i--) {
			if (buffers[bufCount].buffer[i] != '\n') {
				continue;
			}

			if (bufCount == 0 && i == buffers[bufCount].size - 1) {
				continue;
			}

			if (linesCount == n) {
				initBuf = bufCount;
				initBufPos = i + 1;
				if (initBufPos == buffers[bufCount].size) {
					initBufPos = 0;
					initBuf = bufCount - 1;
				}

				goto done;
			}

			linesCount++;
		}

		bufCount++;
		if (filePos == 0) {
			initBufPos = 0;
			initBuf = bufCount - 1;
			goto done;
		}
	}

done:;

	printf("%.*s", (int)buffers[initBuf].size - initBufPos, buffers[initBuf].buffer + initBufPos);
	for (int i = initBuf - 1; i >= 0; i--) {
		printf("%.*s", (int)buffers[i].size, buffers[i].buffer);
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	char path[1000];
	int n;

	readOpts(argc, argv, path, &n);
	tail(n, path);
}