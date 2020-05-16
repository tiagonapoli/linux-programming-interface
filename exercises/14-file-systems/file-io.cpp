#include <pwd.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <algorithm>
using namespace std;

#define N 100000

void printHelp() {
	printf("Usage ./a.out [OPT]\nOptions are:\n");
	printf("  -n: Number of files to create\n");
	printf("  -s: Size in bytes of each file\n");
	printf("  -c: Create mode ('random' or 'ordered')\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], int *n, char *c, int *sz) {
	int opt;

	if (argc < 1) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	*n = 1000;
	*sz = 1;
	strcpy(c, "random");
	while ((opt = getopt(argc, argv, "hn:c:s:")) != -1) {
		switch (opt) {
		case 's':
			*sz = atoi(optarg);
			break;
		case 'n':
			*n = atoi(optarg);
			break;
		case 'c':
			if (strcmp(optarg, "random") != 0 && strcmp(optarg, "ordered") != 0) {
				printHelp();
				exit(EXIT_FAILURE);
			}

			strcpy(c, optarg);
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

double diffclockMs(clock_t clock1, clock_t clock2) {
	double diffticks = clock1 - clock2;
	double diffms = diffticks * 1000.0 / CLOCKS_PER_SEC;
	return diffms;
}

int randomInteger(int low, int high) { return low + (rand() % (high - low)); }

int ids[N];

void calcRandom(int n) {
	ids[0] = rand() % 1000;
	for (int i = 1; i < n; i++) {
		ids[i] = ids[i - 1] + 1 + rand() % 1000;
	}

	random_shuffle(ids, ids + n);
}

void createFiles(int n, int sz) {
	char path[100];
	char content[100];
	int fd;
	int cnt = 0;

	int dirfd = open("./tmp", O_DIRECTORY);
	if (dirfd == -1) {
		printf("Error open %d\n", errno);
		perror("open");
		exit(EXIT_FAILURE);
	}

	clock_t startCreatFiles = clock();
	for (int i = 0; i < n; i++) {
		sprintf(path, "./tmp/x%09d", ids[i]);
		// printf("create %s\n", path);
		fd = open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		if (fd == -1) {
			printf("Error open %d\n", errno);
			perror("open");
			exit(EXIT_FAILURE);
		}

		for (int j = 0; j < sz; j++) {
			content[j] = 'a' + cnt;
			cnt = (cnt + 1) % 26;
		}
		content[sz] = '\0';
		// printf("content [%s]\n", content);

		if (write(fd, content, sz) != sz) {
			printf("Error write %d\n", errno);
			exit(EXIT_FAILURE);
		}

		if (close(fd) == -1) {
			printf("Error close %d\n", errno);
			exit(EXIT_FAILURE);
		}
	}

	clock_t startDirSync = clock();
	if (fsync(dirfd) == -1) {
		printf("Error fsync %d\n", errno);
		perror("fsync");
		exit(EXIT_FAILURE);
	}
	printf("Dir sync took: %f ms\n", diffclockMs(clock(), startDirSync));

	if (close(dirfd) == -1) {
		printf("Error close %d\n", errno);
		exit(EXIT_FAILURE);
	}
}

int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }
void deleteFiles(int n) {
	qsort(ids, n, sizeof(int), compare);
	clock_t startDeleteFiles = clock();

	for (int i = 0; i < n - 1; i++) {
		if (ids[i] == ids[i + 1]) {
			printf("Error: two files with the same name\n");
			exit(EXIT_FAILURE);
		}
	}

	char path[100];
	for (int i = 0; i < n; i++) {
		sprintf(path, "./tmp/x%09d", ids[i]);
		// printf("delete %s\n", path);
		if (unlink(path) == -1) {
			printf("delete %s\n", path);
			perror("unlink");
		}
	}
}

int main(int argc, char *argv[]) {
	clock_t start = clock();
	srand(time(NULL));
	int n, sz;
	char c[10];

	readOpts(argc, argv, &n, c, &sz);
	printf("Number of files: %d\n", n);
	printf("Size of each file: %d\n", sz);

	clock_t startCalcIds = clock();
	if (strcmp(c, "random") == 0) {
		calcRandom(n);
		printf("Create random ids: %f ms\n", diffclockMs(clock(), startCalcIds));
	} else {
		for (int i = 0; i < n; i++) {
			ids[i] = i;
		}

		printf("Create ordered ids: %f ms\n", diffclockMs(clock(), startCalcIds));
	}

	createFiles(n, sz);
	deleteFiles(n);
}