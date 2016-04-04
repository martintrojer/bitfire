
#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>        

#include <stdio.h>
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

//--------------------------------------------------------------------------- 

// Stack size was adjusted for eCos synthetic target -
// the reason for its size is 'sieve.lua' wich seems to be
// very stack hungry
#define STACKSIZE (128*1024) //CYGNUM_HAL_STACK_SIZE_TYPICAL

//--------------------------------------------------------------------------- 

static cyg_thread    thread_s;
static cyg_handle_t  thread_h;
static unsigned char stack[STACKSIZE];

//--------------------------------------------------------------------------- 

static void 
open_std_libs(lua_State *l) 
{
    luaopen_base(l);   lua_settop(l, 0); 
    luaopen_table(l);  lua_settop(l, 0); 
    luaopen_io(l);     lua_settop(l, 0); 
    luaopen_string(l); lua_settop(l, 0); 
    luaopen_debug(l);  lua_settop(l, 0); 
    luaopen_math(l);   lua_settop(l, 0); 
}

static void
thread_prog(cyg_addrword_t data)
{
    lua_State *L = lua_open();  
    open_std_libs(L);
    
    diag_printf("\n\n#### fib.luac ####\n\n");
#include "lua/fib_c.inc"

    diag_printf("\n\n#### fib.lua ####\n\n");
#include "lua/fib.inc"
   
    diag_printf("\n\n#### fibfor.lua ####\n\n");
#include "lua/fibfor.inc"

    diag_printf("\n\n#### bisect.lua ####\n\n");
#include "lua/bisect.inc"
  
    diag_printf("\n\n#### sort.lua ####\n\n");
#include "lua/sort.inc"
  
    diag_printf("\n\n#### factorial.lua ####\n\n");
#include "lua/factorial.inc"
    
    diag_printf("\n\n#### printf.lua ####\n\n");
#include "lua/printf.inc"

    diag_printf("\n\n#### cf.lua ####\n\n");
#include "lua/cf.inc"
 
    diag_printf("\n\n#### sieve.lua ####\n\n");
#include "lua/sieve.inc"
   
    lua_close(L);

    diag_printf("\n\n#### DONE ####\n\n");
}

//--------------------------------------------------------------------------- 

void 
cyg_user_start(void)
{
    cyg_thread_create(4, thread_prog, (cyg_addrword_t) 0,
            "Thread 1", (void *) stack, STACKSIZE,
            &thread_h, &thread_s);

    cyg_thread_resume(thread_h);
}

//---------------------------------------------------------------------------
// end of lua_tests.c 
