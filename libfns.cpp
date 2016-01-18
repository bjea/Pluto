// $Id: libfns.cpp,v 1.4 2015-07-03 14:46:41-07 - - $

#include "libfns.h"
#include "ubigint.h"

//
// This algorithm would be more efficient with operators
// *=, /=2, and is_odd.  But we leave it here.
//

bigint pow (const bigint& base_arg, const bigint& exponent_arg) {
   bigint base (base_arg);
   bigint exponent (exponent_arg);
   //ubigint temp = exponent.uvalue;
   ubigint u_ZERO('0');
   ubigint u_ONE('1');
   ubigint u_TWO('2');

   static const bigint ZERO (u_ZERO, false);
   static const bigint ONE (u_ONE, false);
   static const bigint TWO (u_TWO, false);
   DEBUGF ('^', "base = " << base << ", exponent = " << exponent);
   if (base == ZERO) return ZERO;
   bigint result = ONE;
   if (exponent < ZERO) {
      base = ONE / base;
      exponent = - exponent;
   }
   while (exponent > ZERO) {
      if (exponent % TWO == ONE) {
         result = result * base;
         exponent = exponent - ONE;
      }else {
         base = base * base;
         exponent = exponent / TWO;
      }
   }
   DEBUGF ('^', "result = " << result);
   return result;
}

