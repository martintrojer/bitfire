/*
 * Copyright (c) 2005, 2006
 *
 * James Hook (james@wmpp.com) 
 * Chris Zimman (chris@wmpp.com)
 *
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cyg/infra/cyg_type.h>
//#include <cyg/hal/var_ints.h>
#include <cyg/kernel/kapi.h>

#include <commands.h>
#include <shell_err.h>

static const unsigned char * const thread_state_str[] = {
    "RUN",	/* 0 - Running */
    "SLEEP",	/* 1 - Sleeping */
    "CNTSLP",	/* 2 - Counted sleep */
    "SLPSET",	/* 3 - Sleep set */	
    "SUSP",	/* 4 - Suspended */
    NULL,	/* 5 - INVALID */
    NULL,	/* 6 - INVALID */
    NULL,	/* 7 - INVALID */
    "CREAT",	/* 8 - Creating */
    NULL,	/* 9 - INVALID */
    NULL,	/* 10 - INVALID */
    NULL,	/* 11 - INVALID */
    NULL,	/* 12 - INVALID */
    NULL,	/* 13 - INVALID */
    NULL,	/* 14 - INVALID */
    NULL,	/* 15 - INVALID */
    "EXIT"	/* 16 - Exiting */
};

/*
 * This function produces a list of the threads
 * that are currently scheduled.  The output is roughly
 * analagous to 'ps', with the exception of VSS for obvious
 * reasons
 */

void get_thread_info()
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

    SHELL_PRINT("%-8s %-2s %-6s %-15s %-2s %-2s %-10s %-10s %-10s\n",
	       "-------",
	       "--",
	       "------",
	       "----------------------",
	       "--",
	       "--",
	       "----------",
	       "----------",
	       "----------");
    SHELL_PRINT("%-8s %-2s %-6s %-22s %-2s %-2s %-10s %-10s %-10s\n",
	       "Handle",
	       "ID",
	       "State",
	       "Name",
	       "SP",
	       "CP",
	       "Stack Base",
	       "Stack Size",
	       "Stack Used");
    SHELL_PRINT("%-8s %-2s %-6s %-22s %-2s %-2s %-10s %-10s %-10s\n",
	       "-------",
	       "--",
	       "------",
	       "----------------------",
	       "--",
	       "--",
	       "----------",
	       "----------",
	       "----------");

    do {
	cyg_thread_get_info(*thandleptr, tid, &tinfo);
	
	SHELL_PRINT("%-8d %-2d %-6s %-22s %-2d %-2d 0x%08x 0x%08x 0x%08x\n",
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

    SHELL_PRINT("Total Stack Size:  %d\n", total_stack_size);
    SHELL_PRINT("Total Stack Used:  %d\n", total_stack_used);
#if defined(THREADINFO_DEBUG)
    printf("Here I am:\n");
    printf("Handle-> %d\n", tinfo.handle);
    printf("ID-> %d\n", tinfo.id);
    printf("State-> 0x%08x\n", tinfo.state);
    printf("Name-> '%s'\n", tinfo.name);
    printf("Set Priority-> 0x%08x\n", tinfo.set_pri);
    printf("Current Priority-> 0x%08x\n", tinfo.cur_pri);
    printf("Stack base-> 0x%08x\n", tinfo.stack_base);
    printf("Stack size-> 0x%08x\n", tinfo.stack_size);
    printf("Stack used-> 0x%08x\n", tinfo.stack_used);
#endif	/* defined(THREADINFO_DEBUG) */
}
