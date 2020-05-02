
#include "../../book-src/lib/tlpi_hdr.h"
#include <pwd.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_PROC_CNT 10000

int printHelp() {
	printf("Usage ./a.out [OPT] username\nOptions are:\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[], char **username) {
	int opt;

	if (argc < 2) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	*username = argv[1];
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

uid_t userIdFromName(char *name) {
	struct passwd *info = getpwnam(name);
	errno = 0;
	if (info != NULL) {
		return info->pw_uid;
	}

	if (errno == 0) {
		printf("User '%s' not found\n", name);
		exit(EXIT_FAILURE);
	} else {
		errExit("getpwnam");
	}
}

char **readDirContent(char *dirpath) {
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dirpath)) == NULL) {
		errExit("opendir");
	}

	char **processDirs;
	int maxProc = 1e4;
	processDirs = (char **)malloc(sizeof(char *) * MAX_PROC_CNT);
	for (int i = 0; i < MAX_PROC_CNT; i++) {
	}

	int cnt = 0;
	while ((ent = readdir(dir)) != NULL) {
		int i = 0;
		for (; ent->d_name[i] != '\0'; i++) {
			if (!isdigit(ent->d_name[i])) {
				goto notPid;
			}
		}

		processDirs[cnt] = (char *)malloc(sizeof(8));
		strcpy(processDirs[cnt], ent->d_name);
		cnt++;
	notPid:;
	}

	processDirs[cnt] = NULL;
	return processDirs;
}

void handleProcIOError(const char *fn) {
	if (errno == ESRCH || errno == ENOENT) {
		return;
	}

	errExit(fn);
}

int getProcInfo(const char *pid, int *fieldsCount, char *props[100][2]) {
	char path[50], statusFileContent[10000];
	int cnt, r;
	int fd;

	sprintf(path, "/proc/%s/status", pid);
	if ((fd = open(path, O_RDONLY)) == -1) {
		handleProcIOError("open");
		return -1;
	}

	cnt = 0;
	while ((r = read(fd, &statusFileContent[cnt], 500)) > 0) {
		cnt += r;
	}

	if (r < 0) {
		handleProcIOError("read");
		if (close(fd) == -1) {
			errExit("close");
		}
		return -1;
	}

	statusFileContent[cnt] = '\0';
	char *remaining = statusFileContent;
	*fieldsCount = 0;

	while (remaining != NULL && remaining[0] != '\0') {
		props[*fieldsCount][0] = (char *)malloc(30 * sizeof(char));
		props[*fieldsCount][1] = (char *)malloc(400 * sizeof(char));
		sscanf(remaining, "%[^:]: %[^\n]s", props[*fieldsCount][0], props[*fieldsCount][1]);
		(*fieldsCount)++;
		remaining = strchr(remaining, '\n');
		remaining++;
	}

	if (close(fd) == -1) {
		errExit("close");
	}

	return 0;
}

char *getProcFieldValue(const char *field, int fieldCount, char *props[100][2]) {
	for (int i = 0; i < fieldCount; i++) {
		if (strcmp(field, props[i][0]) == 0) {
			return props[i][1];
		}
	}

	return NULL;
}

void printPropFields(int fieldCount, char *props[100][2]) {
	for (int i = 0; i < fieldCount; i++) {
		printf("[%s] = [%s]\n", props[i][0], props[i][1]);
	}
}

int main(int argc, char *argv[]) {
	char *username;
	uid_t userId;

	readOpts(argc, argv, &username);

	userId = userIdFromName(username);
	char **procs = readDirContent("/proc");
	printf("Process owned by %s[%u]", username, userId);
	for (int i = 0; procs[i] != NULL; i++) {
		char *props[100][2];
		int fieldsCount;
		uid_t realUid;

		if (getProcInfo(procs[i], &fieldsCount, props) == -1) {
			printf("Proc %s is not with us anymore", procs[i]);
			continue;
		}

		sscanf(getProcFieldValue("Uid", fieldsCount, props), "%d", &realUid);
		// printf("Uid: %s\n", getProcFieldValue("Uid", fieldsCount, props));
		if (realUid != userId) {
			continue;
		}

		printf("[%5s] Name: %s", procs[i], getProcFieldValue("Name", fieldsCount, props));
		printf("\n");
		for (int j = 0; j < fieldsCount; j++) {
			free(props[j][0]);
			free(props[j][1]);
		}
	}
}