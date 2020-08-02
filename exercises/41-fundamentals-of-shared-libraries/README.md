# Chapter 41 - Fundamentals of shared libraries

## Question 41-1

---

**Question:**

Try compiling a program with and without the –static option, to see the difference
in size between an executable dynamically linked with the C library and one that is
linked against the static version of the C library.

**Answer:**

Compiling `lib-test.c`:

- `gcc lib-test.c`: Generated an `a.out` with 16,792 bytes.
- `gcc lib-test.c -static`:  Generated an `a.out` with 1,002,080 bytes.
