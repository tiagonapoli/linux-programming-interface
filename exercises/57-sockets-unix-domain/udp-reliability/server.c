#define _XOPEN_SOURCE 500
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include "../../../book-src/lib/tlpi_hdr.h"

#define BUF_SIZE 1024
#define SERVER_SOCKET_PATH "/tmp/reliability_server"

int main() {
	struct sockaddr_un addr;
	int sfd;

	sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sfd == -1) {
		errExit("socket");
	}

	if (remove(SERVER_SOCKET_PATH) == -1 && errno != ENOENT) {
		errExit("remove-%s", SERVER_SOCKET_PATH);
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SERVER_SOCKET_PATH, sizeof(addr.sun_path) - 1);
	printf("Will bind server to %s\n", addr.sun_path);
	if (bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) != 0) {
		errExit("bind");
	}

	struct sockaddr_un cl_addr;
	ssize_t numBytes;
	char buf[BUF_SIZE];
	int cfd;
	usleep(10 * 1000 * 1000);
	int now;
	int ant = -1;
	for (int i = 0;; i++) {
		int len = sizeof(struct sockaddr_un);
		numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr*)&cl_addr, &len);
		if (numBytes < 0) {
			errExit("recvfrom");
		}

		now = atoi(buf);
		if (now != ant + 1) {
			printf("not sequential messages\n");
			exit(EXIT_FAILURE);
		}

		ant = now;
		printf("[%d] Server received %ld bytes from %s: %s\n", now, numBytes, cl_addr.sun_path, buf);
        usleep(1 * 1000 * 1000);
	}
}
