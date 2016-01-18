// $Id: ubigint.cpp,v 1.8 2015-07-03 14:46:41-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

#include "ubigint.h"
#include "bigint.h"
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

ubigint::ubigint (const ubigint& that): ubig_value(that.ubig_value)
{
   /*for (int j = 0; j < that.ubig_value.size(); ++j)
   {
      that.ubig_value.push_back('0');
   }*/
   copy(that.ubig_value.begin(), that.ubig_value.end(), ubig_value.begin());
}

//Newly added.
ubigint::ubigint (udigit_t that)
{
   ubig_value.push_back(that - '0');
}

ubigint ubigint::operator+ (const ubigint& that) const {
   // = ubig_value * 10 + digit - '0';
   ubigint result{};
   int size_this = this->ubig_value.size();
   int size_that = that.ubig_value.size();
   int size_bigger = 0;

   // To add leading '0' to make two ubigints have the same digits.
   (size_this > size_that)? (size_bigger = size_this) : (size_bigger = size_that);

   int carry = 0;
   /***auto it_this = this->ubig_value.begin();
   auto it_that = that.ubig_value.begin();
   while (it_this != this->ubig_value.end() && it_that != that.ubig_value.end())
   {
      if (i == (size_that_new - 1) && ((carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0') >= 10))
      {
         // Convert int back to char to append to vector member of ubigint.
         // Reduced form of: (carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0' - 10 + '0').
         result.ubig_value.push_back(carry + this->ubig_value[i] - '0' + that.ubig_value[i] - 10);
         result.ubig_value[size_this] = '1';
      }

      if ((carry + *it_this + *it_that) >= 10)
      {
         // Convert int back to char to append to vector member of ubigint.
         // Reduced form of: (carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0' - 10 + '0').
         result.ubig_value.push_back((carry + *it_this + *it_that - 10));
         carry = 1;
      }
      else
      {
         // Reduced form of: (carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0' + '0').
         result.ubig_value.push_back(this->ubig_value[i] - '0' + that.ubig_value[i]);
         carry = 0;
      }


      it_this++;
      it_that++;
   }***/

   for (int i = 0; i < size_bigger; ++i)
   {
      // To carry over an additional digit to the most significant digit.
      if (i == (size_bigger - 1) && ((carry + this->ubig_value[i] + that.ubig_value[i]) >= 10))
      {
         // Convert int back to char to append to vector member of ubigint.
         // Reduced form of: (carry + this->ubig_value[i] - '0' + that.ubig_value[i] - '0' - 10 + '0').
         result.ubig_value.push_back(carry + this->ubig_value[i] + that.ubig_value[i] - 10);
         result.ubig_value[size_bigger] = '1';
      }

      if ((carry + this->ubig_value[i] + that.ubig_value[i]) >= 10)
      {
         // Convert int back to char to append to vector member of ubigint.
         result.ubig_value.push_back((carry + this->ubig_value[i] + that.ubig_value[i] - 10));
         carry = 1;
      }
      else
      {
         result.ubig_value.push_back(this->ubig_value[i] + that.ubig_value[i]);
         carry = 0;
      }
   }


   //for (char c : ubig_value)

   return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
   int size_this = this->ubig_value.size();
   int size_that = that.ubig_value.size();
   if (*this < that)
   {
      throw domain_error ("ubigint::operator-(a<b)");
   }
   // How to compare the value of this & that?
   ubigint result{};
   int borrow = 0;
   for (int i = 0; i < size_this; ++i)
   {
      if ((this->ubig_value[i] - that.ubig_value[i] - borrow) < 0)
      {
         result.ubig_value.push_back(this->ubig_value[i] + 10 - borrow - that.ubig_value[i]);
         borrow = 1;
      }
      else
      {
         result.ubig_value.push_back(this->ubig_value[i] - that.ubig_value[i] - borrow);
         borrow = 0;
      }
   }

   return result;
}

ubigint ubigint::operator* (const ubigint& that) const {
   ubigint result{};
   int size_this = this->ubig_value.size();
   int size_that = that.ubig_value.size();
   //result.ubig_value(size_this+size_that);
   int carry_multiply = 0;
   int carry_add = 0;

   for (int i = 0; i < size_this; ++i)
   {
      for (int j = 0; j < size_that; ++j)
      {
         if ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) >= 10)
         {
            if (j == (size_that - 1))
            {
               int i_j_old = result.ubig_value[i+j];
               if ((result.ubig_value[i+j] + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply)) + carry_add) >= 10)
               {
                  result.ubig_value[i+j] = (i_j_old + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply)) + carry_add) % 10;
                  carry_add = (i_j_old + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply))) / 10;
                  result.ubig_value[i+j+1] += carry_add;
               }
               else
               {
                  result.ubig_value[i+j] += ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10) + carry_add;
               }

               carry_add = 0;
               carry_multiply = 0;
            }

            else
            {
               /*int test1 = (ubig_value[i] * that.ubig_value[j] + carry_multiply);
               cout << "test1 = " << test1 << endl;*/

               int i_j_old = result.ubig_value[i+j];
               //cout << "i_j_old = " << i_j_old << endl;
               if ((result.ubig_value[i+j] + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10) + carry_add) >= 10)
               {
                  result.ubig_value[i+j] = (i_j_old + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10) + carry_add) % 10;
                  carry_add = (i_j_old + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10)) / 10;
                  //Newly Added
                  result.ubig_value[i+j+1] += carry_add;
               }
               else
               {
                  result.ubig_value[i+j] += ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10) + carry_add;

               }
               carry_multiply = (this->ubig_value[i] * that.ubig_value[j] + carry_multiply) / 10;
               //Newly Added
               carry_add = 0;
            }
         }
         else
         {
            if (j == (size_that - 1))
            {
               int i_j_old = result.ubig_value[i+j];
               if ((result.ubig_value[i+j] + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) /*% 10*/) + carry_add) >= 10)
               {
                  result.ubig_value[i+j] = (i_j_old + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply)) + carry_add) % 10;
                  carry_add = (i_j_old + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) /*% 10*/)) / 10;
                  result.ubig_value[i+j+1] += carry_add;
               }
               else
               {
                  result.ubig_value[i+j] += ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10) + carry_add;
               }

               carry_add = 0;
               carry_multiply = 0;

            }
            else
            {
               int i_j_old = result.ubig_value[i+j];
               if ((result.ubig_value[i+j] + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10)) >= 10)
               {
                  result.ubig_value[i+j] = (result.ubig_value[i+j] + this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10;
                  carry_add = (i_j_old + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10)) / 10;
                  result.ubig_value[i+j+1] += carry_add;
               }
               else
               {
                  result.ubig_value[i+j] += ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10);
               }

               carry_multiply = 0;
               //Newly Added.
               carry_add = 0;
            }

         }
      }
   }


   return result;
}

