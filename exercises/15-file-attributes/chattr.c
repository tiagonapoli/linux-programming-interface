#include "../../book-src/lib/tlpi_hdr.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <getopt.h>

void printHelp() {
	printf("Usage ./a.out [-+=aAcCdDPsStTu] filepath\n");
}

void readOpts(int argc, char *argv[], char *path, char *op, char *flags) {
	int opt;

	if (argc < 3) {
		printHelp();
		exit(EXIT_FAILURE);
	}

	*op = argv[1][0];
	strcpy(path, argv[2]);
	strcpy(flags, argv[1] + 1);
}

void tweakFlag(char op, int flagNo, int *currentFlags) {
	if (op == '+' || op == '=') {
		*currentFlags |= flagNo;
	} else {
		*currentFlags &= ~flagNo;
	}
}

int main(int argc, char *argv[]) {
	char path[1000], flags[20], op;
	readOpts(argc, argv, path, &op, flags);

	int fd = open(path, O_RDWR);
	if (fd < 0) {
		errExit("open");
	}

	int inodeFlags = 0;
	if (op == '+' || op == '-') {
		if (ioctl(fd, FS_IOC_GETFLAGS, &inodeFlags) < 0) {
			errExit("ioctl");
		}
	}


	int len = strlen(flags);
	for (int i = 0; i < len; i++) {
		switch (flags[i]) {
		case 'a':
			tweakFlag(op, FS_APPEND_FL, &inodeFlags);
			break;
		case 'A':
			tweakFlag(op, FS_NOATIME_FL, &inodeFlags);
			break;
		case 'c':
			tweakFlag(op, FS_COMPR_FL, &inodeFlags);
			break;
		case 'C':
			tweakFlag(op, FS_NOCOW_FL, &inodeFlags);
			break;
		case 'd':
			tweakFlag(op, FS_NODUMP_FL, &inodeFlags);
			break;
		case 'D':
			tweakFlag(op, FS_DIRSYNC_FL, &inodeFlags);
			break;
		case 'P':
			tweakFlag(op, FS_PROJINHERIT_FL, &inodeFlags);
			break;
		case 's':
			tweakFlag(op, FS_SECRM_FL, &inodeFlags);
			break;
		case 'S':
			tweakFlag(op, FS_SYNC_FL, &inodeFlags);
			break;
		case 't':
			tweakFlag(op, FS_NOTAIL_FL, &inodeFlags);
			break;
		case 'T':
			tweakFlag(op, FS_TOPDIR_FL, &inodeFlags);
			break;
		case 'u':
			tweakFlag(op, FS_UNRM_FL, &inodeFlags);
			break;
		default:
			printf("Invalid flag\n");
			exit(EXIT_FAILURE);
		}
	}

	if (ioctl(fd, FS_IOC_SETFLAGS, &inodeFlags) < 0) {
		errExit("ioctl");
	}

	if (close(fd) < 0) {
		errExit("close");
	}
}