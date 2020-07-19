# Chapter 33 - Threads: Further details

## Question 33-1

---

**Question:**

Write a program to demonstrate that different threads in the same process can have
different sets of pending signals, as returned by sigpending(). You can do this by using
pthread_kill() to send different signals to two different threads that have blocked
these signals, and then have each of the threads call sigpending() and display
information about pending signals. (You may find the functions in Listing 20-4
useful.)

**Answer:**

See `pending-signals.c`.

## Question 33-2

---

**Question:**

Suppose that a thread creates a child using fork(). When the child terminates, is it
guaranteed that the resulting SIGCHLD signal will be delivered to the thread that
called fork() (as opposed to some other thread in the process)?

**Answer:**

No, it's not guaranteed. See `pthread_fork.c`.

> Signal dispositions are process-wide; all threads in a process share the same dis-
position for each signal. If one thread uses sigaction() to establish a handler for,
say, SIGINT , then that handler may be invoked from any thread to which the
SIGINT is delivered. Similarly, if one thread sets the disposition of a signal to
ignore, then that signal is ignored by all threads.