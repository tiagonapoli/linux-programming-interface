#define _XOPEN_SOURCE 500
#include <sys/inotify.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ftw.h>
#define BUF_LEN (10 * (sizeof(struct inotify_event) + 300 + 1))

void printHelp() {
	printf("Usage ./a.out directoryPath [OPT]\nOptions are:\n");
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

int watchDescCnt = 0;
char paths[10000][500];
char tmpStr[1000];
int watchDesc[10000];
int inotifyFd;

void watchDir(const char *path) {
	int wd = inotify_add_watch(inotifyFd, path, IN_ALL_EVENTS | IN_ONLYDIR);
    watchDesc[watchDescCnt] = wd;
	printf("Watching [%s] on wd=%d\n", path, watchDesc[watchDescCnt]);
	if (watchDesc[watchDescCnt] < 0) {
		perror("inotify_add_watch");
		exit(EXIT_FAILURE);
	}
    strcpy(paths[wd], path);
	watchDescCnt++;
}

static void handleEvent(struct inotify_event *i) {
	printf("    wd =%2d; ", i->wd);
	if (i->cookie > 0) printf("cookie =%4d; ", i->cookie);
	printf("mask = ");
	if (i->mask & IN_ACCESS) printf("IN_ACCESS ");
	if (i->mask & IN_ATTRIB) printf("IN_ATTRIB ");
	if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
	if (i->mask & IN_CLOSE_WRITE) printf("IN_CLOSE_WRITE ");
	if (i->mask & IN_CREATE) printf("IN_CREATE ");
	if (i->mask & IN_DELETE) printf("IN_DELETE ");
	if (i->mask & IN_DELETE_SELF) printf("IN_DELETE_SELF ");
	if (i->mask & IN_IGNORED) printf("IN_IGNORED ");
	if (i->mask & IN_ISDIR) printf("IN_ISDIR ");
	if (i->mask & IN_MODIFY) printf("IN_MODIFY ");
	if (i->mask & IN_MOVE_SELF) printf("IN_MOVE_SELF ");
	if (i->mask & IN_MOVED_FROM) printf("IN_MOVED_FROM ");
	if (i->mask & IN_MOVED_TO) printf("IN_MOVED_TO ");
	if (i->mask & IN_OPEN) printf("IN_OPEN ");
	if (i->mask & IN_Q_OVERFLOW) printf("IN_Q_OVERFLOW ");
	if (i->mask & IN_UNMOUNT) printf("IN_UNMOUNT ");
    if (i->len > 0) printf("; name =%s", i->name);
	printf("\n");

	if (i->mask & IN_CREATE && i->mask & IN_ISDIR) {
        sprintf(tmpStr, "%s/", paths[i->wd]);
        strcat(tmpStr, i->name);
        watchDir(tmpStr);
    }
}

int addWatch(const char *fpath, const struct stat *sb, int type, struct FTW *ftwbuf) {
	if (type == FTW_D) {
		watchDir(fpath);
	}

	return 0;
}

void watch(char *path) {
	char buf[BUF_LEN];
	struct inotify_event *ev;

	inotifyFd = inotify_init();
	if (inotifyFd < 0) {
		perror("inotify_init");
		exit(EXIT_FAILURE);
	}

	nftw(path, addWatch, 30, 0);

	int numRead = 0;
	int readCnt = 0;
	while (1) {
		numRead = read(inotifyFd, buf, BUF_LEN);
		printf("\n\n[%d] -------------------------------\n", readCnt++);
		if (numRead <= 0) {
			perror("read");
			exit(EXIT_FAILURE);
		}

		printf("Read %ld bytes from inotify fd\n", (long)numRead);
		for (char *p = buf; p < buf + numRead;) {
			ev = (struct inotify_event *)p;
			handleEvent(ev);
			p += sizeof(struct inotify_event) + ev->len;
		}
	}
}

int main(int argc, char *argv[]) {
	char path[500];

	readOpts(argc, argv, path);
	watch(path);
}
