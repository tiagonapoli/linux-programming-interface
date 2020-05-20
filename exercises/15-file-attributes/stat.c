/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 15-1 */

/* t_stat.c

   A program that displays the information returned by the stat()/lstat()
   system calls.

   Usage: t_stat [-l] file

   The '-l' option indicates that lstat() rather than stat() should be used.
*/
// #include <sys/sysmacros.h>
#define _XOPEN_SOURCE 500
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include "../../book-src/lib/file_perms.h"
#include "../../book-src/lib/tlpi_hdr.h"
#include <time.h>

static void displayStatInfo(const struct stat *sb) {
	printf("File type:                ");

	switch (sb->st_mode & S_IFMT) {
	case S_IFREG:
		printf("regular file\n");
		break;
	case S_IFDIR:
		printf("directory\n");
		break;
	case S_IFCHR:
		printf("character device\n");
		break;
	case S_IFBLK:
		printf("block device\n");
		break;
	case S_IFLNK:
		printf("symbolic (soft) link\n");
		break;
	case S_IFIFO:
		printf("FIFO or pipe\n");
		break;
	case S_IFSOCK:
		printf("socket\n");
		break;
	default:
		printf("unknown file type?\n");
		break;
	}

	printf("Device containing i-node: major=%ld   minor=%ld\n", (long)major(sb->st_dev), (long)minor(sb->st_dev));

	printf("I-node number:            %ld\n", (long)sb->st_ino);

	printf("Mode:                     %lo (%s)\n", (unsigned long)sb->st_mode, filePermStr(sb->st_mode, 0));

	if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX)) printf("    special bits set:     %s%s%s\n", (sb->st_mode & S_ISUID) ? "set-UID " : "", (sb->st_mode & S_ISGID) ? "set-GID " : "", (sb->st_mode & S_ISVTX) ? "sticky " : "");

	printf("Number of (hard) links:   %ld\n", (long)sb->st_nlink);

	printf("Ownership:                UID=%ld   GID=%ld\n", (long)sb->st_uid, (long)sb->st_gid);

	if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode)) printf("Device number (st_rdev):  major=%ld; minor=%ld\n", (long)major(sb->st_rdev), (long)minor(sb->st_rdev));

	printf("File size:                %lld bytes\n", (long long)sb->st_size);
	printf("Optimal I/O block size:   %ld bytes\n", (long)sb->st_blksize);
	printf("512B blocks allocated:    %lld\n", (long long)sb->st_blocks);

	printf("Last file access:         %ld.%lu\n", sb->st_atime, sb->st_atimensec);
	printf("Last file modification:   %ld.%lu\n", sb->st_mtime, sb->st_mtimensec);
	printf("Last status change:       %ld.%lu\n", sb->st_ctime, sb->st_ctimensec);
}

int main(int argc, char *argv[]) {
	struct stat sb;
	Boolean statLink; /* True if "-l" specified (i.e., use lstat) */
	int fname;        /* Location of filename argument in argv[] */

	statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;
	/* Simple parsing for "-l" */
	fname = statLink ? 2 : 1;

	if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
		usageErr(
		    "%s [-l] file\n"
		    "        -l = use lstat() instead of stat()\n",
		    argv[0]);

	if (statLink) {
		if (lstat(argv[fname], &sb) == -1) errExit("lstat");
	} else {
		if (stat(argv[fname], &sb) == -1) errExit("stat");
	}

	displayStatInfo(&sb);

	exit(EXIT_SUCCESS);
}
