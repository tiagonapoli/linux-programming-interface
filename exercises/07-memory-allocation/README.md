# Chapter 07: Memory allocation

## Question 7-1
------------

**Question:**

Modify the program in Listing 7-1 ( free_and_sbrk.c ) to print out the current value of the program break after each execution of malloc(). Run the program specifying a small allocation block size. This will demonstrate that malloc() doesn’t employ sbrk() to adjust the program break on each call, but instead periodically allocates larger chunks of memory from which it passes back small pieces to the caller.

**Answer:**

See `free-and-sbrk.c`

## Question 7-2
------------

**Question:**

(Advanced) Implement malloc() and free().

**Answer:**

See `malloc.c`