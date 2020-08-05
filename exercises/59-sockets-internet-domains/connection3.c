#define _DEFAULT_SOURCE
#include "../../book-src/lib/tlpi_hdr.h"
#include "../../book-src/lib/inet_sockets.h"
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
	int sfd1, sfd2, sfd3;
	socklen_t sfd1_addr_len, sfd2_addr_len, sfd3_addr_len;

	sfd1 = inetBind("50000", SOCK_DGRAM, &sfd1_addr_len);
	if (sfd1 < 0) {
		errExit("inetBind");
	}

	sfd2 = inetBind("50001", SOCK_DGRAM, &sfd2_addr_len);
	if (sfd2 < 0) {
		errExit("inetBind");
	}

	sfd3 = inetBind("50002", SOCK_DGRAM, &sfd3_addr_len);
	if (sfd3 < 0) {
		errExit("inetBind");
	}

	void *addr1 = malloc(sfd1_addr_len);
	if (getsockname(sfd1, (struct sockaddr *)addr1, &sfd1_addr_len) < 0) {
		errExit("getsockname");
	}

	void *addr2 = malloc(sfd2_addr_len);
	if (getsockname(sfd2, (struct sockaddr *)addr2, &sfd2_addr_len) < 0) {
		errExit("getsockname");
	}

	if (connect(sfd2, addr1, sfd1_addr_len) < 0) {
		errExit("connect");
	}

	if (connect(sfd1, addr2, sfd2_addr_len) < 0) {
		errExit("connect");
	}

	if (connect(sfd3, addr2, sfd2_addr_len) < 0) {
		errExit("connect");
	}

	char buf[50];

	printf("Writing on sfd2 -> sfd1\n");
	write(sfd2, "a\0", 2);
	int numbytes = read(sfd1, buf, 50);
	printf("Read on sfd1 [%d] [%s]\n\n", numbytes, buf);

	printf("Writing on sfd1 -> sfd2\n");
	write(sfd1, "b\0", 2);
	numbytes = read(sfd2, buf, 50);
	printf("Read on sfd2 [%d] [%s]\n\n", numbytes, buf);

	printf("Writing on sfd3 -> sfd2\n");
	write(sfd3, "b\0", 2);
	printf("Will read on sfd2\n");
	numbytes = read(sfd2, buf, 50);
	printf("Read on sfd2 [%d] [%s]\n\n", numbytes, buf);
}