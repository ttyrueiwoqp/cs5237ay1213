#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "li.h"

static unsigned int maxVal(){
    static unsigned int SValMax=0;
    if(SValMax==0){
        int len = sizeof(unsigned int);
        for(int i=0; i<len; ++i){
            SValMax = ( (SValMax<<8) | 0xFF );
        }
    }
    return SValMax;
}

void LongInt::setZero_() // set this number to be zero
{
	m_data.clear();
}

LongInt::LongInt()
: m_SzOfInt(sizeof(int)), m_data(0), bSign(true)
{
}

LongInt::LongInt(const LongInt& rhs) // copy constructor
: m_SzOfInt(sizeof(unsigned int))
{
	m_data = rhs.m_data;
    bSign = rhs.bSign;
}

LongInt::LongInt(const int& _val)      // take in an integer
: m_SzOfInt(sizeof(unsigned int)), bSign(true)
{
	setZero_();
	m_data.push_back(_val);
}

void LongInt::dump()       // print out this number
{
	std::cout<<"Dump longint data:"<< toString();
}

LongInt& LongInt::operator=(const int& val)
{
	setZero_();
	m_data.push_back(val);
	return *this; // Return a reference to myself.
}

//assignment operator
LongInt& LongInt::operator=(const std::string& s)
{
    bool bNegative;
    std::vector<unsigned int> vec = this->string2Vec(s, bNegative);
    bSign = !bNegative;
    m_data = vec;
    return *this;
}

//assignment operator
LongInt& LongInt::operator=(const LongInt& rhs)
{
    m_data = rhs.m_data;
    bSign = rhs.bSign;
	return *(this);
}

//this function assume that m_data and vec having the same sign
void LongInt::addValue(std::vector<unsigned int> vec)
{
    int carryover = 0;
    int sz1 = m_data.size();
    int sz2 = vec.size();
    int loop = (sz1>sz2)? sz1: sz2;
    for(int i=0; i<loop; ++i)
    {
		if(i==m_data.size()) m_data.push_back(0);
		if(i==vec.size()) break;
        unsigned int val = m_data[i] + vec[i] + carryover;
        carryover = ( val < std::max(m_data[i], vec[i]))? 1: 0;
        m_data[i] = val;
    }
}

//In this function, we v1 - v2 provided v1>v2
std::vector<unsigned int> LongInt::substractValue(std::vector<unsigned int> v1, std::vector<unsigned int> v2)
{
    std::vector<unsigned int> tmp;
    int sz1 = v1.size();
    int sz2 = v2.size();
    int borrow = 0;
    int loop = (sz1>sz2)? sz1: sz2;
    for(int i=0; i<loop; ++i){
        unsigned int num = 0;
		if(i==v1.size()) 
			v1.push_back(0);
		if(i==v2.size()) v2.push_back(0);

		const unsigned int val1 = v1[i];
		const unsigned int val2 = v2[i];

        if(val1 >= val2){
            num = val1 - val2- borrow;
            borrow = 0;
        }
        else{
            num = maxVal() - ( val2 + borrow);
            num += (v1[i] + 1);
            borrow = 1;
        }
        tmp.push_back(num);
    }

    return tmp;
}

//equal return 0, v1>v2 return 1 else -1
int LongInt::compare(std::vector<unsigned int> v1, std::vector<unsigned int> v2)
{
    int i, sz1, sz2;
    while (!v1.empty())
    {
        if( v1.back() == 0)
            v1.pop_back();
        else break;
    }

    while (!v2.empty())
    {
        if( v2.back() == 0)
            v2.pop_back();
        else break;
    }
    sz1 = v1.size();
    sz2 = v2.size();
    if(sz1>sz2) return 1;
    else if( sz2>sz1)   return-1;
    else{
        for(i=sz1-1; i>=0; --i){
            if( v1[i] > v2[i] )   return 1;
            if( v1[i] < v2[i] )   return -1;
        }
    }
    return 0;
}


void reverseString(std::string &theWord)
{	// Reverse the string contained in theWord.
	int i;
	char temp;
	for (i=0; i<theWord.length()/2; i++)
	{
		temp = theWord[i];
		theWord[i] = theWord[theWord.length()-i-1];
		theWord[theWord.length()-i-1] = temp;
	}
}  

