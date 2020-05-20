# Chapter 15: File Attributes

## Question 15-1
------------

**Question:**

Section 15.4 contained several statements about the permissions required for various file-system operations. Use shell commands or write programs to verify or answer the following:

a) Removing all owner permissions from a file denies the file owner access, even though group and other do have access.

b) On a directory with read permission but not execute permission, the names of files in the directory can be listed, but the files themselves can’t be accessed, regardless of the permissions on them.

c) What permissions are required on the parent directory and the file itself in order to create a new file, open a file for reading, open a file for writing, and delete a file? What permissions are required on the source and target directory to rename a file? If the target file of a rename operation already exists, what permissions are required on that file? How does setting the sticky permission bit (chmod +t) of a directory affect renaming and deletion operations?

**Answer:**

a) 
```bash
$ touch a
$ chmod u-rwx ./a
$ ls -l
total 0
----r--r-- 1 tiagonapoli tiagonapoli 0 mai 17 16:06 a
$ cat ./a
cat: ./a: Permission denied
```

b)
```bash
$ mkdir dir
$ touch dir/a
$ touch dir/b
$ ls -l dir
total 4
drwxr-xr-x 2 tiagonapoli tiagonapoli 4096 mai 17 16:10 dir
$ chmod -x dir
$ ls -l dir/
ls: cannot access 'dir/a': Permission denied
ls: cannot access 'dir/b': Permission denied
total 0
-????????? ? ? ? ?            ? a
-????????? ? ? ? ?            ? b
```

Note from book: If experimenting to verify the operation of the directory read permission bit, be aware that some Linux distributions alias the ls command to include flags (e.g., –F) that require access to i-node information for files in the directory, and this requires execute permission on the directory. To ensure that we are using an unadulterated ls, we can specify the full pathname of the command ( /bin/ls ).

c) 

- Create and delete files inside directory: It's necessary to have `write` and `execute` permission on the parent directory, it's not necessary to have permissions on the file (when deleting for example). Also it's necessary to have execute permission on all directories on the path to the parent directory (e.g, to delete ./a/b/x it's necessary to have `execute` permisson on `.`, `./a` and `write` permission on `a/b`).
- Open a file for reading/writing: It's necessary to have `execute` permission on all directories on the filepath and the necessary permission on the file (`write` or `read`).

Excerpt from tlpi:
```
When accessing a file, execute permission is required on all of the directories listed in the pathname. For example, reading the file /home/mtk/x would require execute permission on / , /home , and /home/mtk (as well as read permission on the file x itself). If the current working directory is /home/mtk/sub1 and we access the relative pathname ../sub2/x , then we need execute permission on /home/mtk and /home/mtk/sub2 (but not on / or /home ).
```

- Rename a file: Needs `write` and `execute` permission on both target and source directories - no permissions on the file itself is necessary. Also `execute` permissions are necessary on the path to each parent directory (source parent directory and target parent directory).

- Rename a file to another file that already exists: The same permissions from the before case are needed. No permissions on the source or target files are required.

Sticky bit on directories: retrict deletion - unpriviledged processes can unlink and rename files on the directory only if it has write permission on the directory and owns either the file or the directory.

## Question 15-2
------------

**Question:**

Do you expect any of a file’s three timestamps to be changed by the stat() system call? If not, explain why.

**Answer:**

Based on tests with the command line program `stat`, the timestamps are not changed by the `stat` system call - it just reads information from the file's inode and there's no `last inode access` timing.  

## Question 15-3
------------

**Question:**

On a system running Linux 2.6, modify the program in Listing 15-1 ( t_stat.c ) so that the file timestamps are displayed with nanosecond accuracy.

**Answer:**

See `stat.c`.

## Question 15-4
------------

**Question:**

The access() system call checks permissions using the process’s real user and group IDs. Write a corresponding function that performs the checks according to the process’s effective user and group IDs.

**Answer:**

See `access.c`.

## Question 15-5
------------

**Question:**

As noted in Section 15.4.6, umask() always sets the process umask and, at the same time, returns a copy of the old umask. How can we obtain a copy of the current process umask while leaving it unchanged?

**Answer:**

The process can read and parse `/proc/self/status`. There's a `umask: $VALUE` line there.

Or, the book solution:
```
int currUmask = umask(0);
umask(currUmask);
```
(not thread safe - threads share the process umask)

## Question 15-6
------------

**Question:**

The `chmod a+rX file` command enables read permission for all categories of user, and likewise enables execute permission for all categories of user if file is a directory or execute permission is enabled for any of the user categories for file, as demonstrated in the following example:

```bash
$ ls -ld dir file prog
dr-------- 2 mtk users 48
-r-------- 1 mtk users 19794
-r-x------ 1 mtk users 19336
$ chmod a+rX dir file prog
$ ls -ld dir file prog
dr-xr-xr-x 2 mtk users 48 May 4 12:28 dir
-r--r--r-- 1 mtk users 19794 May 4 12:22 file
-r-xr-xr-x 1 mtk users 19336 May 4 12:21 prog
```

Write a program that uses stat() and chmod() to perform the equivalent of `chmod a+rX`.

**Answer:**

See `chmodrX.c`.

## Question 15-7
------------

**Question:**

Write a simple version of the `chattr(1)` command, which modifies file i-node flags. See the `chattr(1)` man page for details of the chattr command-line interface. (You don’t need to implement the –R, –V, and –v options.)

**Answer:**

See `chattr.c`.