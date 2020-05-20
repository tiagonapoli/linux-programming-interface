#include <stdio.h>
#include <stdlib.h>

void printHelp() { printf("Usage ./a.out oldpath newpath\n"); }

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printHelp();
		exit(EXIT_FAILURE);
	}

    printf("%s -> %s\n", argv[1], argv[2]);
	if (rename(argv[1], argv[2]) < 0) {
		perror("rename");
	}
}