#define _XOPEN_SOURCE 500
#include "../../book-src/lib/tlpi_hdr.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	if (mkdir("test", S_IRUSR | S_IWUSR | S_IXUSR) < 0) {
		errExit("mkdir");
	}

	if (chdir("test") < 0) {
		errExit("chdir");
	}
	int fd = open("myfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		errExit("open");
	}

	if (symlink("myfile", "../mylink") < 0) {
		errExit("symlink");
	}

	if (chmod("../mylink", S_IRUSR) < 0) {
		errExit("chmod");
	}
}