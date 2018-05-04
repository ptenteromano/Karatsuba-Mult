/********************************************************************
 *
 * CISC 4080
 * Project 2: Comparing Polynomial Multiplication Algorithms
 *
 * Author: Philip Tenteromano
 * Data: 22 Febuary 2018
 *
 * Polynomial implementation
 * 
 **********************************************************************/

#include "Polynomial.h"
using namespace std;

/*
 * To find Polynomial degree, call the size of the vector - 1
 */
int Polynomial::degree() const
{
   return data.size() - 1;
}

/*
 * p[i] is the i-th coefficient of the Polynomial p
 * this is the rvalue version
 */
const double Polynomial::operator[](size_t index) const
{
   return (index <= degree()) ? data[index] : 0;
   return data[index];
}

/*
 * p[i] is the i-th coefficient of the Polynomial p
 * this is the lvalue version
 */
double& Polynomial::operator[](size_t index)
{
   for (int i = data.size(); i <= index; i++) data.push_back(0.0);
   return data[index];
}

/*
 * equality operator for Polynomials
 */
bool operator==(const Polynomial& a, const Polynomial& b)
{
   // two doubles will be considered nearly equal if their relative
   // distance is at most epsilon
   const double epsilon {1.0e-10};      
   if (a.degree() != b.degree()) return false;
   for (int i = 0; i <= a.degree(); i++)
      if (abs(a[i] - b[i]) > epsilon) return false;
   return true;
}

/*
 * Ineqality operator for Polynomials
 */
bool operator!=(const Polynomial& a, const Polynomial& b)
{
	return !(a == b);	// simply using the == overloaded operator
}

/*
 * output a Polynomial
 */
ostream& operator<<(ostream& os, const Polynomial& p)
{
   if (p.degree() == -1 || (p.degree() == 0 && p[0] == 0)) {
      cout << 0;
      return os;
   }

   for (int i = 0; i <= p.degree(); i++) {
      if (p[i] != 0) {
         if (i == 0) os << p[0];
         else if (i == 1) os << p[1] << "x";
         else os << p[i] << "x^" << i;
         if (i < p.degree()) cout << " + ";
      }
   }
   return os;
}

/*
 * addition operator for Polynomials 
 */
Polynomial add(const Polynomial& a, const Polynomial& b)
{
   if (a == zero) return b;
   if (b == zero) return a;

   Polynomial sum;
   int deg = max(a.degree(), b.degree());
   for (int i = 0; i <= deg; i++) 
      sum.data.push_back(a[i] + b[i]);
   return sum;
}

/*
 * subtraction operator for Polynomials
 */
Polynomial subtract(const Polynomial& a, const Polynomial& b)
{
   if (b == zero) return a;				// a - 0 = a 

   Polynomial diff;
   int deg = max(a.degree(), b.degree());		// loop through and subtract
   for (int i = 0; i<=deg; i++)
   	 diff.data.push_back(a[i] - b[i]);
  return diff;

}

/*
 * naive multiplication operator for Polynomials
 */
Polynomial naive_mult(const Polynomial& a, const Polynomial& b)
{

   	Polynomial prod;

   	for (int i = 0; i <= a.degree(); i++) 				// multiply i-th of a * the j-th of b and add to C[i+j]
   		for (int j = 0; j <= b.degree(); j++) 
			prod[i+j] += a[i]*b[j];
   
   	return prod;
}

/*
 * Karatsuba multiplication for Polynomials
 */
Polynomial karatsuba(const Polynomial& a, const Polynomial& b)
{
	Polynomial prod;								// primary return 
  	Polynomial LeftA, RightA, LeftB, RightB;		// split variables
	Polynomial p1, p2, p3, p4, p5, p6, p7, p8;		// intermediate variables 

   	int n = max(a.degree(), b.degree()) + 1;		// number of coefficients

	if (n == 1) { 									// base case when down to 1 coefficient
			prod[0] = (a.data[0] * b.data[0]);
			return prod;
	}

	a.split(LeftA, RightA);							// split both polynomials 
	b.split(LeftB, RightB);						
	
	p1 = karatsuba(LeftA, LeftB);					// three recursive calls
	p2 = karatsuba(RightA, RightB);
	p3 = karatsuba(add(LeftA,RightA), add(LeftB, RightB));

	p4 = subtract( p3, p1 );						// after recursion, subtract (p3 - p1 - p2)
	p5 = subtract( p4, p2 );
	
	int half = n/2;  								// we need n/2 for both odd and even degrees
	if (n%2 == 1) { n++; half++; }					// if odd number of coefficients, n++ and n/2++;
	
	p6 = monomial_mult(p2, n);						// padd the largest degrees with zeros
	p7 = monomial_mult(p5, half);					// half-padd the middle degrees

	p8 = add(p7, p1);			//p1 + p2 			// add these up, assign and finally return to call stack
	prod = add(p8,p6);			//p1 + p4

	return prod;
}

/*
 * multiply a Polynomial by x*n
 * padding zeros on left hand side 
 */
Polynomial monomial_mult(const Polynomial& a, int n)
{
   	if (n == 0) return a;

   	Polynomial result;		// result[0] through result[n-1] will be padded with zeros

   	for (int i = 0; i <= a.degree(); i++)	
		result[i+n] = a[i];					// result[i+n] through n + a.degree() will be the original polynomial 

   	return result;
}

/*
 * Private member
 * split a Poylynomial into two pieces of roughly equal degree
 * after
 *      p.split(p_left, p_right)
 * we have
 *      p(x) = p_left(x) + x^n p_right(x)
 * where n is roughly p.degree()/2.  YMMV.
 */
void Polynomial::split(Polynomial& p_left, Polynomial& p_right) const
{
	int n = this->degree();						// find degree of calling polynomial
	int half = n/2;								// need half mark
	int k = 0;									// dummy variable for RHS

	for (int i = 0; i <= n; i++) {
		if (i <= half) 							// initialize LHS up to and including 'half'
			p_left[i] = this->data[i];
		else {									// finish with initalizing RHS
			p_right[k] = this->data[i];
			k++;								//increment dummy variable for [half+1] through p.degree()
		}
	}
	return;
}




