Question 0:

(a) What is the idea behind system calls?

Managing certain functionality, such as writing to a file, through the kernel instead of the user space.
This way the user programs don't have to deal with specific hardware, since it is handled by the kernel.
System calls can be made by including a library provided by the kernel (note that the libary does not make the system calls itself!).
When a user program makes a system call, it is interrupted and goes into kernel mode.
The kernel mode checks which system call should be made by examining the stack of the calling thread (user program) and executes the system call.
It then returns control to the user mode by using an interrupt.

(b) Why cannot the code of the system calls be available simply as a library to user processes? (Tip: you may look at an article in Wikipedia for the answer).

Doing so would surrender control to the user programs, which would cause security risks.
For example, the kernel would not be able to check if a memory address is valid for the running thread/process.

Question 1:
(a) How it is possible to distinguish in syscall handler which system call it is?

The library used to perform syscalls pushes the arguments needed by the syscall on the user programs stack before issuing the iterrupt. The first argument will always be the ID of the syscall.

(b) Where are the arguments of a system call stored (if there are any) and how can you access them?

The arguments are stored on the user programs stack, a pointer to which is found in the interrupt frame (void* esp).
Each argument has a size of one word (32 bit in pintos) which means that argument 1 (always the syscall ID) can be accessed through `int syscall_id = *(int*)(f->esp)`. Argument n can generally be accessed through `type arg_n = *(type*)(f->esp+4*n-1)`.

Question 4:

If user programs were allowed to use the filesystem directly, the kernel would not be able to manage access to the files. For example: User programs could open any and as many files as they want and concurrent access to files could not be managed by the kernel.
