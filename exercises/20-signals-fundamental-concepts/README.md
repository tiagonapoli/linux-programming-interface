# Chapter 20 - Signals: Fundamental Concepts

## Question 20-1

---

**Question:**

As noted in Section 20.3, sigaction() is more portable than signal() for establishing a
signal handler. Replace the use of signal() by sigaction() in the program in Listing 20-7
(sig_receiver.c).

**Answer:**

See `sigaction.c`

## Question 20-2

---

**Question:**

Write a program that shows that when the disposition of a pending signal is
changed to be SIG_IGN , the program never sees (catches) the signal.

**Answer:**

See `sig_ign.c`.

## Question 20-3

---

**Question:**

Write programs that verify the effect of the SA_RESETHAND and SA_NODEFER flags when
establishing a signal handler with sigaction().

**Answer:**

`SA_NODEFER`: see `sigaction_nodefer.c` - when `SA_NODEFER` flag is passed to sigaction the signal being handled is not added to the sigprocmask, so if the same signal is sent again to the process, it can interrupt the execution of the current signal handler and execute the handler for the signal received - the execution is continued after the topmost execution finishes (just like a stack). To see it in action press many Ctrl+C (SIGINT) while executin `sigaction_nodefer.c`'s binary.

`SA_RESETHAND`: see `sigaction_resethand.c`. `SA_RESETHAND` resets the signal handler to the default after the first signal is received. In the example created the first `SIGINT` executes our handler, then because of `SA_RESETHAND` its handler is reset to the default (close the program). After our handler finished, if we send another `SIGINT` our handler is not executed and the program closes.

## Question 20-4

---

**Question:**

Implement siginterrupt() using sigaction().

**Answer:**

See `siginterrupt.c`.
