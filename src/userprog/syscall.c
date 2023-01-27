#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"
#include "lib/kernel/hash.h"
#include "lib/kernel/list.h"
#include <stdlib.h>

static void syscall_handler (struct intr_frame *);

// File descriptor list
struct fd_struct {
  unsigned fd;
  struct list_elem list_elem;
};
typedef struct fd_struct fd_struct;

fd_struct* new_fd_struct(unsigned fd) {
  fd_struct* new = malloc(sizeof(fd_struct));
  new->fd = fd;
  return new;
}

void destroy_fd_struct(fd_struct* destroy) {
  free(destroy);
}

// Process table 
static struct hash process_table;

struct process_struct {
  unsigned pid;
  struct list* open_fds;
  struct hash_elem hash_elem;
};
typedef struct process_struct process_struct;

process_struct* new_process_struct(unsigned pid, struct list* open_fds) {
  process_struct* new = malloc(sizeof(process_struct));
  new->pid = pid;
  new->open_fds = open_fds;
  return new;
}

void destroy_process_struct(process_struct *destroy) {
  // Destroy the list
  struct list *open_fds = destroy->open_fds;
  while (!list_empty(open_fds)) {
    struct list_elem *e = list_pop_front(open_fds);
    fd_struct *fd = list_entry(e, fd_struct, list_elem);
    destroy_fd_struct(fd);
  }
  free(destroy->open_fds);
  free(destroy);
}

unsigned pid_hash(const struct hash_elem *p, void* aux UNUSED) { 
  process_struct* this = (hash_entry(p, process_struct, hash_elem)); 
  return this->pid;
}

bool pid_less_func(const struct hash_elem *a, const struct hash_elem *b, void* aux UNUSED) {
  process_struct* elem_a = (hash_entry(a, process_struct, hash_elem)); 
  process_struct* elem_b = (hash_entry(b, process_struct, hash_elem)); 
  return elem_a->pid < elem_b->pid;
}

struct process_struct* process_lookup(unsigned pid) {
  process_struct process;
  struct hash_elem *e;
  process.pid = pid;
  e = hash_find(&process_table, &process.hash_elem);
  return e != NULL ? hash_entry(e, process_struct, hash_elem) : NULL;
}

  void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");

  // Create the list of fds for single process
  struct list *p_open_fds = malloc(sizeof(struct list));
  list_init(p_open_fds);

  // Create the single process struct
  process_struct* process = new_process_struct(1, p_open_fds);

  hash_init(&process_table, pid_hash, pid_less_func, NULL);
  hash_insert(&process_table, &(process->hash_elem));
}

#define STACK_GET(STACK, ARG, TYPE) (*(TYPE*)(STACK+4*ARG))
#define STACK_VAR(NAME, TYPE, STACK, ARG) TYPE NAME = (*(TYPE*)(STACK+4*ARG))

/* Syscall implementations */
void halt(void) {
  power_off();
}

bool create(char const* file, unsigned initial_size) {
  return filesys_create(file, initial_size);
}

int open(char const* name) {
  struct file* file_ = filesys_open(name);
  // TODO: Choose fds carefully
  process_struct* process = process_lookup(1);
  struct list* open_fds = process->open_fds;
  unsigned i;
  for (i = 2; i < 130; ++i) {
    bool exists = false;

    struct list_elem *elem;
    for (elem = list_begin(open_fds); elem != list_end(open_fds); elem = list_next(open_fds)) {
      fd_struct* fd = list_entry(elem, fd_struct, list_elem);
      if (fd->fd == i) {
        exists = true;
        break;
      }
    }

    if (!exists) break;
  }

  fd_struct* new_fd = new_fd_struct(i);
  list_insert(open_fds, new_fd);
}

  static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  void* stack = f->esp;
  unsigned int syscall_nr = *(int*)stack;
  switch (syscall_nr) {
    /* Project 2 */
    case SYS_HALT: 
      halt();
      break;
    case SYS_EXIT:                  
      break;
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
    case SYS_OPEN:             
                     break;
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
    case SYS_CLOSE:      
                     break;

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

  thread_exit ();
}
