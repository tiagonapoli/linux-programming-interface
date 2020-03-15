# Chapter 03: System Programming Concepts

## Question 3-1
------------

**Question:**

When using the linux-specific reboot() system call to reboot the
system, the second argument, magic2, must be specified as one of a
set of magic numbers (e.g. LINUX_REBOOT_MAGIC2).  What is the
significance of these numbers? (Converting them to hexadecimal
provides a clue)

**Answer:**

```
MAGIC1: 0xFEE1DEAD (-18751827) - Feel dead
MAGIC2: 0x28121969 (672274793) - Linux Torvald's birthday
```

Prevents erroneous calls to reboot. These magic numbers HAVE to be given
to the `magic1` and `magic2` arguments of the `reboot` syscall.