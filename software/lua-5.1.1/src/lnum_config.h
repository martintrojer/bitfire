/*
** $Id: lnum_config.h,v ... $
** Internal Number model
** See Copyright Notice in lua.h
*/

#ifndef lnum_config_h
#define lnum_config_h

/**
** LUA_NUMBER_MODE_LDOUBLE_INT64    long double precision FP (80+), 64-bit integers
** LUA_NUMBER_MODE_DOUBLE (default) double precision for both FP and integers (32-bit)
** LUA_NUMBER_MODE_DOUBLE_INT32     double precision FP, 32-bit integers (if no FPU)
** LUA_NUMBER_MODE_DOUBLE_INT64     double precision FP, 64-bit integers
** LUA_NUMBER_MODE_FLOAT            single precision for both FP and integers (23-bit)
** LUA_NUMBER_MODE_FLOAT_INT32      single precision FP, 32-bit integers
** LUA_NUMBER_MODE_FLOAT_INT64      single precision FP, 64-bit integers
** LUA_NUMBER_MODE_BCD_INT32        Binary Coded Decimals, 32-bit integer optimized
** LUA_NUMBER_MODE_COMPLEX          Complex (a+bi) number mode, both double.
**/
#if defined(LUA_NUMBER_MODE_LDOUBLE_INT64)
# define LUA_NUMBER_MODE " (long double+int64)"
#elif defined(LUA_NUMBER_MODE_DOUBLE)
# define LUA_NUMBER_MODE ""
#elif defined(LUA_NUMBER_MODE_DOUBLE_INT32)
# define LUA_NUMBER_MODE " (double+int32)"
#elif defined(LUA_NUMBER_MODE_DOUBLE_INT64)
# define LUA_NUMBER_MODE " (double+int64)"
#elif defined(LUA_NUMBER_MODE_FLOAT)
# define LUA_NUMBER_MODE " (float)"
#elif defined(LUA_NUMBER_MODE_FLOAT_INT32)
# define LUA_NUMBER_MODE " (float+int32)"
#elif defined(LUA_NUMBER_MODE_FLOAT_INT64)
# define LUA_NUMBER_MODE " (float+int64)"
#elif defined(LUA_NUMBER_MODE_BCD_INT32)
# define LUA_NUMBER_MODE " (BCD)"
#elif defined(LUA_NUMBER_MODE_COMPLEX)
# define LUA_NUMBER_MODE " (complex)"
#else
# error "Number mode undefined (check luaconf.h)"
#endif

/*
** LUA_NUMBER is the type of floating point number in Lua ('float' or 'double')
** LUA_NUMBER_SCAN is the format for reading numbers.
** LUA_NUMBER_FMT is the format for writing numbers.
*/
#if (defined LUA_NUMBER_MODE_LDOUBLE_INT64)
# define LUA_NUMBER long double
# define LUA_NUMBER_SCAN    "%Lg"
# define LUA_NUMBER_FMT     "%.14Lg"
#elif (defined LUA_NUMBER_MODE_FLOAT_INT32) || (defined LUA_NUMBER_MODE_FLOAT_INT64) \
   || (defined LUA_NUMBER_MODE_FLOAT)
# define LUA_NUMBER float
# define LUA_NUMBER_SCAN    "%f"
# define LUA_NUMBER_FMT     "%g"  
#else
# define LUA_NUMBER	double
# define LUA_NUMBER_SCAN    "%lf"
# define LUA_NUMBER_FMT     "%.14g"
# define _LUA_DOUBLE_
#endif

/* LUAI_MAXNUMBER2STR: size of a buffer fitting any number->string result.
 *  normal:  19 (16 digits, sign, point, and \0)
 *  int64:   21 (19 digits, sign, and \0)
 *  complex: double the size (18+18+'i'+\0)
 *
 * Note: RAISE THIS if changing LUA_NUMBER_FMT to more precision!
 */
#ifndef LUA_NUMBER_MODE_COMPLEX
# define LUAI_MAXNUMBER2STR 22
#else
# define LUAI_MAXNUMBER2STR 44
#endif

