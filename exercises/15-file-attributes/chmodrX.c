#include "../../book-src/lib/tlpi_hdr.h"
#include <sys/types.h>
#include <getopt.h>
#include <sys/stat.h>
#include <unistd.h>

void printHelp() {
	printf("Usage ./a.out filepath [OPT]\nOptions are:\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], char *path) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	strcpy(path, argv[1]);
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
	char path[1000];

	readOpts(argc, argv, path);

	struct stat statbuf;
	if (stat(path, &statbuf) < 0) {
		errExit("stat");
	}

	int executeFlags = S_IXUSR | S_IXGRP | S_IXOTH;
	int newmode = statbuf.st_mode | S_IRUSR | S_IRGRP | S_IROTH;
	if (S_ISDIR(statbuf.st_mode) || statbuf.st_mode & executeFlags) {
		newmode |= executeFlags;
	}

	if (chmod(path, newmode) < 0) {
		errExit("chmod");
	}
}