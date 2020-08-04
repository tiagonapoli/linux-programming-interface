#define _XOPEN_SOURCE 500
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include "../../../book-src/lib/tlpi_hdr.h"

#define BUF_SIZE 1024
#define CLIENT_SOCKET_PATH "/tmp/reliability_client"
#define SERVER_SOCKET_PATH "/tmp/reliability_server"

int main() {
	struct sockaddr_un addr, server_addr;
	int sfd;

	sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sfd == -1) {
		errExit("socket");
	}

	if (remove(CLIENT_SOCKET_PATH) == -1 && errno != ENOENT) {
		errExit("remove-%s", CLIENT_SOCKET_PATH);
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	snprintf(addr.sun_path, sizeof(addr.sun_path), CLIENT_SOCKET_PATH);

	if (bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) != 0) {
		errExit("bind");
	}

	memset(&server_addr, 0, sizeof(struct sockaddr_un));
	server_addr.sun_family = AF_UNIX;
	strncpy(server_addr.sun_path, SERVER_SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

	int numBytes;
	char buf[BUF_SIZE];
    
	for (int i = 0;; i++) {
		sprintf(buf, "%d", i);
		ssize_t msgLen = strlen(buf) + 1;
        printf("[%d] Will send %ld bytes\n",i, msgLen);
		if (sendto(sfd, buf, msgLen, 0, (struct sockaddr*) &server_addr, sizeof(struct sockaddr_un)) != msgLen) {
			errExit("sendto");
		}
	}
}