#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "lib/kernel/list.h"

void syscall_init (void);

/* File descriptor list */
struct fd_struct {
    int fd;
    struct file* file_;
    struct list_elem list_elem;
};
typedef struct fd_struct fd_struct;

fd_struct* new_fd_struct(int fd, struct file* file_);
void destroy_fd_struct(fd_struct* destroy);
fd_struct* find_fd_struct(struct list * fds, int fd);

#endif /* userprog/syscall.h */
