#include "pointSet.h"
#include <iostream>

using namespace std;

vector<LongInt> points_x;
vector<LongInt> points_y;
vector<int> weight;

PointSet::PointSet()
{
	// Reset coordinate vectors
//	points_x.clear();
//	points_y.clear();
//	weight.clear();
}

int PointSet::addPoint(LongInt x1, LongInt y1, int w1)
{
	points_x.push_back(x1);
	points_y.push_back(y1);
	weight.push_back(w1);

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

LongInt det3x3(LongInt a, LongInt b, LongInt c,
		      LongInt d, LongInt e, LongInt f,
		      LongInt g, LongInt h, LongInt i){

	return a*e*i + b*f*g + c*d*h
		- c*e*g - b*d*i - a*f*h;
} 

/*************************
	a b 1		 a-e b-f	
det(c d 1) = det(c-e d-f)
	e f 1

returns 1 if counterclockwise, -1 if clockwise, 0 if collinear
**************************/
int orient2D(LongInt a, LongInt b, LongInt c, LongInt d, LongInt e, LongInt f){

	LongInt m = (a - e) * (d - f);
	LongInt n = (b - f) * (c - e);

	if (m == n)
		return 0;
	else if (m > n)
		return 1;
	else
		return -1;
}

/*************************
	ax ay ax*ax+ay*ay 1			ax-dx ay-dy (ax-dx)(ax-dx)+(ay-dy)(ay-dy)
det(bx by bx*bx+by*by 1) = det(	bx-dx by-dy (bx-dx)(bx-dx)+(by-dy)(by-dy)	)
	cx cy cx*cx+cy*cy 1			cx-dx cy-dy (cx-dx)(cx-dx)+(cy-dy)(cy-dy)
	dx dy dx*dx+dy*dy 1

returns:
1 if pIdx lies in counterclockwise orientation of p1, p2, p3,
-1 if pIdx lies in clockwise orientation of p1, p2, p3,
0 if on the circle
useWeights indicates wheter the the point weights should be taken into account
**************************/
int inCircleTest(int p1Idx, int p2Idx, int p3Idx, int pIdx, bool useWeights) 
{

	LongInt a = points_x[p1Idx] - points_x[pIdx];
	LongInt b = points_y[p1Idx] - points_y[pIdx];
	LongInt c = a * a + b * b;
	LongInt d = points_x[p2Idx] - points_x[pIdx];
	LongInt e = points_y[p2Idx] - points_y[pIdx];
	LongInt f = d * d + e * e;
	LongInt g = points_x[p3Idx] - points_x[pIdx];
	LongInt h = points_y[p3Idx] - points_y[pIdx];
	LongInt i = g * g + h * h;

	if (useWeights)
	{
		c = c - weight[p1Idx];
		f = f - weight[p2Idx];
		i = i - weight[p3Idx];
	}

	return det3x3(a, b, c, d, e, f, g, h, i).sign();
}

// check whether the pt lies in the circle formed by 2 pts
int inCircleTest2Pt(int p1Idx, int p2Idx, int pIdx) {

	LongInt a = points_x[pIdx] + points_x[pIdx] - points_x[p1Idx] - points_x[p2Idx];
	LongInt b = points_y[pIdx] + points_y[pIdx] - points_y[p1Idx] - points_y[p2Idx];
	LongInt c = points_x[p1Idx] - points_x[p2Idx];
	LongInt d = points_y[p1Idx] - points_y[p2Idx];

	LongInt e = a * a + b * b;
	LongInt f = c * c + d * d;

	if (e == f)
		return 0;
	else if (e < f)
		return 1;
	else
		return -1;
}

int PointSet::inCircle(int p1Idx, int p2Idx, int p3Idx, int pIdx, bool useWeights)
{
	// Decrement by one because of vector indexing
	p1Idx--; p2Idx--; p3Idx--; pIdx--;

	int orient = orient2D(points_x[p1Idx], points_y[p1Idx], 
		points_x[p2Idx], points_y[p2Idx], 
		points_x[p3Idx], points_y[p3Idx]);
	
	// collinear, return 1 if 3 pts are the same, -1 if all different
	// if 2 pts are the same, check whether the pt lies in the circle formed by 2 different pts
	if (orient == 0)
	{
		// Check whether pts are the same
		bool a = ((points_x[p2Idx] == points_x[p3Idx]) && (points_y[p2Idx] == points_y[p3Idx]));
		bool b = ((points_x[p1Idx] == points_x[p3Idx]) && (points_y[p1Idx] == points_y[p3Idx]));
		bool c = ((points_x[p1Idx] == points_x[p2Idx]) && (points_y[p1Idx] == points_y[p2Idx]));

		if (a && b)
			return 1;	// p1 & p2 & p3 are the same
		else if (a)
			return inCircleTest2Pt(p1Idx, p2Idx, pIdx);	// p2 & p3 are the same
		else if (b)
			return inCircleTest2Pt(p2Idx, p3Idx, pIdx);	// p1 & p3 are the same
		else if (c)
			return inCircleTest2Pt(p1Idx, p3Idx, pIdx);	// p1 & p2 are the same
		else
			return -1;	// collinear
	}

	int isInCircle = inCircleTest(p1Idx, p2Idx, p3Idx, pIdx, useWeights);

	if (isInCircle == 0)
		return 0;
	else if (orient == isInCircle)
		return 1;
	else
		return -1;
}
