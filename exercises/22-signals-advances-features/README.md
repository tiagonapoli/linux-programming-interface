# Chapter 22 - Signals: Advances Features

## Question 22-0

---

**Question:**

Generate and inspect a core dump.

**Answer:**

See `core-dump.c`

## Question 22-1

---

**Question:**

Section 22.2 noted that if a stopped process that has established a handler for and
blocked SIGCONT is later resumed as a consequence of receiving a SIGCONT , then the
handler is invoked only when SIGCONT is unblocked. Write a program to verify this.
Recall that a process can be stopped by typing the terminal suspend character
(usually Control-Z) and can be sent a SIGCONT signal using the command kill –CONT
(or implicitly, using the shell fg command).

**Answer:**

See `sigcont.c`.

- Run the binary.
- Send a SIGSTOP to it.
- Send a SIGCONT to it, note that this signal will be added to pendingSigs, as it's on the sigprocmask.
- Wait until the block on SIGCONT is removed - it will then execute the handler registered.


## Question 22-2

---

**Question:**

If both a realtime and a standard signal are pending for a process, SUSv3 leaves it
unspecified which is delivered first. Write a program that shows what Linux does in
this case. (Have the program set up a handler for all signals, block signals for a
period time so that you can send various signals to it, and then unblock all signals.)

**Answer:**

See `sig-order.c` - Realtime signals come first.

## Question 22-3

---

**Question:**

Section 22.10 stated that accepting signals using sigwaitinfo() is faster than the use
of a signal handler plus sigsuspend(). The program signals/sig_speed_sigsuspend.c ,
supplied in the source code distribution for this book, uses sigsuspend() to
alternately send signals back and forward between a parent and a child process.
Time the operation of this program to exchange one million signals between the
two processes. (The number of signals to exchange is provided as a command-line
argument to the program.) Create a modified version of the program that instead
uses sigwaitinfo(), and time that version. What is the speed difference between the
two programs?

**Answer:**

See `./benchmark/`.

| Command | Mean [s] | Min [s] | Max [s] | Relative |
|:---|---:|---:|---:|---:|
| `./sigsuspend.out 100000` | 1.513 ± 0.159 | 1.259 | 1.767 | 1.99 ± 0.22 |
| `./sigwaitinfo.out 100000` | 0.760 ± 0.023 | 0.718 | 0.798 | 1.00 |


## Question 22-4

---

**Question:**

Implement the System V functions sigset(), sighold(), sigrelse(), sigignore(), and
sigpause() using the POSIX signal API.

**Answer:**

[NOT_DONE]


