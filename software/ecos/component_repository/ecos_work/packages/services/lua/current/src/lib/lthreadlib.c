//==========================================================================
//
//      lthreadlib.c
//
//
//
//==========================================================================
//####MITCOPYRIGHTBEGIN####
//
// -------------------------------------------
//
// Portions of this software derived from Lua 
// and LuaThreads see Copyright Notice in lua.h 
//
// -------------------------------------------
//
//####MITCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     Savin Zlobec
// Original data: Roberto Ierusalimschy and Diego Nehab
// Date:          2003-07-21
// Description:   eCos Lua multi threading support lib 
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <stdlib.h>
#include <pkgconf/lua.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#include "lua.h"
#include "lauxlib.h"
#include "lstate.h"

// --------------------------------------------------------------------------

static lua_ecos_thread_t *terminated_threads = NULL;

// --------------------------------------------------------------------------

static int 
class_register(lua_State      *L, 
               const luaL_reg *methods, 
               const luaL_reg *meta, 
               const char     *type)
{
    luaL_openlib(L, type, methods, 0);  
    luaL_newmetatable(L, type);
    luaL_openlib(L, 0, meta, 0); 
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, -3);             
    lua_rawset(L, -3);                 
    lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, -3);              
    lua_rawset(L, -3);                  
    lua_pop(L, 1);                    

    return 1;                      
}

static void* 
check_user_data(lua_State *L, int index, const char *type)
{
    void *data;

    luaL_checktype(L, index, LUA_TUSERDATA);
    data = luaL_checkudata(L, index, type);

    if (NULL == data) 
        luaL_typerror(L, index, type);

    return data;
}

static void
set_user_data_type(lua_State *L, const char *type)
{
    luaL_getmetatable(L, type);
    lua_setmetatable(L, -2);
}

// --------------------------------------------------------------------------

void
lua_ecosfreeterminatedthreads(void)
{
    cyg_scheduler_lock();
    {
        lua_ecos_thread_t *thread = terminated_threads;

        while (thread != NULL && thread->handle != cyg_thread_self())
        {
            cyg_thread_delete(thread->handle);
            terminated_threads = thread->next;
            free(thread->stack);
            free(thread);
            thread = terminated_threads;
        }
    }
    cyg_scheduler_unlock();
}

// --------------------------------------------------------------------------

#define E_THREAD "thread"

static void
thread_prog(cyg_addrword_t data) 
{
    lua_State        *L     = (lua_State *) data;
    lua_ecos_state_t *state = (lua_ecos_state_t *) lua_getuserspace(L);
    int i, n, ref;
   
    // get arguments table size 
    n = luaL_getn(L, 1);

    // push arguments from table 
    for (i = 1; i <= n; i++) 
        lua_rawgeti(L, 1, i);
    lua_remove(L, 1);

    // run program  
    lua_call(L, n, 0);

    ref = state->thread->ref;
    
    // put ourselves into terminated list
    cyg_scheduler_lock();
    {
        state->thread->next = terminated_threads;
        terminated_threads = state->thread;
        state->thread = NULL;
    }
    cyg_scheduler_unlock();

    // unreference the lua thread  
    lua_unref(L, ref);
}

