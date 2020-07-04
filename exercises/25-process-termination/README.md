# Chapter 25 - Process Termination

## Question 25-1

---

**Question:**

If a child process makes the call exit(–1), what exit status (as returned by WEXITSTATUS() )
will be seen by the parent?

**Answer:**

See `negative-exit-status.c`.

The value seen by the parent is 255. This is probably because o the 2's complement representation.
The 8 least significant bits represent the exit status of the child - for -1 we have the 8 least
significant bits to be `2**8-1 = 255`. If we do the test for -2 for example we'll have `2**8-2=254`
exit status.
