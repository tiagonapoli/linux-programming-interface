# Chapter 04: File I/O: The universal I/O model

## Question 4-1
------------

**Question:**

The tee command reads its standard input until end-of-file, writing a copy of the input
to standard output and to the file named in its command-line argument. (We show
an example of the use of this command when we discuss FIFOs in Section 44.7.)
Implement tee using I/O system calls. By default, tee overwrites any existing file with
the given name. Implement the –a command-line option (tee –a file), which causes tee
to append text to the end of a file if it already exists. (Refer to Appendix B for a
description of the getopt() function, which can be used to parse command-line
options.)

**Answer:**

See `tee.c`.


## Question 4-2
------------

**Question:**

Write a program like cp that, when used to copy a regular file that contains holes
(sequences of null bytes), also creates corresponding holes in the target file.

**Answer:**

See `cpholes.c`

