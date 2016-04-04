//
// myFixed
//
// trivial 16.16 fixed-point math in ANSI-C
// Martin Trojer, 2003
//
// Target must supply 'long long'
//
// $Id: myfixed.c,v 1.3 2003/11/25 13:48:26 aphex Exp $
//

#include "myfixed.h"

fixed myfixAdd(fixed a, fixed b)
{
  return a+b; // no need to use these
}

fixed myfixSub(fixed a, fixed b)
{
  return a-b; // no need to use these
}

fixed myfixMul(fixed a, fixed b)
{
  signed long long res = a;
  res = res * b + 32768;         // 2**15
  return (fixed)(res>>16);
  
}

fixed myfixDiv(fixed a, fixed b)
{
  signed long long res = a;
  res = (((res<<17)/b)+1)/2;
  return (fixed)res;
}

