#include "pointSet.h"
#include <iostream>
#include <vector>

using namespace std;

vector<LongInt> points_x;
vector<LongInt> points_y;

PointSet::PointSet()
{
	// Reset coordinate vectors
	points_x.clear();
	points_y.clear();
}

int PointSet::addPoint(LongInt x1, LongInt y1)
{
	points_x.push_back(x1);
	points_y.push_back(y1);

	// Return number of points stored (== index to the added point)
	return points_x.size();
}

// Helper, returns twice the signed area of the triangle defined
// by p1, p2 and p3.
LongInt triangleSign(int p1Idx, int p2Idx, int p3Idx)
{
	LongInt res = (points_x[p1Idx] - points_x[p3Idx]) * (points_y[p2Idx] - points_y[p3Idx])
		- (points_x[p2Idx] - points_x[p3Idx]) * (points_y[p1Idx] - points_y[p3Idx]);
	return res;
}

// Checks whether the points p1 and p2 is on the same side of the line from a to b
// Returns 1 if they are, -1 if they are not or 0 if at least one of the points
// is on the border of the triangle.
int PointSet::inTri(int p1Idx, int p2Idx, int p3Idx, int pIdx)
{
	// Decrement by one because of vector indexing
	p1Idx--; p2Idx--; p3Idx--; pIdx--;
	LongInt b1, b2, b3;

	b1 = triangleSign(pIdx, p1Idx, p2Idx);
	if (b1.eqZero()) 
		return 0;

	b2 = triangleSign(pIdx, p2Idx, p3Idx);
	if (b2.eqZero()) 
		return 0;

	b3 = triangleSign(pIdx, p3Idx, p1Idx);
	if (b3.eqZero()) 
		return 0;

	return ((b1.sign() == b2.sign()) && (b2.sign() == b3.sign())) ? 1 : -1;
}

int PointSet::inCircle(int p1Idx, int p2Idx, int p3Idx, int pIdx)
{
	return 0;
}
