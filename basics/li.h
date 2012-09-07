#ifndef LIH
#define LIH

#include <string>
#include <vector>
using namespace std;

class LongInt {

	private:
		int nSign; // 1 or -1 or 0, only handled at operator level
		vector<int> num; // int are stored in NDIGIT digits
		const static int NDIGIT = 4;	// for vector operation
		const static int DIVIDER = 10000; // for vector operation, 10E4
		const static int NTOOM = 3;	// Toom-3 multiplication

	protected:
		void setZero_(); // set this number to be zero
		//LongInt add(LongInt&);	// add param, both positive
		//LongInt subtract(LongInt&);	// subtract param, both positive, assume abs(this) > abs(param)
		//LongInt multiply(int, int); // mutiply by int which is at [int] position
		//int absCompare(LongInt&); // check abs values, 1 if greater, 0 if equal, -1 if less
		//void removeLeadingZeros(); // remove the leading zeros
		
		vector<int> add(vector<int>, vector<int>);	// add param, both positive
		vector<int> addAllVec(vector<vector<int>>);	// add all elements in a vector, each represents a long int number
		vector<int> subtract(vector<int>, vector<int>);	// subtract param, both positive, assume abs(this) > abs(param)
		vector<int> multiply(vector<int>, vector<int>);	// mutiply, both positive
		void toom_3(vector<int>&, vector<int>&, int, int, int, int, int, int);	// Toom-3 multiplication
		int absCompare(vector<int>, vector<int>);	// check abs values, 1 if greater, 0 if equal, -1 if less
		void removeLeadingZeros(vector<int>&);	// remove the leading zeros

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
		vector<int> getNum();	// return the num

		double doubleValue(); // return a double approximation (you can assume that it will not be out of bound)

		string toString(); // return a string representation of this number

	friend LongInt operator-(LongInt&); 
};

LongInt operator-(LongInt&); // negative sign


#endif