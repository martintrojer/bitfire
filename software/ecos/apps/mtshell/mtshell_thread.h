// mtshell - Martin Trojer shell (martin.trojer@gmail.com)
// Shell thread creation

#ifndef _MTSHELL_THREAD_H
#define _MTSHELL_THREAD_H

#include <cyg/kernel/kapi.h>
#include "mtshell.h"
    
#define NAME_SIZE 32
    
typedef struct {
	void (*func)(cyg_addrword_t data);
	char name[NAME_SIZE];			// Name of thread
	unsigned char *stack_ptr;		// Pointer to the stack
	unsigned int stack_size;		// Size of the stack
	int prio;						// Prio
	char args[ARG_SIZE];			// Argument to thread function
	cyg_thread thread;				// Thread object
	cyg_handle_t thread_handle;		// Thread handle
} thread_info;
    
void mtshell_create_thread(thread_info *ti);
void mtshell_thread_cleanup(thread_info *ti);

#endif
