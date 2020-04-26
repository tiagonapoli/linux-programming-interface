# Chapter 08: Users and Groups

## Question 8-1
------------

**Question:**

When we execute the following code, which attempts to display the usernames for two different user IDs, we find that it displays the same username twice. Why is this?
```
printf("%s %s\n", getpwuid(uid1)->pw_name, getpwuid(uid2)->pw_name);
```
**Answer:**

From the manual:
```
 The return value may point to a static area, and may be overwritten by subsequent calls to getpwent(3), getpwnam(), or getpwuid()
```
The subsequent calls to `getpwnam` have overwitten the content on memory from the previous call.

## Question 8-2
------------

**Question:**

Implement getpwnam() using setpwent(), getpwent(), and endpwent().

**Answer:**

See `getpwnam.c`
