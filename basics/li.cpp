#include "li.h"
#include <iostream>


void LongInt::setZero_() {
	num.clear();
	num.push_back(0);
	nSign = 0;
}

//// add param, both positive
//LongInt LongInt::add(LongInt& param) {
//	LongInt result;
//	result.nSign = 1;
//
//	int incr = 0;
//	int size = 0;
//	int a = (int) num.size();
//	int b = (int) param.num.size();
//	bool paramLonger = false;	
//	
//	// get the size of the shorter vector
//	if (a < b) {
//		size = a;
//		paramLonger = true;
//	} else {
//		size = b;
//	}
//	
//	// add the vectors from [0] to [size - 1]
//	for (int i = 0; i < size; i++) {
//		int sum = num[i] + param.num[i] + incr;
//		if (sum >= DIVIDER) {
//			incr = 1;
//			result.num.push_back(sum - DIVIDER);
//		} else {
//			incr = 0;
//			result.num.push_back(sum);
//		}
//	}
//
//	// add the rest by direct copying from the longer vector
//	if (!paramLonger) {
//		for (int i = size; i < a; i++) {
//			result.num.push_back(num[i]);
//		}
//	} else {
//		for (int i = size; i < b; i++) {
//			result.num.push_back(param.num[i]);
//		}
//	}
//	
//	// add the incr at [size]
//	if (incr == 1) {
//		if ((int) result.num.size() > size) {
//			result.num[size] += incr;
//		} else {
//			result.num.push_back(incr);
//		}
//	}
//
//	return result;
//}

// add param, both positive
vector<int> LongInt::add(vector<int> a, vector<int> b) {
	vector<int> result;

	int incr = 0;
	int size = 0;
	int p = (int) a.size();
	int q = (int) b.size();
	bool paramLonger = false;	
	
	// get the size of the shorter vector
	if (p < q) {
		size = p;
		paramLonger = true;
	} else {
		size = q;
	}
	
	// add the vectors from [0] to [size - 1]
	for (int i = 0; i < size; i++) {
		int sum = a[i] + b[i] + incr;
		if (sum >= DIVIDER) {
			incr = 1;
			result.push_back(sum - DIVIDER);
		} else {
			incr = 0;
			result.push_back(sum);
		}
	}

	// add the rest by direct copying from the longer vector
	if (!paramLonger) {
		for (int i = size; i < p; i++) {
			result.push_back(a[i]);
		}
	} else {
		for (int i = size; i < q; i++) {
			result.push_back(b[i]);
		}
	}
	
	// add the incr at [size]
	if (incr == 1) {
		if ((int) result.size() > size) {
			result[size] += incr;
		} else {
			result.push_back(incr);
		}
	}

	return result;
}

vector<int> LongInt::addAllVec(vector<vector<int>> a) {
	
	int totalSize = a.size();	// max should be DIVIDER, else split in recursion
	
	while (totalSize > DIVIDER) {
		vector<vector<int>> temp;
		for (int i = 0; i < DIVIDER; i++) {
			temp.push_back(a.back());
			a.pop_back();
		}
		a.push_back(addAllVec(temp));
		totalSize = a.size();
	}

	// track the size
	vector<int> tracker(totalSize);
	for (int i = 0; i < totalSize; i++) {
		tracker[i] = a[i].size();
	}

	vector<int> result;
	int j = 0;
	int incr = 0;
	int remain = totalSize;
	while (remain > 0) {
		int sum = incr;
		incr = 0;
		// loop though a[i] at index j
		for (int i = 0; i < totalSize; i++)	 {
			if (tracker[i] == -1) {			// a[i] already finished
				
			} else if (tracker[i] == j) {	// finish adding a[i]				
				tracker[i] = -1;
				remain--;
			} else if (a[i][j] == 0) {
			
			} else {
				int temp = sum + a[i][j];
				while (temp >= DIVIDER) {
					temp = temp - DIVIDER;
					incr++;
				}
				sum = temp;
			}
		}
		
		result.push_back(sum);
		j++;
	}
	
	if (incr != 0)
		result.push_back(incr);

	return result;
}

