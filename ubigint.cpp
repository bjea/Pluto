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

ubigint::ubigint (vector<unsigned char> that): ubig_value (that) {
   DEBUGF ('~', this << " -> " << (*this))
}

ubigint::ubigint (const string& that) {

   string that_noLeadingZero = that;

   // To remove leading zeros of input string,
   // http://stackoverflow.com/questions/25726531/how-to-remove-leading-zeros-from-string-using-c
   that_noLeadingZero.erase(0, min(that_noLeadingZero.find_first_not_of('0'), that_noLeadingZero.size()-1));

   // Use const_reverse_iterator crbegin(), crend() to scan a string
   // from the end of the string (least significant digit) to the
   // beginning of the string (most significant digit).
   for (auto rev_that = that_noLeadingZero.crbegin(); rev_that != that_noLeadingZero.crend(); ++rev_that)
   {
         ubig_value.push_back(*rev_that - '0');
   }
}

ubigint::ubigint (const ubigint& that): ubig_value(that.ubig_value)
{
   copy(that.ubig_value.begin(), that.ubig_value.end(), ubig_value.begin());
}

//Constructor using an unsigned char to initialize.
ubigint::ubigint (udigit_t that)
{
   ubig_value.push_back(that - '0');
}

ubigint ubigint::operator+ (const ubigint& that) const {
   unsigned long size_this = this->ubig_value.size();
   unsigned long size_that = that.ubig_value.size();

   ubigint result;

   int carry = 0;

   unsigned long i = 0UL;

   while(i < size_this && i < size_that)
   {
      int thisValue = this->ubig_value[i];
      int thatValue = that.ubig_value[i];

      int sum = thisValue + thatValue + carry;

      if(sum/10)
      {
         result.ubig_value.push_back(sum%10);
         carry = sum/10;
      }
      else
      {
         result.ubig_value.push_back(sum);
         carry = 0;
      }

      i++;
   }

   while (i < size_this)
   {
      int thisValue = this->ubig_value[i];
      int sum = thisValue + carry;

      if (sum / 10)
      {
         result.ubig_value.push_back(sum % 10);
         carry = sum/10;
      }
      else
      {
         result.ubig_value.push_back(sum);
         carry = 0;
      }

      i++;
   }

   while (i < size_that)
   {
      int thatValue = that.ubig_value[i];
      int sum = thatValue + carry;

      if (sum / 10)
      {
         result.ubig_value.push_back(sum % 10);
         carry = sum/10;
      }
      else
      {
         result.ubig_value.push_back(sum);
         carry = 0;
      }

      i++;
   }

   if (carry)
   {
      result.ubig_value.push_back(carry);
   }

   return result;
}

ubigint ubigint::operator- (const ubigint& that) const {


   // To compare the value of this & that to avoid handling (a - b) when a < b.
   if (*this < that)
   {
      throw domain_error ("ubigint::operator-(a<b)");
   }

   unsigned long size_this = this->ubig_value.size();
   unsigned long size_that = that.ubig_value.size();

   ubigint result;

   int borrow = 0;

   unsigned long i = 0UL;

   while (i < size_this && i < size_that)
   {
      int thisValue = this->ubig_value[i];
      int thatValue = that.ubig_value[i];

      int difference = thisValue - thatValue - borrow;

      if (difference < 0)
      {
         result.ubig_value.push_back(difference + 10);
         borrow = 1;
      }
      else
      {
         result.ubig_value.push_back(difference);
         borrow = 0;
      }

      i++;
   }

   while (i < size_this)
   {
      int thisValue = this->ubig_value[i];
      int difference = thisValue - borrow;

      if (difference < 0)
      {
         result.ubig_value.push_back(difference + 10);
         borrow = 1;
      }
      else
      {
         result.ubig_value.push_back(difference);
         borrow = 0;
      }

      i++;
   }

   // To erase leading zero by index,
   // http://stackoverflow.com/questions/875103/how-to-erase-element-from-stdvector-by-index
   if (result.ubig_value[result.ubig_value.size()-1] == 0)
   {
      result.ubig_value.erase(result.ubig_value.begin() + (result.ubig_value.size() - 1));
   }


   return result;
}

