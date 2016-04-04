/*
** $Id: lnum.h,v ... $
** Internal Number model
** See Copyright Notice in lua.h
*/

#ifndef lnum_h
#define lnum_h

#include <math.h>

#include "lobject.h"


/*
** The luai_num* macros define the primitive operations over numbers.
*/
/* AK 30-Aug-05: Is there actually any real use of defining these? */
#define luai_numadd(a,b)	((a)+(b))
#define luai_numsub(a,b)	((a)-(b))
#define luai_nummul(a,b)	((a)*(b))
#define luai_numdiv(a,b)	((a)/(b))
#define luai_nummod(a,b)	((a) - floor((a)/(b))*(b))
#define luai_numpow(a,b)	(pow(a,b))
#define luai_numunm(a)		(-(a))
#define luai_numeq(a,b)	    ((a)==(b))
#define luai_numlt(a,b)	    ((a)<(b))
#define luai_numle(a,b)	    ((a)<=(b))
#define luai_numisnan(a)	(!luai_numeq((a), (a)))

#ifdef LUA_NUMBER_MODE_COMPLEX
void luai_vectunm( lua_Number a, lua_Number b, lua_Number *ra, lua_Number *rb );
void luai_vectadd( lua_Number a, lua_Number b, lua_Number c, lua_Number d,
                   lua_Number *ra, lua_Number *rb );
void luai_vectsub( lua_Number a, lua_Number b, lua_Number c, lua_Number d,
                   lua_Number *ra, lua_Number *rb );
void luai_vectmul( lua_Number a, lua_Number b, lua_Number c, lua_Number d,
                   lua_Number *ra, lua_Number *rb );
void luai_vectmod( lua_Number a, lua_Number b, lua_Number c, lua_Number d,
                   lua_Number *ra, lua_Number *rb );
void luai_vectdiv( lua_Number a, lua_Number b, lua_Number c, lua_Number d,
                   lua_Number *ra, lua_Number *rb );
void luai_vectpow( lua_Number a, lua_Number b, lua_Number c, lua_Number d,
                   lua_Number *ra, lua_Number *rb );
#endif

lu_bool try_addint( lua_Integer *r, lua_Integer ib, lua_Integer ic );
lu_bool try_subint( lua_Integer *r, lua_Integer ib, lua_Integer ic );
lu_bool try_mulint( lua_Integer *r, lua_Integer ib, lua_Integer ic );
lu_bool try_divint( lua_Integer *r, lua_Integer ib, lua_Integer ic );
lu_bool try_modint( lua_Integer *r, lua_Integer ib, lua_Integer ic );
lu_bool try_powint( lua_Integer *r, lua_Integer ib, lua_Integer ic );
lu_bool try_unmint( lua_Integer *r, lua_Integer ib );

LUAI_FUNC int luaO_str2d (const char *s, lua_Number *res1, lua_Integer *res2);
lu_bool luaO_num2buf( char *s, const TValue *o );

#endif
