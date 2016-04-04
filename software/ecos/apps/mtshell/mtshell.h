// mtshell - Martin Trojer shell (martin.trojer@gmail.com)
// simple eCos shell

#ifndef _MTSHELL_H
#define _MTSHELL_H

#define SHELL_PROMPT "mtshell>"
#define MOUNT_POINT "/"

#define COM_BUF_LEN 64
#define ARG_SIZE 64
#define SHELL_HIST_SIZE	32

#define USE_LUA

// Get a char from stdin (with timeout)
// returns: 0==error/timeout,1==Data OK
int getchar_timeout(char *ch, int secs);

void get_arg(char *str, char *a, int num);

typedef struct
{
	char *name;
	char *expl;
	int (*func)(char *s);
	
} command;

command command_list[];

#endif
