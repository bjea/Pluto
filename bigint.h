// $Id: bigint.h,v 1.28 2015-07-03 14:46:41-07 - - $

#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <exception>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>
#include "debug.h"
#include "relops.h"
#include "ubigint.h"

//using namespace std;

class bigint {
   friend ostream& operator<< (ostream&, const bigint&);
   private:
    ubigint uvalue;
    bool is_negative {false};
   public:

      bigint() = default; // Needed or will be suppressed.
      //bigint (ubigint);
      bigint (const ubigint&, bool is_negative = false);
      explicit bigint (const string&);
    //Newly added.
    //bigint (unsigned char that);

      bigint operator+() const;
      bigint operator-() const;
      bigint operator+ (const bigint&) const;
      bigint operator- (const bigint&) const;
      bigint operator* (const bigint&) const;
      bigint operator/ (const bigint&) const;
      bigint operator% (const bigint&) const;

      bool operator== (const bigint&) const;
      bool operator<  (const bigint&) const;
};

#endif

