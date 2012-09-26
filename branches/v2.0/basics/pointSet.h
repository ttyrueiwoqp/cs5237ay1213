#ifndef POINTSETH
#define POINTSETH

#include "li.h"
#include <vector>

extern vector<LongInt> points_x;
extern vector<LongInt> points_y;

class PointSet {

public:

	PointSet();

	int addPoint(LongInt x1,LongInt y1); 
	// add an ith(according to the adding sequence)  point (x1,y1), return i.

	static int inTri(int p1Idx, int p2Idx, int p3Idx, int pIdx);
	// return 1 if the point p is inside the triangle p1p2p3, by reference of the indices, -1 if outside, 0 if degenerate

	static int inCircle(int p1Idx, int p2Idx, int p3Idx, int pIdx);
	// return 1 if the point p is inside the circumcircle of p1p2p3
};

#endif