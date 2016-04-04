
#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>        

#include <stdio.h>
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

//--------------------------------------------------------------------------- 

#define STACKSIZE CYGNUM_HAL_STACK_SIZE_TYPICAL

//--------------------------------------------------------------------------- 

static cyg_thread    thread_s;
static cyg_handle_t  thread_h;
static unsigned char stack[STACKSIZE];

//--------------------------------------------------------------------------- 

static int
delay(lua_State *L)
{
    int n = lua_gettop(L);

    if (n > 0 && lua_isnumber(L, 1))
        cyg_thread_delay((int)lua_tonumber(L, 1));
    return 0;
}

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
    lua_register(L, "delay", delay);
    
#include "lua/life_c.inc"

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
// end of lua_life.c 
