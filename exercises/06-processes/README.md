# Chapter 06: Processes

## Question 6-1
------------

**Question:**

Compile the program in Listing 6-1 ( mem_segments.c ), and list its size using ls –l. Although the program contains an array (mbuf) that is around 10 MB in size, the executable file is much smaller than this. Why is this?

**Answer:**

Uninitialized variables and arrays are not added to the executable binary - they are initialized as zero when the process starts, on the `unintialized data segment` of the process memory map.


## Question 6-2
------------

**Question:**

Write a program to see what happens if we try to longjmp() into a function that has already returned.

**Answer:**

It got Segmentation Fault. The call to `setjmp` stores info on the stack pointer, instruction pointer and other info, so that called functions, deeper in the stack, can come back to the current stack state. If the call occurs before in the stack, the stored stack pointer value is pointing to a place in memory that is no more necessarily holding the previous stack state, so the behavior is undefined.

See `longjump.c`

## Question 6-3
------------

**Question:**

Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary, code that directly modifies environ. Your version of unsetenv() should check to see whether there are multiple definitions of an environment variable, and remove them all (which is what the glibc version of unsetenv() does).

**Answer:**

See `myenv.c`