//convert string to vector
std::vector<unsigned int> LongInt::string2Vec(std::string s, bool& negative)
{
    std::vector<unsigned int> vec;
	std::string stmp = s;
	reverseString(stmp);
    if( this->isNegative(stmp) )
    {
        int sz = stmp.size();
        stmp = stmp.substr(0, sz-1);
        negative = true;
    }
    else negative = false;

    unsigned int num;
	const int numChar = m_SzOfInt * 2;
	int sz = stmp.size();
    double dtmp = static_cast<double>((sz*1.0 / numChar));
	int nloop = static_cast<int>(ceil(dtmp));
	int rem = stmp.size() % numChar;
    for(int i=0; i<nloop; ++i)
    {
        std::istringstream iss;
        int off = i*numChar;
        std::string s2 = stmp.substr(off,numChar);
		reverseString(s2);
        iss.str(s2);
		if(!(iss >> std::hex >> num).fail() ){
			vec.push_back(num);
		}
		else{
			std::cerr<<"\nInvalid conversation from string to integer: "<<s2;
			exit(1);
		}
    }
    return vec;
}


LongInt LongInt::operator+(const int& val) {
    if(m_data.size() == 0){
        if(val<0)   bSign = false;
        m_data.push_back(val);
    }
    return (*this);
}

LongInt LongInt::operator+(const LongInt& rhs)
{
    std::string str = rhs.toString();
    LongInt li = this->operator+(str);
    return li;
}

LongInt LongInt::operator-(const LongInt& rhs)
{
    std::string str = rhs.toString();
    LongInt li = this->operator-(str);
    return li;
}
//void LongInt::setValue( std::vector<unsigned int> data,  )
//{
//	m_data = data;
//}

LongInt LongInt::operator-()
{
	this->bSign = !this->bSign;
	return(*this);
}

LongInt LongInt::operator+(const std::string& str)
{
    bool bNegative;
    std::string sval;
	//to store the original value
	std::vector<unsigned int> OriginData = m_data;
	bool bOriginSign = bSign;

	

    std::vector<unsigned int> v2 = this->string2Vec(str, bNegative);
    int sz = str.size();
    if( (!bNegative) == bSign)
    { //both +ve or -ve
        if( isNegative(str) )//remove the sign '-' from str if -ve
           sval = str.substr(0, sz-1);
        addValue( v2 );
    }
    else
    {
        //sign is not the same
        std::vector<unsigned int> result;
        std::vector<unsigned int> v1 = m_data;
        if(compare(v1, v2) == 1){// v1 is greater than v2
            result = substractValue(v1, v2);
            m_data = result;
        }
        else {
            result = substractValue(v2, v1);
            m_data = result;
            bSign = !bSign;
        }
    }
	LongInt li1 = *this;
	//retrieve back the data
	m_data = OriginData;
	bSign = bOriginSign;
    return li1;
}

LongInt LongInt::operator-(const int& i) {
    return (*this) - LongInt(i);
}

LongInt LongInt::operator-(const std::string s)
{
    bool bNegative;
    std::string str = s;
    int sz = str.size();
    std::vector<unsigned int> v2 = this->string2Vec(str, bNegative);
    if( bSign && bNegative){
        addValue( v2 );
    }else if( !bSign && !bNegative){
        addValue( v2 );
    }else if( bSign && !bNegative){
        std::vector<unsigned int> result;
        std::vector<unsigned int> v1 = m_data;
        if(compare(v1, v2) != -1){// v1 is greater than v2
            result = substractValue(v1, v2);
            m_data = result;
        }else {
            result = substractValue(v2, v1);
            m_data = result;
            bSign = false;
        }
    }else{
        std::vector<unsigned int> result;
        std::vector<unsigned int> v1 = m_data;
        str = str.substr(0, sz-1);
        if(compare(v1, v2) == 1){// v1 is greater than v2
            result = substractValue(v1, v2);
            m_data = result;
        }else {
            result = substractValue(v2, v1);
            m_data = result;
            bSign = true;
        }
    }
	return *(this);
}