ubigint ubigint::operator* (const ubigint& that) const {
   unsigned long size_this = this->ubig_value.size();
   unsigned long size_that = that.ubig_value.size();
   ubigint result{};
   result.ubig_value.resize(size_this+size_that);
   int carry_multiply = 0;
   int carry_add = 0;

   for (unsigned long i = 0UL; i < size_this; ++i)
   {
      for (unsigned long j = 0UL; j < size_that; ++j)
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
               int i_j_old = result.ubig_value[i+j];

               if ((result.ubig_value[i+j] + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10) + carry_add) >= 10)
               {
                  result.ubig_value[i+j] = (i_j_old + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10) + carry_add) % 10;
                  carry_add = (i_j_old + ((this->ubig_value[i] * that.ubig_value[j] + carry_multiply) % 10)) / 10;
                  //To carry over an additional digit to the next more significant digit.
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

   if (result.ubig_value[result.ubig_value.size()-1] == 0)
   {
      result.ubig_value.erase(result.ubig_value.begin()+(result.ubig_value.size()-1));
   }

   return result;
}

void ubigint::multiply_by_2() {
   /*ubigint this_times2;
   this_times2 = (*this) + (*this);
   *this = this_times2;*/
   int carry = 0;
   unsigned long size_original = this->ubig_value.size();
   this->ubig_value.resize(this->ubig_value.size() + 1);
   for (unsigned long i = 0; i < size_original; ++i)
   {
      if ((carry + this->ubig_value[i] * 2) >= 10)
      {
         if (i == size_original - 1)
         {
            this->ubig_value[i] = (this->ubig_value[i] * 2 + carry) % 10;
            //this->ubig_value.resize(this->ubig_value.size() + 1);
            this->ubig_value[i+1] = 1;
            break;
         }
         else
         {
            this->ubig_value[i] = (this->ubig_value[i] * 2 + carry) % 10;
            carry = 1;
         }
      }

      else
      {
         ubig_value[i] = (carry + ubig_value[i] * 2);
         carry = 0;
      }

   }
   if (ubig_value[ubig_value.size()-1] == 0)
   {
      ubig_value.erase(ubig_value.begin()+(ubig_value.size()-1));
   }
}

void ubigint::divide_by_2() {
   int carry = 0;

   for (auto it = this->ubig_value.rbegin(); it != this->ubig_value.rend(); ++it)
   {
      if ((*it + carry) >= 2)
      {
         if ((*it) % 2)
         {
            *it = (carry + *it) / 2;
            carry = 10;
         }
         else
         {
            *it = (*it + carry) / 2;
            carry = 0;
         }
      }
      else if((*it + carry) != 0)
      {
         *it = 0;
         carry = 10;
      }

      if (this->ubig_value[this->ubig_value.size()-1] == 0)
      {
         this->ubig_value.erase(this->ubig_value.begin()+(this->ubig_value.size()-1));
      }
   }
}


ubigint::quot_rem ubigint::divide (const ubigint& that) const {

   if (that.ubig_value.size() == 0) throw domain_error ("ubigint::divide: by 0");
   ubigint zero('0');

   if (that == zero) throw domain_error ("ubigint::divide: by 0");
   //
   ubigint power_of_2('1');
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
      for (unsigned long i = 0; i < that.ubig_value.size(); ++i)
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
      else if(this->ubig_value.size() < that.ubig_value.size())
   {
      return true;
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
      else if(this->ubig_value.size() < that.ubig_value.size())
   {
      return true;
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
}

ostream& operator<< (ostream& out, const ubigint& that) {

   ubigint that_copy = that;
   long that_size = that_copy.ubig_value.size();

   // Displaying the ubigint number in multiple lines like dc style.
   for (long i = that_size - 1; i >= 0; --i)
   {

      out<< int(that_copy.ubig_value[i]);
      // To add "\" and a new line every 60 digits,
      // + 10 because "bigint('+' " takes 10 digits.
      if ((that_size - i + 10) % 60 == 0)
      {
         out<< "\\" << endl;
      }
   }
   return out;
}

