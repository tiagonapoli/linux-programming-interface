/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 59-5 */

/* is_seqnum.h

   Header file for is_seqnum_sv.c and is_seqnum_cl.c.
*/
#ifndef MY_SEQNUM_H
#define MY_SEQNUM_H

#define _POSIX_C_SOURCE 200112L
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include "../../../book-src/lib/tlpi_hdr.h"
#include "../../../book-src/lib/read_line.h"
#include "../../../book-src/lib/inet_sockets.h"

#define PORT_NUM "50000" /* Port number for server */

#define INT_LEN 30

#endif