//// subtract param, both positive, assume abs(this) > abs(param)
//LongInt LongInt::subtract(LongInt& param) {
//	LongInt result;
//	result.nSign = 1;
//
//	int decr = 0;
//	int a = (int) num.size();
//	int b = (int) param.num.size();
//	bool paramLonger = false;
//	
//	// subtract the vectors from [0] to [b - 1]
//	for (int i = 0; i < b; i++) {
//		int sum = num[i] - decr - param.num[i];
//		if (sum < 0) {
//			decr = 1;
//			result.num.push_back(sum + DIVIDER);			
//		} else {
//			decr = 0;
//			result.num.push_back(sum);
//		}
//	}
//
//	// add the rest by direct copying from num
//	for (int i = b; i < a; i++) {
//		result.num.push_back(num[i]);
//	}
//	
//	// deduct the decr at [b]
//	// decr == 1 implies that num[b] is valid, since abs(this) > abs(param)
//	if (decr == 1) {
//		result.num[b] -= decr;
//	}
//
//	result.removeLeadingZeros();
//				
//	return result;
//}

// subtract param, both positive, assume abs(a) > abs(b)
vector<int> LongInt::subtract(vector<int> a, vector<int> b) {
	vector<int> result;

	int decr = 0;
	int p = (int) a.size();
	int q = (int) b.size();
	bool paramLonger = false;
	
	// subtract the vectors from [0] to [q - 1]
	for (int i = 0; i < q; i++) {
		int sum = a[i] - decr - b[i];
		if (sum < 0) {
			decr = 1;
			result.push_back(sum + DIVIDER);			
		} else {
			decr = 0;
			result.push_back(sum);
		}
	}

	// add the rest by direct copying from a
	for (int i = q; i < p; i++) {
		result.push_back(a[i]);
	}
	
	// deduct the decr at [q]
	// decr == 1 implies that result[q] is valid, since abs(a) > abs(b)
	if (decr == 1) {
		result[q] -= decr;
	}

	removeLeadingZeros(result);
				
	return result;
}

///***********************************************************************
//	mutiply by param which is at [n] position, where 0 < param < DIVIDER
//	only consider positive number multiplication
//	the whole idea is 
//	aaaabbbbccccdddd * eeee		// result
//	= bbbb0000dddd * eeee		// a
//	+ aaaa0000cccc0000 * eeee	// b
//************************************************************************/
//LongInt LongInt::multiply(int param, int n) {
//	// multiply by 0
//	int size = (int) num.size();
//	if (size == 0)
//		return 0;
//
//	LongInt result;
//	LongInt a;	
//	result.nSign = 1;
//	a.nSign = 1;
//
//	for (int m = 0; m < n; m++) {
//		a.num.push_back(0);
//	}
//	for (int i = 0; i < size; i += 2) {
//		int prod = num[i] * param;
//		a.num.push_back(prod % DIVIDER);
//		a.num.push_back(prod / DIVIDER);
//	}
//	if (size == 1) {
//		result = a;
//		result.removeLeadingZeros();
//		return result;
//	}
//
//	LongInt b;
//	b.nSign = 1;
//	for (int m = 0; m < n + 1; m++) {
//		b.num.push_back(0);
//	}
//	for (int j = 1; j < size; j += 2) {
//		int prod = num[j] * param;
//		b.num.push_back(prod % DIVIDER);
//		b.num.push_back(prod / DIVIDER);
//	}
//
//	result = a.add(b);
//	result.removeLeadingZeros();
//	
//	return result;
//}

/***********************************************************************
	Using Toom-Cook multiplication
	Only consider positive long int a, b
	0 is handled by caller
************************************************************************/
vector<int> LongInt::multiply(vector<int> a, vector<int> b) {

	vector<vector<int>> prod;
	vector<int> result;

	// add 0s to make the vector size of a, b multiples of NTOOM * 2, NTOOM
	while (a.size() % (NTOOM << 1) != 0)
		a.push_back(0);
	while (b.size() % NTOOM != 0)
		b.push_back(0);

	int p = a.size();
	int q = b.size();

	for (int j = 0; j < q; j += NTOOM) {
		vector<int> w, x, y, z;
		for (int k = 0; k < j; k++) {
			w.push_back(0);
			x.push_back(0);
			y.push_back(0);
			z.push_back(0);
		}

		for (int k = 0; k < NTOOM; k++) {
			y.push_back(0);
			z.push_back(0);
		}

		for (int i = 0; i < p; i += (NTOOM << 1)) {
			toom_3(w, x, a[i], a[i+1], a[i+2], b[j], b[j+1], b[j+2]);
		}
		for (int i = NTOOM; i < p; i += (NTOOM << 1)) {
			toom_3(y, z, a[i], a[i+1], a[i+2], b[j], b[j+1], b[j+2]);	
		}

		prod.push_back(w);
		prod.push_back(x);
		prod.push_back(y);
		prod.push_back(z);
	}

	result = addAllVec(prod);
	removeLeadingZeros(result);

	return result;
}

