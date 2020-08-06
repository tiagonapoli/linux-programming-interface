# Chapter 61 - Sockets: Advanced Topics

## Question 61-1

---

**Question:**

Suppose that the program in Listing 61-2 ( is_echo_cl.c ) was modified so that,
instead of using fork() to create two processes that operate concurrently, it instead
used just one process that first copies its standard input to the socket and then
reads the server’s response. What problem might occur when running this client?
(Look at Figure 58-8, on page 1190.)

**Answer:**

The socket buffer for incoming data would possibly get full and then the kernel would refuse to ACK new incoming data to this socket:

From [source](https://eklitzke.org/how-tcp-sockets-work);

> If the receive buffer is full and the other end of the TCP connection tries to send additional data, the kernel will refuse to ACK the packets. This is just regular TCP congestion control.

Also important ([source](https://stackoverflow.com/a/14244450)):

> Data is cleared from the buffer when the remote side acknowledges it.

No data will be dropped though, due to the reliability of TCP. What will probably happen is: the client's socket receive buffer will get full, then the client will not ACK new received buffers. The server send buffer then will get full at some point too, and then the server will not be able to do further `write`s (it will probably block). Depending on the server implementation, the socket receive buffer will get full, resulting in no ACKs for new incoming data, then at some point the client's socket send buffer will get full too, resulting the client to be blocked on `write` to the socket. (this would result in a "deadlock", known as the Bounded Buffer deadlock problem).

## Question 61-2

---

**Question:**

Implement pipe() in terms of socketpair(). Use shutdown() to ensure that the resulting
pipe is unidirectional.

**Answer:**

TODO

## Question 61-3

---

**Question:**

Implement a replacement for sendfile() using read(), write(), and lseek().

**Answer:**

TODO

## Question 61-4

---

**Question:**

Write a program that uses getsockname() to show that, if we call listen() on a TCP
socket without first calling bind(), the socket is assigned an ephemeral port number.

**Answer:**

See `listen.c`.

## Question 61-5

---

**Question:**

Write a client and a server that permit the client to execute arbitrary shell
commands on the server host. (If you don’t implement any security mechanism in
this application, you should ensure that the server is operating under a user
account where it can do no damage if invoked by malicious users.) The client
should be executed with two command-line arguments:

```bash
$ ./is_shell_cl server-host 'some-shell-command'
```

After connecting to the server, the client sends the given command to the server,
and then closes its writing half of the socket using shutdown(), so that the server sees
end-of-file. The server should handle each incoming connection in a separate child
process (i.e., a concurrent design). For each incoming connection, the server
should read the command from the socket (until end-of-file), and then exec a shell
to perform the command. Here are a couple hints:

- See the implementation of system() in Section 27.7 for an example of how to execute a shell command.
- By using dup2() to duplicate the socket on standard output and standard error, the execed command will automatically write to the socket.

**Answer:**

See a simplified implementation on `./remote_execution`

## Question 61-5

---

**Question:**

Section 61.13.1 noted that an alternative to out-of-band data would be to create two
socket connections between the client and server: one for normal data and one for
priority data. Write client and server programs that implement this framework.
Here are a few hints:

- The server needs some way of knowing which two sockets belong to the same
  client. One way to do this is to have the client first create a listening socket
  using an ephemeral port (i.e., binding to port 0). After obtaining the ephemeral
  port number of its listening socket (using getsockname()), the client connects its
  “normal” socket to the server’s listening socket and sends a message containing
  the port number of the client’s listening socket. The client then waits for the
  server to use the client’s listening socket to make a connection in the opposite
  direction for the “priority” socket. (The server can obtain the client’s IP
  address during the accept() of the normal connection.)

- Implement some type of security mechanism to prevent a rogue process from
  trying to connect to the client’s listening socket. To do this, the client could
  send a cookie (i.e., some type of unique message) to the server using the nor-
  mal socket. The server would then return this cookie via the priority socket so
  that the client could verify it.

- In order to experiment with transmitting normal and priority data from the cli-
  ent to the server, you will need to code the server to multiplex the input from
  the two sockets using select() or poll() (described in Section 63.2).

**Answer:**

TODO