static int 
e_thread_new(lua_State *L)
{
    lua_State        *L1;
    lua_ecos_state_t *state1;
    cyg_addrword_t   *lst;
    int ref, prio;

    // thread priority 
    prio = luaL_checkint(L, 1);

    // check arguments 
    luaL_checktype(L, 2, LUA_TFUNCTION);
    luaL_checktype(L, 3, LUA_TTABLE);
 
    // create new lua thread 
    L1 = lua_newthread(L);
    if (L1 == NULL)
    {
        luaL_error(L, "cannot create new stack");
        return 0; 
    }
    
    // free any terminated threads
    lua_ecosfreeterminatedthreads();
 
    // allocate new lua-ecos thread struct 
    state1 = (lua_ecos_state_t *) lua_getuserspace(L1);
    state1->thread = (lua_ecos_thread_t*) malloc(sizeof(lua_ecos_thread_t));
    if (!state1->thread)
    {
        luaL_error(L, "out of memory");
        return 0;
    }
    
    // reference the lua thread, so it won't get garbage collected 
    state1->thread->ref = lua_ref(L, 4);
    
    // adjust stacksize 
    lua_settop(L, 3);

    // move table and function from parent to child
    ref = lua_ref(L, 2);
    lua_getref(L1, ref);
    lua_unref(L, ref);  
    ref = lua_ref(L, 2);
    lua_getref(L1, ref);
    lua_unref(L, ref);  
    
    // create new ecos thread 
    state1->thread->stack = malloc(CYGNUM_LUA_THREAD_STACK_SIZE);
    if (!state1->thread->stack) 
    {
        lua_unref(L, state1->thread->ref);
        free(state1->thread);
        luaL_error(L, "out of memory");
        return 0;
    }
    
    state1->thread->next = NULL;

    cyg_thread_create(prio, thread_prog, (cyg_addrword_t) L1, "Lua thread",
                      state1->thread->stack,
                      CYGNUM_LUA_THREAD_STACK_SIZE,
                      &state1->thread->handle,
                      &state1->thread->thread_s);
    
    // create a pointer to new lua state wich will be passed back 
    lst = (cyg_addrword_t *) lua_newuserdata(L, sizeof(cyg_addrword_t));
    *lst = (cyg_addrword_t) L1;
    set_user_data_type(L, E_THREAD);

    return 1;
}

static int
e_thread_delay(lua_State *L)
{
    int delay = luaL_checkint(L, 1);
    cyg_thread_delay(delay);
    return 0;
}

static int
e_thread_resume(lua_State *L)
{
    lua_State *L1;
    lua_ecos_state_t *state;
   
    L1 = (lua_State *)
        *((cyg_addrword_t *) check_user_data(L, 1, E_THREAD));
    state = (lua_ecos_state_t *) lua_getuserspace(L1);
    
    cyg_thread_resume(state->thread->handle);
    return 0;
}

static int
e_thread_suspend(lua_State *L)
{
    lua_State *L1;
    lua_ecos_state_t *state;
   
    L1 = (lua_State *)
        *((cyg_addrword_t *) check_user_data(L, 1, E_THREAD));
    state = (lua_ecos_state_t *) lua_getuserspace(L1);
 
    cyg_thread_suspend(state->thread->handle);
    return 0;
}

static const luaL_reg e_thread_methods[] = {
    {"new",     e_thread_new},
    {"delay",   e_thread_delay},
    {"suspend", e_thread_suspend},
    {"resume",  e_thread_resume},
    {NULL, NULL}
};

static int 
e_thread_gc(lua_State *L)
{
    // nothing to do here - this is just 
    // holds the pointer to thread's lua_State
    return 0;
}

static int 
e_thread_tostring(lua_State *L)
{
    char buff[32];
    sprintf(buff, "%p", lua_touserdata(L, 1));
    lua_pushfstring(L, "Thread (%s)", buff);
    return 1;
}

static const luaL_reg e_thread_meta[] = {
    {"__gc",       e_thread_gc},
    {"__tostring", e_thread_tostring},
    {NULL, NULL}
};

static int 
e_thread_register(lua_State *L)
{
    return class_register(L, e_thread_methods, e_thread_meta, E_THREAD);
}

// --------------------------------------------------------------------------

#define E_MUTEX "mutex"

static int 
e_mutex_new(lua_State *L)
{
    cyg_mutex_t *mx;
    mx = (cyg_mutex_t *) lua_newuserdata(L, sizeof(cyg_mutex_t)); 
    cyg_mutex_init(mx);
    set_user_data_type(L, E_MUTEX);
    return 1;
}

