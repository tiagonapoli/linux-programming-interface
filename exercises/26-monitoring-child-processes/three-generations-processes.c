#include <stdio.h>
#include <stdlib.h>
#include "../../book-src/lib/tlpi_hdr.h"

int generation = 0;
bool done = 0;

void do_fork() {
	switch (fork()) {
	case -1:
		errExit("fork");
	case 0:
		done = 0;
		generation++;
	}

	if (done) return;
	printf("Do work on generation %d - PID %d\n", generation, getpid());
	done = 1;
	if (generation == 0) {
		sleep(3);
		printf("[parent] Will exit\n");
		exit(0);
	} else if (generation == 1) {
		do_fork();
		sleep(1);
		printf("[child] Will exit\n");
		exit(0);
	} else {
		printf("[grandchild] Parent: %d\n", getppid());
		sleep(2);
		printf("[grandchild] Parent: %d\n", getppid());
		sleep(2);
		printf("[grandchild] Parent: %d\n", getppid());
		printf("[grandchild] Will exit\n");
		exit(0);
	}
}

int main() {
	setbuf(stdout, NULL);
	do_fork();
}