#define _XOPEN_SOURCE 600
#include <ftw.h>
#include <getopt.h>
#include <string.h>
#include <sys/xattr.h>
#include <map>
#include <string>
using namespace std;

void printHelp() {
	printf("Usage ./a.out filepath [OPT]\nOptions are:\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], char *path) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	strcpy(path, argv[1]);
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

int tot = 0;
map<string, int> fileTypes;

int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb) {
	tot++;
	if (type == FTW_NS) { /* Could not stat() file */
		fileTypes["statFail"]++;
	} else {
		switch (sbuf->st_mode & S_IFMT) { /* Print file type */
		case S_IFREG:
			fileTypes["regular"]++;
			break;
		case S_IFDIR:
			fileTypes["directory"]++;
			break;
		case S_IFCHR:
			fileTypes["characterDev"]++;
			break;
		case S_IFBLK:
			fileTypes["blockDev"]++;
			break;
		case S_IFLNK:
			fileTypes["symlink"]++;
			break;
		case S_IFIFO:
			fileTypes["fifo"]++;
            printf("fifo: %s\n", &pathname[ftwb->base]);
			break;
		case S_IFSOCK:
			fileTypes["socket"]++;
            printf("socket: %s\n", &pathname[ftwb->base]);
			break;
		default:
			fileTypes["unkown"]++;
			break; /* Should never happen (on Linux) */
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {
	char path[500];
	readOpts(argc, argv, path);

	if (nftw(path, dirTree, 20, FTW_MOUNT) == -1) {
		perror("nftw");
		exit(EXIT_FAILURE);
	}

	for (auto it : fileTypes) {

		printf("%s: %d [%.3f]\n", it.first.c_str(), it.second, (double)it.second / (double)tot);
	}
	printf("Total files: %d\n", tot);

	exit(EXIT_SUCCESS);
}
