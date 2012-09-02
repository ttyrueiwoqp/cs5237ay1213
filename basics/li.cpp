#include "li.h"

void LongInt::setZero_() {
	num.clear();
	num.push_back(0);
}

LongInt::LongInt() {
	LongInt(0);
}

LongInt::LongInt(LongInt& param) {
	num = param.num;
}

LongInt::LongInt(int i) {
	num.clear();
	while (i >= DIVIDER) {
		num.push_back(i % DIVIDER);
		i /= DIVIDER;
	};
	num.push_back(i % DIVIDER);
}

void LongInt::dump() {
	//TODO
	printf("start\n");
	for (int i = 0; i < (int) num.size(); i++) {
		printf("%i - %i\n", num[i], i);
	}
	printf("end\n");
}

LongInt& LongInt::operator=(int i) {
	return LongInt(i);
}

LongInt& LongInt::operator=(LongInt& param) {
	num = param.num;
	return *this;
}

LongInt LongInt::operator+(LongInt& param) {
	LongInt result;
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
		result.num.push_back(sum % DIVIDER);
		incr = (sum >= DIVIDER) ? 1 : 0;
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
		if (result.num.size() > size) {
			result.num[size] += incr;
		} else {
			result.num.push_back(incr);
		}
	}

	return result;
			
}

LongInt LongInt::operator-(LongInt& param) {
	//TODO
	return *this;
}

LongInt LongInt::operator*(LongInt& param) {
	//TODO
	return *this;
}

bool LongInt::operator>(LongInt& param) {
	int a = (int) num.size();
	int b = (int) param.num.size();

	if (a > b) {
		return true;
	} else if (a < b) {
		return false;
	} else {
		for (int i = a - 1; i >= 0; i--) {
			if (num[i] > param.num[i]){
				return true;
			}
		}
		return false;
	}
}

bool LongInt::operator<(LongInt& param) {
	int a = (int) num.size();
	int b = (int) param.num.size();

	if (a < b) {
		return true;
	} else if (a > b) {
		return false;
	} else {
		for (int i = a - 1; i >= 0; i--) {
			if (num[i] < param.num[i]){
				return true;
			}
		}
		return false;
	}
}

bool LongInt::operator==(LongInt& param) {
	int a = (int) num.size();
	int b = (int) param.num.size();

	if (a == b) {
		for (int i = a - 1; i >= 0; i--) {
			if (num[i] != param.num[i]){
				return false;
			}
		}
		return true;
	} else {
		return false;
	}
}

bool LongInt::eqZero() {
	int size = (int) num.size();
	for (int i = 0; i < size; i++) {
		if (num[i] != 0) {
			return false;
		}
	}
	return true;
}

int LongInt::sign() {
	//TODO
	return 0;
}

double LongInt::doubleValue() {
	//TODO
	return 0.0;
}