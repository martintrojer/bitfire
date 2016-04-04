//
// myFixed
//
// trivial 16.16 fixed-point math in ANSI-C
// Martin Trojer, 2003
//
// Target must supply 'long long'
//
// $Id: myfixed.h,v 1.3 2003/11/25 13:48:26 aphex Exp $
//

#ifndef _MYFIXED_H
#define _MYFIXED_H

typedef signed int fixed;	// 16.16 fixed-point

#define INT_TO_FIXED(x)         ((x) << 16)
#define FLOAT_TO_FIXED(x)       ((signed int)(x * 65536.0 + 0.5))
#define FIXED_TO_INT(x)         ((x) >> 16)
#define FIXED_TO_FLOAT(x)       (((float)x) / 65536.0)
#define ROUND_FIXED_TO_INT(x)   (((x) + 0x8000) >> 16)

#define ONE             INT_TO_FIXED(1)
#define FIXED_PI        205887L
#define FIXED_2PI       411775L
#define FIXED_E         178144L
#define FIXED_ROOT2      74804L
#define FIXED_ROOT3     113512L
#define FIXED_GOLDEN    106039L

#ifdef __cplusplus

extern "C" fixed myfixAdd(fixed a, fixed b);   // don't use these
extern "C" fixed myfixSub(fixed a, fixed b);   // don't use these
extern "C" fixed myfixMul(fixed a, fixed b);
extern "C" fixed myfixDiv(fixed a, fixed b);

#else

fixed myfixAdd(fixed a, fixed b);   // don't use these
fixed myfixSub(fixed a, fixed b);   // don't use these
fixed myfixMul(fixed a, fixed b);
fixed myfixDiv(fixed a, fixed b);

#endif

#endif
