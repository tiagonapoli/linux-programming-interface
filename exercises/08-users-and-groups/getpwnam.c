#define _XOPEN_SOURCE 1000
#include "../../book-src/lib/tlpi_hdr.h"
#include <unistd.h>
#include <assert.h>
#include <pwd.h>

struct passwd* mygetpwnam(const char* name) {
	struct passwd* p;
	while ((p = getpwent()) != NULL) {
		if (strcmp(name, p->pw_name) == 0) {
			printf("%s %d\n", p->pw_name, (int)p->pw_uid);
			endpwent();

			return p;
		}
	}
	endpwent();
}

int main() { mygetpwnam("tiagonapoli"); }