void LongInt::toom_3(vector<int>& m, vector<int>& n, int a0, int a1, int a2, int b0, int b1, int b2) {

	int p[5], q[5], r[5], t[5];

	int pt = a0 + a2;
	p[0] = a0;
	p[1] = pt + a1;
	p[2] = pt - a1; //p(-1)
	p[3] = ((p[2] + a2) << 1) - a0; //p(-2)
	p[4] = a2;

	int qt = b0 + b2;
	q[0] = b0;
	q[1] = qt + b1;
	q[2] = qt - b1; //q(-1)
	q[3] = ((q[2] + b2) << 1) - b0; //q(-2)
	q[4] = b2;

	for (int i = 0; i < 5; i++)
		r[i] = p[i] * q[i];

	t[0] = r[0];
	t[4] = r[4];
	t[3] = (r[3] - r[1]) / 3;
	t[1] = (r[1] - r[2]) >> 1;
	t[2] = (r[2] - r[0]);
	t[3] = ((t[2] - t[3]) >> 1) + (r[4] << 1);
	t[2] = t[2] + t[1] - r[4];
	t[1] = t[1] - t[3];

	//if (t[1] >= 100000000){
	//	cout << "t0\t" << t[1] << endl;
	//	cout << a0 << "\t"; cout << a1 << "\t"; cout << a2 << endl;
	//	cout << b0 << "\t"; cout << b1 << "\t"; cout << b2 << endl;
	//}
	for (int i = 0; i < 5; i += 2) {
		m.push_back(t[i] % DIVIDER);
		m.push_back(t[i] / DIVIDER);
	}

	n.push_back(0);
	for (int j = 1; j < 5; j += 2) {
		n.push_back(t[j] % DIVIDER);
		n.push_back(t[j] / DIVIDER);
	}
	n.push_back(0);	// make m.size() == n.size() == 6

}

//// check abs values, 1 if greater, 0 if equal, -1 if less
//int LongInt::absCompare(LongInt& param) {
//	int a = (int) num.size();
//	int b = (int) param.num.size();
//
//	if (a > b) {
//		return 1;
//	} else if (a < b) {
//		return -1;
//	} else {
//		for (int i = a - 1; i >= 0; i--) {
//			if (num[i] > param.num[i]){
//				return 1;
//			} else if (num[i] < param.num[i]) {
//				return -1;
//			}
//		}
//		return 0; // equal
//	}
//}

// check abs values, 1 if greater, 0 if equal, -1 if less
int LongInt::absCompare(vector<int> a, vector<int> b) {
	int p = (int) a.size();
	int q = (int) b.size();

	if (p > q) {
		return 1;
	} else if (p < q) {
		return -1;
	} else {
		for (int i = p - 1; i >= 0; i--) {
			if (a[i] > b[i]){
				return 1;
			} else if (a[i] < b[i]) {
				return -1;
			}
		}
		return 0; // equal
	}
}

//// remove the leading zeros
//void LongInt::removeLeadingZeros() {
//
//	int size = num.size();
//	for (int i = size - 1; i > 0; i--) {
//		if (num[i] == 0)
//			num.pop_back();
//		else
//			break;
//	}
//}

// remove the leading zeros
void LongInt::removeLeadingZeros(vector<int>& param) {

	int size = param.size();
	for (int i = size - 1; i > 0; i--) {
		if (param[i] == 0)
			param.pop_back();
		else
			break;
	}
}

LongInt::LongInt() {
	num.clear();
	nSign = 0;
}

LongInt::LongInt(LongInt& param) {
	num = param.num;
	nSign = param.nSign;
}

/*****************************************************
	int, e.g. 1234567890 is stored in num as follows:
	num[0] = 7890;
	num[1] = 3456;
	num[2] = 12;
	nSign = 1;
*****************************************************/
LongInt::LongInt(int i) {
	num.clear();

	if (i > 0) {
		nSign = 1;
	} else if (i < 0) {
		nSign = -1;
		i = 0 - i;
	} else {
		nSign = 0;
	}

	while (i >= DIVIDER) {
		num.push_back(i % DIVIDER);
		i /= DIVIDER;
	}
	num.push_back(i % DIVIDER);
}

