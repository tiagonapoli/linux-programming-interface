#include <pwd.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vector>
using namespace std;

#define MAX_PROC_CNT 40000

static vector<pid_t> adj[MAX_PROC_CNT];
static int inDegree[MAX_PROC_CNT];
static bool valid[MAX_PROC_CNT];
static char name[MAX_PROC_CNT][100];
static char props[100][2][1000];

void printHelp() {
	printf("Usage ./a.out [OPT]\nOptions are:\n");
	printf("  -h: Show this message\n");
}

void readOpts(int argc, char *argv[]) {
	int opt;

	if (argc < 1) {
		printHelp();
		exit(EXIT_FAILURE);
	}

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
		printf("Error on opendir");
		exit(EXIT_FAILURE);
	}

	char **processDirs;
	processDirs = (char **)malloc(sizeof(char *) * MAX_PROC_CNT);

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

int readProcInfo(pid_t pid, int *fieldsCount) {
	char line[1000];
	char path[50];
	int cnt;
	FILE *f = NULL;

	printf("Reading %d\n", pid);
	sprintf(path, "/proc/%d/status", pid);
	if ((f = fopen(path, "r")) == NULL) {
		perror("fopen");
		return -1;
	}

	cnt = 0;
	while (fgets(line, 1000, f) != NULL) {
		sscanf(line, "%[^:]: %[^\n]", props[cnt][0], props[cnt][1]);
		cnt++;
	}

	*fieldsCount = cnt;

	if (ferror(f)) {
		perror("fscanf");
		return -1;
	}

	if (fseek(f, 0, SEEK_SET) != 0) {
		perror("fseek");
		return -1;
	}

	if (fclose(f) == EOF) {
		perror("close");
		return -1;
	}

	return 0;
}

char *getProcFieldValue(const char *field, int fieldCount) {
	for (int i = 0; i < fieldCount; i++) {
		if (strcmp(field, props[i][0]) == 0) {
			return props[i][1];
		}
	}

	return NULL;
}

void printTree(int u, int h) {
	for (int i = 0; i < h; i++) {
		printf("  ");
	}

	printf("+-- %s [%d]\n", name[u], u);
	for (pid_t v : adj[u]) {
		printTree(v, h + 1);
	}
}

void buildAndPrintTree() {
	int fieldsCount;
	pid_t ppid, pid;
	char **procs = readDirContent("/proc");

	for (int i = 0; procs[i] != NULL; i++) {
		pid = atoi(procs[i]);
		if (readProcInfo(pid, &fieldsCount) == -1) {
			printf("Proc %s is not with us anymore", procs[i]);
			continue;
		}

		sscanf(getProcFieldValue("PPid", fieldsCount), "%d", &ppid);
		sscanf(getProcFieldValue("Name", fieldsCount), "%s", name[pid]);
		adj[ppid].push_back(pid);
		inDegree[pid]++;
		valid[pid] = 1;
	}

	for (int i = 0; i < 10; i++) {
		if (valid[i] == 1 && inDegree[i] == 0) {
			printTree(i, 0);
			printf("\n\n");
		} else if (valid[i] == 0 && inDegree[i] == 0 && adj[i].size() > 0) {
			strcpy(name[i], "unknownProc");
			printTree(i, 0);
			printf("\n\n");
		}
	}
}

int main(int argc, char *argv[]) {
	pid_t pid;
	int iter, intervalSec;
	readOpts(argc, argv);
	buildAndPrintTree();
}