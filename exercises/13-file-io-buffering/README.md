# Chapter 13: File I/O Buffering

## Question 13-1

---

Using the time built-in command of the shell, try timing the operation of the program in Listing 4-1 ( copy.c ) on your system.

**Question:**

a) Experiment with different file and buffer sizes. You can set the buffer size using the –DBUF_SIZE=nbytes option when compiling the program.

b) Modify the open() system call to include the O_SYNC flag. How much difference does this make to the speed for various buffer sizes?

c) Try performing these timing tests on a range of file systems (e.g., ext3, XFS, Btrfs, and JFS). Are the results similar? Are the trends the same when going from small to large buffer sizes?

**Answer:**

a)

4 MiB file size:
| Buffer Size [b] | Mean [s] | Min [s] | Max [s] | Relative |
|:---|---:|---:|---:|---:|
| 1 | 10.848 ± 2.274 | 8.095 | 13.874 | 3606.10 ± 824.28 |
| 8 | 1.198 ± 0.215 | 0.987 | 1.429 | 398.07 ± 80.29 |
| 64 | 0.127 ± 0.015 | 0.113 | 0.161 | 42.37 ± 6.41 |
| 512 | 0.018 ± 0.003 | 0.016 | 0.026 | 6.09 ± 1.06 |
| 1024 | 0.010 ± 0.002 | 0.009 | 0.017 | 3.41 ± 0.62 |
| 2048 | 0.006 ± 0.001 | 0.005 | 0.012 | 2.00 ± 0.38 |
| 4096 | 0.004 ± 0.000 | 0.003 | 0.007 | 1.30 ± 0.18 |
| 8192 | 0.003 ± 0.000 | 0.003 | 0.005 | 1.16 ± 0.14 |
| 16384 | 0.003 ± 0.000 | 0.003 | 0.005 | 1.08 ± 0.12 |
| 32768 | 0.003 ± 0.000 | 0.002 | 0.004 | 1.00 |

b)
4 MiB file size: The times were too long to wait

1 KiB with O_SYNC:
| Buffer Size [b] | Mean [s] | Min [s] | Max [s] | Relative |
|:---|---:|---:|---:|---:|
| 1 | 9.662 ± 4.618 | 2.758 | 13.739 | 1767.32 ± 923.92 |
| 8 | 1.690 ± 0.469 | 1.491 | 2.955 | 309.20 ± 107.95 |
| 64 | 0.106 ± 0.077 | 0.035 | 0.190 | 19.35 ± 14.61 |
| 512 | 0.008 ± 0.001 | 0.005 | 0.019 | 1.42 ± 0.39 |
| 1024 | 0.005 ± 0.001 | 0.003 | 0.016 | 1.00 |
| 2048 | 0.015 ± 0.002 | 0.006 | 0.033 | 2.78 ± 0.67 |
| 4096 | 0.015 ± 0.002 | 0.013 | 0.034 | 2.78 ± 0.67 |
| 8192 | 0.015 ± 0.002 | 0.014 | 0.034 | 2.79 ± 0.68 |
| 16384 | 0.015 ± 0.002 | 0.014 | 0.035 | 2.80 ± 0.69 |
| 32768 | 0.015 ± 0.002 | 0.014 | 0.034 | 2.79 ± 0.67 |

1 KiB no O_SYNC:
| Buffer Size [b] | Mean [ms] | Min [ms] | Max [ms] | Relative |
|:---|---:|---:|---:|---:|
| 1 | 1.8 ± 0.3 | 1.5 | 5.8 | 4.74 ± 2.59 |
| 8 | 0.6 ± 0.2 | 0.4 | 2.2 | 1.49 ± 0.89 |
| 64 | 0.4 ± 0.2 | 0.2 | 4.0 | 1.00 |
| 512 | 0.4 ± 0.2 | 0.2 | 3.8 | 1.15 ± 0.79 |
| 1024 | 0.5 ± 0.7 | 0.2 | 19.1 | 1.32 ± 1.93 |
| 2048 | 0.6 ± 0.2 | 0.2 | 4.3 | 1.45 ± 0.98 |
| 4096 | 0.7 ± 0.5 | 0.3 | 6.5 | 1.84 ± 1.70 |
| 8192 | 0.5 ± 0.2 | 0.2 | 4.7 | 1.37 ± 0.95 |
| 16384 | 0.7 ± 0.7 | 0.2 | 22.7 | 1.72 ± 2.14 |
| 32768 | 0.6 ± 0.3 | 0.2 | 5.3 | 1.63 ± 1.14 |

## Question 13-2

---

**Question:**

Time the operation of the `filebuff/write_bytes.c` program (provided in the source code distribution for this book) for various buffer sizes and file systems.

