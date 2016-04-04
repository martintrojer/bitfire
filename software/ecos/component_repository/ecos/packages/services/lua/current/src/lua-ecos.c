//==========================================================================
//
//      lua-ecos.c
//
//
//
//==========================================================================
//####MITCOPYRIGHTBEGIN####
//
// -------------------------------------------
//
// See Copyright Notice in lua.h 
//
// -------------------------------------------
//
//####MITCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     Savin Zlobec
// Date:          2003-07-21
// Description:   eCos Lua code 
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/lua.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#include <stdlib.h>

#include "lua.h"
#include "lstate.h"

// --------------------------------------------------------------------------

#ifdef CYGIMP_LUA_THREAD_SAFE

#ifdef CYGIMP_LUA_THREAD_TRUE_PREEMPT

static void alarm_hook_handler(lua_State *L, lua_Debug *ar);

static void
set_alarm_hook(lua_State *L)
{
    lua_ecos_state_t *state = (lua_ecos_state_t *) lua_getuserspace(L);

    if (lua_gethook(L) != alarm_hook_handler)
    {
        state->old_hook  = lua_gethook(L);
        state->old_mask  = lua_gethookmask(L);
        state->old_count = lua_gethookcount(L);
        lua_sethook(L, alarm_hook_handler, LUA_MASKCALL | 
                                           LUA_MASKRET  | 
                                           LUA_MASKCOUNT, 1);
    }
}

static void
reset_alarm_hook(lua_State *L)
{
    lua_ecos_state_t *state = (lua_ecos_state_t *) lua_getuserspace(L);

    if (lua_gethook(L) == alarm_hook_handler)
    {
        lua_sethook(L, state->old_hook, state->old_mask, state->old_count);
    }
}

static void
alarm_hook_handler(lua_State *L, lua_Debug *ar)
{
    // before lua vm calls this hook it unlocks the
    // mutex, so here we can be preempted 
    cyg_scheduler_lock();
    {
        reset_alarm_hook(L);
    }
    cyg_scheduler_unlock();
    cyg_thread_yield();
}

static void
alarm_handler(cyg_handle_t alarm, cyg_addrword_t data)
{
    lua_State        *L     = (lua_State *)data;
    lua_ecos_state_t *state = (lua_ecos_state_t *)lua_getuserspace(L);
    cyg_tick_count_t  time;
    
    time = cyg_current_time() - state->lock_timestamp;

    // set the alarm hook if the current thread has had
    // the mutex locked for more then the permitted amount
    // of time  
    if (state->locked == 1 && time >= CYGNUM_LUA_THREAD_TICK)
    { 
        set_alarm_hook(L);
    }
}

#endif // CYGIMP_LUA_THREAD_TRUE_PREEMPT

void 
lua_ecoslock(lua_State* L)
{
    lua_ecos_state_t *state = (lua_ecos_state_t *) lua_getuserspace(L);
    
    cyg_mutex_lock(state->mutex);
#ifdef CYGIMP_LUA_THREAD_TRUE_PREEMPT    
    cyg_scheduler_lock();
    {
        state->lock_timestamp = cyg_current_time();
        state->locked = 1;
    }
    cyg_scheduler_unlock();
#endif
}

void 
lua_ecosunlock(lua_State* L)
{
    lua_ecos_state_t *state = (lua_ecos_state_t *) lua_getuserspace(L);

#ifdef CYGIMP_LUA_THREAD_TRUE_PREEMPT    
    cyg_scheduler_lock();
    {
        state->locked = 0;
        reset_alarm_hook(L);
    }
    cyg_scheduler_unlock();
#endif
    cyg_mutex_unlock(state->mutex);
}
 
void 
lua_ecosuserstateopen(lua_State *L) 
{
    lua_ecos_state_t *state = (lua_ecos_state_t *) lua_getuserspace(L);
#ifdef CYGIMP_LUA_THREAD_TRUE_PREEMPT    
    cyg_handle_t h;
#endif

    // create new mutex if this is the main thread, 
    // if not inherit mutex from the main thread - threads
    // wich share the global state must also share the  
    // mutex - to synchronize the access
    
    if (L == G(L)->mainthread)
    {
        state->mutex = (cyg_mutex_t *) malloc(sizeof(cyg_mutex_t));
        cyg_mutex_init(state->mutex);
    }
    else
    {
        lua_ecos_state_t *pstate;
        pstate = (lua_ecos_state_t *) lua_getuserspace(G(L)->mainthread);
        state->mutex = pstate->mutex;
    }

#ifdef CYGBLD_LUA_THREAD_LIB    
    // set ecos-lua thread data to NULL - only child threads
    // store data in here and they do it later on
    state->thread = NULL;
#endif  
    
#ifdef CYGIMP_LUA_THREAD_TRUE_PREEMPT  
    // locked flag and old lua hook data
    state->locked    = 0;
    state->old_hook  = NULL;
    state->old_mask  = 0;
    state->old_count = 0;
   
    // create an alarm used to interrupt the thread
    // in case it locks the mutex for more than the
    // premitted amount of time
    cyg_clock_to_counter(cyg_real_time_clock(), &h); 
    cyg_alarm_create(h, alarm_handler, (cyg_addrword_t) L, 
                     &state->tick_to_h, &state->tick_to);
    cyg_alarm_initialize(state->tick_to_h, 
                         cyg_current_time() + CYGNUM_LUA_THREAD_TICK, 
                         CYGNUM_LUA_THREAD_TICK);
#endif    
}

#ifdef CYGBLD_LUA_THREAD_LIB    
extern void lua_ecosfreeterminatedthreads(void);
#endif

void 
lua_ecosuserstateclose(lua_State *L) 
{
    lua_ecos_state_t *state = (lua_ecos_state_t *) lua_getuserspace(L);

#ifdef CYGIMP_LUA_THREAD_TRUE_PREEMPT    
    cyg_alarm_delete(state->tick_to_h);
#endif
    
    // only free the mutex if this is the main thread
    if (L == G(L)->mainthread)
    {
        cyg_mutex_unlock(state->mutex);
        cyg_mutex_destroy(state->mutex);
        free(state->mutex);
    }

#ifdef CYGBLD_LUA_THREAD_LIB    
    // if we are here the thread should 
    // already be in terminated threads list
    // (in case of mainthread state->thread 
    // should always be NULL)
    CYG_ASSERTC(NULL == state->thread);
    // free any terminated threads
    lua_ecosfreeterminatedthreads();
#endif  
}

#endif // CYGIMP_LUA_THREAD_SAFE

// --------------------------------------------------------------------------
// EOF lua-ecos.c
