#define _XOPEN_SOURCE 500
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include "../../book-src/lib/tlpi_hdr.h"
#define BUF_SIZE 1024

int createAndBindSocket(const char* path, struct sockaddr_un* addr) {
	int sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sfd == -1) {
		errExit("socket");
	}

	if (remove(path) == -1 && errno != ENOENT) {
		errExit("remove-%s", path);
	}

	memset(addr, 0, sizeof(struct sockaddr_un));
	addr->sun_family = AF_UNIX;
	snprintf(addr->sun_path, sizeof(addr->sun_path), "%s", path);

	printf("Creating %s socket\n", addr->sun_path);
	if (bind(sfd, (struct sockaddr*)addr, sizeof(struct sockaddr_un)) != 0) {
		errExit("bind");
	}

	return sfd;
}

int main() {
	struct sockaddr_un addr1, addr2, addr3;
	int sfd1, sfd2, sfd3;

	sfd1 = createAndBindSocket("/tmp/a.sock", &addr1);
	sfd2 = createAndBindSocket("/tmp/b.sock", &addr2);
	sfd3 = createAndBindSocket("/tmp/c.sock", &addr3);

	if (connect(sfd1, (struct sockaddr*)&addr2, sizeof(struct sockaddr_un)) < 0) {
		errExit("connect");
	}

	if (sendto(sfd3, "msg test\0", 9, 0, (struct sockaddr*)&addr1, sizeof(struct sockaddr_un)) != 9) {
		errExit("sendto");
	}
}