void ubigint::multiply_by_2() {
      int carry = 0;
      for (int i = 0; i < this->ubig_value.size(); ++i)
      {
         if ((carry + this->ubig_value[i] * 2) >= 10)
         {
            if ((i == this->ubig_value.size() - 1))
            {
               this->ubig_value[i] = this->ubig_value[i] * 2 + carry - 10;
               this->ubig_value[i+1] = 1;
            }
            else
            {
               this->ubig_value[i] = this->ubig_value[i] * 2 + carry - 10;
               carry = 1;
            }
         }
         else
         {
            /*if ((carry + this->ubig_value[i] * 2) >= 10)
            {


            }
            else
            {*/
               this->ubig_value[i] = this->ubig_value[i] * 2 + carry;
               carry = 0;
            //}
         }

      }

}

void ubigint::divide_by_2() {
   //ubig_value /= 2;

   int carry = 0;
   for (int i = this->ubig_value.size() - 1; i >= 0; --i)
   {
      /***if ((i == this->ubig_value.size() - 1) && (carry + this->ubig_value[i] * 2) >= 10)
      {
         this->ubig_value[i] = this->ubig_value[i] * 2 + carry - 10;
         this->ubig_value[i+1] = '1';
      }
      else
      {***/
         if ((this->ubig_value[i] + carry) >= 2)
         {
            /*if (i == 0)
            {

            }
            else
            {*/
               if ((this->ubig_value[i]) % 2)
               {
                  this->ubig_value[i] = (carry + this->ubig_value[i]) / 2;
                  carry = 10;
               }
               else
               {
                  this->ubig_value[i] = (this->ubig_value[i] + carry) / 2;
                  carry = 0;
               }
            //}

         }
         else
         {
            this->ubig_value[i] = '0';
            carry = 10;
         }
      //}

   }

}


ubigint::quot_rem ubigint::divide (const ubigint& that) const {
   //static const ubigint zero = 0;
   if (that.ubig_value.size() == 0) throw domain_error ("ubigint::divide: by 0");
   // How to catch a string of "000" stored as a vector?

   //ubigint power_of_2 = 1;
   //ubigint divisor = that; // right operand, divisor
   //ubigint quotient = 0;
   //ubigint remainder = *this; // left operand, dividend

   ubigint zero('0');
   //zero.ubig_value.push_back(0);
   if (that == zero) throw domain_error ("ubigint::divide: by 0");
   //
   ubigint power_of_2('1');
   //power_of_2.ubig_value.push_back(1);
   ubigint divisor = that; // right operand, divisor
   ubigint quotient{};
   ubigint remainder = *this;

   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (zero < power_of_2) {
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
   if (this->ubig_value.size() != that.ubig_value.size())
   {
      return false;
   }
   else
   {
      for (int i = 0; i < that.ubig_value.size(); ++i)
      {
         if (this->ubig_value[i] != that.ubig_value[i])
         {
            return false;
         }
      }
      return true;
   }
}

bool ubigint::operator< (const ubigint& that) const {
   if (this->ubig_value.size() > that.ubig_value.size())
   {
      return false;
   }
   else
   {
      for (int i = that.ubig_value.size() - 1; i >= 0; --i)
      {
         if (this->ubig_value[i] > that.ubig_value[i])
         {
            return false;
         }
         else if (this->ubig_value[i] == that.ubig_value[i])
         {
            continue;
         }
         else
         {
            return true;
         }
      }

   }
   //return ubig_value < that.ubig_value;
   return false;
}

bool ubigint::operator<= (const ubigint& that) const {
   if (this->ubig_value.size() > that.ubig_value.size())
   {
      return false;
   }
   else
   {
      for (int i = that.ubig_value.size() - 1; i >= 0; --i)
      {
         if (this->ubig_value[i] > that.ubig_value[i])
         {
            return false;
         }
         else if (i == 0 && (this->ubig_value[i] == that.ubig_value[i]))
         {
            return true;
         }
         else if (this->ubig_value[i] == that.ubig_value[i])
         {
            continue;
         }
         else
         {
            return true;
         }
      }

   }

   return false;

   //return ubig_value < that.ubig_value;
}

ostream& operator<< (ostream& out, const ubigint& that) {

   //vector<udigit_t>
   ubigint that_copy = that;
   for (auto rev_that = that_copy.ubig_value.rbegin(); rev_that != that_copy.ubig_value.rend(); ++rev_that)
   {
      out<< int(*rev_that);
   }

   return out;
   //return out << "ubigint(" << that.ubig_value << ")";
}

