#include "trist.h"
#include <list>
#include <iostream>
#include <assert.h>

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
	clear();
}

// return the number of triangles
int Trist::noTri()
{
	return triangles.size();
}

void Trist::clear()
{
	triangles.clear();
}

int Trist::makeTri3(int triIdx, int pIndex1 )
{
	int i, j;
	int idxArr[3];

	TriRecord oldTri;
	//triIdx;
	oldTri = triangles[triIdx];
	idxArr[0] = makeTri(oldTri.vi_[0], oldTri.vi_[1], pIndex1, false);
	idxArr[1] = makeTri(oldTri.vi_[0], oldTri.vi_[2], pIndex1, false);
	idxArr[2] = makeTri(oldTri.vi_[1], oldTri.vi_[2], pIndex1, false);
	triangles[triIdx].valid = false;

#if 1
	for(i=0; i<6; ++i)
	{
		OrTri orIdx = oldTri.fnext_[i];
		if( orIdx >= 0 )
		{
			int triIdx = orIdx>>3;
			int version = orIdx & 0x07;
			TriRecord tri = triangles[triIdx];
			int i_enext1 = enext(version);;
			int i_enext2 = enext(i_enext1);
			int i_enext3 = enext(i_enext2);
			int i_v1 = (i_enext1<3)? tri.vi_[i_enext1]: tri.vi_[ sym(i_enext1) ];
			int i_v2 = (i_enext2<3)? tri.vi_[i_enext2]: tri.vi_[ sym(i_enext2) ];
			int i_v3 = (i_enext3<3)? tri.vi_[i_enext3]: tri.vi_[ sym(i_enext3) ];		
			for(j=0; j<3; ++j)
			{
				if( i_v1 == triangles[ idxArr[j] ].vi_[0] && i_v2 == triangles[ idxArr[j] ].vi_[1] && i_v3 != triangles[ idxArr[j] ].vi_[2] ){
					fmerge( idxArr[j]<<3, triIdx<<3);
					break;
				}
			}
		}
	}
	for(i=1; i<3; ++i)
		fmerge( idxArr[0]<<3, idxArr[i]<<3);
#else
	fmerge(triangles[ idxArr[0] ]);
	fmerge(triangles[ idxArr[1] ]);
	fmerge(triangles[ idxArr[2] ]);
#endif
	return noTri();
}


// glue two neighbouring triangles, result abd = fnext(abc)
void Trist::fmerge(OrTri abc, OrTri abd) 
{
	int i, k;
	int triIdx1 = abc>>3;
	int triIdx2 = abd>>3;
	TriRecord& tri1 = triangles[triIdx1];
	TriRecord& tri2 = triangles[triIdx2];

	for(i=0; i<6; ++i)
	{
		int i_enext1 = enext(i);
		int i_enext2 = enext(i_enext1);
		int i_enext3 = enext(i_enext2);
		int i_v1 = (i_enext1<3)? tri1.vi_[i_enext1]: tri1.vi_[ sym(i_enext1) ];
		int i_v2 = (i_enext2<3)? tri1.vi_[i_enext2]: tri1.vi_[ sym(i_enext2) ];
		int i_v3 = (i_enext3<3)? tri1.vi_[i_enext3]: tri1.vi_[ sym(i_enext3) ];

		bool bOk = false;
		for(k=0; k<6; ++k)
		{
			int k_enext1 = enext(k);;
			int k_enext2 = enext(k_enext1);
			int k_enext3 = enext(k_enext2);
			int k_v1 = (k_enext1<3)? tri2.vi_[k_enext1]: tri2.vi_[sym(k_enext1)];
			int k_v2 = (k_enext2<3)? tri2.vi_[k_enext2]: tri2.vi_[sym(k_enext2)];
			int k_v3 = (k_enext3<3)? tri2.vi_[k_enext3]: tri2.vi_[sym(k_enext3)];

			if( i_v1 == k_v1 && i_v2 == k_v2 && i_v3 != k_v3 )
			{
				tri1.fnext_[i] = triIdx2<<3 | k;
				tri2.fnext_[k] = triIdx1<<3 | i;
				bOk = true;
				break;
			}
		}
	}
}

// detach triangle abc with all its neighbours (undo fmerge)
void Trist::fdetach(OrTri abc)
{

}

// Add a triangle into the Trist with the three point indices
// Moreover, automatically establish the fnext pointers to its neigbhours if autoMerge = true
int Trist::makeTri(int pIndex1,int pIndex2,int pIndex3,bool autoMerge)
{
	//Add a triangle
	TriRecord newTriangle;
	newTriangle.valid = true;
	newTriangle.vi_[0] = pIndex1;
	newTriangle.vi_[1] = pIndex2;
	newTriangle.vi_[2] = pIndex3;
	for( size_t i=0; i<6; ++i )	newTriangle.fnext_[i] = -1;
	if( autoMerge )	fmerge(newTriangle);
	triangles.push_back(newTriangle);
	return triangles.size() - 1; // return the index of the triangle
}

