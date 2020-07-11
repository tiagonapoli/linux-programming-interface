# Chapter 27 - Process Execution

## Question 27-1

---

**Question:**

The final command in the following shell session uses the program in Listing 27-3
to exec the program xyz. What happens?

```bash
$ echo $PATH
/usr/local/bin:/usr/bin:/bin:./dir1:./dir2
$ ls -l dir1
total 8
-rw-r--r-- 1 mtk users 7860 Jun 13 11:55 xyz
$ ls -l dir2
total 28
-rwxr-xr-x 1 mtk users 27452 Jun 13 11:55 xyz
$ ./t_execlp xyz
```

**Answer:**

The file `dir2/xyz` is executed, since is the first file named `xyz` found in the `$PATH` list
that is **executable**.

## Question 27-2

---

**Question:**

Use execve() to implement execlp(). You will need to use the stdarg(3) API to handle
the variable-length argument list supplied to execlp(). You will also need to use
functions in the malloc package to allocate space for the argument and environment
vectors. Finally, note that an easy way of checking whether a file exists in a particular
directory and is executable is simply to try execing the file.

**Answer:**

See `execve.c`.

## Question 27-3

---

**Question:**

What output would we see if we make the following script executable and exec() it?

```
#!/bin/cat -n
Hello world
```

**Answer:**

`exec` will detect the script should be interpret by `/bin/cat` and will execute the
following:

```
/bin/cat -n script-path
```

Then `/bin/cat` will output:

```
1 #!/bin/cat -n
2 Hello world
```

See `cat-script`.

## Question 27-4

---

**Question:**

What is the effect of the following code? In what circumstances might it be useful?

```c
childPid = fork();
if (childPid == -1) errExit("fork1");
if (childPid == 0) { /* Child */
    switch (fork()) {
    case -1: errExit("fork2");
    case 0:  /* Grandchild */
        /* ----- Do real work here ----- */
        exit(EXIT_SUCCESS); /* After doing real work */
    default:
         exit(EXIT_SUCCESS); /* Make grandchild an orphan */
    }
}

/* Parent falls through to here */
if (waitpid(childPid, &status, 0) == -1) errExit("waitpid");

/* Parent carries on to do other things */
```

**Answer:**

The parent will not have to deal with the grandchild process. It won't
need to wait for it, since in the end grandchild will be a zombie process
from the child process and be delegated to the init or systemd process to
deal with.

This may be useful when the parent doesn't care about a process result it
wants to start - the parent won't have to wait for pids or setup SIGCHLD handlers.

## Question 27-5

---

**Question:**

When we run the following program, we find it produces no output. Why is this?

```c
int main(int argc, char *argv[]) {
    printf("Hello world");
    execlp("sleep", "sleep", "0", (char *) NULL);
}
```

**Answer:**

`printf` is line buffered, so when we do `printf("Hello world");` the string
will be buffered in user space and not flushed yet to the kernel. However a exec
follows the printf, which will discard the program's stack, data and heap (thus
discarding the stdio buffer holding the `Hello world`).

See `printf-exec.`. If you add a `\n` in the end of `Hello world` the message
will be written to the terminal.

## Question 27-6

---

**Question:**

Suppose that a parent process has established a handler for SIGCHLD and also
blocked this signal. Subsequently, one of its children exits, and the parent then
does a wait() to collect the child’s status. What happens when the parent unblocks
SIGCHLD ? Write a program to verify your answer. What is the relevance of the result
for a program calling the system() function?

**Answer:**

When the parent unblock SIGCHLD the handler will be called (SIGCHLD is blocked but
when the process receives one or more SIGCHLD it's marked as pending and when SIGCHLD is
unblocked it's delivered) - see `sigchld.c`.

If the SIGCHLD handler calls `wait` it may:

- Return ECHILD error if there's no other child processes to wait.
- Block until another child process finishes (not the one that raised the SIGCHLD).

This situation will happen if we use `system` and have a custom SIGCHLD disposition.
