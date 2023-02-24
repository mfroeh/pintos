#include "userprog/syscall.h"
#include "devices/input.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "lib/kernel/list.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include <stdio.h>
#include <syscall-nr.h>

#define STACK_VAR(NAME, TYPE, STACK, ARG)                                      \
  TYPE NAME = (*(TYPE *)(STACK + 4 * ARG))
#define INT_MAX 0x7FFFFFFF

static void syscall_handler(struct intr_frame *);

// The fd to give out
int next_fd = 2;

fd_struct *new_fd_struct(int fd, struct file *file_) {
  fd_struct *new = malloc(sizeof(fd_struct));
  new->fd = fd;
  new->file_ = file_;
  return new;
}

void destroy_fd_struct(fd_struct *destroy) {
  file_close(destroy->file_);
  free(destroy);
}

fd_struct *find_fd_struct(struct list *fds, int fd) {
  struct list_elem *elem;
  for (elem = list_begin(fds); elem != list_end(fds); elem = list_next(elem)) {
    fd_struct *elem_fd = list_entry(elem, fd_struct, list_elem);
    if (elem_fd->fd == fd) {
      return elem_fd;
    }
  }

  return NULL;
}

/* Syscall implementations */
void halt(void) { power_off(); }

bool create(char const *file, unsigned initial_size) {
  return filesys_create(file, initial_size);
}

int open(char const *name, struct thread *cur_thread) {
  struct file *file_ = filesys_open(name);
  if (file_ == NULL || cur_thread->fd_count == 128) {
    return -1;
  }

  if (next_fd != INT_MAX) {
    int fd = next_fd++;
    fd_struct *new_fd = new_fd_struct(fd, file_);
    list_push_back(&cur_thread->fds, &new_fd->list_elem);
    cur_thread->fd_count++;
    return fd;
  }

  return -1;
}

int read(int fd, void *buffer, unsigned size, struct thread *cur_thread) {
  if (fd == 0) {
    for (unsigned i = 0; i < size; ++i) {
      *(char *)(buffer + i) = input_getc();
    }
    return size;
  }

  // Else if fd is open
  if (fd > 1) {
    fd_struct *elem_fd = find_fd_struct(&cur_thread->fds, fd);
    return elem_fd == NULL ? -1 : file_read(elem_fd->file_, buffer, size);
  }

  return -1;
}

int write(int fd, void const *buffer, unsigned size,
          struct thread *cur_thread) {
  if (fd == 1) {
    putbuf(buffer, size);
    return size;
  }

  if (fd > 1) {
    fd_struct *elem_fd = find_fd_struct(&cur_thread->fds, fd);
    return elem_fd == NULL ? -1 : file_write(elem_fd->file_, buffer, size);
  }

  return -1;
}

void close(int fd, struct thread *cur_thread) {
  fd_struct *elem_fd = find_fd_struct(&cur_thread->fds, fd);
  if (elem_fd != NULL) {
    list_remove(&elem_fd->list_elem);
    cur_thread->fd_count--;
    destroy_fd_struct(elem_fd);
  }
}

int exec(char const* cmdline) {
  tid_t tid = process_execute(cmdline);
  if (tid == TID_ERROR) {
    printf("Failed to create process!\n");
  }
  return tid;
}

int wait(tid_t pid) {
  

  return 0;
}

int exit(int status) {
  thread_current()->exit_code = status;
  thread_exit();
  return status;
}

/* Syscall handler */
void syscall_init(void) {
  intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void syscall_handler(struct intr_frame *f UNUSED) {
  void *stack = f->esp;
  unsigned int syscall_nr = *(int *)stack;
  struct thread *cur_thread = thread_current();

  printf("syscall_handler: system call %d made by %d!\n", syscall_nr, cur_thread->tid);
  switch (syscall_nr) {
  /* Project 2 */
  case SYS_HALT: {
    halt();
    break;
  }
  case SYS_EXIT: {
    STACK_VAR(status, int, stack, 1);
    f->eax = exit(status);
    break;
  }
  case SYS_EXEC: {
    STACK_VAR(cmdline, char const*, stack, 1);
    f->eax = exec(cmdline);
    break;
  }
  case SYS_WAIT:
  {
    STACK_VAR(pid, tid_t, stack, 1);
    f->eax = wait(pid);
    break;
  }
  case SYS_CREATE: {
    STACK_VAR(file, char const *, stack, 1);
    STACK_VAR(initial_size, unsigned, stack, 2);
    f->eax = create(file, initial_size);
    break;
  }
  case SYS_REMOVE:
    break;
  case SYS_OPEN: {
    STACK_VAR(file, char const *, stack, 1);
    f->eax = open(file, cur_thread);
    break;
  }
  case SYS_FILESIZE:
    break;
  case SYS_READ: {
    STACK_VAR(fd, int, stack, 1);
    STACK_VAR(buffer, void *, stack, 2);
    STACK_VAR(size, unsigned, stack, 3);
    f->eax = read(fd, buffer, size, cur_thread);
    break;
  }
  case SYS_WRITE: {
    STACK_VAR(fd, int, stack, 1);
    STACK_VAR(buffer, void const *, stack, 2);
    STACK_VAR(size, unsigned, stack, 3);
    f->eax = write(fd, buffer, size, cur_thread);
    break;
  }
  case SYS_SEEK:
    break;
  case SYS_TELL:
    break;
  case SYS_CLOSE: {
    STACK_VAR(fd, int, stack, 1);
    close(fd, cur_thread);
    break;
  }

    /* Project 3 and optionally project 4. */
  case SYS_MMAP:
    break;
  case SYS_MUNMAP:
    break;

    /* Project 4 only. */
  case SYS_CHDIR:
    break;
  case SYS_MKDIR:
    break;
  case SYS_READDIR:
    break;
  case SYS_ISDIR:
    break;
  case SYS_INUMBER:
    break;
  }
}
