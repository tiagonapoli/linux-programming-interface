#define _XOPEN_SOURCE 500

#include "../../book-src/lib/tlpi_hdr.h"
#include <pwd.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_PROC_CNT 40000
static char fds[300][1000];

void printHelp() {
	printf("Usage ./a.out PID [OPT]\nOptions are:\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], pid_t *pid) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

    *pid = atoi(argv[1]);

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

char **readDirContent(const char *dirpath) {
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dirpath)) == NULL) {
		printf("Error on opendir %s", dirpath);
		errMsg("opendir");
		return NULL;
	}

	char **processDirs;
	processDirs = (char **)malloc(sizeof(char *) * MAX_PROC_CNT);

	int cnt = 0;
	errno = 0;
	while ((ent = readdir(dir)) != NULL) {
		int i = 0;
		for (; ent->d_name[i] != '\0'; i++) {
			if (!isdigit(ent->d_name[i])) {
				goto notNumber;
			}
		}

		processDirs[cnt] = (char *)malloc(sizeof(8));
		strcpy(processDirs[cnt], ent->d_name);
		cnt++;
	notNumber:;
		errno = 0;
	}

	if (errno != 0) {
		printf("Error reading dir '%s'\n", dirpath);
		errMsg("readdir");
		return NULL;
	}

	processDirs[cnt] = NULL;
	return processDirs;
}

int readProcFd(pid_t pid) {
    if(pid == getpid()) {
        printf("Own PID, returning...");
        return 0;
    }

	char fdDir[50], fdPath[50];
	FILE *f = NULL;

	printf("-- Reading %d --\n", pid);
	sprintf(fdDir, "/proc/%d/fd", pid);
	char **fds = readDirContent(fdDir);
	if (fds == NULL) {
		printf("Error reading FDs from process %d\n", pid);
		return -1;
	}

	char fdInfo[300][500];
	for (int i = 0; fds[i] != NULL; i++) {
		sprintf(fdPath, "/proc/%d/fd/%s", pid, fds[i]);
		int r = readlink(fdPath, fdInfo[i], 500);
        if (r == -1) {
			printf("Error reading fd[%s] from process %d\n", fds[i], pid);
			return -1;
		}

        if(r >= 200) {
            printf("Error reading fd[%s] from process %d - BUF limit - used %d\n", fds[i], pid, r);
			exit(EXIT_FAILURE);
            return -1;
        }

        fdInfo[i][r] = '\0';
	}

	for (int i = 0; fds[i] != NULL; i++) {
		printf("%s\n", fdInfo[i]);
	}
	printf("\n");

	return 0;
}

int main(int argc, char *argv[]) {
	pid_t pid;
    readOpts(argc, argv, &pid);
    readProcFd(pid);
}