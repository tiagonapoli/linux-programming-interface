#include "readLine.h"
#include <errno.h>
#include <stdio.h>

void readLineBufInit(int fd, struct readline_buf* rlbuf) {
	rlbuf->len = 0;
	rlbuf->next = 0;
	rlbuf->fd = fd;
}

ssize_t readLineBuf(struct readline_buf* rlbuf, char* buffer, size_t n) {
	ssize_t bufpos = 0;

	if (n <= 0 || buffer == NULL) {
		errno = EINVAL;
		return -1;
	}

	for (;;) {
		for (int i = rlbuf->next; i < rlbuf->len; i++) {
			if (bufpos < n - 1) {
				buffer[bufpos++] = rlbuf->buf[i];
			}

			if (rlbuf->buf[i] == '\n') {
				rlbuf->next = i + 1;
				goto finished;
			}
		}

		rlbuf->next = 0;
		rlbuf->len = read(rlbuf->fd, rlbuf->buf, READLINE_MAX_BUF);
		if (rlbuf->len < 0) {
			return -1;
		}

		if (rlbuf->len == 0) { /* EOF */
			goto finished;
		}
	}

finished:;
	buffer[bufpos] = '\0';

	return bufpos;
}