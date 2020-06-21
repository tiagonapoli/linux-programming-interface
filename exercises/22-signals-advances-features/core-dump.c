#include <stdlib.h>

int main() {
	int v[1000];

	for (int i = 0; i < 1000; i++) {
		v[i] = i;
	}

	abort();
}