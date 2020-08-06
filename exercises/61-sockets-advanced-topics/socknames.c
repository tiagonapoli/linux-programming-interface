/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 61-3 */

/* socknames.c

   Demonstrate the use of getsockname() and getpeername() to retrieve the local
   and peer socket addresses.
*/
#include "../../book-src/lib/tlpi_hdr.h"
#include "../../book-src/lib/inet_sockets.h"

int main(int argc, char *argv[]) {
	int listenFd, acceptFd1, acceptFd2, connFd1, connFd2;
	socklen_t len; /* Size of socket address buffer */
	void *addr;    /* Buffer for socket address */
	char addrStr[IS_ADDR_STR_LEN];

	if (argc != 2 || strcmp(argv[1], "--help") == 0) usageErr("%s service\n", argv[0]);

	/* Create listening socket, obtain size of address structure */

	listenFd = inetListen(argv[1], 5, &len);
	if (listenFd == -1) errExit("inetListen");

	connFd1 = inetConnect(NULL, argv[1], SOCK_STREAM);
	if (connFd1 == -1) errExit("inetConnect");

	connFd2 = inetConnect(NULL, argv[1], SOCK_STREAM);
	if (connFd2 == -1) errExit("inetConnect");

	acceptFd1 = accept(listenFd, NULL, NULL);
	if (acceptFd1 == -1) errExit("accept");

	acceptFd2 = accept(listenFd, NULL, NULL);
	if (acceptFd2 == -1) errExit("accept");

	addr = malloc(len);
	if (addr == NULL) errExit("malloc");

	if (getsockname(connFd1, addr, &len) == -1) errExit("getsockname");
	printf("getsockname(connFd1):   %s\n", inetAddressStr(addr, len, addrStr, IS_ADDR_STR_LEN));

	if (getsockname(connFd2, addr, &len) == -1) errExit("getsockname");
	printf("getsockname(connFd2):   %s\n", inetAddressStr(addr, len, addrStr, IS_ADDR_STR_LEN));

	if (getsockname(acceptFd1, addr, &len) == -1) errExit("getsockname");
	printf("getsockname(acceptFd1): %s\n", inetAddressStr(addr, len, addrStr, IS_ADDR_STR_LEN));

	if (getsockname(acceptFd2, addr, &len) == -1) errExit("getsockname");
	printf("getsockname(acceptFd2): %s\n", inetAddressStr(addr, len, addrStr, IS_ADDR_STR_LEN));

	sleep(30); /* Give us time to run netstat(8) */
	exit(EXIT_SUCCESS);
}
