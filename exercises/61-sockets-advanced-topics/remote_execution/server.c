#define _DEFAULT_SOURCE
#include "../../../book-src/lib/tlpi_hdr.h"
#include "../../../book-src/lib/inet_sockets.h"
#include "../../../book-src/lib/read_line.h"
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#define BACKLOG 5
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

void handleConn(int connFd) {
	char cmd[100];
    
    close(STDIN_FILENO);
	if (signal(SIGCHLD, SIG_DFL) < 0) {
		errExit("signal");
	}

	int numBytes = readLine(connFd, cmd, 100);
	if (numBytes < 0) {
		if (shutdown(connFd, SHUT_RDWR) < 0) {
			errMsg("shutdown");
		}
		errExit("readLine");
	}

    cmd[numBytes-1] = '\0';
    printf("Run command [%d][%s] from child process\n", numBytes, cmd);
    dup2(connFd, STDOUT_FILENO);
    dup2(connFd, STDERR_FILENO);

	execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);
	_exit(127);
}

int main() {
	int sfd, connFd;
	socklen_t addrlen, claddrlen;
	struct sockaddr_storage claddr;
	char addrStr[ADDRSTRLEN];

	if (signal(SIGCHLD, SIG_IGN) < 0) {
		errExit("signal");
	}

	sfd = inetListen("50000", BACKLOG, &addrlen);
	if (sfd < 0) {
		errExit("inetListen");
	}

	int flags = fcntl(sfd, F_GETFD);
	if (flags < 0) {
		errExit("fcntl");
	}

	flags |= FD_CLOEXEC;
	if (fcntl(sfd, F_SETFD, flags) < 0) {
		errExit("fcntl");
	}

	for (;;) {
		int connFd = accept(sfd, (struct sockaddr *)&claddr, &claddrlen);
		if (connFd < 0) {
			errMsg("accept");
			continue;
		}

		printf("Connection from %s\n", inetAddressStr((struct sockaddr *)&claddr, claddrlen, addrStr, ADDRSTRLEN));

		pid_t childPid;
		switch (childPid = fork()) {
		case -1:
			errMsg("fork");
			continue;
		case 0:
			handleConn(connFd);
			exit(EXIT_SUCCESS);
		default:
			close(connFd);
		}
	}
}