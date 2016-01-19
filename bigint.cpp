// $Id: bigint.cpp,v 1.73 2015-07-03 14:46:41-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <vector>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"
#include "ubigint.h"

bigint::bigint (ubigint that): uvalue (that), is_negative (false) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';

   if (is_negative)
   {
      const string uthat = that.substr(1);
      ubigint temp(uthat);
      this->uvalue = temp;
   }
   else
   {
      ubigint temp(that);
      this->uvalue = temp;
   }

}

bigint bigint::operator+() const {

   return *this;
}

bigint bigint::operator-() const {
   return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
   bigint result;
   if (!this->is_negative && !that.is_negative)
   {
      result.uvalue = this->uvalue + that.uvalue;
      result.is_negative = false;
   }
   else if (!this->is_negative && that.is_negative)
   {
      if (that.uvalue <= this->uvalue)
      {
         result.uvalue = this->uvalue - that.uvalue;
         result.is_negative = false;
      }
      else
      {
         result.uvalue = that.uvalue - this->uvalue;
         result.is_negative = true;
      }
   }
   else if (this->is_negative && !that.is_negative)
   {
      if (that.uvalue <= this->uvalue)
      {
         result.uvalue = this->uvalue - that.uvalue;
         result.is_negative = true;
      }
      else
      {
         result.uvalue = that.uvalue - this->uvalue;
         result.is_negative = false;
      }
   }
   else if (this->is_negative && that.is_negative)
   {
      result.uvalue = this->uvalue + that.uvalue;
      result.is_negative = true;
   }

   return result;
}

bigint bigint::operator- (const bigint& that) const {

   bigint result;

   if (!this->is_negative && !that.is_negative)
   {
      if (that.uvalue <= this->uvalue)
      {
         result.uvalue = this->uvalue - that.uvalue;
         result.is_negative = false;
      }
      else
      {
         result.uvalue = that.uvalue - this->uvalue;
         result.is_negative = true;
      }
   }
   if (!this->is_negative && that.is_negative)
   {
      result.uvalue = this->uvalue + that.uvalue;
      result.is_negative = false;
   }
   if (this->is_negative && !that.is_negative)
   {
      result.uvalue = this->uvalue + that.uvalue;
      result.is_negative = true;

   }
   if (this->is_negative && that.is_negative)
   {
      if (that.uvalue <= this->uvalue)
      {
         result.uvalue = this->uvalue - that.uvalue;
         result.is_negative = true;
      }
      else
      {
         result.uvalue = that.uvalue - this->uvalue;
         result.is_negative = false;
      }
   }

   return result;
}

bigint bigint::operator* (const bigint& that) const {
   bigint result;
   if ((!this->is_negative && !that.is_negative) || (this->is_negative && that.is_negative))
   {
      result.uvalue = this->uvalue * that.uvalue;
      result.is_negative = false;
   }
   if ((!this->is_negative && that.is_negative) || (this->is_negative && !that.is_negative))
   {
      result.uvalue = this->uvalue * that.uvalue;
      result.is_negative = true;
   }

   return result;
}


bigint bigint::operator/ (const bigint& that) const {
   bigint result;
   if ((!this->is_negative && !that.is_negative) || (this->is_negative && that.is_negative))
   {
      result.uvalue = this->uvalue / that.uvalue;
      result.is_negative = false;
   }
   if ((!this->is_negative && that.is_negative) || (this->is_negative && !that.is_negative))
   {
      result.uvalue = this->uvalue / that.uvalue;
      result.is_negative = true;
   }

   return result;
}

bigint bigint::operator% (const bigint& that) const {
   bigint result;
   if ((!this->is_negative && !that.is_negative) || (this->is_negative && that.is_negative))
   {
      result.uvalue = this->uvalue % that.uvalue;
      result.is_negative = false;
   }
   if ((!this->is_negative && that.is_negative) || (this->is_negative && !that.is_negative))
   {
      result.uvalue = this->uvalue % that.uvalue;
      result.is_negative = true;
   }

   return result;
}

bool bigint::operator== (const bigint& that) const {
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative) return is_negative;
   return is_negative ? that.uvalue < uvalue
                      : uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
   ubigint that_uvalue;
   that_uvalue = that.uvalue;
   /***for (auto rev_that = that_copy.uvalue.ubig_value.rbegin(); rev_that != that_copy.ubig_value.rend(); ++rev_that)
   {
      out<< int(*rev_that);
   }
   return out;***/
   return out << "bigint(" << (that.is_negative ? "'-'" : "'+'")
              << that_uvalue << ")";
}

