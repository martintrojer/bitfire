// mtshell - Martin Trojer shell (martin.trojer@gmail.com)
// The ps (thread info) function

#include <stdio.h>

#ifndef _PS_FUNC_H
#define _PS_FUNC_H

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

int ps_func(char *s);

#endif
