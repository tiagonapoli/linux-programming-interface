#define _DEFAULT_SOURCE
#include "../../book-src/lib/tlpi_hdr.h"
#include "../../book-src/lib/inet_sockets.h"
#include <netdb.h>

char *getAddrStr(const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrLen) {
	char host[NI_MAXHOST], service[NI_MAXSERV];

	int ret = getnameinfo(addr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
	if (ret == 0) {
		snprintf(addrStr, addrStrLen, "(%s, %s)", host, service);
	} else {
		printf("getnameinfo: %s\n", gai_strerror(ret));
		snprintf(addrStr, addrStrLen, "(?UNKNOWN?)");
	}

	return addrStr;
}

int main(int argc, char *argv[]) {
	socklen_t len; /* Size of socket address buffer */
	void *addr;    /* Buffer for socket address */
	char addrStr[IS_ADDR_STR_LEN];

	int sfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen(sfd, 5) != 0) {
		errExit("listen");
	}

	len = sizeof(struct sockaddr_in6);
	addr = malloc(len);
	if (addr == NULL) errExit("malloc");

	if (getsockname(sfd, addr, &len) == -1) errExit("getsockname");
	printf("getsockname(sfd):   %s\n", getAddrStr(addr, len, addrStr, IS_ADDR_STR_LEN));

	sleep(30); /* Give us time to run netstat(8) */
	exit(EXIT_SUCCESS);
}
