#include <sys/un.h>
#include <sys/socket.h>
#include "../../../book-src/lib/tlpi_hdr.h"
#define SERVER_PATH "/tmp/sequence_number"
#define BUF_SIZE 2

int main(int argc, char *argv[]) {
	struct sockaddr_un addr;
	int sfd;
	ssize_t numRead;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX, SOCK_STREAM, 0); /* Create client socket */
	if (sfd == -1) errExit("socket");

	/* Construct server address, and make the connection */

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SERVER_PATH, sizeof(addr.sun_path) - 1);

	if (connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) errExit("connect");

	/* Copy stdin to socket */

    if(write(sfd, "1000\0$", 6) != 6) {
        errExit("write");
    }

    printf("Written data\n");
    int data_sz = 0;
    char data[BUF_SIZE];
    while ((numRead = read(sfd, buf, BUF_SIZE)) > 0) {
        strncpy(data + data_sz, buf, numRead);
        data_sz += numRead;
    }

    int res = atoi(data);
    printf("Received %d from the server\n", res);
	exit(EXIT_SUCCESS); /* Closes our socket; server sees EOF */
}