LongInt LongInt::operator*(const std::string str) {
	
	std::string s1, s2, stemp;
	std::vector< std::string > strList;
	
	s1 = this->toString();
	reverseString(s1);
	if( !bSign )
		s1 = s1.substr(0, str.size()-1);

	s2 = str;
	reverseString(s2);
	if( this->isNegative(str) )	
		s2 = s2.substr(0, str.size()-1);

	const int sz = s2.size(); 
	for( int i=0; i<sz; ++i){
		char buffer[10];
		unsigned short carryover = 0;
		unsigned short num2;
		
		stemp.clear();
		char c = s2[i];
		num2 = atoi(&c);

		for( int j=0; j<s1.size(); ++j )
		{
			unsigned short num1;
			c = s1[j];
			num1 = atoi(&c);
		
			unsigned short val = (num1 * num2) + carryover;
			itoa (val&0xf,buffer,16);
			stemp.insert( stemp.begin(), buffer[0]);
			carryover = ((val>>4) & 0xf);
		}
		if( carryover > 0 ){
			itoa (carryover&0xf,buffer,16);
			stemp.insert( stemp.begin(), buffer[0]);
		}
		for( int k=0; k<i; ++k )
			stemp.insert( stemp.begin(), '0');
		strList.push_back( stemp );
	}

	setZero_();
	for(int i=0; i<strList.size(); ++i )
	{
		bool bNeg;
		std::string s = strList[i];
		reverseString(s);
		this->addValue( this->string2Vec(s, bNeg) );
	}
	if( bSign == (!this->isNegative(str)) )
		bSign = true;
	else bSign = false;
    return (*this) /** LongInt(i)*/;
}

LongInt LongInt::operator*(const int& i) {
	
    return (*this) * LongInt(i);
}

LongInt LongInt::operator*(const LongInt& rhs)
{
	std::string str = rhs.toString();
	if( !rhs.bSign) str = str.substr(0, str.size()-1);
	LongInt li = this->operator *(str);
	this->bSign = (this->bSign == rhs.bSign);
	
	return li;
}

bool LongInt::operator<(const LongInt& rhs)
{
    int retval;
    if( this->bSign && rhs.bSign){
        retval = compare(m_data, rhs.m_data);
        if(retval==-1)   return true;
        else return false;
    }else if( this->bSign && !rhs.bSign){
        return false;
    }else if( !this->bSign && rhs.bSign){
        return true;
    }else{
        retval = compare(m_data, rhs.m_data);
        if(retval>0)   return true;
        else return false;
    }
}

bool LongInt::operator>(const LongInt& rhs)
{
    int retval;
    if( this->bSign && rhs.bSign){
        retval = compare(m_data, rhs.m_data);
        if(retval==1)   return true;
        else return false;
    }else if( this->bSign && !rhs.bSign){
        return true;
    }else if( !this->bSign && rhs.bSign){
        return false;
    }else{
        retval = compare(m_data, rhs.m_data);
        if(retval<0)   return true;
        else return false;
    }
}

bool LongInt::operator==(const LongInt& rhs)
{
    if( this->bSign == rhs.bSign){
        return ( compare(m_data, rhs.m_data) == 0);
    }
    return false;
}

bool LongInt::eqZero()  // return true if it is zero
{
    for(unsigned int i=0; i<m_data.size(); ++i)
    {
        if(m_data[i] != 0)
            return 0;
    }
    return 0;
}

int LongInt::sign()     // return +1 if it's positive, -1 if it's negative, 0 if it's zero
{
    if(bSign)   return 1;
    return eqZero()? 0:1;
}

bool LongInt::isNegative(std::string s)
{
	int pos = s.find('-');	//return -1 mean cannot find a -ve sign
    return (pos != std::string::npos );
}

const std::string LongInt::toString() const
{
    std::stringstream ss;
    if(!bSign) ss<<"-";

	std::vector<unsigned int>::const_reverse_iterator it;
    //for(unsigned int i=0; i<m_data.size(); ++i)
	for(it=m_data.rbegin(); it!=m_data.rend(); ++it)
    { 
		unsigned int num  = *it; 
        ss<<std::hex<<num;
    }
    return ss.str();
}

double LongInt::doubleValue() // return a double approximation (you can assume that it will not be out of bound)
{
    return 0.0;
}
