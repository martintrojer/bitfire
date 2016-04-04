/*

 This module contains routines to process easily
	- command line options
	- initialization file
	- runtime debugging
	
	
	(c) 1985 - 2005 Martti.Lumme(a)iki.fi

*/


#include "options.h"
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
//  Options handlers and definitions and help texts
time_t startTime;
int DebugLevel=0;
int Magic=0;
char *IniFile=0;


void Exit(int c,int line, char *file)
{
	printf("die(%u) at %s line %u\n",c,file,line);
	exit(c);
}

#include <stdarg.h>
int debug(int level,const char *fmt, ...)
{
static int cnt=0;
    va_list ap;

    if (DebugLevel & (1<<level)){
		if(!cnt) printf("#cnt:time:level:description\n");
		printf("#%3u:%3u:%u:",cnt++,time(0) - startTime,level);
		va_start(ap, fmt);
		vprintf( fmt, ap);
		printf("\n");
		va_end(ap);
	}
	return level;
}


FILE *logfile=0;
char *LogFileName;


void logprintf(const char *fmt, ...)
{
static int cnt=0;
time_t tim;
    va_list ap;
	if(!*LogFileName) return ;
	if(!logfile){
		logfile = fopen(LogFileName,"a");
		if(!logfile){
			perror("log open");
			*LogFileName=0;
		}
	}
    {
		if(!cnt) {
			time(&tim);
			fprintf(logfile,"Log start %s",ctime(&tim));
		}
		fprintf(logfile,"%3u:",cnt++);
		va_start(ap, fmt);
		vfprintf(logfile, fmt, ap);
		fprintf(logfile,"\n");
		va_end(ap);
	}

}

char *dump(char *txt,void *p,int len)
{
static unsigned char asc[17];
static char *bf=0;
static lastlen=0;
int i;
char *bp;
unsigned char *x;
time_t t;
	if(lastlen <= len){
		if(bf) free(bf);
		bf = (char*)malloc(7 * len);
		lastlen=len;
	}
	bp = bf;
	x = p;
	time(&t);
	sprintf(bp,"\n%s: len=%u \n",txt,len);


	for(i = 0; i < len; i++){
		if(i % 16 == 0){
			bp += sprintf(bp,"%04x: ",i);
			asc[16]=0;
		}
		bp += sprintf(bp,"%02x ",x[i]);
		asc[i % 16] = isprint(x[i])|| x[i]>=0xa0?x[i]:'.';
		if(i % 16 == 15) bp += sprintf(bp,"'%s'\n",asc);
	}
	i %= 16;
	asc[i] = 0;
	if(i != 0) bp += sprintf(bp,"%*c'%s'\n",3*(16 - i),' ',asc);
	return bf;
}

void g_inet_addr(char *t, unsigned long *x)
{
  if(x) *x = inet_addr(t);
}

void g_int(char *t, int *x)
{
  if(x) *x = strtol(t,0,0);
}

void g_chk(char *t, int *x)
{
}

void g_bool(char *t, char *x)
{
  if(x) {
	if(t)
		switch(*t){
		case '+':
		case '1':*x=1;break;
		case '-': 
		case '0':*x=0;break;
		}
	else *x = (*x==0);
	}
}

void g_oct(char *t, int *x)
{
  if(x) *x = strtol(t,0,8);
}

void g_hex(char *t, int *x)
{
  if(x) *x = strtol(t,0,16);
}


void g_long(char *t, long *x)
{
  if(x) *x = strtol(t,0,0);
}

void g_short(char *t, short *x)
{
  if(x) *x = strtol(t,0,0);
}
void g_set(char *t, int *x)
{
char *e;
int i,j;
	if(!x) return;
	for(*x = 0;*t; t=e){
		i = strtol(t,&e,0);
		if(e == t) break;
		if(*e =='-'){
			e++;
			j = strtol(e,&e,0);
			*x |= (1<<(j+1)) -(1<<(i));
		}
		else *x |= (1<<i);
		if(*e == ',') e++;
		else break;
	}
}

void g_debug(char *t, int *x)
{
	g_set(t,x);
	if(!*x) printf("debug is %u\n",*x);
	if(*x) *x |= 1;  // zero always on
}

void g_text(char *t, char **x)
{
	if(x){
		if(*x) free(*x);
		*x = strdup(t);
	}
}

void g_xtime(char *t, void *x)
{
  *(time_t*)x = time(0) + strtol(t,0,0);
}

void g_none(char *t, void *x)
{
  printf("testing %s\n",t);
}

typedef struct{
	char *key;
	char *def;
	char *help;
	p_t prog;
	void *addr;
} option_t;

static option_t **Options=0;
static int OptionCnt=0;
static int OptionTop=0;

int ocmp(const void *a, const void *b)
{
option_t *aa,*bb;
	if(!a) return 1;
	if(!b) return -1;
	aa = *(option_t **)a;
	bb = *(option_t **)b;

	return strcmp(aa->key,bb->key);
}

