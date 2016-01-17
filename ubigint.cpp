// $Id: ubigint.cpp,v 1.8 2015-07-03 14:46:41-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

#include "ubigint.h"
#include "debug.h"
/*
ubigint::ubigint (unsigned long that): ubig_value (that) {
   DEBUGF ('~', this << " -> " << ubig_value)
}**/

ubigint::ubigint (const string& that) {
   // Use const_reverse_iterator crbegin(), crend() to scan a string
   // from the end of the string (least significant digit) to the
   // beginning of the string (most significant digit).
   for (auto rev_that = that.crbegin(); rev_that != that.crend(); ++rev_that)
   {
      ubig_value.push_back(*rev_that - '0');
   }
}

ubigint::ubigint (const ubigint& that): ubig_value{that.ubig_value}
{
   /*for (int j = 0; j < that.ubig_value.size(); ++j)
   {
      that.ubig_value.push_back('0');
   }*/
   copy(that.ubig_value[0], that.ubig_value[that.ubig_value.size()], ubig_value.begin());
}

ubigint ubigint::operator+ (const ubigint& that) const {
   // = ubig_value * 10 + digit - '0';
   ubigint result;
   int size_this = this->ubig_value.size();
   int size_that = that.ubig_value.size();

   // To add leading '0' to make two ubigints have the same digits.
   if (size_this > size_that)
   {

      for (int j = size_that; j < size_this; ++j)
      {
         that.ubig_value.push_back('0');
      }
   }
   else
   {
      for (int j = size_this; j < size_that; ++j)
      {
         this->ubig_value.push_back('0');
      }
   }

   int size_that_new = that.ubig_value.size();

   int carry = 0;
   for (int i = 0; i < size_that_new; ++i)
   {
      // To carry over an additional digit to the most significant digit.
      if (i == (size_that_new - 1) && ((carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0') >= 10))
      {
         // Convert int back to char to append to vector member of ubigint.
         // Reduced form of: (carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0' - 10 + '0').
         result.ubig_value.push_back(carry + this->ubig_value[i] - '0' + that.ubig_value[i] - 10);
         result.ubig_value[size_this] = '1';
      }

      if ((carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0') >= 10)
      {
         // Convert int back to char to append to vector member of ubigint.
         // Reduced form of: (carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0' - 10 + '0').
         result.ubig_value.push_back((carry + this->ubig_value[i] - '0' + that.ubig_value[i] - 10));
         carry = 1;
      }
      else
      {
         // Reduced form of: (carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0' + '0').
         result.ubig_value.push_back(this->ubig_value[i] - '0' + that.ubig_value[i]);
         carry = 0;
      }
   }


   //for (char c : ubig_value)

   return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
   int size_this = this->ubig_value.size();
   int size_that = that.ubig_value.size();
   if (size_this < size_that)
   {
      throw domain_error ("ubigint::operator-(a<b)");
   }
   // How to compare the value of this & that
   ubigint result;
   int borrow = 0;
   for (int i = 0; i < size_this; ++i)
   {

      if ((this->ubig_value[i] - '0' - that.ubig_value[i] - '0' - borrow) < 0)
      {

         result.ubig_value.push_back(this->ubig_value[i] - '0' - borrow + 10 - that.ubig_value[i] - '0');
         borrow = 1;
      }
      else
      {
         result.ubig_value.push_back(this->ubig_value[i] - '0' - that.ubig_value[i] - '0');
         borrow = 0;
      }
   }

   return result;
}

ubigint ubigint::operator* (const ubigint& that) const {
   ubigint result;
   int size_this = this->ubig_value.size();
   int size_that = that.ubig_value.size();
   int carry_multiply = 0;
   int carry_add = 0;
   for (int i = 0; i < size_this; ++i)
   {
      for (int j = 0; j < size_that; ++j)
      {
         if (((this->ubig_value[i] - '0') * (that.ubig_value[j] - '0') + carry_multiply) >= 10)
         {
            carry_multiply = ((this->ubig_value[i] - '0') * (that.ubig_value[j] - '0') + carry_multiply) / 10;
            if ((result.ubig_value[i+j] + (((this->ubig_value[i] - '0') * (that.ubig_value[j] - '0') + carry_multiply) % 10)) >= 10)
            {
               carry_add = (result.ubig_value[i+j] + (((this->ubig_value[i] - '0') * (that.ubig_value[j] - '0') + carry_multiply) % 10)) / 10;
               result.ubig_value[i+j] += (((this->ubig_value[i] - '0') * (that.ubig_value[j] - '0') + carry_multiply) % 10);
            }
            else
            {
               result.ubig_value[i+j] += (((this->ubig_value[i] - '0') * (that.ubig_value[j] - '0') + carry_multiply) % 10);
            }
         }
         else
         {
            result.ubig_value[i+j] += ((this->ubig_value[i] - '0') * (that.ubig_value[j] - '0') + carry_multiply);
         }
      }
   }

   return result;
}

void ubigint::multiply_by_2() {
   //ubigint result;

      int carry = 0;
      for (int i = 0; i < this->ubig_value.size(); ++i)
      {

         if ((carry + (this->ubig_value[i] - '0') * 2) >= 10)
         {
            carry = 1;
            ubig_value[i] = (this->ubig_value[i] - '0') * 2 + carry - 10;
         }
         else
         {
            ubig_value[i] = (this->ubig_value[i] - '0') * 2;
         }
      }

}

void ubigint::divide_by_2() {
   ubig_value /= 2;
}


ubigint::quot_rem ubigint::divide (const ubigint& that) const {
   static const ubigint zero = 0;
   if (that == zero) throw domain_error ("ubigint::divide: by 0");
   ubigint power_of_2 = 1;
   ubigint divisor = that; // right operand, divisor
   ubigint quotient = 0;
   ubigint remainder = *this; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {quotient, remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return divide (that).first;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return divide (that).second;
}

bool ubigint::operator== (const ubigint& that) const {
   return ubig_value == that.ubig_value;
}

bool ubigint::operator< (const ubigint& that) const {
   return ubig_value < that.ubig_value;
}

ostream& operator<< (ostream& out, const ubigint& that) { 
   return out << "ubigint(" << that.ubig_value << ")";
}

