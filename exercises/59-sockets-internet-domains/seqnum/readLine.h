#ifndef READLINE_H
#define READLINE_H

#include <unistd.h>
#define READLINE_MAX_BUF 3

struct readline_buf {
	int fd;
	char buf[READLINE_MAX_BUF];
	int next;
	ssize_t len;
};

void readLineBufInit(int fd, struct readline_buf *rlbuf);

ssize_t readLineBuf(struct readline_buf *rlbuf, char *buffer, size_t n);

#endif