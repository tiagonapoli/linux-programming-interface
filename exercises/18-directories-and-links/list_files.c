#define _POSIX_C_SOURCE 1
#include "../../book-src/lib/tlpi_hdr.h"
#include <dirent.h>
#include <stddef.h>

static void /* List all files in directory 'dirpath' */
listFiles(const char *dirpath) {
	DIR *dirp;
	struct dirent *dp, **res;
	Boolean isCurrent; /* True if 'dirpath' is "." */

	isCurrent = strcmp(dirpath, ".") == 0;

	dirp = opendir(dirpath);
	if (dirp == NULL) {
		errMsg("opendir failed on '%s'", dirpath);
		return;
	}

	int nameMax = pathconf(dirpath, _PC_NAME_MAX);
	if (nameMax == -1) /* Indeterminate or error */
		nameMax = 255;   /* So take a guess */

	printf("NAME MAX: %d\n\n", nameMax);
	dp = (struct dirent *)malloc(offsetof(struct dirent, d_name) + nameMax + 1);
	if (dp == NULL) {
		errExit("malloc");
	}

	res = (struct dirent **)malloc(sizeof(struct dirent *));
	if (res == NULL) {
		errExit("malloc");
	}

	for (;;) {
		if (readdir_r(dirp, dp, res) != 0) {
			errExit("readdir_r");
		}

		if (*res == NULL) break;

		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue; /* Skip . and .. */

		if (!isCurrent) printf("%s/", dirpath);
		printf("%s\n", dp->d_name);
	}

	if (errno != 0) errExit("readdir");

	if (closedir(dirp) == -1) errMsg("closedir");

	free(dp);
}

int main(int argc, char *argv[]) {
	if (argc > 1 && strcmp(argv[1], "--help") == 0) usageErr("%s [dir-path...]\n", argv[0]);

	if (argc == 1) /* No arguments - use current directory */
		listFiles(".");
	else
		for (argv++; *argv; argv++) listFiles(*argv);

	exit(EXIT_SUCCESS);
}
