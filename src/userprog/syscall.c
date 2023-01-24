#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

static void syscall_handler (struct intr_frame *);

  void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

#define STACK_GET(STACK, ARG, TYPE) (*(TYPE*)(STACK+4*ARG))
#define STACK_VAR(NAME, TYPE, STACK, ARG) TYPE NAME = (*(TYPE*)(STACK+4*ARG))

/* Syscall implementations */
void halt(void) {
    power_off();
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
      break;
    case SYS_CREATE: {
      STACK_VAR(file, char*, stack, 1);
      STACK_VAR(initial_size, unsigned, stack, 2);
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
