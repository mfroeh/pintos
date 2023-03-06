# Question 1: 
One may synchronize access to files by locking the whole file while read- ing/writing. Think about why it is not a good idea.

Processes would have to wait get access to any file, regardless of if that file is already being written to or read from by another process.

# Question 2: 
What is the readers/writers problem? Which modifications of readers- writers synchronization algorithm exist? Find pseudo code for the algorithm that prioritizes readers.

The readers/writers problem occurs if more than one process wants access to the same data to either write to it or to read from it. If more than one process is given access without synchronization, the resulting reads/writes could lead to unwanted results (e.g. one process overwrites the content that was just written by antoher process). 

Pseudo code prioritize reader:
```c
// Reader:
while (true){
    wait(mutex);
    read_count++;
    if (read_count == 1) /* first reader */
        wait(rw_mutex);
    signal(mutex);
    /* READ */
    wait(mutex);
    read_count--;
    if (read_count == 0) /* last reader */
    signal(rw_mutex);
    signal(mutex);
}

// Writer:
while (true) {
    wait(rw_mutex);
    /* WRITE */
    signal(rw_mutex);
}
```

# Question 3:
Think of a scenario where the concurrent access to a file system with no
synchronization causes a problem such as inconsistency or corruption of
the file system.

One process is trying to remove a file and one process is trying to create a file with the same name. If the execution of both those actions is interleaved, the creation of a file on the disk at the same position as the removed file may be allowed. The removing process may therefore remove parts of the file to be created without the file creating process noticing and leaving the disk in a corrupt state.

# Question 4:
What is the difference between the inode and the file object?

The inode is the metadata of the file on disk containing its disksector and size. A file is the in memory representation of the data on disk and always contains the inode associated with it. There may be more than one file per inode.

# Question 5:
Consider the following set of actions, which are provided in the following
chronological order. What will student.txt contain?

Assume synchronization of the file system.

Process A: create(student.txt, 1000)
- Create a new file
Process A: fd = open(student.txt)
- Opening the file returns a valid fd
Process B: remove(student.txt)
- Files directory entry is removed and inode of file is flagged for deletion 
Process C: create(student.txt, 1000)
- Creates a new file on disk, this one is different from the one created before
Process C: fd = open(student.txt)
- Opening the new file returns a valid fd 
Process C: write(fd, ”AAA”, 3)
- Write to the new file
Process A: write(fd, ”BBB”, 3)
- Write to the old file
Process A: close(fd)
- Close the old file, which triggers its removal from disk
Process C: close(fd)
- Close the new file

The file "student.txt" content is "AAA".

# Control Questions:

- What is the difference between file_open and filesys_open? 

filesys_open looks up the inode of the file by calling lookup on the root directory and then invokes file_open on the found inode. file_open allocates a new file object for the inode and returns it.

- Which functions from inode.c are called when you call filesys_remove,
filesys_open, filebclose, file_read, and file_write?

inode_open, inode_close, inode_read_at, inode_write_at, inode_remove.

- When you remove the file, what is removed first, the file name from the
directory or the file content from the disk? How and when is the file
content removed?

The file name is erased first by dir_remove, then inode_remove flags the inode for deletion.
If inode_close called by the last thread that had a file with that inode open and the inode was flagged for deletion its content is removed from the disk.

- What happens if you attempt to remove an open file?

The files inode is flagged for removal, it's entry removed from the directoy and it's contents are removed from disk as soon as the last thread that had a fd to the file closes the fd.

- How can you keep track of the position in a file?

The file struct contains a field with the position in the file.

- Can you open a file, on which filesys remove has been called?

No because the directories entry is removed so the lookup call in filesys_open will fail.

- Find where free_map_allocate and free_map_release are used in inode.c.

free_map_allocate is called when a new inode is created on disk (inode_create) and free_map_release is called when an inode is removed from disk (inside inode_close).

- There are few levels where you can add your implementation of the readers-
writers problem: system calls, files, and inodes. Think about advantages
and disadvantages of each approach. Which level is the most appropriate? Motivate your answer.

System calls:
- We would do synchronization only if a user program uses a file. We would have to keep track of how files are opened. No real advantages, would cause problems with files being opened without syscalls.

Files:
- Again, we would synchronize only if we are not operating on the inodes directly. Some functions may directly invoke inode functions, which would not be synchronized. In addition, we have no good way to keep track of the semaphores, since there may be more than one file per inode.

Inodes:
- We get synchronization for every time the disk is altered, which is what we want. We keep track of all instances of inodes being open inside the same inode object, which means that we can store our semaphores for the RW algorithm with them. We can free the semaphores once the last opener of an inode stops execution.
