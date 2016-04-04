// mtshell - Martin Trojer shell (martin.trojer@gmail.com)
// Cleanup thread

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <cyg/io/io.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/hal_arch.h> 
#include <cyg/kernel/kapi.h> 

#include "mtshell_thread.h"
#include "thread_cleanup.h"

cyg_mbox clean_mbox;
cyg_handle_t clean_mbox_handle;

static unsigned char clean_stack[CYGNUM_HAL_STACK_SIZE_MINIMUM];
static thread_info clean_ti;

static void cleanup_thread(cyg_addrword_t data)
{
    while(1) 
	{
		thread_info *ti = cyg_mbox_get(clean_mbox_handle);
		cyg_thread_delay(100);	// wait a sec for the thread to reach exit mode
		
		if (ti!=NULL)
		{		
			printf("Clean_Thread: Killing handle %d",ti->thread_handle);
			printf(" stack_ptr 0x%x\n",ti->stack_ptr);
//			cyg_thread_kill(ti->thread_handle);
			cyg_thread_delete(ti->thread_handle);
			
			free(ti->stack_ptr);
			free(ti);

		}
    }
}

void create_cleanup_thread(void)
{	
	cyg_mbox_create(&clean_mbox_handle, &clean_mbox);
	
	clean_ti.stack_ptr = clean_stack;
	clean_ti.stack_size = CYGNUM_HAL_STACK_SIZE_MINIMUM;
	strcpy(clean_ti.name,"Cleanup Thread");
	clean_ti.prio = 5;
	clean_ti.func = cleanup_thread;

    mtshell_create_thread(&clean_ti);
}