/*
** LUA_INTEGER is the integer type used by lua_pushinteger/lua_tointeger/lua_isinteger.
** It needs to be defined even if all-floating point number modes were used
** (LUA_NUMBER_MODE_DOUBLE, LUA_NUMBER_MODE_COMPLEX).
**
** LUA_INTFRMLEN is the length modifier for integer conversions in 'string.format'.
** LUA_INTFRM_T is the integer type correspoding to the previous length modifier.
*/
#if defined(LUA_NUMBER_MODE_LDOUBLE_INT64) || defined(LUA_NUMBER_MODE_DOUBLE_INT64) \
    || defined(LUA_NUMBER_MODE_FLOAT_INT64) || defined(LUA_USELONGLONG)
# define LUA_INTEGER	long long
# define lua_str2ul     strtoull
# define lua_str2ul_t   unsigned long long
# define LUA_INTFRMLEN	"ll"
# define LUA_INTFRM_T	long long
# define LUA_INTEGER_MAX 0x7fffffffffffffffLL       /* 2^63-1 */ 
# define LUA_INTEGER_MIN (-LUA_INTEGER_MAX - 1LL)   /* -2^63 */
# define _LUA_INT64_
#else
/* On most machines, ptrdiff_t gives a good choice between int or long. */
# define LUA_INTEGER    ptrdiff_t
# define lua_str2ul     strtoul
# define lua_str2ul_t   unsigned  /* 'unsigned ptrdiff_t' is invalid */
# define LUA_INTFRMLEN	"l"
# define LUA_INTFRM_T	long
# define LUA_INTEGER_MAX 0x7FFFFFFF             /* 2^31-1 */
# define LUA_INTEGER_MIN (-LUA_INTEGER_MAX -1)  /* -2^31 */
#endif


/*
@@ lua_number2int is a macro to convert lua_Number to int.
@@ lua_number2integer is a macro to convert lua_Number to lua_Integer.
** CHANGE them if you know a faster way to convert a lua_Number to
** int (with any rounding method and without throwing errors) in your
** system. In Pentium machines, a naive typecast from double to int
** in C is extremely slow, so any alternative is worth trying.
*/

/* On a Pentium, resort to a trick */
#if defined(_LUA_DOUBLE_) && !defined(LUA_ANSI) && !defined(__SSE2__) && \
    (defined(__i386) || defined (_M_IX86) || defined(__i386__))

/* On a Microsoft compiler, use assembler */
# if defined(_MSC_VER)
#  define lua_number2int(i,d)   __asm fld d   __asm fistp i
# else

/* the next trick should work on any Pentium, but sometimes clashes
   with a DirectX idiosyncrasy */
union luai_Cast { double l_d; long l_l; };
#  define lua_number2int(i,d) \
  { volatile union luai_Cast u; u.l_d = (d) + 6755399441055744.0; (i) = u.l_l; }
# endif

# ifndef _LUA_INT64_
#  define lua_number2integer    lua_number2int
# endif

/* this option always works, but may be slow */
#else
# define lua_number2int(i,d)        ((i)=(int)(d))
#endif

/* TBD: the following line may be compiler specific, and is controversial. Some compilers
 *      (OS X gcc 4.0?) may choke on double->long long conversion (since it can lose
 *      precision; double does not have 63-bit mantissa). Others do require 'long long'
 *      there.  TO BE TESTED ON MULTIPLE SYSTEMS, AND COMPILERS.  -- AKa 12-Oct-06
 */
 /* define lua_number2integer(i,d)   { int v; lua_number2int(v,d); (i)=(lua_Integer)(v); }
 */
#ifndef lua_number2integer
# define lua_number2integer(i,d)    ((i)=(lua_Integer)(d))
#endif


/*
** LUA_COMPLEX is the type of complex number in Lua (needs C99)
*/
#ifdef LUA_NUMBER_MODE_COMPLEX
# define LUA_COMPLEX double complex
#endif


/*
** LUAI_UACNUMBER is the result of an 'usual argument conversion' over a number.
** LUAI_UACINTEGER the same, over an integer.
*/
#define LUAI_UACNUMBER	double

/* TBD: lua_sprintf("%d",v) has problems in 64 bit operation; can this solve them?
*/
#define LUAI_UACINTEGER LUA_INTFRM_T


#undef _LUA_INT64_
#undef _LUA_DOUBLE_
#endif

