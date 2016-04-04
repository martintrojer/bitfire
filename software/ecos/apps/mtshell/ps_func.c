// mtshell - Martin Trojer shell (martin.trojer@gmail.com)
// The ps (thread info) function

#include "ps_func.h"

#include <cyg/kernel/kapi.h>

/*
 * This function produces a list of the threads
 * that are currently scheduled.  The output is roughly
 * analagous to 'ps', with the exception of VSS for obvious
 * reasons
 */

int ps_func(char *s)
{
	cyg_handle_t thandle = 0, *thandleptr = &thandle;
	cyg_uint16 tid;
	cyg_thread_info tinfo;
	int total_stack_size = 0, total_stack_used = 0;
    /*
	* Because we're running this on ourselves,
	* it's basically guaranteed to be OK the first
	* time through
	*/
	cyg_thread_get_next(thandleptr, &tid);

	printf(	"%-8s %-2s %-6s %-15s %-2s %-2s %-10s %-10s %-10s\n",
			"-------",
			"--",
			"------",
			"----------------------",
			"--",
			"--",
			"----------",
			"----------",
			"----------");
	printf(	"%-8s %-2s %-6s %-22s %-2s %-2s %-10s %-10s %-10s\n",
		   	"Handle",
			"ID",
			"State",
			"Name",
			"SP",
			"CP",
			"Stack Base",
			"Stack Size",
			"Stack Used");
	printf(	"%-8s %-2s %-6s %-22s %-2s %-2s %-10s %-10s %-10s\n",
		   	"-------",
			"--",
			"------",
			"----------------------",
			"--",
			"--",
			"----------",
			"----------",
			"----------");

	do 
	{
		cyg_thread_get_info(*thandleptr, tid, &tinfo);
	
		printf("%-8d %-2d %-6s %-22s %-2d %-2d 0x%08x 0x%08x 0x%08x\n",
			   tinfo.handle,
	
		tinfo.id,
		thread_state_str[tinfo.state],
		tinfo.name,
		tinfo.set_pri,
		tinfo.cur_pri,
		tinfo.stack_base,
		tinfo.stack_size,
		tinfo.stack_used);

		total_stack_size += tinfo.stack_size;
		total_stack_used += tinfo.stack_used;
	} while(cyg_thread_get_next(thandleptr, &tid));

	printf("Total Stack Size:  %d\n", total_stack_size);
	printf("Total Stack Used:  %d\n", total_stack_used);
}
