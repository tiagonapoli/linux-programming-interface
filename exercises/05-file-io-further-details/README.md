# Chapter 05: File I/O: Further Details

## Question 5-1
------------

**Question:**

Modify the program in Listing 5-3 to use standard file I/O system calls (open() and
lseek()) and the off_t data type. Compile the program with the _FILE_OFFSET_BITS
macro set to 64, and test it to show that a large file can be successfully created.

**Answer:**

Apparently _FILE_OFFSET_BITS doesn't do nothing on 64bit systems:
```
On 64 bit systems this macro has no effect since the *64 functions are identical to the normal functions.
```
https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html


## Question 5-2
------------

**Question:**

Write a program that opens an existing file for writing with the O_APPEND flag, and then seeks to the beginning of the file before writing some data. Where does the data appear in the file? Why?

**Answer:**

It seems with this flag the `lseek` doesn't have any effect while using `SEEK_SET` flag.

```
O_APPEND
The file is opened in append mode. Before each write(2), the file offset is positioned at the end of the file, as if with lseek(2)
```

`lseek` will still have effect on reading the file, but while writing the above behavior will happen.

See `append-seek.c`

## Question 5-3
------------

**Question:**

This exercise is designed to demonstrate why the atomicity guaranteed by opening a file with the O_APPEND flag is necessary. Write a program that takes up to three command-line arguments:

```
$ atomic_append filename num-bytes [x]
```

This file should open the specified filename (creating it if necessary) and append num-bytes bytes to the file by using write() to write a byte at a time. By default, the program should open the file with the O_APPEND flag, but if a third command-line argument (x) is supplied, then the O_APPEND flag should be omitted, and instead the program should perform an lseek(fd, 0, SEEK_END) call before each write(). Run two instances of this program at the same time without the x argument to write 1 million bytes to the same file:

```
$ atomic_append f1 1000000 & atomic_append f1 1000000
```

Repeat the same steps, writing to a different file, but this time specifying the x
argument:

```
$ atomic_append f2 1000000 x & atomic_append f2 1000000 x
```

List the sizes of the files f1 and f2 using `ls –l` and explain the difference.

**Answer:**

Because of race condition sometimes a process A wrote bytes into the end of the file just after B pointed to the old end of the file. Then B overwrote bytes written by A, instead of writing to the end of the file.

O_APPEND flag guarantees this won't happen because writing to the end of the file becomes a atomic operation.

See `atomic-append.c`


## Question 5-4
------------

**Question:**

Implement dup() and dup2() using fcntl() and, where necessary, close(). (You may ignore the fact that dup2() and fcntl() return different errno values for some error cases.) For dup2(), remember to handle the special case where oldfd equals newfd. In this case, you should check whether oldfd is valid, which can be done by, for example, checking if fcntl(oldfd, F_GETFL) succeeds. If oldfd is not valid, then the function should return –1 with errno set to EBADF .

**Answer:**

See `dup.c`

## Question 5-5
------------

**Question:**

Write a program to verify that duplicated file descriptors share a file offset value and open file status flags.

**Answer:**

See `dup-check.c`

## Question 5-6
------------

**Question:**

After each of the calls to write() in the following code, explain what the content of the output file would be, and why:

```
fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
fd2 = dup(fd1);
fd3 = open(file, O_RDWR);
write(fd1, "Hello,", 6);
write(fd2, "world", 5);
lseek(fd2, 0, SEEK_SET);
write(fd1, "HELLO,", 6);
write(fd3, "Gidday", 6);
```

**Answer:**

- `write(fd1, "Hello,", 6)`: "Hello,"
- `write(fd2, "world", 5)`: "Hello,world"
- `write(fd1, "HELLO,", 6)`: "HELLO,world"
- `write(fd3, "Gidday", 6)`: "Giddayworld"

`fd2` is a duplicate of `fd1`, so changes on `fd1` or `fd2` offset are shared.

`fd3` is independent to `fd1` and `fd2`, so in the last write it points to the beginning of the file, that's because it overwrites `HELLO`.

See `output-test.c`


## Question 5-7
------------

**Question:**

Implement readv() and writev() using read(), write(), and suitable functions from the malloc package (Section 7.1.2).

**Answer:**

See `scatter-gather.c`