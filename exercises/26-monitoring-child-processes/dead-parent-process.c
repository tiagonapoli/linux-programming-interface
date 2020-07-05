#include <stdio.h>
#include <stdlib.h>
#include "../../book-src/lib/tlpi_hdr.h"

int main() {
	setbuf(stdout, NULL);
	switch (fork()) {
	case -1:
		errExit("fork");
	case 0:
		printf("Parent pid before: %d\n", getppid());
		sleep(3);
		printf("Parent pid after %d\n", getppid());
		exit(0);
	}

	sleep(1);
	printf("Parent will finish\n");
	exit(0);
}