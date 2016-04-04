// mtshell - Martin Trojer shell (martin.trojer@gmail.com)
// LUA functions

#include "mtshell.h"

#ifdef USE_LUA

#include <stdio.h>
#include <malloc.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <lua/lua.h>
#include <pkgconf/system.h>

#include "lua_funcs.h"
#include "mtshell_thread.h"

static int sleep(lua_State *L)
{
	int ticks = lua_tonumber(L, 1);
	
	cyg_thread_delay(ticks);
	return 0;
}

static void open_std_libs(lua_State *l)
{
	luaopen_base(l);   lua_settop(l, 0);
	luaopen_table(l);  lua_settop(l, 0);
	luaopen_io(l);     lua_settop(l, 0);
	luaopen_string(l); lua_settop(l, 0);
	luaopen_debug(l);  lua_settop(l, 0);
	luaopen_math(l);   lua_settop(l, 0);
	luaopen_thread(l); lua_settop(l, 0);
#ifndef CYGPKG_HAL_SYNTH
	luaopen_bitfire(l);   lua_settop(l, 0);
#endif

	lua_register(l,"sleep",sleep);
}

#ifndef PROMPT
#define PROMPT		"> "
#endif

#ifndef PROMPT2
#define PROMPT2		">> "
#endif

#ifndef PROGNAME
#define PROGNAME	"lua"
#endif
static const char *progname = PROGNAME;

#ifndef MAXINPUT
#define MAXINPUT	512
#endif
#ifndef lua_saveline
#define lua_saveline(L,line)	/* empty */
#endif

static char *my_fgets(char *buffer, int len, void *stream)
{
	int i=0;
	char ch;
	
	memset(buffer,0,len);
	
	while(i<len) {
		ch = getchar();
		
		if (isprint(ch)) {
			buffer[i] = ch;
			putchar(ch);
		}
		if (ch=='\n' || ch=='\r') {
			buffer[i] = ch;
			putchar('\n');
			break;
		}	
		if (ch==0x15)			/* Ctrl-U */
		{	
			if(i) 
			{
				while(i) {
					putchar('\b');
					putchar(' ');
					putchar('\b');
					i--;
				}
			}
			continue;
		}
		if (ch==0x7F || ch== '\b')		/* Backspace */
		{
			if(i) 
			{
				i--;		/* Pointer back once */
				putchar('\b');	/* Cursor back once */
				putchar(' ');	/* Erase last char in screen */
				putchar('\b');	/* Cursor back again */
			}
			continue;
		}
		if (ch==0x3)		// ctrl-c exists
			return NULL;
		
		i++;
	}
	return buffer;
}

static int lua_readline (lua_State *l, const char *prompt) {
	static char buffer[MAXINPUT];
	if (prompt) {
		fputs(prompt, stdout);
		fflush(stdout);
	}
	if (my_fgets(buffer, sizeof(buffer), stdin) == NULL)
		return 0;  /* read fails */
	else {
		lua_pushstring(l, buffer);
		return 1;
	}
}
static void l_message (const char *pname, const char *msg) {
	if (pname) fprintf(stderr, "%s: ", pname);
	fprintf(stderr, "%s\n", msg);
}
static void print_version (void) {
	l_message(NULL, LUA_VERSION "  " LUA_COPYRIGHT);
}
static const char *get_prompt (lua_State *L, int firstline) {
	const char *p = NULL;
	lua_pushstring(L, firstline ? "_PROMPT" : "_PROMPT2");
	lua_rawget(L, LUA_GLOBALSINDEX);
	p = lua_tostring(L, -1);
	if (p == NULL) p = (firstline ? PROMPT : PROMPT2);
	lua_pop(L, 1);  /* remove global */
	return p;
}
static int report (lua_State *L, int status) {
	const char *msg;
	if (status) {
		msg = lua_tostring(L, -1);
		if (msg == NULL) msg = "(error with no message)";
		l_message(progname, msg);
		lua_pop(L, 1);
	}
	return status;
}
static int lcall (lua_State *L,int narg, int clear) {
	int status;
	int base = lua_gettop(L) - narg;  /* function index */
	lua_pushliteral(L, "_TRACEBACK");
	lua_rawget(L, LUA_GLOBALSINDEX);  /* get traceback function */
	lua_insert(L, base);  /* put it under chunk and args */
	status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
	lua_remove(L, base);  /* remove traceback function */
	return status;
}
static int incomplete (lua_State *L, int status) {
	if (status == LUA_ERRSYNTAX &&
		   strstr(lua_tostring(L, -1), "near `<eof>'") != NULL) {
		lua_pop(L, 1);
		return 1;
		   }
		   else
			   return 0;
}static int load_string (lua_State *L) {
	int status;
	lua_settop(L, 0);
	if (lua_readline(L, get_prompt(L,1)) == 0)  /* no input? */
		return -1;
	if (lua_tostring(L, -1)[0] == '=') {  /* line starts with `=' ? */
		lua_pushfstring(L, "return %s", lua_tostring(L, -1)+1);/* `=' -> `return' */
		lua_remove(L, -2);  /* remove original line */
	}
	for (;;) {  /* repeat until gets a complete line */
		status = luaL_loadbuffer(L, lua_tostring(L, 1), lua_strlen(L, 1), "=stdin");
		if (!incomplete(L,status)) break;  /* cannot try to add lines? */
		if (lua_readline(L, get_prompt(L,0)) == 0)  /* no more input? */
			return -1;
		lua_concat(L, lua_gettop(L));  /* join lines */
	}
	lua_saveline(L, lua_tostring(L, 1));
	lua_remove(L, 1);  /* remove line */
	return status;
}

