# Chapter 21 - Signals: Signal Handlers

## Question 21-1

---

**Question:**

Implement abort().

**Answer:**

Here's an implementation of abort from glibc: 

https://code.woboq.org/userspace/glibc/stdlib/abort.c.html

However it seems to be missing stdio flushes (doing some research it seems that the stdio flushes were removed from glibc).

This reddit post has some interesting discussion on how the abort implementation works.

https://www.reddit.com/r/raldi/comments/iu0f7/every_programmer_should_read_the_source_code_to/