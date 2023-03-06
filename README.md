# Question 1: How are you going to track all children of a given parent and the parent of every children?
Every thread will have a pointer to its parent and every parent thread will have a pcb struct that will have a list containing all of the children associated with the parent.

# Question 2: What is the information that you will include in the data structures to be shared between parent and children?

We will include the exit code, which is the exit code of the last child that exited. We will also include the alive_count indicating how many children+parents are alive, such that we can destroy the pcb when the last thread exits. Additionally we will store the list of child processes and a tid to the thread that we are currently waiting on (related to lab5).


# Question 3: Which synchronization mechanisms can be applied?

We used a semaphore in process_execute that makes sure that the parent is blocked until the child has executed, that is, at the very least has loaded itself into memory and setup it's stack inside of process_start. We up the semaphore after process start to allow the parent to execute again.
