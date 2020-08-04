#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include "../../../book-src/lib/tlpi_hdr.h"
#define BUF_SZ 2
#define SERVER_PATH "/tmp/sequence_number"
#define SERVER_BACKLOG 5

int main() {
	struct sockaddr_un addr;
	char buf[BUF_SZ];
	int sfd;

	if (strlen(SERVER_PATH) + 1 > sizeof(addr.sun_path)) {
		fatal("Server socket path too big: %s", SERVER_PATH);
	}

	if (remove(SERVER_PATH) != 0 && errno != ENOENT) {
		errExit("remove");
	}

	memset(addr.sun_path, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path, SERVER_PATH);

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd < 0) {
		errExit("socket");
	}

	if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) != 0) {
		errExit("bind");
	}

	if (listen(sfd, SERVER_BACKLOG) != 0) {
		errExit("listen");
	}

	int seq = 0;
	int connfd;
	int readBytes;

	char incomingSeq[BUF_SZ];
	for (;;) {
		connfd = accept(sfd, NULL, NULL);
		if (connfd < 0) {
			errExit("accept");
		}

        printf("Accepted connection\n");

		int last = 0;
		while ((readBytes = read(connfd, buf, BUF_SZ)) > 0) {
            printf("%d\n", readBytes);
			strncpy(incomingSeq + last, buf, readBytes);
            last += readBytes;
			for (int i = 0; i < readBytes; i++) {
                printf("[%c][%c]\n", buf[i], incomingSeq[i]);
				if (buf[i] == '$') {
					goto readDone;
				}
			}

		}

		readDone:;

		if (readBytes < 0) {
			errExit("read");
		}

		printf("Incoming data: [%s]\n", incomingSeq);
		int incoming = atoi(incomingSeq);
		seq += incoming;
        printf("Updated seq: %d\n", seq);
		sprintf(buf, "%d", seq);
		int msgSz = strlen(buf);
		if (write(connfd, buf, msgSz) != msgSz) {
			errExit("write");
		}

		if (close(connfd)) {
			errExit("close");
		}
	}
}
