1. What is busy waiting? Why should the programmer avoid busy waiting
in a concurrent operating system?

Busy waiting is when a process has to run in order to find out if should still wait or continue exucution.
In a concurrent operating systems (suppose a single CPU core) this means that other processes can not be scheduled, whilst a process is checking if it should run.

2. Explain difference between Yield and Sleep.

When sleeping, a process suspends itself for a set amount of time (e.g. ticks) until it can be scheduled again, by the os adding the process to the ready queue once the process has slept long enough.
Yielding immediatly places the current process back in to the ready queue.

3. What is the difference between locks and semaphores? (Hint: there are
two main differences). What is a deadlock?

TODO

Deadlock: In a deadlock, two or more processes cannot continue execution because they are waiting on a resource that is made available by another process.
This other process cannot generate the resource since it is waiting on the other process.
There is a cyclic wait between the processes.
