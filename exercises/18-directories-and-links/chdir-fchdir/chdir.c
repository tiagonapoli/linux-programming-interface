#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

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
	char path[500];
	int n;
	readOpts(argc, argv, &n);

	if (getcwd(path, 500) == NULL) {
		perror("getcwd");
		exit(EXIT_FAILURE);
	}

    printf("%s\n", path);
	for (int i = 0; i < n; i++) {
		chdir(path);
	}
}