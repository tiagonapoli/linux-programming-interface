#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <fcntl.h>

void printHelp() {
	printf("Usage ./a.out numberOfRepetitions [OPT]\nOptions are:\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], int *n) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	*n = atoi(argv[1]);
	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
		case 'h':
			printHelp();
			exit(EXIT_SUCCESS);
		default:
			printHelp();
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[]) {
	int n;
	readOpts(argc, argv, &n);

	int fd = open(".", O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i++) {
		fchdir(fd);
	}
}