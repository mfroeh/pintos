Preparatory question 0:
(a) What is the idea behind system calls?

Managing certain functionality, such as writing to a file, through the kernel instead of the user space.
This way the user programs don't have to deal with specific hardware, since it is handled by the kernel.
System calls can be made by including a library provided by the kernel (note that the libary does not make the system calls itself!).
When a user program makes a system call, it is interrupted and goes into kernel mode.
The kernel mode checks which system call should be made by examining the stack of the calling thread (user program) and executes the system call.
It then returns control to the user mode by using an interrupt.

(b) Why cannot the code of the system calls be available simply as a library
to user processes? (Tip: you may look at an article in Wikipedia for the answer).

Doing so would surrender control to the user programs, which would cause security risks.
For example, the kernel would not be able to check if a memory address is valid for the running thread/process.

