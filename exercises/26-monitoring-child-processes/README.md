# Chapter 26 - Monitoring child processes

## Question 26-1

---

**Question:**

Write a program to verify that when a child’s parent terminates, a call to getppid()
returns 1 (the process ID of init).

**Answer:**

See `dead-parent-process.c`.

In my system - Ubuntu - the orphan processes are reassigned to a `systemd` process.

POSIX says the following on orphaned processes:

> The parent process ID of all of the existing child processes and zombie processes of the calling process
> shall be set to the process ID of an implementation-defined system process. That is, these processes shall
> be inherited by a special system process.

https://pubs.opengroup.org/onlinepubs/9699919799/functions/_Exit.html

## Question 26-2

---

**Question:**

Suppose that we have three processes related as grandparent, parent, and child,
and that the grandparent doesn’t immediately perform a wait() after the parent
exits, so that the parent becomes a zombie. When do you expect the grandchild to
be adopted by init (so that getppid() in the grandchild returns 1): after the parent
terminates or after the grandparent does a wait()? Write a program to verify your
answer.

**Answer:**

See `three-generations-processes.c`

After the second generation terminates the third generation is already adopted by init.

## Question 26-3

---

**Question:**

Replace the use of waitpid() with waitid() in Listing 26-3 ( child_status.c ). The call to
printWaitStatus() will need to be replaced by code that prints relevant fields from
the siginfo_t structure returned by waitid().

**Answer:**

See `child_status.c`.

## Question 26-4

---

**Question:**

Listing 26-4 ( make_zombie.c ) uses a call to sleep() to allow the child process a chance
to execute and terminate before the parent executes system(). This approach
produces a theoretical race condition. Modify the program to eliminate the race
condition by using signals to synchronize the parent and child.

**Answer:**

I had a doubt in this question - I implemented a solution in which the child sends a signal
to the parent announcing that the child executed, so in theory it could run `_exit` and terminate. 
The child exits right after sending the signal to its parent, but I think this is not enough: the child
could be suspended after sending the signal to its parent and before exiting completely. We could
use `atexit` on the child, but I think this wouldn't solve this race condition either.

To solve this I used `waitid` with the `WNOWAIT` flag.
