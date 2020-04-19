#include "../../book-src/lib/tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>

int customReadv(int fd, const struct iovec *iov, int iovcnt) {
	int i;
	char *buffer;
	size_t sz;
	size_t bytes;
	sz = 0;

	for (i = 0; i < iovcnt; i++) {
		sz += iov[i].iov_len;
	}

	buffer = (char *)malloc(sz);
	bytes = read(fd, buffer, sz);
	if (bytes == -1) errExit("read");

	printf("buffer: %s\n", buffer);

	sz = 0;
	for (i = 0; i < iovcnt; i++) {
		memcpy(iov[i].iov_base, &buffer[sz], min(bytes, iov[i].iov_len));
		printf("[%d]: %ld [internalBuffer base %ld]\n", i, min(bytes, iov[i].iov_len), sz);
        sz += min(bytes, iov[i].iov_len);
		bytes -= min(bytes, iov[i].iov_len);
		printf("remaining bytes: %ld\n\n", bytes);
	}
}

int customWritev(int fd, const struct iovec *iov, int iovcnt) {
	int i;
	char *buffer;
	size_t sz;
	sz = 0;

	for (i = 0; i < iovcnt; i++) {
		sz += iov[i].iov_len;
	}

	buffer = (char *)malloc(sz);
	sz = 0;
	for (i = 0; i < iovcnt; i++) {
		memcpy(&buffer[sz], iov[i].iov_base, iov[i].iov_len);
		sz += iov[i].iov_len;
	}

	printf("Buffer to be written [%ld bytes]: %s\n", sz, buffer);

	return write(fd, buffer, sz);
}

int printHelp() {
	printf("Usage ./a.out [OPT] fileToRead bytesToRead\nOptions are:\n");
	printf("  -h: Show this message\n");
}

int readOpts(int argc, char *argv[], char **filename, size_t *bytes) {
	int opt;

	if (argc < 3) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	*filename = argv[1];
	*bytes = atol(argv[2]);
}

int main(int argc, char *argv[]) {
	int fdInput, fdOutput;
	size_t bytes;
	int i, iovcnt;
	char *inputFile;

	char inBuffer[1000];
	struct iovec in[1000];

	readOpts(argc, argv, &inputFile, &bytes);

	for (i = 0, iovcnt = 0; i < bytes; iovcnt++) {
		in[iovcnt].iov_len = min(bytes - i, 2);
		in[iovcnt].iov_base = &inBuffer[i];
		i += in[iovcnt].iov_len;
	}

	fdInput = open(inputFile, O_RDONLY);
	if (fdInput == -1) errExit("open");
	customReadv(fdInput, in, iovcnt);

	printf("Buffer: %s\n", inBuffer);

	fdOutput = open("out.tmp", O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR);
	if (fdInput == -1) errExit("open");
	if (customWritev(fdOutput, in, iovcnt) == -1) errExit("write");
}