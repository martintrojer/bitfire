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
#include <cyg/infra/diag.h>
#include <cyg/io/devtab.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
//#include <cyg/hal/var_ints.h>
//#include <cyg/hal/plf_io.h>
#include <cyg/kernel/kapi.h>
#include <cyg/io/io.h>
#include <cyg/hal/hal_arch.h>

#include <ctype.h>
#include <stdlib.h>
#include <shell.h>
#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

void get_thread_info();

shell_cmd("ps",
	 "Shows a list of threads",
	 "",
	 ps);

shell_cmd("dump",
	 "Shows a memory dump",
	 "",
	 hexdump);

shell_cmd("help",
	 "Displays a list of commands",
	 "",
	 help_func);

shell_cmd("?",
	 "Displays a list of commands",
	 "",
	 help_func2);

shell_cmd("kill",
	 "Kills a running thread",
	 "[thread ID]",
	 thread_kill);

shell_cmd("sp",
	 "Sets a threads priority",
	 "[thread ID]",
	 set_priority);

shell_cmd("timeron",
	 "Enables the timer interrupt",
	 "",
	 timer_on);

shell_cmd("timeroff",
	 "Enables the timer interrupt",
	 "",
	 timer_off);

shell_cmd("version",
	 "Shows build version",
	 "",
	 print_build_tag);

CMD_DECL(ps)
{
    get_thread_info();
    
    return SHELL_OK;
}

CMD_DECL(set_priority)
{
    cyg_handle_t thandle = 0;
    cyg_priority_t cur_pri, set_pri;
    unsigned int tid, pri;
    char *erptr = NULL;

    if(argc == 2) {
	thandle = strtol(argv[0], &erptr, 0);
	
	if(erptr && *erptr) {
	    SHELL_PRINT("Value '%s' is not a valid thread ID\n", argv[1]);
	    return SHELL_OK;
	}   	    

	set_pri = strtol(argv[1], &erptr, 0);
	
	if(erptr && *erptr) {
	    SHELL_PRINT("Value '%s' is not a valid thread priority\n", argv[2]);
	    return SHELL_OK;
	}

	cur_pri = cyg_thread_get_current_priority(thandle);
	
	SHELL_PRINT("Changing thread %d priority from %d to %d\n", thandle, cur_pri, set_pri);

	cyg_thread_set_priority(thandle, set_pri);

	cur_pri = cyg_thread_get_current_priority(thandle);
	SHELL_PRINT("Thread %d priority now @ %d\n", thandle, cur_pri);
    }
    else SHELL_PRINT("Usage: sp [tid] [priority]\n");

    return SHELL_OK;
}
	

CMD_DECL(help_func)
{
    ncommand_t *shell_cmd = __shell_CMD_TAB__;

    const char cmds[] = "Commands", dsr[] = "Descriptions";
    const char usage[] = "Usage", location[] = "File Location";
    unsigned char helpar[sizeof(cmds) + sizeof(dsr) + sizeof(usage) + sizeof(location) + 10 ];
    unsigned char i;

    snprintf(helpar, sizeof(helpar) - 1, "%%-11s %%-60s %%-20s %%-20s\n");
    
    SHELL_PRINT(helpar, cmds, dsr, usage, location);

    for(i = 0; i < sizeof(cmds) - 1; i++) putchar('-');
    SHELL_PRINT("    ");
    for(i = 0; i < sizeof(dsr) - 1; i++) putchar('-');
    SHELL_PRINT("                                                 ");
    for(i = 0; i < sizeof(usage) - 1; i++) putchar('-');
    SHELL_PRINT("                ");
    for(i = 0; i < sizeof(location) - 1; i++) putchar('-');
    putchar('\n');

    while(shell_cmd != &__shell_CMD_TAB_END__) {
	SHELL_PRINT("%-11s %-60s %-20s %-20s\n",
		   shell_cmd->name,
		   shell_cmd->descr,
		   shell_cmd->usage,
		   shell_cmd->file);
	shell_cmd++;
    }

    return SHELL_OK;
}

CMD_DECL(help_func2)
{
    return(help_func(argc, argv));
}

CMD_DECL(hexdump)
{
    unsigned int i = 0, j = 0;
    unsigned int len = 100, width = 16;
    unsigned char *buf;
    char *cp = NULL;

    switch(argc) {

    case 1:
	buf = (unsigned char *)strtol(argv[0], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid address\n", argv[0]);
	    return SHELL_OK;
	}

	break;

    case 2:
	buf = (unsigned char *)strtol(argv[0], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid address\n", argv[0]);
	    return SHELL_OK;
	}   

	len = strtol(argv[1], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid length\n", argv[1]);
	    return SHELL_OK;
	}   	    

	break;

    case 3:
	buf = (unsigned char *)strtol(argv[0], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid address\n", argv[0]);
	    return SHELL_OK;
	}   

	len = strtol(argv[1], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid length\n", argv[1]);
	    return SHELL_OK;
	}   	    

	width = strtol(argv[2], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid width\n", argv[2]);
	    return SHELL_OK;
	}   	    	
	
	break;

    default:
	SHELL_PRINT("Usage: hexdump [address] [length] [width]\n");
	return SHELL_OK;
    }

    SHELL_PRINT("%08X: ", (unsigned int)buf);
    
    for(i = 0; i < len; i++) {
	if(i && !(i % width)) {
	    j = i - width;
	    SHELL_PRINT("\t\t");
	    for(; j < i; j++) SHELL_PRINT("%c", isprint(buf[j]) ? buf[j] : '.');
	    SHELL_PRINT("\n%08X: ", (unsigned int)buf + i);
	    j = 0;
	}
	SHELL_PRINT("%02X ", buf[i]);
	j++;
    }
    
    if(j) {
	for(i = width - j; i > 0; i--) SHELL_PRINT("   ");
	SHELL_PRINT("\t\t"); 
	for(i = len - j; i < len; i++) SHELL_PRINT("%c", isprint(buf[i]) ? buf[i] : '.');
    }
    SHELL_PRINT("\n");

    return SHELL_OK;
}

CMD_DECL(thread_kill)
{
    cyg_handle_t th;
    char *erptr = NULL;

    if(argc != 1) {
	SHELL_DEBUG_PRINT("Usage: kill [tid]\n");
	return SHELL_OK;
    }

    th = strtol(argv[0], &erptr, 0);

    if(*erptr) {
	SHELL_PRINT("Invalid thread handle\n");
	return SHELL_OK;
    }

    cyg_thread_kill(th);
    cyg_thread_delete(th);

    return SHELL_OK;
}

CMD_DECL(timer_on)
{
    SHELL_PRINT("Turning timer interrupt on\n");
    cyg_interrupt_unmask(CYGNUM_HAL_INTERRUPT_RTC);

    return 0;
}

CMD_DECL(timer_off)
{
    SHELL_PRINT("Turning timer interrupt off\n");

    cyg_interrupt_mask(CYGNUM_HAL_INTERRUPT_RTC);

    return 0;
}
