#include "../../book-src/lib/tlpi_hdr.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>

void printHelp() {
	printf("Usage ./a.out filepath [OPT]\nOptions are:\n");
	printf("  -r: Check read\n");
	printf("  -w: Check write\n");
	printf("  -x: Check execute\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], char *path, int *r, int *w, int *x) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	strcpy(path, argv[1]);
	*r = *w = *x = 0;
	while ((opt = getopt(argc, argv, "hrwx")) != -1) {
		switch (opt) {
		case 'r':
			*r = R_OK;
			break;
		case 'w':
			*w = W_OK;
			break;
		case 'x':
			*x = X_OK;
			break;
		case 'h':
			printHelp();
			exit(EXIT_SUCCESS);
		default:
			printHelp();
			exit(EXIT_FAILURE);
		}
	}
}

int myaccess(const char *path, int mode) {
	uid_t euid = geteuid();
	gid_t egid = getegid();

	struct stat statbuf;
	if (stat(path, &statbuf) < 0) {
		if (errno == ENOENT) {
			printf("File doesn't exists\n");
			return -1;
		}

		errExit("stat");
	}

    printf("owner uid: %d\n", statbuf.st_uid);
    printf("owner group: %d\n", statbuf.st_gid);
	if (euid == 0) {
		return 0;
	}

	int read, write, execute;
	if (euid == statbuf.st_uid) {
		read = S_IRUSR;
		write = S_IWUSR;
		execute = S_IXUSR;
	} else if (egid == statbuf.st_gid) {
		read = S_IRGRP;
		write = S_IWGRP;
		execute = S_IXGRP;
	} else {
		read = S_IROTH;
		write = S_IWOTH;
		execute = S_IXOTH;
	}

	if ((mode & R_OK) && !(read & statbuf.st_mode)) {
		printf("Can't read\n");
		errno = EACCES;
		return -1;
	}

	if ((mode & W_OK) && !(write & statbuf.st_mode)) {
		printf("Can't write\n");
		errno = EACCES;
		return -1;
	}

	if ((mode & X_OK) && !(execute & statbuf.st_mode)) {
		printf("Can't execute\n");
		errno = EACCES;
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[]) {
	char path[1000];
	int r, w, x;
	readOpts(argc, argv, path, &r, &w, &x);
	printf("uid: %d\n", getuid());
	printf("gid: %d\n", getgid());

	printf("euid: %d\n", geteuid());
	printf("egid: %d\n\n", getegid());

	char checkStr[1000];
	strcpy(checkStr, "Check [");
	if (r) strcat(checkStr, "R");
	if (w) strcat(checkStr, "W");
	if (x) strcat(checkStr, "X");
	strcat(checkStr, "]");
	printf("%s\n", checkStr);
	printf("access '%s': %d\n", path, myaccess(path, r | w | x));
}