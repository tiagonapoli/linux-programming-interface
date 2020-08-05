#define _POSIX_C_SOURCE 200112L
#define _DEFAULT_SOURCE
#include "../../book-src/lib/tlpi_hdr.h"
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
#define SERVER_PORT "50000"
#define BACKLOG 5
#define REQ_MAX_LEN 100
#define RES_MAX_LEN 100

ssize_t readLine(int fd, void *buffer, size_t n) {
	ssize_t numRead;
	size_t totRead;
	char *buf;
	char ch;

	if (n <= 0 || buffer == NULL) {
		errno = EINVAL;
		return -1;
	}

	buf = buffer; /* No pointer arithmetic on "void *" */

	totRead = 0;
	for (;;) {
		numRead = read(fd, &ch, 1);

		if (numRead == -1) {
			if (errno == EINTR) /* Interrupted --> restart read() */
				continue;
			else
				return -1; /* Some other error */

		} else if (numRead == 0) { /* EOF */
			if (totRead == 0)        /* No bytes read; return 0 */
				return 0;
			else /* Some bytes read; add '\0' */
				break;

		} else {                 /* 'numRead' must be 1 if we get here */
			if (totRead < n - 1) { /* Discard > (n - 1) bytes */
				totRead++;
				*buf++ = ch;
			}

			if (ch == '\n') break;
		}
	}

	*buf = '\0';
	return totRead;
}

int storage[100];
int main() {
	struct sockaddr_storage claddr;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	char host[NI_MAXHOST], service[NI_MAXSERV];
	char str_addr[ADDRSTRLEN];
	char reqStr[REQ_MAX_LEN];
	char response[RES_MAX_LEN];

	if (signal(SIGPIPE, SIG_IGN) != 0) {
		errExit("signal");
	}

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(NULL, SERVER_PORT, &hints, &result) != 0) {
		errExit("getaddrinfo");
	}

	int sfd;
	int optval = 1;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (sfd <= 0) {
			continue;
		}

		if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
			errExit("setsockopt");
		}

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
			int err = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
			if (err < 0) {
				printf("getnameinfo: %s\n", gai_strerror(err));
				exit(EXIT_FAILURE);
			}

			printf("(%s, %s)\n", host, service);
			break;
		}

		close(sfd);
	}

	if (rp == NULL) {
		fatal("Couldn't bind socket to any address");
	}

	freeaddrinfo(result);

	if (listen(sfd, BACKLOG) < 0) {
		errExit("listen");
	}

	int addrlen, cfd;
	for (;;) {
		addrlen = sizeof(struct sockaddr_storage);
		cfd = accept(sfd, (struct sockaddr *)&claddr, &addrlen);
		if (cfd < 0) {
			errMsg("accept");
			continue;
		}

		if (getnameinfo((struct sockaddr *)&claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
			snprintf(str_addr, ADDRSTRLEN, "(%s, %s)", host, service);
		} else {
			snprintf(str_addr, ADDRSTRLEN, "(?UNKOWN?)");
		}

		printf("Connection from %s\n", str_addr);

		if (readLine(cfd, reqStr, REQ_MAX_LEN) <= 0) {
			errMsg("readLine");
			close(cfd);
			continue;
		}

		int k, v;
		char op;
		sscanf(reqStr, "%c %d %d", &op, &k, &v);
		if (k >= 100) {
			close(cfd);
			continue;
		}

		if (op == 'S') {
			printf("Set [%d]=%d\n", k, v);
			storage[k] = v;
		}

		sprintf(response, "[%d]=%d\n", k, storage[k]);
		int resLen = strlen(response);
		if (write(cfd, response, resLen) != resLen) {
			errMsg("write");
			close(cfd);
			continue;
		}

		if (close(cfd) == -1) {
			errMsg("close");
		}
	}
}
