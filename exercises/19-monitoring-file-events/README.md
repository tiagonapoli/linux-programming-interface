# Chapter 19: Monitoring File Events

## Question 19-1

---

**Question:**

Write a program that logs all file creations, deletions, and renames under the
directory named in its command-line argument. The program should monitor
events in all of the subdirectories under the specified directory. To obtain a list of
all of these subdirectories, you will need to make use of nftw() (Section 18.9). When
a new subdirectory is added under the tree or a directory is deleted, the set of
monitored subdirectories should be updated accordingly.

**Answer:**

See `watch-directory-tree.c`.