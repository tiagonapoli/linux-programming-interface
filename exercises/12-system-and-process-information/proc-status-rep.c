
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

void readOpts(int argc, char *argv[], char **pid, int *iter, int *intervalSec) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	*pid = argv[1];
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

void printProcInfo(const char *pid, int iter, int intervalSec) {
	char path[50], statusFileContent[10000];
	int cnt, r;
	int fd;

	sprintf(path, "/proc/%s/status", pid);
	if ((fd = open(path, O_RDONLY)) == -1) {
		errExit("open");
	}

	for (int i = 0; i < iter; i++) {
		printf("Iteration %d\n", i);

		cnt = 0;
		while ((r = read(fd, &statusFileContent[cnt], 500)) > 0) {
			cnt += r;
		}

		if (r < 0) {
			printf("Error on READ\n");
            // break; // Use break to test if close(fd) will error on ESRCH
			errExit("read");
		}

		if (lseek(fd, 0, SEEK_SET) < 0) {
			// break;
			errExit("lseek");
		}

        statusFileContent[cnt] = '\0';
		printf("%s\n", statusFileContent);
        printf("-- Sleep %d --\n", intervalSec);
		sleep(intervalSec);
	}

	printf("Will close fd\n");
	if (close(fd) == -1) {
		errExit("close");
	}
	printf("Closed fd\n");
}

int main(int argc, char *argv[]) {
	char *pid;
    int iter, intervalSec;
	readOpts(argc, argv, &pid, &iter, &intervalSec);
	printf("Status for PID %s\n", pid);
    printProcInfo(pid, iter, intervalSec);
}