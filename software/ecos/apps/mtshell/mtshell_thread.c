// mtshell - Martin Trojer shell (martin.trojer@gmail.com)
// Shell thread creation

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/io/io.h> 
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/kernel/kapi.h>

#include "mtshell_thread.h"
#include "thread_cleanup.h"

void mtshell_thread_cleanup(thread_info *ti)
{
    if(ti)
		cyg_mbox_put(clean_mbox_handle, ti);
}

void mtshell_create_thread(thread_info *ti)
{
	cyg_thread_create(	ti->prio,
						ti->func,
						(cyg_addrword_t)ti,
						ti->name,
						(void *)ti->stack_ptr,
						ti->stack_size,
						&ti->thread_handle,
						&ti->thread);

	/* Start the thread running */
	cyg_thread_resume(ti->thread_handle);
	
//	printf("mtshell_create_thread(): %s",ti->name);
//	printf(" stack_ptr 0x%x\n",ti->stack_ptr);
}
