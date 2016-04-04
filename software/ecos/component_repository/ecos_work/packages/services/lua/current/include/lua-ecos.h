#ifndef CYGONCE_LUA_ECOS_H
#define CYGONCE_LUA_ECOS_H
//==========================================================================
//
//      lua-ecos.h
//
//
//
//==========================================================================
//####MITCOPYRIGHTBEGIN####
//
// -------------------------------------------
//
// Portions of this software derived from Lua, 
// see Copyright Notice in lua.h 
//
// -------------------------------------------
//
//####MITCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     Savin Zlobec
// Original data: Roberto Ierusalimschy
// Date:          2003-07-20
// Description:   eCos Lua configuration
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/lua.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_ass.h>
#include <cyg/kernel/kapi.h>

//
// number type configuration for Lua core
//

#ifdef CYGIMP_LUA_NUMBER_TYPE_DOUBLE 
#define LUA_NUMBER          double
#define LUA_NUMBER_SCAN     "%lf"
#define LUA_NUMBER_FMT      "%.14g"
#endif

#ifdef CYGIMP_LUA_NUMBER_TYPE_FLOAT 
#define LUA_NUMBER          float
#define LUA_NUMBER_SCAN     "%f"
#define LUA_NUMBER_FMT      "%.5g"
#endif

#ifdef CYGIMP_LUA_NUMBER_TYPE_LONG
#define LUA_NUMBER          long
#define LUA_NUMBER_SCAN     "%ld"
#define LUA_NUMBER_FMT      "%ld"
#define lua_str2number(s,p) strtol((s), (p), 10)
#endif

#ifdef CYGIMP_LUA_NUMBER_TYPE_INT
#define LUA_NUMBER          int
#define LUA_NUMBER_SCAN     "%d"
#define LUA_NUMBER_FMT      "%d"
#define lua_str2number(s,p) ((int) strtol((s), (p), 10))
#endif

#ifdef CYGIMP_LUA_USE_FASTROUND 
#define lua_number2int(i,d)	__asm__("fldl %1\nfistpl %0":"=m"(i):"m"(d))
#endif

//
// Lua math lib configuration
//

#ifdef CYGIMP_LUA_USE_DEGREES
#define USE_DEGREES
#endif

//
// Lua asserts
//

#define lua_assert(_c_) CYG_ASSERTC(_c_)

//
// Lua threads configuration
//

struct lua_State;

void *lua_getuserspace(struct lua_State *L);

#ifdef CYGIMP_LUA_THREAD_SAFE

#define lua_lock(L)    lua_ecoslock(L)
#define lua_unlock(L)  lua_ecosunlock(L)
#define LUA_USERSTATE  lua_ecos_state_t 

#define lua_userstateopen  lua_ecosuserstateopen
#define lua_userstateclose lua_ecosuserstateclose

#ifdef CYGBLD_LUA_THREAD_LIB
typedef struct lua_ecos_thread_t
{
    cyg_thread                thread_s;
    cyg_handle_t              handle;
    void                     *stack;
    int                       ref;
    struct lua_ecos_thread_t *next;
} lua_ecos_thread_t;

void luaopen_thread(struct lua_State *L);
#endif

typedef struct lua_ecos_state_t
{
    cyg_mutex_t       *mutex;
#ifdef CYGBLD_LUA_THREAD_LIB    
    lua_ecos_thread_t *thread;
#ifdef CYGIMP_LUA_THREAD_TRUE_PREEMPT    
    cyg_alarm          tick_to;
    cyg_handle_t       tick_to_h;
    int                locked;
    cyg_tick_count_t   lock_timestamp;
    void              *old_hook;
    int                old_mask;
    int                old_count;
#endif
#endif    
} lua_ecos_state_t;

void lua_ecoslock(struct lua_State* L);
void lua_ecosunlock(struct lua_State* L);
void lua_ecosuserstateopen(struct lua_State* L);
void lua_ecosuserstateclose(struct lua_State* L);

#endif // CYGIMP_LUA_THREAD_SAFE 

#endif // CYGONCE_LUA_ECOS_H

// --------------------------------------------------------------------------
// EOF lua-ecos.h
