# Chapter 12: System and process information

## Question 12-1
------------

**Question:**

Write a program that lists the process ID and command name for all processes being run by the user named in the program’s command-line argument. (You may find the userIdFromName() function from Listing 8-1, on page 159, useful.) This can be done by inspecting the Name: and Uid: lines of all of the /proc/ PID /status files onthe system. Walking through all of the /proc/ PID directories on the system requires the use of readdir(3), which is described in Section 18.8. Make sure your program correctly handles the possibility that a /proc/ PID directory disappears between the time that the program determines that the directory exists and the time that it tries to open the corresponding /proc/ PID  status file.

**Answer:**

See `process-list.c`

## Question 12-2
------------

**Question:**

Write a program that draws a tree showing the hierarchical parent-child relationships of all processes on the system, going all the way back to init. For each process, the program should display the process ID and the command being executed. The output of the program should be similar to that produced by pstree(1), although it does need not to be as sophisticated. The parent of each process on the system can be found by inspecting the PPid: line of all of the /proc/ PID /status files on the system. Be careful to handle the possibility that a process’s parent (and thus its /proc/ PID directory) disappears during the scan of all /proc/ PID directories.

**Answer:**

See `process-tree.c`

## Question 12-3
------------

**Question:**

Write a program that lists all processes that have a particular file pathname open. This can be achieved by inspecting the contents of all of the `/proc/PID/fd/*` symbolic links. This will require nested loops employing readdir(3) to scan all /proc/ PID directories, and then the contents of all `/proc/PID/fd` entries within each `/proc/PID` directory. To read the contents of a `/proc/PID/fd/n` symbolic link requires the use of readlink(), described in Section 18.5.

**Answer:**

See `file-usage.c`