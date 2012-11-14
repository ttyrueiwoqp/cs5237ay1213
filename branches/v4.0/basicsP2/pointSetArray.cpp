#include <iostream>
#include <math.h>
#include "pointSetArray.h"

using namespace std;

const LongInt LI_Invalid("99999999999999999999999999999");
// put the x,y values into x1,y1, and return 1 if the point pIndex exists
int PointSetArray::getPoint (int pIndex, LongInt& x1,LongInt& y1)
{
	// Decrement by one because of vector indexing
	pIndex--;

	if (pIndex < 0 || pIndex >= (int)points_x.size())
		return 0; // points does not exist
	x1 = points_x[pIndex];
	y1 = points_y[pIndex];
	return 1;
}

int PointSetArray::getPoint (int pIndex, LongInt& x1,LongInt& y1, int& w)
{
	// Decrement by one because of vector indexing
	pIndex--;

	if (pIndex < 0 || pIndex >= (int)points_x.size())
		return 0; // points does not exist
	x1 = points_x[pIndex];
	y1 = points_y[pIndex];
	w = weight[pIndex];
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
	weight.clear();
}

void PointSetArray::print()
{
	static int LastPtSize = 0;
	if(  LastPtSize != points_x.size() )
	{
		LastPtSize = points_x.size();
		for( unsigned int i=0; i<points_x.size(); ++i )
		{
			LongInt x1 = points_x[i];
			LongInt y1 = points_y[i];
			std::cout<<"\nThe XY in string format ["<<i<<"] value is = ["<<x1.toString()<<", "<<y1.toString()<<"]";
			std::cout<<"\nThe XY in double format ["<<i<<"] value is = ["<<x1.doubleValue()<<", "<<y1.doubleValue()<<"]";
 		}
	}
}

void PointSetArray::eraseLastPoint()
{
	points_x.pop_back();
	points_y.pop_back();
}

//check if the point exist return point_idx otherwise -1 
int PointSetArray::checkPointExist(LongInt& x,LongInt& y)
{
	const double C_Tolerance = 2.0;
	for( unsigned int i=0; i<points_x.size(); ++i)
	{
		LongInt dx = points_x[i]-x;  
		LongInt dy = points_y[i]-y;
		if( (fabs(dx.doubleValue()) < C_Tolerance) && (fabs(dy.doubleValue()) < C_Tolerance) )
			return i+1;
	}
	return -1;
}

void PointSetArray::setLastPointWeight(int w)
{
	weight.back() = w;
}