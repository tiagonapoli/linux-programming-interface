#include "../../book-src/lib/tlpi_hdr.h"
#include <getopt.h>
#include <string.h>
#include <sys/xattr.h>

void printHelp() {
	printf("Usage ./a.out filepath eaName eaValue [OPT]\nOptions are:\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], char *path, char *eaName, char *eaValue) {
	int opt;

	if (argc < 4) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	strcpy(path, argv[1]);
	strcpy(eaName, argv[2]);
	strcpy(eaValue, argv[3]);
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
	char eaName[100], eaValue[100];

	readOpts(argc, argv, path, eaName, eaValue);

	if (setxattr(path, eaName, eaValue, strlen(eaValue), 0) < 0) {
		errExit("setxattr");
	}
}