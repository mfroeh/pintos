#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/malloc.h"

#define STACK_GET(STACK, ARG, TYPE) (*(TYPE*)(STACK+4*ARG))
#define STACK_VAR(NAME, TYPE, STACK, ARG) TYPE NAME = (*(TYPE*)(STACK+4*ARG))
#define INT_MAX 0x7FFFFFFF

static void syscall_handler (struct intr_frame *);

// The fd to give out
int next_fd = 2;

fd_struct* new_fd_struct(int fd, struct file* file_) {
    fd_struct* new = malloc(sizeof(fd_struct));
    new->fd = fd;
    new->file_ = file_;
    return new;
}

void destroy_fd_struct(fd_struct* destroy) {
    file_close(destroy->file_);
    free(destroy);
}

/* Syscall implementations */
void halt(void) {
    power_off();
}

bool create(char const* file, unsigned initial_size) {
    return filesys_create(file, initial_size);
}

int open(char const* name, struct thread* cur_thread) {
    struct file* file_ = filesys_open(name);
    if (file_ == NULL || cur_thread->fd_count == 128) {
        return -1; 
    }

    if (next_fd != INT_MAX) {
        int fd = next_fd++;
        fd_struct *new_fd = new_fd_struct(fd, file_);
        list_push_back(&cur_thread->fds, &new_fd->list_elem);
        cur_thread->fd_count++;
        return fd;
    } else {
        // TODO: Find new file descriptor
        return -1;
    }
}

void close(int fd, struct thread* cur_thread) {
    // TODO: Should we handle fd is 0,1?

    struct list_elem* it;
    for (it = list_begin(&cur_thread->fds); it != list_end(&cur_thread->fds); it = list_next(it)) {
        fd_struct *fd_str = list_entry(it, fd_struct, list_elem);
        if (fd_str->fd == fd) {
            list_remove(it);
            destroy_fd_struct(fd_str);
            return;
        }
    }

    // TODO: if we arrived here, no fd with that ID was open
}

int exit(int status) {
    thread_exit();
    return status;
}

/* Syscall handler */
void syscall_init (void) 
{
    intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void syscall_handler (struct intr_frame *f UNUSED) 
{
    printf ("system call!\n");
    void* stack = f->esp;
    unsigned int syscall_nr = *(int*)stack;
    struct thread* cur_thread = thread_current();
    switch (syscall_nr) {
        /* Project 2 */
        case SYS_HALT: 
            halt();
            break;
        case SYS_EXIT:{
                          STACK_VAR(status, int, stack, 1);
                          f->eax = exit(status);
                          break;
                      }
        case SYS_EXEC:                 
                      break;
        case SYS_WAIT:                
                      break;
        case SYS_CREATE: {
                             STACK_VAR(file, char const*, stack, 1);
                             STACK_VAR(initial_size, unsigned, stack, 2);
                             f->eax = create(file, initial_size);
                             break;
                         }
        case SYS_REMOVE:            
                         break;
        case SYS_OPEN: {
                           STACK_VAR(file, char const*, stack, 1);
                           f->eax = open(file, cur_thread);
                           break;
                       }
        case SYS_FILESIZE:        
                       break;
        case SYS_READ:           
                       break;
        case SYS_WRITE:         
                       break;
        case SYS_SEEK:         
                       break;
        case SYS_TELL:        
                       break;
        case SYS_CLOSE:       {
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
    // thread_exit ();
}

//// Process table 
//static struct hash process_table;
//
//struct process_struct {
//  unsigned pid;
//  struct list* open_fds;
//  struct hash_elem hash_elem;
//};
//typedef struct process_struct process_struct;
//
//process_struct* new_process_struct(unsigned pid, struct list* open_fds) {
//  process_struct* new = malloc(sizeof(process_struct));
//  new->pid = pid;
//  new->open_fds = open_fds;
//  return new;
//}
//
//void destroy_process_struct(process_struct *destroy) {
//  // Destroy the list
//  struct list *open_fds = destroy->open_fds;
//  while (!list_empty(open_fds)) {
//    struct list_elem *e = list_pop_front(open_fds);
//    fd_struct *fd = list_entry(e, fd_struct, list_elem);
//    destroy_fd_struct(fd);
//  }
//  free(destroy->open_fds);
//  free(destroy);
//}
//
//unsigned pid_hash(const struct hash_elem *p, void* aux UNUSED) { 
//  process_struct* this = (hash_entry(p, process_struct, hash_elem)); 
//  return this->pid;
//}
//
//bool pid_less_func(const struct hash_elem *a, const struct hash_elem *b, void* aux UNUSED) {
//  process_struct* elem_a = (hash_entry(a, process_struct, hash_elem)); 
//  process_struct* elem_b = (hash_entry(b, process_struct, hash_elem)); 
//  return elem_a->pid < elem_b->pid;
//}
//
//struct process_struct* process_lookup(unsigned pid) {
//  process_struct process;
//  struct hash_elem *e;
//  process.pid = pid;
//  e = hash_find(&process_table, &process.hash_elem);
//  return e != NULL ? hash_entry(e, process_struct, hash_elem) : NULL;
//}

// Create the single process struct
// process_struct* process = new_process_struct(1, p_open_fds);

// hash_init(&process_table, pid_hash, pid_less_func, NULL);
// hash_insert(&process_table, &(process->hash_elem));
