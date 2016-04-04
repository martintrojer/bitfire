// mtshell - Martin Trojer shell (martin.trojer@gmail.com)
// simple eCos shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <pkgconf/system.h>

#include "mtshell.h"
#include "mtshell_thread.h"
#include "base_funcs.h"
#include "ps_func.h"
#include "xmodem.h"

#ifdef USE_LUA
#include "lua_funcs.h"
#endif

//=============================================================================
//=============================================================================

command command_list[] = {	
//						 	{"test", "test", test_func},
							{"?", "Write this help text", help_func},
							{"cat", "Cat a file <cat file>", cat_func},
							{"fsinfo", "File system info", fsinfo_func},
							{"help", "Write this help text", help_func}, 
							{"kill", "kill thread <kill handle>", kill_func},
#ifdef USE_LUA
							{"luai", "Interactive LUA", luaint_func},
						   	{"lua", "LUA a file in background <lua file>", lua_func_bg},
//							{"luafg", "LUA a file in foreground", lua_func},
#endif
							{"ls", "List filesystem contents", ls_func},
							{"meminfo", "Memory info", meminfo_func},
//							{"mk", "Make file with name arg1", mkf_func},
							{"ps", "List info about threads", ps_func},
							{"rm", "Delete file <rm file>", rmf_func},
							{"xmodem", "Recieve file via xmodem <xmodem file size>", xmodem_func},
							{0,0,0} };

#define SHELL_STACK_SIZE 0x2000
static unsigned char mtshell_stack[SHELL_STACK_SIZE];
static thread_info stack_ti;

//=============================================================================
// Shell history
//=============================================================================

typedef struct _shell_hist_t {
	char command_buf[COM_BUF_LEN + 1];
	unsigned int pos;
	struct _shell_hist_t *prev, *next;
} shell_hist_t;

shell_hist_t *
		add_shell_history_node(shell_hist_t *history, unsigned int pos)
{
	shell_hist_t *s;

	if(!(s = (shell_hist_t *)malloc(sizeof(shell_hist_t))))
		return(NULL);

	memset(s, 0, sizeof(shell_hist_t));

	if(history) 
		history->next = s;

	s->prev = history;
	s->pos = pos;

	return s;
}

shell_hist_t *
		build_hist_list(unsigned int size)
{
	int i = size;
	shell_hist_t *sh = NULL, *start = NULL;

	while(i) {
		if(i == size) {
			if(!(start = sh = add_shell_history_node(NULL, i))) {
				printf("Aiee -- add_shell_history_node() returned NULL\n");
				return(NULL);
			}
		}
		else {
			if(!(sh = add_shell_history_node(sh, i))) {
				printf("Aiee -- add_shell_history_node() returned NULL\n");
				return(NULL);
			}
		}
		i--;
	}

	sh->next = start;
	start->prev = sh;

	return start;
}

//=============================================================================
// Args
//=============================================================================

// Get a command line argument
void get_arg(char *str, char *a, int num)
{
	int offset=0, argnr=0, i=0, len = strlen(str);
	
	a[0] = 0;
	
	while(i<=len)
	{
		if (i==len || str[i]==' ')
		{
			if (num == argnr) 
			{
				int j=offset;
				while(j<i && (j-offset)<ARG_SIZE) 
				{
					a[j-offset] = str[j];
					j++;
				}
				a[j-offset]=0;
				break;
			}
			argnr++;
			offset = i+1;
		}
		i++;
	}
}

//=============================================================================
// getchar_timout
// used by xmodem
//=============================================================================

// Get a char from stdin (with timeout)
// returns: 0==error/timeout,1==Data OK
int getchar_timeout(char *ch, int secs)
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	/* Watch stdin (fd 4) to see when it has input. */
	FD_ZERO(&rfds);
	FD_SET(4, &rfds);

	/* Wait up to five seconds. */
	tv.tv_sec = secs;
	tv.tv_usec = 0;

	retval = select(1, &rfds, NULL, NULL, &tv);
	/* Don't rely on the value of tv now! */

	if (retval == -1)
		return 0;		// This is an error
	else if (retval) {
		*ch = getchar();
		return 1;
	}
	else
		return 0; 		// This is an timeout

}

// Call an entered command
int call_command(char *str)
{
	int ctr=0;
	command c = command_list[ctr++];
	
	char arg0[ARG_SIZE];	
	get_arg(str,arg0,0);	// Find arg0

	while(c.name!=0) {
		if (strcmp(c.name,arg0)==0) {
			c.func(str);
			break;
		}
		c = command_list[ctr++];		
	}
	if (c.name==0)
		return -1;

	return 0;
}

#if 0
void test_thread(cyg_addrword_t data)
{
	thread_info *ti = (thread_info *) data;
	printf("Test\n");
	
	mtshell_thread_cleanup(ti);
	cyg_thread_exit();
}

//unsigned char tstack[CYGNUM_HAL_STACK_SIZE_MINIMUM];
//thread_info t_ti;
	
int test_func(char *s)
{
	unsigned char *l_stack = malloc(CYGNUM_HAL_STACK_SIZE_MINIMUM);
	thread_info *l_ti = malloc(sizeof(thread_info));
		
	l_ti->func = test_thread;
	strcpy(l_ti->name,"Test");
	l_ti->stack_ptr = l_stack;
	l_ti->stack_size = CYGNUM_HAL_STACK_SIZE_MINIMUM;
	l_ti->prio = 10;
	strcpy(l_ti->args,s);
	    
	mtshell_create_thread(l_ti);
}
#endif

