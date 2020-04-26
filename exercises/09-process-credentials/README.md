# Chapter 09: Process Credentials

## Question 9-1
------------

**Question:**

Assume in each of the following cases that the initial set of process user IDs is `real=1000 effective=0 saved=0 file-system=0`. What would be the state of the user IDs after the following calls?
a) `setuid(2000);`
b) `setreuid(–1, 2000);`
c) `seteuid(2000);`
d) `setfsuid(2000);`
e) `setresuid(–1, 2000, 3000);`

**Answer:**
a) real=2000 effective=2000 saved=2000 file-system=2000
b) real=1000 effective=2000 saved=2000 file-system=2000
c) real=1000 effective=2000 saved=0 file-system=2000 
d) real=1000 effective=0 saved=0 file-system=2000
e) real=1000 effective=2000 saved=3000 file-system=2000

## Question 9-2
------------

**Question:**

Is a process with the following user IDs privileged? Explain your answer. 
```
real=0 effective=1000 saved=1000 file-system=1000
```
**Answer:**

No, it can go to a privileged state however, changing the effective uid with the real uid. A privileged process is defined to be one that has its effective uid equal to the root's uid.

## Question 9-3
------------

**Question:**

Implement initgroups() using setgroups() and library functions for retrieving information from the password and group files (Section 8.4). Remember that a process must be privileged in order to be able to call setgroups().

**Answer:**

See `initgroups.c`

## Question 9-4
------------

**Question:**

If a process whose user IDs all have the value X executes a set-user-ID program whose user ID, Y, is nonzero, then the process credentials are set as follows:
```
real=X effective=Y saved=Y
```
(We ignore the file-system user ID, since it tracks the effective user ID.)

Show the setuid(), seteuid(), setreuid(), and setresuid() calls, respectively, that would be used to perform the following operations:

a) Suspend and resume the set-user-ID identity (i.e., switch the effective user ID to the value of the real user ID and then back to the saved set-user-ID).
b) Permanently drop the set-user-ID identity (i.e., ensure that the effective user ID and the saved set-user-ID are set to the value of the real user ID).

(This exercise also requires the use of getuid() and geteuid() to retrieve the process’s real and effective user IDs.) Note that for certain of the system calls listed above, some of these operations can’t be performed.

**Answer:**

Suppose that using the getters we discovered X and Y, then:

a)
```
seteuid(X);
seteuid(Y);
```

b)
```
setresuid(X,X,X);
```


## Question 9-5
------------

**Question:**

Repeat the previous exercise for a process executing a set-user-ID-root program, which has the following initial set of process credentials:
```
real=X effective=0 saved=0
```

**Answer:**

a)
```
seteuid(X);
seteuid(0);
```

b)
```
setuid(X);
```