# Chapter 29 - Threads: introduction

## Question 29-1

---

**Question:**

What possible outcomes might there be if a thread executes the following code:

```c
pthread_join(pthread_self(), NULL);
```

Write a program to see what actually happens on Linux. If we have a variable, tid,
containing a thread ID, how can a thread prevent itself from making a call,
pthread_join(tid, NULL), that is equivalent to the above statement?

**Answer:**

See `self_join.c`. On Linux the call to `pthread_join` returns an error:

```
EDEADLK/EDEADLOCK Resource deadlock avoided
```

To avoid joining itself the thread can check if `tid` equals to `pthread_self()`.

```c
if (!pthread_equal(pthread_self(), tid)) {
    int s = pthread_join(tid, NULL);
    if (s != 0) {
        errExitEN(s, "pthread_join");
    }
}
```

## Question 29-2

---

**Question:**

Aside from the absence of error checking and various variable and structure
declarations, what is the problem with the following program?

```c
static void *threadFunc(void *arg) {
    struct someStruct *pbuf = (struct someStruct *) arg;
    /* Do some work with structure pointed to by 'pbuf' */
}

int main(int argc, char *argv[]) {
    struct someStruct buf;
    pthread_create(&thr, NULL, threadFunc, (void *) &buf);
    pthread_exit(NULL);
}
```

**Answer:**

After the main thread exists the variables allocated in main's stack
become undefined and accesses to variables that were previously there
will return undefined variables. The `buf` variable is allocated
in main's stack and used on `threadFunc` - after the `main` thread finishes
by calling `pthread_exit` other threads shouldn't access variables that
were previously on its stack.
