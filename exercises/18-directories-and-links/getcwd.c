#define _XOPEN_SOURCE 500
#include "../../book-src/lib/tlpi_hdr.h"
#include <dirent.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

void mygetcwd(char *cwd) {
	int initFd;

	initFd = open(".", O_RDONLY);
	if (initFd < 0) {
		errExit("open");
	}

	char tmp[500];
	cwd[0] = '\0';
	struct stat upperDirStats, dirStats, curStats;
	DIR *dir;
	struct dirent *dirEntry;

	if (stat(".", &upperDirStats) < 0) {
		errExit("stat");
	}

	while (1) {
		dirStats = upperDirStats;
		if (stat("..", &upperDirStats) < 0) {
			errExit("stat");
		}

		if (dirStats.st_dev == upperDirStats.st_dev && dirStats.st_ino == upperDirStats.st_ino) {
			/* Is root dir '/' */
			sprintf(tmp, "/%s", cwd);
			strcpy(cwd, tmp);
			break;
		}

		if (chdir("..") < 0) {
			errExit("chdir");
		}

		dir = opendir(".");
		if (dir == NULL) {
			errExit("opendir");
		}

		for (;;) {
			errno = 0;
			dirEntry = readdir(dir);
			if (dirEntry == NULL) break;

			if (strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) continue; /* Skip . and .. */

			if (stat(dirEntry->d_name, &curStats) < 0) {
				errExit("stat");
			}

			if (curStats.st_dev == dirStats.st_dev && curStats.st_ino == dirStats.st_ino) {
				sprintf(tmp, "%s%s%s", dirEntry->d_name, cwd[0] == '\0' ? "" : "/", cwd);
				strcpy(cwd, tmp);
				break;
			}
		}

		if (errno != 0) errExit("readdir");

		if (closedir(dir) == -1) errMsg("closedir");
	}

	if (fchdir(initFd) < 0) {
		errExit("fchdir");
	}
}

int main(int argc, char *argv[]) {
	char path[500];
	mygetcwd(path);
	printf("%s\n", path);

	char cwd[500];
	printf("%s\n", getcwd(cwd, 500));
}
