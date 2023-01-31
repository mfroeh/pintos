1. What is the macro corresponding to the syscall number in this case?

The number is 9, which corresponds to SYS_WRITE.

2. What is the second parameter related to and what does it mean in this
case?

The parameter is 1 which is the file descriptor for STDOUT_FILENO.

3. Use GDB to print the string that the pointer in the third parameter refers
to. Hint: Use the x/s command variant to examine memory and treat it
as a string. You need to dereference the pointer using an asterisk to access
the contents of the memory (just like in C). Since GDB doesn’t know the
data type of the memory location that the pointer points to (it’s a void
pointer), you also need to cast it to a (char **).

Using x/s on *(char**)(f->esp+8) shows "You got it, use your debugging skills during the labs!\n".
