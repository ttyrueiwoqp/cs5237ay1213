#ifndef POINTSETARRAYH
#define POINTSETARRAYH

#include "..\basics\pointSet.h"

using namespace std;

class PointSetArray : public PointSet {

public:

	int getPoint (int pIndex, LongInt& x1,LongInt& y1); // put the x,y values into x1,y1, and return 1 if the point pIndex exists
	int noPt();                                         // return the number of points
	void eraseAllPoints();                              // erase all points in this array
	void print();										// for debugging only
	void eraseLastPoint();								// remove the last point when it fail in tri test
	int checkPointExist(LongInt& x1,LongInt& y1);		// check if the point exist return point idx otherwise -1 
	void setLastPointWeight(int w);
};

#endif