# Chapter 59 - Sockets: Internet Domains

## Question 59-1

---

**Question:**

When reading large quantities of data, the readLine() function shown in Listing 59-1
is inefficient, since a system call is required to read each character. A more efficient
interface would read a block of characters into a buffer and extract a line at a time from
this buffer. Such an interface might consist of two functions. The first of these
functions, which might be called readLineBufInit(fd, &rlbuf), initializes the bookkeeping
data structure pointed to by rlbuf. This structure includes space for a data buffer,
the size of that buffer, and a pointer to the next “unread” character in that buffer.
It also includes a copy of the file descriptor given in the argument fd. The second
function, readLineBuf(&rlbuf), returns the next line from the buffer associated with
rlbuf. If required, this function reads a further block of data from the file descriptor
saved in rlbuf. Implement these two functions. Modify the programs in Listing 59-6
( is_seqnum_sv.c ) and Listing 59-7 ( is_seqnum_cl.c ) to use these functions.

**Answer:**

See `./seqnum`.

## Question 59-2

---

**Question:**

Modify the programs in Listing 59-6 ( is_seqnum_sv.c ) and Listing 59-7 ( is_seqnum_cl.c ) to
use the inetListen() and inetConnect() functions provided in Listing 59-9 ( inet_sockets.c ).

**Answer:**

See `./seqnum2`.

## Question 59-3

---

**Question:**

Write a UNIX domain sockets library with an API similar to the Internet domain
sockets library shown in Section 59.12. Rewrite the programs in Listing 57-3
( us_xfr_sv.c , on page 1168) and Listing 57-4 ( us_xfr_cl.c , on page 1169) to use this
library.

**Answer:**

TODO

## Question 59-4

---

**Question:**

Write a network server that stores name-value pairs. The server should allow names
to be added, deleted, modified, and retrieved by clients. Write one or more client
programs to test the server. Optionally, implement some kind of security
mechanism that allows only the client that created the name to delete it or to
modify the value associated with it.

**Answer:**

See a simple version on `kv_server.c`.

## Question 59-5

---

**Question:**

Suppose that we create two Internet domain datagram sockets, bound to specific
addresses, and connect the first socket to the second. What happens if we create a
third datagram socket and try to send (sendto()) a datagram via that socket to the
first socket? Write a program to determine the answer.

**Answer:**

Packages are silently dropped. See `./connection3.c`.
