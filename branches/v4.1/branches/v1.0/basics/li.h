#ifndef LIH
#define LIH
#include <string>
#include<vector>

class LongInt {
public:
	LongInt();
	LongInt(const LongInt&); // copy constructor

	LongInt(const int&);      // take in an integer
	void dump();       // print out this number

	LongInt& operator=(const int&);
	LongInt& operator=(const std::string&);
	LongInt& operator=(const LongInt&);
	LongInt operator+(const LongInt&);
	LongInt operator+(const std::string&);
	LongInt operator+(const int& i);

	LongInt operator*(const std::string s);
	LongInt operator*(const LongInt&);
	LongInt operator*(const int& i);

	LongInt operator-(const LongInt&);
	LongInt operator-(const std::string str);
	LongInt operator-(const int& i);
	LongInt operator-();

	bool operator>(const LongInt&);
	bool operator<(const LongInt&);
	bool operator==(const LongInt&);

	bool eqZero();  // return true if it is zero
	int sign();     // return +1 if it's positive, -1 if it's negative, 0 if it's zero
	bool isNegative(std::string s);

	double doubleValue(); // return a double approximation (you can assume that it will not be out of bound)
	const std::string toString() const;
	//void setValue( std::vector<unsigned int> data ); 
	//friend LongInt operator-(LongInt);

protected:
	void setZero_(); // set this number to be zero
	void addValue(std::vector<unsigned int> vec);
	std::vector<unsigned int> string2Vec(std::string s, bool& negative);
	int compare(std::vector<unsigned int> v1, std::vector<unsigned int> v2);
	std::vector<unsigned int> substractValue(std::vector<unsigned int> v1, std::vector<unsigned int> v2);

private:
	int m_SzOfInt;
	std::vector<unsigned int> m_data;
	bool bSign;// indicate negative value

};

//LongInt operator-(LongInt rhs){ // negative sign
//	rhs.bSign = !rhs.bSign;
//	return rhs;
//}
#endif

