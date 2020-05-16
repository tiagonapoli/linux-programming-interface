# Chapter 14: File Systems

## Question 14-1
------------

**Question:**

Write a program that measures the time required to create and then remove a large number of 1-byte files from a single directory. The program should create files with names of the form xNNNNNN , where NNNNNN is replaced by a random six-digit number. The files should be created in the random order in which their names are generated, and then deleted in increasing numerical order (i.e., an order that is different from that in which they were created). The number of files (NF) and the directory in which they are to be created should be specifiable on the command line. Measure the times required for different values of NF (e.g., in the range from 1000 to 20,000) and for different file systems (e.g., ext2, ext3, and XFS). What patterns do you observe on each file system as NF increases? How do the various file systems compare? Do the results change if the files are created in increasing numerical order ( x000001 , x000001 , x0000002 , and so on) and then deleted in the same order? If so, what do you think the reason(s) might be? Again, do the results vary across file-system types?

**Answer:**

See `bench-fs.sh` and `file-io.c`.

Tests on ext4:

| Command | Mean [ms] | Min [ms] | Max [ms] | Relative |
|:---|---:|---:|---:|---:|
| `./a.out -n 400 -c random` | 138.9 ± 29.0 | 85.7 | 200.6 | 1.00 |
| `./a.out -n 800 -c random` | 170.8 ± 28.0 | 136.2 | 220.3 | 1.23 ± 0.33 |
| `./a.out -n 1600 -c random` | 227.3 ± 118.0 | 152.0 | 554.3 | 1.64 ± 0.92 |
| `./a.out -n 3200 -c random` | 255.7 ± 22.7 | 224.2 | 292.6 | 1.84 ± 0.42 |
| `./a.out -n 6400 -c random` | 325.1 ± 58.0 | 240.4 | 444.2 | 2.34 ± 0.64 |
| `./a.out -n 12800 -c random` | 493.9 ± 30.1 | 433.1 | 547.5 | 3.56 ± 0.77 |
| `./a.out -n 25600 -c random` | 1202.8 ± 253.1 | 909.7 | 1689.3 | 8.66 ± 2.57 |
| `./a.out -n 51200 -c random` | 1823.1 ± 71.5 | 1655.0 | 1910.3 | 13.13 ± 2.79 |

| Command | Mean [ms] | Min [ms] | Max [ms] | Relative |
|:---|---:|---:|---:|---:|
| `./a.out -n 400 -c ordered` | 170.3 ± 11.5 | 157.9 | 197.2 | 1.00 |
| `./a.out -n 800 -c ordered` | 188.6 ± 26.3 | 165.0 | 253.8 | 1.11 ± 0.17 |
| `./a.out -n 1600 -c ordered` | 199.7 ± 27.8 | 149.8 | 232.9 | 1.17 ± 0.18 |
| `./a.out -n 3200 -c ordered` | 243.8 ± 46.0 | 181.8 | 341.0 | 1.43 ± 0.29 |
| `./a.out -n 6400 -c ordered` | 321.3 ± 42.8 | 281.7 | 429.3 | 1.89 ± 0.28 |
| `./a.out -n 12800 -c ordered` | 455.9 ± 32.5 | 397.4 | 512.1 | 2.68 ± 0.26 |
| `./a.out -n 25600 -c ordered` | 734.9 ± 56.1 | 659.7 | 868.0 | 4.32 ± 0.44 |
| `./a.out -n 51200 -c ordered` | 1532.9 ± 188.4 | 1365.2 | 1931.9 | 9.00 ± 1.26 |
