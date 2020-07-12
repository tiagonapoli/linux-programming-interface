# Chapter 28 - Process creation and program execution in more detail

## Question 28-1

---

**Question:**

Write a program to see how fast the fork() and vfork() system calls are on your
system. Each child process should immediately exit, and the parent should wait()
on each child before creating the next. Compare the relative differences for these
two system calls with those of Table 28-3. The shell built-in command time can be
used to measure the execution time of a program.

**Answer:**

Creating 5\*10^3 processses:

| Command      |      Mean [s] | Min [s] | Max [s] |    Relative |
| :----------- | ------------: | ------: | ------: | ----------: |
| fork 1MB vm  | 1.710 ± 0.086 |   1.571 |   1.874 | 6.84 ± 0.37 |
| fork 2MB vm  | 1.835 ± 0.070 |   1.755 |   1.957 | 7.34 ± 0.32 |
| fork 4MB vm  | 2.085 ± 0.066 |   2.012 |   2.227 | 8.34 ± 0.31 |
| vfork 1MB vm | 0.280 ± 0.054 |   0.243 |   0.383 | 1.12 ± 0.22 |
| vfork 2MB vm | 0.262 ± 0.020 |   0.245 |   0.317 | 1.05 ± 0.08 |
| vfork 4MB vm | 0.250 ± 0.005 |   0.243 |   0.260 |        1.00 |

The differences on vfork tests were probably due to some kind of caching (tried different
consecutive test orders and the last one was the fastest, independently from the VM allocated).
