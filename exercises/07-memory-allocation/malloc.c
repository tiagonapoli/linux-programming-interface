#define _DEFAULT_SOURCE
#include "../../book-src/lib/tlpi_hdr.h"
#include <unistd.h>
#include <assert.h>

size_t MB = 1024 * 1024;

struct memBlockMeta {
	size_t size;
	struct memBlockMeta* next;
	int occupied;
};

#define MEM_BLOCK_META_SIZE sizeof(struct memBlockMeta)

struct memBlockMeta* memBlockList = NULL;

// Implements first-fit
struct memBlockMeta* findFreeBlock(size_t sz, struct memBlockMeta** last) {
	for (struct memBlockMeta* p = memBlockList; p != NULL; p = p->next) {
		*last = p;
		if (p->occupied == 0 && sz <= p->size) {
			return p;
		}
	}

	return NULL;
}

// Allocates memory on heap
struct memBlockMeta* requestSpace(size_t sz) {
	struct memBlockMeta* p = (struct memBlockMeta*)sbrk(sz + MEM_BLOCK_META_SIZE);
	if (p == (void*)-1) return NULL;
	p->occupied = 0;
	p->next = NULL;
	p->size = sz;
	return p;
}

void printMemBlockList() {
	printf("--- MemBlockList ---\n");
	for (struct memBlockMeta* p = memBlockList; p != NULL; p = p->next) {
		printf("Size: %lu   Occupied: %d\n", p->size, p->occupied);
	}
	printf("--------------------\n\n");
}

void* myMalloc(size_t sz) {
	struct memBlockMeta* p;
	struct memBlockMeta* last;

	if (sz == 0) return NULL;

	p = findFreeBlock(sz, &last);
	if (p == NULL) {
		if (memBlockList != NULL) {
			p = requestSpace(sz);
			last->next = p;
		} else {
			p = requestSpace(sz);
			memBlockList = p;
		}
	}

	if (p == NULL) return p;
	p->occupied = 1;
	return p + 1;
}

void* myFree(void* ptr) {
	struct memBlockMeta* p = (struct memBlockMeta*)ptr - 1;
	assert(p != NULL);
	assert(p->occupied == 1);
	p->occupied = 0;
	printf("freeCall - [%lu][%d]\n", p->size, p->occupied);
}

void printBrkAddress() { printf("Program break address: %10p\n", sbrk(0)); }

void aloc(size_t sz) {
	long long* arr = (long long*)myMalloc(sz * sizeof(long long));
	printMemBlockList();
	printBrkAddress();

	for (long long i = 0; i < sz; i++) {
		arr[i] = i;
		printf("[%lld]: [%lld]\n", i, arr[i]);
	}

	// myFree(arr);
	printMemBlockList();
	printf("\n\n");
}

int main() {
	long long* arr;
	size_t sz = 30;

	printf("Initial program break address: %10p\n", sbrk(0));
	printf("Initial program break address: %10p\n", sbrk(0));

	aloc(10);
	aloc(20);
	aloc(10);

	printf("Program break address: %10p\n", sbrk(0));
}