void main_cleanup()
{
    // 'main' can't kill itself.

	cyg_handle_t thandle = 0, *thandleptr = &thandle;
	cyg_uint16 tid;
	cyg_thread_info tinfo;

	cyg_thread_get_next(thandleptr, &tid);

	do {
		cyg_thread_get_info(*thandleptr, tid, &tinfo);
		if(!strcmp(tinfo.name, "main")) {
			cyg_thread_kill(thandle);
			cyg_thread_delete(thandle);
		}
	}
	while(cyg_thread_get_next(thandleptr, &tid));
}

//=============================================================================
// Main Thread loop
//=============================================================================

void shell_thread(cyg_addrword_t data)
{
	char ch;
	char command_buf[COM_BUF_LEN + 1];
	unsigned char i = 0;
	shell_hist_t *sh;
	
	main_cleanup();
	
	// unbuffer stdout/stdin
	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	setvbuf(stdin, (char *)NULL, _IONBF, 0);
	
	sh = build_hist_list(SHELL_HIST_SIZE);
	
	printf(SHELL_PROMPT" ");
		
	// main command loop
	while(1)
	{
		do { 
			ch = getchar();
		} while(	!isprint(ch) && 
					(ch != '\b') &&		/* Backspace */ 
					(ch != 0x7F) &&		/* Backspace */
					(ch != 0x0A) &&		/* LF */
					(ch != 0x0D) &&		/* CR */
					(ch != 0x15) &&		/* Ctrl-U */
					(ch != 0x10) &&		/* Ctrl-N */
					(ch != 0x0E) &&		/* Ctrl-P */
					(ch != 0x41) &&		/* Up arrow */
					(ch != 0x42)		/* Down arrow */
			   );
		
		switch(ch)
		{
			case 0x5A:		/* These are both junk -- discard */
			case 0x5B:
				break;

			case 0x0E:		/* Ctrl-P */
			case 0x41:		/* Up arrow */
				/* If there's any input on the line already, erase it */
				if(i) {
					int x = i;
					while(x) {
						putchar('\b');
						putchar(' ');
						putchar('\b');
						x--;
					}		
				}
		
				sh = sh->prev;
				strncpy(command_buf, sh->command_buf, sizeof(command_buf) - 1);
				i = strlen(command_buf);
				printf("%s", command_buf);
				break;

			case 0x10:		/* Ctrl-N */
			case 0x42:		/* Down arrow */
				/* If there's any input on the line already, erase it */
				if(i) {
					int x = i;
					while(x) {
						putchar('\b');
						putchar(' ');
						putchar('\b');
						x--;
					}
				}
		
				sh = sh->next;
				strncpy(command_buf, sh->command_buf, sizeof(command_buf) - 1);
				i = strlen(command_buf);
				printf("%s", command_buf);
				break;
				
			case 0x0A:				/* LF */
			case 0x0D:				/* CR */
				if(!i)				/* commandbuf is NULL, begin a new line */
					printf("\n"SHELL_PROMPT" ");
				else
				{	
					if(command_buf[i - 1]==' ') 
						i--;
					command_buf[i] = '\0';
					printf("\n");
					if (call_command(command_buf))
						printf("No such command.\n");
					
					i=0;
					command_buf[i] = '\0';
					printf(SHELL_PROMPT" ");
				}
				break;

			case 0x15:			/* Ctrl-U */
				if(i) {
					while(i) {
						putchar('\b');
						putchar(' ');
						putchar('\b');
						i--;
					}
				}
				break;

			case 0x7F:
			case '\b':		/* Backspace */
				if(i) {
					i--;		/* Pointer back once */
					putchar('\b');	/* Cursor back once */
					putchar(' ');	/* Erase last char in screen */
					putchar('\b');	/* Cursor back again */
				}
				break;
					
					
			case ' ':
				/* Don't allow continuous or begin space(' ') */
				if((!i) || (i > COM_BUF_LEN) || (command_buf[i - 1] == ' ')) {
					/* do nothing */
				}
				else {
					command_buf[i] = ch;
					i++;
#ifndef CYGPKG_HAL_SYNTH
					putchar(ch);	/* display and store ch */
#endif
				}
				break;
								 
			default:			/* Normal key */
				if(i < COM_BUF_LEN) {
					command_buf[i] = ch;
					i++;
#ifndef CYGPKG_HAL_SYNTH
					putchar(ch);  /* Display and store ch */
#endif
				}
				break;		
		}
	}
}

//=============================================================================
// Program entry
// Called before scheduler is started
//=============================================================================

void cyg_user_start(void)
{
	FILE *sout;		// The new stdout .. will get fd=3
	FILE *sin;		// The new stdin  .. will get fd=4
	
#ifndef CYGPKG_HAL_SYNTH
	sout = fopen("/dev/ser1", "w");	
	stdout = stderr = sout;						// Redirect stdout/stderr

	sin = fopen("/dev/ser1", "r");				// Redirect stdin
	stdin = sin;
#endif
	
	stack_ti.func = &shell_thread;
	strcpy(stack_ti.name,"MTShell");
	stack_ti.stack_ptr = &mtshell_stack;
	stack_ti.stack_size = SHELL_STACK_SIZE;
	stack_ti.prio = 28;	
	mtshell_create_thread(&stack_ti);

//	create_cleanup_thread();

	mount(0,MOUNT_POINT,"mtffs");
}
	
	
#ifdef CYGPKG_HAL_SYNTH
void cyg_hal_plf_serial_putc( void )
{
}
#endif
