#ifndef LIH
#define LIH

#include <string>
#include <vector>
using namespace std;

class LongInt {

	private:
		int nSign; // 1 or -1 or 0, only handled at operator level
		vector<int> num; // int are stored in NDIGIT digits
		static const int NDIGIT = 4;	// for vector operation
		static const int DIVIDER = 10000; // for vector operation, 10E4
		static const int NTOOM = 3;	// Toom-3 multiplication

	protected:
		void setZero_(); // set this number to be zero
		void add(const vector<int>&, const vector<int>&, vector<int>&);	// add param, both positive
		void addAllVec(const vector<vector<int>>&, vector<int>&);	// add all elements in a vector, each represents a long int number
		void subtract(const vector<int>&, const vector<int>&, vector<int>&);	// subtract param, both positive, assume abs(this) > abs(param)
		void multiply(const vector<int>&, const vector<int>&, vector<int>&);	// mutiply, both positive
		void toom_3(int, int, int, int, int, int, vector<int>&);	// Toom-3 multiplication
		int absCompare(const vector<int>&, const vector<int>&);	// check abs values, 1 if greater, 0 if equal, -1 if less
		void removeLeadingZeros(vector<int>&);	// remove the leading zeros

	public:
		LongInt();
		LongInt(const LongInt&); // copy constructor
		LongInt(int);      // take in an integer 
		LongInt(string);   // take in an integer string
		void dump();       // print out this number

		LongInt& operator=(int); 
		LongInt& operator=(const LongInt&);  
		LongInt operator+(const LongInt&); 
		LongInt operator+(int i) {return (*this) + LongInt(i);};
		LongInt operator-(const LongInt&);
		LongInt operator-(int i) {return (*this) + LongInt(i);};
		LongInt operator*(const LongInt&);
		LongInt operator*(int i) {return (*this) * LongInt(i);};
		
		bool operator>(const LongInt&);
		bool operator<(const LongInt&);
		bool operator==(const LongInt&);

		bool eqZero();  // return true if it is zero
		int sign();     // return +1 if it's positive, -1 if it's negative, 0 if it's zero

		double doubleValue(); // return a double approximation (you can assume that it will not be out of bound)

		string toString(); // return a string representation of this number

	friend LongInt operator-(const LongInt&); 
};

LongInt operator-(const LongInt&); // negative sign


#endif