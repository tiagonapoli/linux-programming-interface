#include <setjmp.h>
#include "../../book-src/lib/tlpi_hdr.h"

static jmp_buf env;

static void f1() {
	volatile int a, b;

	a = 111;
	b = 222;

	switch (setjmp(env)) {
	case 0:
		a = 333;
		b = 444;
		return;
	case 1:
		printf("Returned to f1: %d %d\n", a, b);
		return;
	}
}

int main(int argc, char *argv[]) {
	f1();
	longjmp(env, 1);
	printf("End program");
}
