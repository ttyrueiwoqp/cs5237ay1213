#ifndef LIH
#define LIH

#include <string>
#include <vector>
using namespace std;

class LongInt {

	private:
		int nSign; // 1 or -1 or 0
		vector<int> num; // int should be 4 digits
		const static int NDIGIT = 4;	// for vector operation
		const static int DIVIDER = 10000; // for vector operation, 10E4

	protected:
		void setZero_(); // set this number to be zero
		LongInt add(LongInt&);	// add param, both positive
		LongInt subtract(LongInt&);	// subtract param, both positive, assume abs(this) > abs(param)
		LongInt multiply(int, int); // mutiply by int which is at [int] position
		int absCompare(LongInt&); // check abs values, 1 if greater, 0 if equal, -1 if less
		void removeLeadingZeros(); // remove the leading zeros

	public:
		LongInt();
		LongInt(LongInt&); // copy constructor
		LongInt(int);      // take in an integer 
		LongInt(string);   // take in an integer string
		void dump();       // print out this number

		LongInt& operator=(int); 
		LongInt& operator=(LongInt&);  
		LongInt operator+(LongInt&); 
		LongInt operator+(int i) {return (*this) + LongInt(i);};
		LongInt operator-(LongInt&);
		LongInt operator-(int i) {return (*this) + LongInt(i);};
		LongInt operator*(LongInt&);
		LongInt operator*(int i) {return (*this) * LongInt(i);};
		
		bool operator>(LongInt&);
		bool operator<(LongInt&);
		bool operator==(LongInt&);

		bool eqZero();  // return true if it is zero
		int sign();     // return +1 if it's positive, -1 if it's negative, 0 if it's zero

		double doubleValue(); // return a double approximation (you can assume that it will not be out of bound)

		string toString(); // return a string representation of this number

	friend LongInt operator-(LongInt&); 
};

LongInt operator-(LongInt&); // negative sign


#endif