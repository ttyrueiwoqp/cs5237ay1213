#include "trist.h"
#include <vector>
#include <iostream>

using namespace std;

vector<TriRecord> triangles;

Trist::Trist()
{
	en_[0] = 1;
	en_[1] = 2;
	en_[2] = 0;
	en_[3] = 5;
	en_[4] = 3;
	en_[5] = 4;

	triangles.clear();
}

// return the number of triangles
int Trist::noTri()
{
	return triangles.size();
}

// Add a triangle into the Trist with the three point indices
// Moreover, automatically establish the fnext pointers to its neigbhours if autoMerge = true
int Trist::makeTri(int pIndex1,int pIndex2,int pIndex3,bool autoMerge)
{
	// TODO: Need to take care of automerge!

	TriRecord newTriangle;
	newTriangle.vi_[0] = pIndex1;
	newTriangle.vi_[1] = pIndex2;
	newTriangle.vi_[2] = pIndex3;
	triangles.push_back(newTriangle);

	return triangles.size() - 1; // return the index of the triangle
}

// Delete a triangle, but you can assume that this is ONLY used by the IP operation
// You may want to make sure all its neighbours are detached (below)
void Trist::delTri(OrTri tri) 
{
	
}
		
OrTri Trist::enext(OrTri ef)
{
	// Get the triangles index and the triangle version
	int tInx = ef >> 3;
	int tVersion = ef & 7;

	tVersion = en_[tVersion]; // find enext version
	return (tInx << 3) | tVersion;
}

OrTri Trist::sym(OrTri ef)
{
	int tInx = ef >> 3;
	int tVersion = ef & 7;

	tVersion = (tVersion + 3) % 6; // find the symmetric version
	return (tInx << 3) | tVersion;
}

OrTri fnext(OrTri ef)
{
	// This is obviously wrong
	return ef;
}

// return the three indices of the three vertices by OrTri
void Trist::getVertexIdx(OrTri tri, int& pIdx1,int& pIdx2,int& pIdx3)
{
	// Get the index of the triangle
	int tInx = tri >> 3;

	if (tInx < 0 || tInx >= (int)triangles.size())
		cerr << "Illegal triangle index: " << tInx << endl;
	else
	{
		pIdx1 = triangles[tInx].vi_[0];
		pIdx2 = triangles[tInx].vi_[1];
		pIdx3 = triangles[tInx].vi_[2];
	}
}

// the index of the first vertex of OrTri, e.g. org(bcd) => b
int Trist::org(OrTri tri)
{
	int tInx = tri >> 3;
	int tVersion = tri & 7;

	return (tVersion < 3) ? triangles[tInx].vi_[tVersion] :
		triangles[tInx].vi_[(tVersion +1) % 3];
}

// the index of the second vertex of OrTri, e.g. org(bcd) => c
int Trist::dest(OrTri tri)
{
	int tInx = tri >> 3;
	int tVersion = tri & 7;

	return (tVersion < 3) ? triangles[tInx].vi_[(tVersion +1) % 3] :
		triangles[tInx].vi_[tVersion -3];
}

// glue two neighbouring triangles, result abd = fnext(abc)
void Trist::fmerge(OrTri abc, OrTri abd) 
{

}

// detach triangle abc with all its neighbours (undo fmerge)
void Trist::fdetach(OrTri abc)
{

}

// A suggested function: you may want this function to return all the OrTri
// that are incident to this point
// Ignore this if you don't feel a need
void Trist::incidentTriangles(int ptIndex,int& noOrTri, OrTri* otList)
{

}