// glue two neighbouring triangles, result abd = fnext(abc)
void Trist::fmerge(TriRecord& tri) 
{
	int i, j, k;
	for(i=0; i<6; ++i){
		int i_enext1 = enext(i);;
		int i_enext2 = enext(i_enext1);
		int i_enext3 = enext(i_enext2);
		int i_v1 = (i_enext1<3)? tri.vi_[i_enext1]: tri.vi_[ sym(i_enext1) ];
		int i_v2 = (i_enext2<3)? tri.vi_[i_enext2]: tri.vi_[ sym(i_enext2) ];
		int i_v3 = (i_enext3<3)? tri.vi_[i_enext3]: tri.vi_[ sym(i_enext3) ];

		const int triSz = noTri();
		for(j=0; j<triSz; ++j)
		{
			bool bOk = false;
			if( triangles[j].valid )
			{
				for(k=0; k<6; ++k)
				{
					int k_enext1 = enext(k);;
					int k_enext2 = enext(k_enext1);
					int k_enext3 = enext(k_enext2);
					int k_v1 = (k_enext1<3)? triangles[j].vi_[k_enext1]: triangles[j].vi_[sym(k_enext1)];
					int k_v2 = (k_enext2<3)? triangles[j].vi_[k_enext2]: triangles[j].vi_[sym(k_enext2)];
					int k_v3 = (k_enext3<3)? triangles[j].vi_[k_enext3]: triangles[j].vi_[sym(k_enext3)];

					if( i_v1 == k_v1 && i_v2 == k_v2 && i_v3 != k_v3 )
					{
						tri.fnext_[i] = j<<3 | k;
						triangles[j].fnext_[k] = (triSz-1)<<3 | i;
						bOk = true;
						break;
					}
				}
			}
			if(bOk)	break;
		}
	}
}


void Trist::delTriPt(int ptIdx)
{
	int i, k;
	int noOrTri; 
	OrTri otList[10];
	incidentTriangles(ptIdx, noOrTri, otList);
	std::list<int> ptList;
	//Extract the points that connected to ptIdx 
	for(i=0; i<noOrTri; ++i )
	{ 
		int triIdx = otList[i]>>3;
		if( triIdx < triangles.size() )
		{
			TriRecord tri = triangles[triIdx];
			tri.valid = false;
			for( int j=0; j<3; ++j )
			{
				if( tri.vi_[j] != ptIdx )
					ptList.push_back( tri.vi_[j] );
			}
			
		}
	}
	ptList.sort();
	ptList.unique(); 
	
	if( ptList.size() == 3 )
	{
		int pts[3];
		std::list<int>::iterator it=ptList.begin();
		pts[0] = (*it); ++it;
		pts[1] = (*it); ++it;
		pts[2] = (*it);

		//Delete triangle
		for(i=0; i<noOrTri; ++i)	delTri(otList[i]);
	
		//Look for triangle that previously deleted
		for( i=0; i<triangles.size(); ++i )
		{
			TriRecord& tri = triangles[i];
			if( !tri.valid )
			{
				bool bOk = false;
				for(k=0; k<6; ++k)
				{
					int k_enext1 = enext(k);;
					int k_enext2 = enext(k_enext1);
					int k_enext3 = enext(k_enext2);
					int k_v1 = (k_enext1<3)? tri.vi_[k_enext1]: tri.vi_[sym(k_enext1)];
					int k_v2 = (k_enext2<3)? tri.vi_[k_enext2]: tri.vi_[sym(k_enext2)];
					int k_v3 = (k_enext3<3)? tri.vi_[k_enext3]: tri.vi_[sym(k_enext3)];

					if( pts[0] == k_v1 && pts[1] == k_v2 && pts[2] == k_v3 )					
					{
						tri.valid = true;
						this->fmerge(tri);
						break;
					}
				}
			}
		}
	}
}

// Delete a triangle, but you can assume that this is ONLY used by the IP operation
// You may want to make sure all its neighbours are detached (below)
void Trist::delTri(OrTri ottri) 
{
	int nVer;
	int triIdx = ottri>>3;
	TriRecord& tri = triangles[triIdx];
	tri.valid = false;
	for( int i=0; i<6; ++i)
	{
		OrTri fnextIdx = tri.fnext_[i];
		if(fnextIdx > -1 )
		{
			triIdx = fnextIdx>>3;
			nVer = fnextIdx & 7;
			triangles[triIdx].fnext_[nVer] = -1;	
		}
	}
}

// A suggested function: you may want this function to return all the OrTri
// that are incident to this point
// Ignore this if you don't feel a need
void Trist::incidentTriangles(int ptIndex,int& noOrTri, OrTri* otList)
{
	noOrTri = 0;
	for(int i=0; i<triangles.size(); ++i )
	{
		if( triangles[i].valid )
		{
			if( (triangles[i].vi_[0] == ptIndex) || 
				(triangles[i].vi_[1] == ptIndex) ||
				(triangles[i].vi_[2] == ptIndex) )
			{
				otList[noOrTri] = i<<3;
				noOrTri++;
			}
		}
	}
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

OrTri Trist::fnext(OrTri ef)
{
	// This is obviously wrong
	return ef;
}

// return the three indices of the three vertices by OrTri
bool Trist::getVertexIdx(OrTri tri, int& pIdx1,int& pIdx2,int& pIdx3)
{
	// Get the index of the triangle
	int tInx = tri >> 3;
 
	if (tInx < 0 || tInx >= (int)triangles.size())
	{
		cerr << "Illegal triangle index: " << tInx << endl;
	}
	else
	{
		if( triangles[tInx].valid )
		{
			pIdx1 = triangles[tInx].vi_[0];
			pIdx2 = triangles[tInx].vi_[1];
			pIdx3 = triangles[tInx].vi_[2];
			return true;
		}
	}
	return false;
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