static void manual_input (lua_State *L) {
	int status;
	const char *oldprogname = progname;
	progname = NULL;
	while ((status = load_string(L)) != -1) {
		if (status == 0) status = lcall(L,0, 0);
		report(L,status);
		if (status == 0 && lua_gettop(L) > 0) {  /* any result to print? */
			lua_getglobal(L, "print");
			lua_insert(L, 1);
			if (lua_pcall(L, lua_gettop(L)-1, 0, 0) != 0)
				l_message(progname, lua_pushfstring(L, "error calling `print' (%s)",
						  lua_tostring(L, -1)));
		}
	}
	lua_settop(L, 0);  /* clear stack */
	fputs("\n", stdout);
	progname = oldprogname;
}

static unsigned char l_stack[0x10000];
static thread_info l_ti;
static thread_created = 0;

static void lua_thread_func(cyg_addrword_t data)
{	
	thread_info *ti = (thread_info *) data;
	char arg1[ARG_SIZE];
	
	thread_created = 1;
	
	get_arg(ti->args,arg1,1);
	if (strlen(arg1)>0)
	{
		lua_State *L = lua_open();
		open_std_libs(L);
		lua_dofile(L,arg1);
		lua_close(L);	
	}

//	mtshell_thread_cleanup(ti);
//	cyg_thread_exit();	
}

int luaint_thread_func(cyg_addrword_t data)
{	
	thread_created = 1;
	
	lua_State *L = lua_open();
	open_std_libs(L);
	print_version();
	manual_input(L);
	lua_close(L);	
}

int lua_func_bg(char *s)
{
	if (thread_created && l_ti.thread_handle!=0) {
		cyg_thread_delete(l_ti.thread_handle);		
		thread_created = 0;
	}
	
	l_ti.func = lua_thread_func;
	strcpy(l_ti.name,"Lua");
	l_ti.stack_ptr = l_stack;
	l_ti.stack_size = 0x10000;
	l_ti.prio = 29;
	strcpy(l_ti.args,s);
	    
	mtshell_create_thread(&l_ti);
}

int luaint_func(char *s)
{	
	if (thread_created && l_ti.thread_handle!=0) {
		cyg_thread_delete(l_ti.thread_handle);	
		l_ti.thread_handle=0;
		thread_created = 0;
	}
	
	l_ti.func = luaint_thread_func;
	strcpy(l_ti.name,"Lua");
	l_ti.stack_ptr = l_stack;
	l_ti.stack_size = 0x10000;
	l_ti.prio = 10;
	strcpy(l_ti.args,s);
	    
	mtshell_create_thread(&l_ti);
}

int lua_func(char *s)
{		
	char arg1[ARG_SIZE];
	
	get_arg(s,arg1,1);
	if (strlen(arg1)>0)
	{
		lua_State *L = lua_open();
		open_std_libs(L);
		lua_dofile(L,arg1);
		lua_close(L);	
	}
}

#endif