**Answer:**

Writing 4MiB file (ext4):
| Buffer Size [B] | Mean [s] | Min [s] | Max [s] | Relative |
|:---|---:|---:|---:|---:|
| 1 | 8.460 ± 1.767 | 6.614 | 10.434 | 1531.98 ± 341.99 |
| 8 | 1.117 ± 0.187 | 0.869 | 1.307 | 202.27 ± 37.51 |
| 64 | 0.137 ± 0.019 | 0.118 | 0.166 | 24.84 ± 3.95 |
| 512 | 0.020 ± 0.003 | 0.016 | 0.034 | 3.56 ± 0.69 |
| 1024 | 0.013 ± 0.003 | 0.010 | 0.023 | 2.44 ± 0.52 |
| 2048 | 0.009 ± 0.002 | 0.006 | 0.017 | 1.63 ± 0.31 |
| 4096 | 0.007 ± 0.001 | 0.006 | 0.010 | 1.18 ± 0.18 |
| 8192 | 0.006 ± 0.001 | 0.005 | 0.010 | 1.09 ± 0.15 |
| 16384 | 0.006 ± 0.000 | 0.005 | 0.008 | 1.00 |
| 32768 | 0.006 ± 0.001 | 0.005 | 0.009 | 1.03 ± 0.14 |

Writing 4MiB file (tmpfs):
| Buffer Size [B] | Mean [s] | Min [s] | Max [s] | Relative |
|:---|---:|---:|---:|---:|
| 1 | 3.008 ± 0.150 | 2.831 | 3.298 | 1060.60 ± 109.24 |
| 8 | 0.390 ± 0.018 | 0.356 | 0.418 | 137.59 ± 13.99 |
| 64 | 0.066 ± 0.008 | 0.054 | 0.076 | 23.10 ± 3.59 |
| 512 | 0.012 ± 0.001 | 0.008 | 0.015 | 4.33 ± 0.49 |
| 1024 | 0.008 ± 0.000 | 0.007 | 0.012 | 2.65 ± 0.29 |
| 2048 | 0.005 ± 0.000 | 0.004 | 0.006 | 1.73 ± 0.20 |
| 4096 | 0.004 ± 0.000 | 0.003 | 0.005 | 1.26 ± 0.17 |
| 8192 | 0.003 ± 0.000 | 0.003 | 0.005 | 1.11 ± 0.15 |
| 16384 | 0.003 ± 0.000 | 0.002 | 0.005 | 1.04 ± 0.18 |
| 32768 | 0.003 ± 0.000 | 0.002 | 0.004 | 1.00 |

## Question 13-3

---

**Question:**

What is the effect of the following statements?

```
fflush(fp);
fsync(fileno(fp));
```

**Answer:**

`fflush(fp)`: Flush stdio library buffer associated with `fp` to kernel (using `write` syscall).

`fsync(fileno(fp))`: Flush to disk (or disk cache) the kernel buffers from fp's underlying file descriptor.

## Question 13-4

---

**Question:**

Explain why the output of the following code differs depending on whether standard output is redirected to a terminal or to a disk file.
```
printf("If I had more time, \n");
write(STDOUT_FILENO, "I would have written you a shorter letter.\n", 43);
```

**Answer:**

From [File I/O Essentials](https://static.packt-cdn.com/downloads/File_IO_Essentials.pdf):

```
Okay, back to the stdio APIs and their buffering. Practically, this is how they're set up to work on the system by default (so that maximum performance is achieved whenever applicable):

- If the stream refers to a regular file (a file on the filesystem), I/O performed on the stream will be fully (block) buffered (a stdio I/O buffer of size 1 page will cache it).

- If the stream refers to a terminal device – a common example being stdout – I/O performed on the stream will be line buffered (a stdio I/O buffer of size 128 bytes will cache it).

- If the stream is stderr, I/O performed on the stream will always be unbuffered (it will not be cached at all at the library layer and writes will proceed immediately via write(2) to the OS layers). This is usually desirable for error output.
```

`stdio` library implements user space buffering - depending on the stream type (file, terminal, etc) the default buffer type will be different, thus causing the behavior on this example.

Using `write` will send data directly to a kernel space buffer, while `printf` will send data to a user space buffer first - this user space buffer will, at some point, be flushed to kernel space buffer.

## Question 13-5

---

**Question:**

The command tail [ –n num ] file prints the last num lines (ten by default) of the named file. Implement this command using I/O system calls (lseek(), read(), write(), and so on). Keep in mind the buffering issues described in this chapter, in order to make the implementation efficient.

**Answer:**

See `tail.c`