LongInt::LongInt(string numStr) {
	num.clear();
	if (numStr.empty()) {
		nSign = 0;
	} else {
		// delete minus sign
		if (numStr[0] == '-') {
			nSign = -1;
			numStr.erase(0, 1);
		} else {
			nSign = 1;
		}

		// all chars except the first few
		int size = (int) numStr.size();
		for (int i = size - NDIGIT; i >= 0; i -= NDIGIT) {
			string subStr = numStr.substr(i, NDIGIT);
			num.push_back(atoi(subStr.c_str()));
		}

		// first few char
		size = size % NDIGIT;
		if (size != 0)
			num.push_back(atoi(numStr.substr(0, size).c_str()));
	}
}

void LongInt::dump() {
	cout << toString() << endl;
}

LongInt& LongInt::operator=(int i) {
	LongInt temp(i);
	num = temp.num;
	nSign = temp.nSign;
	return *this;
}

LongInt& LongInt::operator=(LongInt& param) {
	num = param.num;
	nSign = param.nSign;
	return *this;
}

LongInt LongInt::operator+(LongInt& param) {
	LongInt result;

	if (eqZero()) {
		result = param;
	} else if (param.eqZero()) {
		result = *this;
	} else if (sign() == param.sign()) {
		result.num = add(num, param.num);
		result.nSign = sign();
	} else {
		// different sign, subtract
		int temp = absCompare(num, param.num);
		if (temp == 0) {
			result.setZero_();
		} else if (temp > 0) {
			result.num = subtract(num, param.num);
			result.nSign = sign();
		} else {
			result.num = subtract(param.num, num);
			result.nSign = 0 - sign();
		}
	}

	return result;			
}

LongInt LongInt::operator-(LongInt& param) {
	LongInt result;

	if (eqZero()) {
		result = param;
		result.nSign = 0 - result.nSign;
	} else if (param.eqZero()) {
		result = *this;
	} else if (sign() == param.sign()) {
		// same sign, subtract
		int temp = absCompare(num, param.num);
		if (temp == 0) {
			result.setZero_();
		} else if (temp > 0) {
			result.num = subtract(num, param.num);
			result.nSign = sign();
		} else {
			result.num = subtract(param.num, num);
			result.nSign = 0 - sign();
		}

	} else {
		result.num = add(num, param.num);
		result.nSign = sign();
	}

	return result;
}

LongInt LongInt::operator*(LongInt& param) {
	LongInt result;
	result.setZero_();

	if (eqZero() || param.eqZero())		
		return result;
	
	result.num = multiply(num, param.num);
	result.nSign = sign() * param.sign();

	return result;
}

bool LongInt::operator>(LongInt& param) {
	if (sign() > param.sign())
		return true;
	else if (sign() < param.sign())
		return false;
	else if (sign() == 0)
		return false;

	int a = (int) num.size();
	int b = (int) param.num.size();
	int result = absCompare(num, param.num);

	return (sign() == result) ? true : false;
}

bool LongInt::operator<(LongInt& param) {
	if (sign() < param.sign())
		return true;
	else if (sign() > param.sign())
		return false;
	else if (sign() == 0)
		return false;

	int a = (int) num.size();
	int b = (int) param.num.size();
	int result = absCompare(num, param.num);

	return (sign() == 0 - result) ? true : false;
}

bool LongInt::operator==(LongInt& param) {
	if (sign() != param.sign())
		return false;

	int a = (int) num.size();
	int b = (int) param.num.size();

	if (a != b) {
		return false;
	} else {
		for (int i = a - 1; i >= 0; i--) {
			if (num[i] != param.num[i]){
				return false;
			}
		}
		return true;
	}
}

bool LongInt::eqZero() {
	if (sign() == 0)
		return true;

	int size = (int) num.size();
	for (int i = size - 1; i >= 0; i--) {
		if (num[i] != 0) {
			return false;
		}
	}
	return true;
}

int LongInt::sign() {
	return nSign;
}

vector<int> LongInt::getNum() {
	return num;
}

double LongInt::doubleValue() {
	//TODO
	return 0.0;
}

string LongInt::toString() {
	string zero = "0";
	if (sign() == 0) {
		return zero;
	}

	string result;
	char buffer[100];
	int size = (int) num.size();
	for (int i = 0; i < size - 1; i++) {
		string s = _itoa(num[i], buffer, 10);
		while (s.size() < NDIGIT)
			s = zero + s;
		result = s + result;
	}

	// first few chars
	string first = _itoa(num[size - 1], buffer, 10);
	result = first + result;
	
	// minus sign
	if (sign() == -1) {
		string minus = "-";
		result = minus + result;
	}
	return result;
}

LongInt operator-(LongInt& param) {
	LongInt result(param);
	result.nSign = 0 - param.nSign;
	return result;
}