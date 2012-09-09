#include "lmath.h"
#include <iostream>

int signDet(LongInt x1, LongInt y1, LongInt w1,
			LongInt x2, LongInt y2, LongInt w2,
			LongInt x3, LongInt y3, LongInt w3)
{
	LongInt res = x1*y2*w3 + y1*w2*x3 + w1*x2*y3 -
		w1*y2*x3 - y1*x2*w3 - x1*w2*y3;
	return (res > 0) ? 1 : (res == 0) ? 0 : -1;
}


int signDet(LongInt x[3][3])
{
	return signDet(x[0][0], x[0][1], x[0][2], x[1][0], x[1][1], x[1][2], x[2][0], x[2][1], x[2][2]);
}