# Chapter 57 - Sockets: Unix Domain

## Question 57-1

---

**Question:**

In Section 57.3, we noted that UNIX domain datagram sockets are reliable. Write
programs to show that if a sender transmits datagrams to a UNIX domain
datagram socket faster than the receiver reads them, then the sender is eventually
blocked, and remains blocked until the receiver reads some of the pending
datagrams.

**Answer:**

See `./udp-reliability`.
The server is started and then waits for 10s before starting to receive datagrams. At the same time,
if the client is started, it will send lots of datagrams - at some point the `sendto` call will block.
The server, after the initial delay, will start to receive one datagram at a time,
with 1s delay between each - it's possible then to verify that the `sendto` call on the client blocks for each
datagram to be sent.

## Question 57-2

---

**Question:**

Rewrite the programs in Listing 57-3 ( us_xfr_sv.c ) and Listing 57-4 ( us_xfr_cl.c ) to
use the Linux-specific abstract socket namespace (Section 57.6).

**Answer:**

See `./socket-namespace`.

## Question 57-3

---

**Question:**

Reimplement the sequence-number server and client of Section 44.8 using UNIX
domain stream sockets.

**Answer:**

See `./sequence-number`.

## Question 57-4

---

**Question:**

Suppose that we create two UNIX domain datagram sockets bound to the paths
/somepath/a and /somepath/b , and that we connect the socket /somepath/a to /somepath/b .
What happens if we create a third datagram socket and try to send (sendto()) a
datagram via that socket to /somepath/a ? Write a program to determine the answer.
If you have access to other UNIX systems, test the program on those systems to see
if the answer differs.

**Answer:**

See `connection3.c`. The `sendto` call errored with:
```
ERROR [EPERM Operation not permitted] sendto
```
