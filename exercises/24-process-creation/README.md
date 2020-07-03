# Chapter 24 - Process Creation

## Question 24-1

---

**Question:**

After a program executes the following series of fork() calls, how many new
processes will result (assuming that none of the calls fails)?

```
fork();
fork();
fork();
```

**Answer:**

- The first `fork()` will have a father process and a child process.
- The second `fork()` will be executed by the 2 existent processes, resulting in 4 processes in total.
- The third `fork()` will be executed by the 4 existent processes, resulting in 8 processes in total.

## Question 24-2

---

**Question:**

Write a program to demonstrate that after a vfork(), the child process can close a
file descriptor (e.g., descriptor 0) without affecting the corresponding file descriptor
in the parent.

**Answer:**

See `vfork.c`.

## Question 24-3

---

**Question:**

Assuming that we can modify the program source code, how could we get a core
dump of a process at a given moment in time, while letting the process continue
execution?

**Answer:**

## Question 24-4

WOW! Doing a fork(), which will copy on write the parent's stack, data, heap and
text segments then signaling the child to abort:

```ts
if(fork() == 0) {
    raise(SIGABRT)
}
```

**Question:**

Experiment with the program in Listing 24-5 ( fork_whos_on_first.c ) on other UNIX
implementations to determine how these implementations schedule the parent
and child processes after a fork().

**Answer:**

In Linux kernel at 5.4.0-39-generic the parent runs first.

## Question 24-5

---

**Question:**

Suppose that in the program in Listing 24-6, the child process also needed to wait
on the parent to complete some actions. What changes to the program would be
required in order to enforce this?

**Answer:**

We'll have to implement on the child a similar signaling logic that is implemented on the father.

The father would wait the first signal from the child, and the child, just before signaling the father,
would have to block the signal we are using to manage the processes and then signal the father and
sigwaitinfo for for the sync signal. Then the father would receive the signal and continue work, and
finally would signal tha child, which then will be able to continue its work.