static int 
e_mutex_lock(lua_State *L) 
{
    cyg_mutex_t *mx;
    mx = (cyg_mutex_t *) check_user_data(L, 1, E_MUTEX);
    lua_pushboolean(L, cyg_mutex_lock(mx));
    return 1;
}

static int 
e_mutex_unlock(lua_State *L) 
{
    cyg_mutex_t *mx;
    mx = (cyg_mutex_t *) check_user_data(L, 1, E_MUTEX);
    cyg_mutex_unlock(mx);
    return 0;
}

static const luaL_reg e_mutex_methods[] = {
    {"new",     e_mutex_new},
    {"lock",    e_mutex_lock},
    {"unlock",  e_mutex_unlock},
    {NULL, NULL}
};

static int 
e_mutex_gc(lua_State *L)
{
    cyg_mutex_t *mx;
    mx = (cyg_mutex_t *) check_user_data(L, 1, E_MUTEX);
    cyg_mutex_destroy(mx);
    return 0;
}

static int 
e_mutex_tostring(lua_State *L)
{
    char buff[32];
    sprintf(buff, "%p", lua_touserdata(L, 1));
    lua_pushfstring(L, "Mutex (%s)", buff);
    return 1;
}

static const luaL_reg e_mutex_meta[] = {
    {"__gc",       e_mutex_gc},
    {"__tostring", e_mutex_tostring},
    {NULL, NULL}
};

static int 
e_mutex_register(lua_State *L)
{
    return class_register(L, e_mutex_methods, e_mutex_meta, E_MUTEX);
}

// --------------------------------------------------------------------------

#define E_COND "cond"

static int 
e_cond_new(lua_State *L) 
{
    cyg_mutex_t *mx;
    cyg_cond_t  *cond;
    mx   = (cyg_mutex_t *) check_user_data(L, 1, E_MUTEX);
    cond = (cyg_cond_t *) lua_newuserdata(L, sizeof(cyg_cond_t));  
    cyg_cond_init(cond, mx);
    set_user_data_type(L, E_COND);
    return 1;
}

static int 
e_cond_wait(lua_State *L) 
{
    cyg_cond_t *cond;
    cond = (cyg_cond_t *) check_user_data(L, 1, E_COND);
    lua_pushboolean(L, cyg_cond_wait(cond));
    return 1;
}

static int 
e_cond_signal(lua_State *L) 
{
    cyg_cond_t *cond;
    cond = (cyg_cond_t *) check_user_data(L, 1, E_COND);
    cyg_cond_signal(cond);
    return 0;
}

static int
e_cond_broadcast(lua_State *L)
{
    cyg_cond_t *cond;
    cond = (cyg_cond_t *) check_user_data(L, 1, E_COND);
    cyg_cond_broadcast(cond);
    return 0;
}

static const luaL_reg e_cond_methods[] = {
    {"new",       e_cond_new},
    {"wait",      e_cond_wait},
    {"signal",    e_cond_signal},
    {"broadcast", e_cond_broadcast},
    {NULL, NULL}
};

static int 
e_cond_gc(lua_State *L) 
{
    cyg_cond_t *cond;
    cond = (cyg_cond_t *) check_user_data(L, 1, E_COND);
    cyg_cond_destroy(cond);
    return 0;
}

static int 
e_cond_tostring(lua_State *L)
{
    char buff[32];
    sprintf(buff, "%p", lua_touserdata(L, 1));
    lua_pushfstring(L, "Cond (%s)", buff);
    return 1;
}

static const luaL_reg e_cond_meta[] = {
    {"__gc",       e_cond_gc},
    {"__tostring", e_cond_tostring},
    {NULL, NULL}
};

static int 
e_cond_register(lua_State *L)
{
    return class_register(L, e_cond_methods, e_cond_meta, E_COND);
}

// --------------------------------------------------------------------------

void 
luaopen_thread(lua_State *L) 
{
    e_thread_register(L);
    e_mutex_register(L);
    e_cond_register(L);
}

// --------------------------------------------------------------------------
// EOF lthreadlib.c