int oAdd(char *key,char *help,char *def, p_t prog, void *addr)
{
option_t **tmp;
option_t *op;
//printf("oADD: %s=%s\n",key,def);
	if(OptionCnt>=OptionTop){
		OptionTop=OptionTop+10;
		tmp = (option_t**)malloc(sizeof(option_t*) * OptionTop);
		if(!tmp) die(2);
		if(OptionCnt) memcpy(tmp,Options,sizeof(option_t*) * OptionCnt);
		if(Options) free(Options);
		Options = tmp;
	}

	op = (option_t*)malloc(sizeof(option_t));
	if(!op) die(3);
	op->key = key;
	op->def = def;
	op->help = help;
	op->prog = prog;
	op->addr = addr;
	if(prog && def && addr) prog(def,addr);
	Options[OptionCnt++] = op;
	if(OptionCnt==1){
		oInit();
	}
	qsort(Options,OptionCnt,sizeof(option_t*),ocmp);
}

static void help(char *t, char *v)
{
option_t *op;
int i;

	if(t) printf(t,v);
	printf("Usage:prog [option] files\n");
	for(i=0;i<OptionCnt;i++){
		op = Options[i];
		printf("%s %*s %s",op->key,20-strlen(op->key)," ",op->help);
		if(op->def && *op->def) printf("='%s'",op->def);
		printf("\n");
	}
	printf("Version %u " __DATE__ " " __TIME__" (c)2005 Martti.Lumme(at)iki.fi\n",Magic);
	exit(0);
}

static void defile(char *t, char *v)
{
	if(!t) return;
	if(IniFile) free(IniFile);
	IniFile = strdup(t);
	oDefaults(IniFile);
}

void oFile(char *n,void *p)
{
	printf("FILE:'%s'\n",n);
}

int oInit()
{
static char done=0;

	if(done) return 0;
	done=1;

	oAdd("- # --debug","Debug level","0",			(p_t)g_debug,			&DebugLevel);
	oAdd("- # --logfile",	"Logfilename. If empty no logfile ","backup.log",(p_t)g_text,		&LogFileName);	
	oAdd("-?  --help",	"Read screen. Note than on # option needs parameter","",	(p_t)help,	0);

	time(&startTime);
	return 1;
}

// do the key in option or inifile match with the identifiers

static int found(char *hole, char *key)
{
char *p;

	if(hole[1] != key[1]){ 			// match single char id 
		p = strstr(hole,"--");
		if(!p) return 0; 			// match with text id 
		while(*p == '-') p++;		// remove the possible sign if read as option
		while(*key == '-') key++;  
		if(strcmp(p,key)) return 0;
	}
	if(hole[2] == '#') return 2; 	// need to skip next parameter
	else return 1;	
}

int oOption(int argc, char *argv[ ],p_t fileProg)
{
int i,j,r;
char *arg;
struct stat sb;
option_t *opt;
	
	oInit(); 
	if(!Magic) if(!stat(argv[0],&sb)) Magic= sb.st_ino;
	
	for(i=1; i<argc; i++){
		arg = argv[i];
		if(*arg == '-'){
			for(j=0;j<OptionCnt;j++){
				opt = Options[j];
				r = found(opt->key,arg);
				if(opt->prog) {
					if(r == 1){
						if(opt->prog == (p_t)&g_chk){
							if(opt->addr)
								*(int*)opt->addr = strtol(opt->def,0,0);
						}
						else opt->prog("",opt->addr);
						break;
					}
					if(r == 2){ // -x# but not --x
						if(arg[2] && arg[1]!='-') opt->prog(arg+2,opt->addr);
						else if(i+1<argc) opt->prog(argv[++i],opt->addr);
						else printf("Cannot find parameter for option '%s'\n",arg);
						break;
					}	
				}
			}
			if(j==OptionCnt){
				help("Invalid usage of '%s'\n",arg);
			}
		}else{
			if(fileProg) {
				fileProg(arg,0);
			}
		}
	}
}

//  Defaults hardcoded and then from file
void oDefaults(char *filename)
{
FILE *f;
char ln[100];
char *cp;
option_t *op;
int i;

	oInit();	
// defaults from file.ini

	if(!*filename) filename=IniFile;
	if(!filename || !*filename) return;
	
	f = fopen(filename,"r");
	if(!f) return;

	while(!feof(f)){
		fgets(ln,sizeof(ln),f);

		cp = strchr(ln,'\n');
		if(cp) *cp = 0;
		cp = strchr(ln,'\r');
		if(cp) *cp = 0;

		if(*ln) // to avoid testing with NULL
		  if(*ln != '#') {
			cp = strchr(ln,'=');
			if(cp) *cp++ = 0;
			for(i=0;i<OptionCnt;i++){
				op = Options[i];
				if(found(op->key,ln)) {
					if(op->prog) op->prog(cp,op->addr);
					break;
				}
			}
		}
	}
	fclose(f);
}
