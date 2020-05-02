
#include "../../book-src/lib/tlpi_hdr.h"
#include <pwd.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

/*

This program was made to test what would happen if a 'read' system
call was made on an open file descriptor initially pointing to a
valid process file - /proc/PID/status - but at some point the process
is finished.

Result:
ESRCH No such process
*/

int printHelp() {
	printf("Usage ./a.out [OPT] PID readIterations intervalSeconds\nOptions are:\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], pid_t *pid, int *iter, int *intervalSec) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	*pid = atoi(argv[1]);
	*iter = atoi(argv[2]);
	*intervalSec = atoi(argv[3]);
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

void parseStatusFile(char *content) {}

void readProcInfo(pid_t pid, int iter, int intervalSec) {
	char props[100][2][1000];
	char line[1000];
	char path[50];
	int cnt;
	FILE *f = NULL;

	sprintf(path, "/proc/%d/status", pid);
	if ((f = fopen(path, "r")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < iter; i++) {
		printf("Iteration %d\n", i);

		cnt = 0;
		while (fgets(line, 1000, f) != NULL) {
			sscanf(line, "%[^:]: %[^\n]", props[cnt][0], props[cnt][1]);
			printf("[%d] [%s]: [%s]\n", cnt, props[cnt][0], props[cnt][1]);
			cnt++;
		}

		if (ferror(f)) {
			printf("Error on FGETS\n");
			break; // Use break to test if close(fd) will error on ESRCH
			perror("fscanf");
			exit(EXIT_FAILURE);
		}

		if (fseek(f, 0, SEEK_SET) != 0) {
			perror("fseek");
			exit(EXIT_FAILURE);
		}

		printf("-- [%d] Sleep %d --\n\n\n", i, intervalSec);
		sleep(intervalSec);
	}

	printf("Will close fd\n");
	if (fclose(f) == EOF) {
		perror("close");
		exit(EXIT_FAILURE);
	}
	printf("Closed fd\n");
}

int main(int argc, char *argv[]) {
	pid_t pid;
	int iter, intervalSec;
	readOpts(argc, argv, &pid, &iter, &intervalSec);
	printf("Status for PID %d\n", pid);
	readProcInfo(pid, iter, intervalSec);
}