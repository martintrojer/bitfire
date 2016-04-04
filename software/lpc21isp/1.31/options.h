#ifndef OPTIONS_T
#define OPTIONS_T

void g_inet_addr(char *t, unsigned long *x); 	 	// read in x ip-address
void g_int(char *t, int *x);		// read in integer,long,short
void g_chk(char *t, int *x);		// if token seen in option *x get the default value 
void g_bool(char *t, char *x);		// read in boolean
void g_oct(char *t, int *x);		// read in integer,long,short
void g_hex(char *t, int *x);		// read in integer,long,short
void g_long(char *t, long *x);
void g_short(char *t, short *x);
void g_set(char *t, int *x);		// read in x a set defined by bits and ranges separated with commas e.g. 3,7-9
void g_text(char *t, char **x);		// read in x a null terminated string
void g_xtime(char *t, void *x);		// read in x current time extended with t seconds

typedef void (*p_t)(char *, void *x);

int oInit();						// initialize options system, done also automatically in oAdd, oOptions

int oAdd(char *key,char *help,char *def, p_t prog, void *addr);	// add a new commanline & inifile option
int oOption(int argc, char *argv[ ],p_t fileProg);   // process command line options
void oDefaults(char *filename);						// process inifile options

char *dump(char *txt,void *p,int len);				// return string containing dump of p by len bytes headed with txt
int debug(int level,const char *fmt, ...);			// output the line if level in set of --debug as debug set
void logprintf(const char *fmt, ...);				// log lines to file given with --logfile

#define die(c) Exit(c,__LINE__,__FILE__)

/*
	example of usage: 	
	
	main(uint argc, char *argv[])
	{
		oAdd("-p# --ports","listening SIP Port","9200",		(p_t)g_int,			&SipPort);
		oOption(argc,argv,datafiles);
		return process();
	}
*/

extern int DebugLevel;
#define DebugNeed(x) (DebugLevel & (1<<x))
#define dprintf printf("%s:%u:",__FILE__,__LINE__);debug

#endif
