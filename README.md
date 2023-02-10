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


Question 2:

(a) Where are the user-mode stack and the kernel-mode stack of a process located?
The user virtual address space starts from address 0 till PHYS_BASE. The User mode stack is located below PHYS_BASE and it grows downward. The kernel virtual address space starts from PHYS_BASE up to 4GB and that is where the kernel-mode stack is also located.

(b) How can you address the user-mode stack in the kernel code, particularly, in an interrupt handler?
In the interrupt handler you can access the user-mode stack by using the interrupt frame struct `intr_frame` and accessing the user-mode stack pointer like `void *esp = f->esp`.

(c) What is the reason of having two stacks instead of one?
This is to prevent problems in the user stack causing a crash in the kernel itself. The kernel has to be independent of any errors that may come due to  user programs and that isolation in memeory provides that trust for the kernel to that its memory wont be affected by any other program.



Question 3
When a user program executes a system call like open(), an address to a string containing the file name is provided as an argument.

(a) In which memory is this string stored, and (b) how can we access it in the kernel code?
The string is stored in user virtual memory address space and the kernel accesses it using a special function call `process_vm_readv()` which takes the PID and the pointer to a structure of describing the data to be read.

(c) Specify the situations when accessing the data via that pointer can lead to problems.
The Invalid Pointer : If the pointer passed to the function in (3b) above, the function will throw an error. Invalid pointers can be unitialized pointers and location outside the user virtual address space.

Buffer Overflow : Since the kernel copies the data from the user mode stack to the kernel virtual address space buffer, it may happen that the
kernel buffer is not big enough to accomodate the user mode stack. This would also cause a error.

(d) What can be done about it?
Pointer Validation:




Question 4:

If user programs were allowed to use the filesystem directly, the kernel would not be able to manage access to the files. For example: User programs could open any and as many files as they want and concurrent access to files could not be managed by the kernel.

Question 5:

