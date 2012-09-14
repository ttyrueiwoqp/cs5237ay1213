#include "pointSetArray.h"

using namespace std;

// put the x,y values into x1,y1, and return 1 if the point pIndex exists
int PointSetArray::getPoint (int pIndex, LongInt& x1,LongInt& y1)
{
	if (pIndex < 0 || pIndex >= (int)points_x.size())
		return 0; // points does not exist
	x1 = points_x[pIndex];
	y1 = points_y[pIndex];
	return 1;
}

// return the number of points
int PointSetArray::noPt()
{
	return points_x.size(); // == points_y.size()
}

// erase all points in this array
void PointSetArray::eraseAllPoints()
{
	points_x.clear();
	points_y.clear();
}