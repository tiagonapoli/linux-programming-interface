#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE

#include "../../book-src/lib/tlpi_hdr.h"
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <limits.h>

int myinitgroups(const char *user, gid_t group) {
	gid_t *groups;
	size_t gCnt = 0;

	groups = malloc(sizeof(gid_t) * NGROUPS_MAX);
	if (groups == NULL) errExit("malloc");
	groups[gCnt++] = group;

	struct passwd* userInfo = getpwnam(user);
	if(userInfo == NULL) errExit("getpwnam");
	groups[gCnt++] = userInfo->pw_gid;

	struct group *grP;
	while ((grP = getgrent()) != NULL) {
        for (int i = 0; grP->gr_mem[i] != NULL; i++) {
			if (strcmp(grP->gr_mem[i], user) == 0) {
				groups[gCnt++] = grP->gr_gid;
				break;
			}
		}
	}

	endgrent();

	return setgroups(gCnt, groups);
}

void printGroups(const char *msg) {
	gid_t *groups;
	size_t groupCnt = getgroups(0, groups);
	groups = (gid_t *)malloc(sizeof(gid_t) * groupCnt);
	getgroups(groupCnt, groups);
	printf("%s\n", msg);
	for (int i = 0; i < groupCnt; i++) {
		printf("%u ", groups[i]);
	}
	printf("\n\n");
}

int main() {
	printGroups("Groups before");
	if(myinitgroups("tiagonapoli", 10) == -1) {
        errExit("setgroups");
    };
    
	printGroups("Groups after");
}