# Chapter 23 - Timers and Sleeping

## Question 23-1

---

**Question:**

Although alarm() is implemented as a system call within the Linux kernel, this is
redundant. Implement alarm() using setitimer().

**Answer:**

See `alarm.c`.

## Question 23-2

---

**Question:**

Try running the program in Listing 23-3 (t_nanosleep.c) in the background with a
60-second sleep interval, while using the following command to send as many
SIGINT signals as possible to the background process:

```
$ while true; do kill -INT pid; done
```

You should observe that the program sleeps rather longer than expected. Replace
the use of nanosleep() with the use of clock_gettime() (use a CLOCK_REALTIME clock) and
clock_nanosleep() with the TIMER_ABSTIME flag. (This exercise requires Linux 2.6.)
Repeat the test with the modified program and explain the difference.

**Answer:**

> Although nanosleep() allows nanosecond precision when specifying the sleep interval,
> the accuracy of the sleep interval is limited to the granularity of the software clock
> (Section 10.6). If we specify an interval that is not a multiple of the software clock,
> then the interval is rounded up.

The behavior explained above explains why we have a sleeping time bigger than expected.
When the nanosleep call is interrupted by a signal and we reeschedule it with the remaining
sleep time it may be rounded up so we get extra sleep. With a high frequency signal this
rounding may happend very frequently and we end up with a lot of extra sleep.

Using TIMER_ABSTIME eliminates this repetitive rounding and we get the correct sleep time.

From the text bookÇ

> This rounding behavior means that if signals are received at a high rate, then there
> is a problem with the approach employed in the program in Listing 23-3. The problem
> is that each restart of nanosleep() will be subject to rounding errors, since the returned
> remain time is unlikely to be an exact multiple of the granularity of the software
> clock. Consequently, each restarted nanosleep() will sleep longer than the value
> returned in remain by the previous call. In the case of an extremely high rate of signal
> delivery (i.e., as or more frequent than the software clock granularity), the process
> may never be able to complete its sleep.

See `clock_nanosleep.c`.

## Question 23-3

---

**Question:**

Write a program to show that if the evp argument to timer_create() is specified as
NULL, then this is equivalent to specifying evp as a pointer to a sigevent structure with
sigev_notify set to SIGEV_SIGNAL, sigev_signo set to SIGALRM, and si_value.sival_int set to
the timer ID.

**Answer:**

See `timer-create.c`.

## Question 23-4

---

**Question:**

Modify the program in Listing 23-5 (ptmr_sigev_signal.c) to use sigwaitinfo() instead
of a signal handler.

**Answer:**

See `ptmr_sigev_signal.c`.
