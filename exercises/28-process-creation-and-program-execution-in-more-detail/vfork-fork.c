#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>

void test_fork(int iter) {
	int child;
	for (int i = 0; i < iter; i++) {
		switch ((child = fork())) {
		case -1:
			exit(EXIT_FAILURE);
		case 0:
			exit(EXIT_SUCCESS);
		}

		if (wait(NULL) != child) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
	}
}

void test_vfork(int iter) {
	int child;
	for (int i = 0; i < iter; i++) {
		switch ((child = vfork())) {
		case -1:
			exit(EXIT_FAILURE);
		case 0:
			exit(EXIT_SUCCESS);
		}

		if (wait(NULL) != child) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc < 4) {
		printf("Usage:\n./a.out fork|vfork ITERATIONS BYTES_ALLOCATED\n");
		exit(EXIT_FAILURE);
	}

	size_t bytes = atol(argv[3]);
	int iter = atoi(argv[2]);
	char *p = malloc(bytes);
	memset(p, 1, bytes);
	printf("%d\n", p[bytes - 1]);

	if (p == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[1], "vfork") == 0) {
		printf("Test vfork - %d iterations - %ld bytes\n", iter, bytes);
		test_vfork(iter);
	} else {
		printf("Test fork - %d iterations - %ld bytes\n", iter, bytes);
		test_fork(iter);
	}
}