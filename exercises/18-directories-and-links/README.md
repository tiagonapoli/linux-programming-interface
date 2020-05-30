# Chapter 18: Directories and Links

## Question 18-1

---

**Question:**

In Section 4.3.2, we noted that it is not possible to open a file for writing if it is
currently being executed (open() returns –1, with errno set to ETXTBSY ). Nevertheless,
it is possible to do the following from the shell:

```
$ cc -o longrunner longrunner.c
$ ./longrunner &                                Leave running in background
$ vi longrunner.c                               Make some changes to the source code
$ cc -o longrunner longrunner.c
```

The last command overwrites the existing executable of the same name. How is this
possible? (For a clue, use ls –li to look at the i-node number of the executable file
after each compilation.)

**Answer:**

Probably the old `a.out` is unlinked and a new `a.out` is created - the two have different i-node numbers. It's ok to unlink a file that's being executed - the file name will be removed from the parent directory entry but the file itself will still exist until the process exits.

## Question 18-2

---

**Question:**

Why does the call to chmod() in the following code fail?

```c
mkdir("test", S_IRUSR | S_IWUSR | S_IXUSR);
chdir("test");
fd = open("myfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
symlink("myfile", "../mylink");
chmod("../mylink", S_IRUSR);
```

**Answer:**

In this example the symlink was created with a relative path `myfile` (when using the `symlink` syscall the target path doesn't take into account the current path of the process - it is interpreted as is). Also:

```
A relative symbolic link is interpreted relative to the location of the link itself.
```

So, when derefencing the symlink `/absolute/path/to/mylink`, it will point to `/absolute/path/to/myfile`, which doens't exist.

Notice that when creating a symlink the existence of the target is not necessary nor checked.

## Question 18-3

---

**Question:**

Implement realpath().

**Answer:**

[NOT_DONE]

## Question 18-4

---

**Question:**

Modify the program in Listing 18-2 ( list_files.c ) to use readdir_r() instead of readdir().

**Answer:**

See `list_files.c`.

## Question 18-5

---

**Question:**

Implement a function that performs the equivalent of getcwd(). A useful tip for
solving this problem is that you can find the name of the current working directory
by using opendir() and readdir() to walk through each of the entries in the parent
directory ( .. ) to find an entry with the same i-node and device number as the
current working directory (i.e., respectively, the st_ino and st_dev fields in the stat
structure returned by stat() and lstat()). Thus, it is possible to construct the
directory path by walking up the directory tree (chdir(“..”)) one step at a time and
performing such scans. The walk can be finished when the parent directory is the
same as the current working directory (recall that /.. is the same as / ). The caller
should be left in the same directory in which it started, regardless of whether your
getcwd() function succeeds or fails (open() plus fchdir() are handy for this purpose).

**Answer:**

See `getcwd.c`.

## Question 18-6

---

**Question:**

Modify the program in Listing 18-3 ( nftw_dir_tree.c ) to use the FTW_DEPTH flag. Note
the difference in the order in which the directory tree is traversed.

**Answer:**

See `nftw_dir_tree.c`.

## Question 18-7

---

**Question:**

Write a program that uses nftw() to traverse a directory tree and finishes by
printing out counts and percentages of the various types (regular, directory,
symbolic link, and so on) of files in the tree.

**Answer:**

See `nftw-test-drive.c`.

## Question 18-8

---

**Question:**

Implement nftw(). (This will require the use of the opendir(), readdir(), closedir(), and
stat() system calls, among others.)

**Answer:**

[NOT_DONE]

## Question 18-9

---

**Question:**

In Section 18.10, we showed two different techniques (using fchdir() and chdir(),
respectively) to return to the previous current working directory after changing the
current working directory to another location. Suppose we are performing such an
operation repeatedly. Which method do you expect to be more efficient? Why?
Write a program to confirm your answer.

**Answer:**

I expect `fchdir` to be more efficient - `chdir` probably has to do more operations to change the current directory, like resolving the pathname it receives, also it has to pass a much more complex structure to kernel space (a string, instead of just an int).

| Command                |    Mean [ms] | Min [ms] | Max [ms] |    Relative |
| :--------------------- | -----------: | -------: | -------: | ----------: |
| `./chdir.out 1000000`  | 643.8 ± 12.7 |    626.5 |    668.6 | 2.98 ± 0.06 |
| `./fchdir.out 1000000` |  216.2 ± 1.7 |    213.5 |    220.4 |        1.00 |
