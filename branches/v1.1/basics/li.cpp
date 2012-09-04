#include "li.h"

void LongInt::setZero_() {
	num.clear();
	num.push_back(0);
	nSign = 0;
}

// add param, both positive
LongInt LongInt::add(LongInt& param) {
	LongInt result;
	result.nSign = 1;

	int incr = 0;
	int size = 0;
	int a = (int) num.size();
	int b = (int) param.num.size();
	bool paramLonger = false;	
	
	// get the size of the shorter vector
	if (a < b) {
		size = a;
		paramLonger = true;
	} else {
		size = b;
	}
	
	// add the vectors from [0] to [size - 1]
	for (int i = 0; i < size; i++) {
		int sum = num[i] + param.num[i] + incr;
		if (sum >= DIVIDER) {
			incr = 1;
			result.num.push_back(sum - DIVIDER);
		} else {
			incr = 0;
			result.num.push_back(sum);
		}
	}

	// add the rest by direct copying from the longer vector
	if (!paramLonger) {
		for (int i = size; i < a; i++) {
			result.num.push_back(num[i]);
		}
	} else {
		for (int i = size; i < b; i++) {
			result.num.push_back(param.num[i]);
		}
	}
	
	// add the incr at [size]
	if (incr == 1) {
		if ((int) result.num.size() > size) {
			result.num[size] += incr;
		} else {
			result.num.push_back(incr);
		}
	}

	return result;
}

// subtract param, both positive, assume abs(this) > abs(param)
LongInt LongInt::subtract(LongInt& param) {
	LongInt result;
	result.nSign = 1;

	int decr = 0;
	int a = (int) num.size();
	int b = (int) param.num.size();
	bool paramLonger = false;
	
	// subtract the vectors from [0] to [b - 1]
	for (int i = 0; i < b; i++) {
		int sum = num[i] - decr - param.num[i];
		if (sum < 0) {
			decr = 1;
			result.num.push_back(sum + DIVIDER);			
		} else {
			decr = 0;
			result.num.push_back(sum);
		}
	}

	// add the rest by direct copying from num
	for (int i = b; i < a; i++) {
		result.num.push_back(num[i]);
	}
	
	// deduct the decr at [b]
	// decr == 1 implies that num[b] is valid, since abs(this) > abs(param)
	if (decr == 1) {
		result.num[b] -= decr;
	}

	// remove leading 0s
	int c = result.num.size();
	for (int i = c - 1; i >= 0; i--) {
		if (result.num[i] == 0)
			result.num.pop_back();
		else
			break;
	}
			
	return result;
}

// multiply by 0 < param < DIVIDER, both positive
LongInt LongInt::multiply(int param, int n) {
	int size = (int) num.size();
	if (size == 0)
		return 0;
	if (size == 1)
		return num[0] * param;

	LongInt result;
	LongInt a;
	LongInt b;
	result.nSign = 1;
	a.nSign = 1;
	b.nSign = 1;

	for (int m = 0; m < n; m++) {
		a.num.push_back(0);
	}
	for (int i = 0; i < size; i += 2) {
		int prod = num[i] * param;
		a.num.push_back(prod % DIVIDER);
		a.num.push_back(prod / DIVIDER);
	}

	for (int m = 0; m < n + 1; m++) {
		b.num.push_back(0);
	}
	for (int j = 1; j < size; j += 2) {
		int prod = num[j] * param;
		b.num.push_back(prod % DIVIDER);
		b.num.push_back(prod / DIVIDER);
	}

	result = a.add(b);
	
	return result;
}

int LongInt::absCompare(LongInt& param) {
	int a = (int) num.size();
	int b = (int) param.num.size();

	if (a > b) {
		return 1;
	} else if (a < b) {
		return -1;
	} else {
		for (int i = a - 1; i >= 0; i--) {
			if (num[i] > param.num[i]){
				return 1;
			} else if (num[i] < param.num[i]) {
				return -1;
			}
		}
		return 0; // equal
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

LongInt::LongInt(int i) {
	num.clear();

	if (i > 0)
		nSign = 1;
	else if (i < 0)
		nSign = -1;
	else
		nSign = 0;

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
		if (numStr[0] == '-') {
			nSign = -1;
			numStr.erase(0, 1);
		} else {
			nSign = 1;
		}

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
	int nonZeroNum = 0;
	printf("start\n");
	if (sign() == 0) {
		printf("0");
		printf("\nend\n");
		return;
	}
	if (sign() == -1)
		printf("-");
	for (int i = (int) num.size() - 1; i >= 0; i--) {
		if (!(num[i] == 0 && nonZeroNum == 0))
			nonZeroNum++;
		if (nonZeroNum == 1) {
			printf("%i", num[i]);
		} else if (nonZeroNum > 1) {
			if (num[i] < 10)
				printf("000%i", num[i]);
			else if (num[i] < 100)
				printf("00%i", num[i]);
			else if (num[i] < 1000)
				printf("0%i", num[i]);
			else if (num[i] < 10000)
				printf("%i", num[i]);
		}
			
	}
	printf("\nend\n");
}

LongInt& LongInt::operator=(int i) {
	LongInt result(i);
	num = result.num;
	nSign = result.nSign;
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
		result = add(param);
		result.nSign = sign();
	} else {
		
		int temp = absCompare(param);
		if (temp == 0) {
			result.setZero_();
		} else if (temp > 0) {
			result = subtract(param);
			result.nSign = sign();
		} else {
			result = param.subtract(*this);
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

		int temp = absCompare(param);
		if (temp == 0) {
			result.setZero_();
		} else if (temp > 0) {
			result = subtract(param);
			result.nSign = sign();
		} else {
			result = param.subtract(*this);
			result.nSign = 0 - sign();
		}

	} else {
		result = add(param);
		result.nSign = sign();
	}

	return result;
}

LongInt LongInt::operator*(LongInt& param) {
	LongInt result;
	result.setZero_();

	if (eqZero() || param.eqZero())		
		return result;

	int a = (int) num.size();
	int b = (int) param.num.size();

	for (int i = 0; i < b; i++) {
		result = result.add(multiply(param.num[i], i));
	}

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
	int result = absCompare(param);

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
	int result = absCompare(param);

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
	for (int i = 0; i < size; i++) {
		if (num[i] != 0) {
			return false;
		}
	}
	return true;
}

int LongInt::sign() {
	return nSign;
}

double LongInt::doubleValue() {
	//TODO
	return 0.0;
}