
#include "basicsP2\pointSetArray.h"
#include "basicsP2\Trist.h"

#include "math.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>

#include "GL\glut.h"
using namespace std;


#define T2C				1		//Tranform to center
#define INF 10000000

double WinWidth = 1000;
double WinHeight = 700;
double HalfWinWidth	 = (WinWidth/2);
double HalfWinHeight = (WinHeight/2);

float scaleVal = 1.0;
PointSetArray points; // The points added by the user.
Trist triangles; // The current triangles

void drawAPoint(double x,double y)
{
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(0,0,0);
#if T2C
	x *= scaleVal;
	y *= scaleVal;	
	x += HalfWinWidth;
	y += HalfWinHeight;
#endif
	glVertex2d(x,y);
	glEnd();
	glPointSize(1);
}

void drawALine(double x1,double y1, double x2, double y2)
{
	glPointSize(1);
	glBegin(GL_LINE_LOOP);
	glColor3f(0,0,1);
#if T2C
	x1 *= scaleVal;
	y1 *= scaleVal;
	x2 *= scaleVal;
	y2 *= scaleVal;

	x1 += HalfWinWidth;
	y1 += HalfWinHeight;
	x2 += HalfWinWidth;
	y2 += HalfWinHeight;
#endif
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glEnd();
	glPointSize(1);
}

void drawATriangle(double x1,double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_POLYGON);

	//to set randon color for all triangle
	double col1, col2, col3;
	col1 = (double)rand() /RAND_MAX;
	col2 = (double)rand() /RAND_MAX; 
	col3 = (double)rand() /RAND_MAX; 
	glColor3f( col1, col2, col3);
	
	// Only one colour
	//glColor3f( 0.0, 1.0, 1.0);

#if T2C
	x1 *= scaleVal;
	y1 *= scaleVal;
	x2 *= scaleVal;
	y2 *= scaleVal;
	x3 *= scaleVal;
	y3 *= scaleVal;

	x1 += HalfWinWidth;
	y1 += HalfWinHeight;
	x2 += HalfWinWidth;
	y2 += HalfWinHeight;
	x3 += HalfWinWidth;
	y3 += HalfWinHeight;
#endif
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	// Draw all triangles
	for ( int i = 0; i < triangles.noTri(); i++)
	{
		int v1, v2, v3;
		if( triangles.getVertexIdx(i << 3, v1, v2, v3) )
		{
			LongInt x1, y1, x2, y2, x3, y3;
			int res1 = points.getPoint(v1, x1, y1);
			int res2 = points.getPoint(v2, x2, y2);
			int res3 = points.getPoint(v3, x3, y3);

			// Ignore dummy helper points
			if ( (y1 < -(INF-1) || y1 > (INF-1)) ||
				(y2 < -(INF-1) || y2 > (INF-1)) ||
				(y3 < -(INF-1) || y3 > (INF-1)) )
				continue;

			if (res1 != 1 || res2 != 1 || res3 != 1)
				cout << "Error, wrong triangle point index" << endl;
			else
			{
				drawATriangle(x1.doubleValue(), y1.doubleValue(),
					x2.doubleValue(), y2.doubleValue(),
					x3.doubleValue(), y3.doubleValue());

				// Draw triangle outline
				drawALine(x1.doubleValue(), y1.doubleValue(), x2.doubleValue(), y2.doubleValue());
				drawALine(x2.doubleValue(), y2.doubleValue(), x3.doubleValue(), y3.doubleValue());
				drawALine(x3.doubleValue(), y3.doubleValue(), x1.doubleValue(), y1.doubleValue());
			}
		}
	}

	// Draw all points
	for (int i = 1; i <= points.noPt(); i++)
	{
		LongInt x, y;
		int res = points.getPoint(i, x, y);
		if (res != 1)
			cout << "Error, wrong point index" << endl;
		else
			drawAPoint(x.doubleValue(), y.doubleValue());
	}

	glPopMatrix();
	glutSwapBuffers();
}

void processIP(LongInt x, LongInt y)
{
	int ptIdx = points.checkPointExist(x, y);
	if( ptIdx < 1 ) // only add the point if it does not exist already
		int PtSz = points.addPoint(x, y); // add the point to the list of points
}

// Flip all edges in the link of pIdx until they are
// all LD. pIdx is a vertex of all triangles given
// by the three indices i idxArr
void makeEdgesLD(int* idxArr, int pIdx)
{
	for (int i = 0; i < 3; i++)
	{
		int noTri;
		int w[3];
		triangles.getVertexIdx(idxArr[i] << 3, w[0], w[1], w[2]);
			
		int adjTri[6];
		triangles.adjacentTriangles(idxArr[i], noTri, &adjTri[0]);

		// noTri is (probably) never more than 3
		for (int j = 0; j < noTri; j++)
		{
			if (adjTri[j] == idxArr[0] || adjTri[j] == idxArr[1] || adjTri[j] == idxArr[2])
				continue; // consider only triangles that do not have pIdx as a vertex
			
			int v[3];
			triangles.getVertexIdx(adjTri[j] << 3, v[0], v[1], v[2]);
			if ( PointSet::inCircle(v[0], v[1], v[2], pIdx) == 1)
			{
				int connectToP;
				// One vertex of v is not a vertex of w
				for (int k = 0; k < 3; k++)
					if (v[k] != w[0] && v[k] != w[1] && v[k] != w[2])
						connectToP = k;
				
				// The edge between v[(conncetToP+1)%3] && v[(conncetToP+2)%3]
				// should be flipped to the edge between pIdx and v[connectToP]
				// WE WANT THIS TO BE ANIMATED!

				// Remove the old triangles
				triangles.delTri(adjTri[j] << 3);
				triangles.delTri(idxArr[i] << 3);
	
				// Add the new triangles
				triangles.makeTri(pIdx, v[connectToP], v[(connectToP+1)%3], true);
				triangles.makeTri(pIdx, v[connectToP], v[(connectToP+2)%3], true);
			}
			
		}
	}
}

void processCD()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	int start = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds; cerr << "Start: " << start << endl;

	triangles.clear();
	// Add "dummy" big triangle
	triangles.makeTri(1, 2, 3);

	// Is there a need to scramble the points?
	// I assume here that their order is already random enough
	for (int j = 4; j <= points.noPt(); j++)
	{
		// Iterator thru' all triangles to check for intri
		for ( int i = 0; i < triangles.noTri(); i++)
		{
			int v1, v2, v3, v4;
			v4 = j;
			if( triangles.getVertexIdx(i << 3, v1, v2, v3) )
			{
				//Click inside one of the triangles
				if( PointSet::inTri(v1, v2, v3, v4) == 1 )
				{
					int idxArr[3]; // Indices to the new triangles
					triangles.makeTri3(i, v4, &idxArr[0]);
					//Flip edges if necessary
					makeEdgesLD(&idxArr[0], v4);
					break;
				}
			}
		}	
	}

	GetLocalTime(&st);
	int end = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds; cerr << "End: " << end << endl;
	cerr << "Elapsed Time(ms): " << end-start << endl;
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	WinWidth = w;
	WinHeight = h;
	HalfWinWidth = (WinWidth/2);
	HalfWinHeight = (WinHeight/2);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
}

void setupDataStructures()
{
	triangles.clear();
	points.eraseAllPoints();

	// Setup points for big enclosing triangle
	int v1 = points.addPoint(LongInt(-INF), LongInt(INF));
	int v2 = points.addPoint(LongInt(INF), LongInt(INF));
	int v3 = points.addPoint(LongInt(0), LongInt(-INF));
}

void readFile(){

	string line_noStr;

	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation
	string outputAns = "Answer of your computation"; // the answer you computed

	ifstream inputFile("input.txt",ios::in);

	LongInt li;

	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << "input.txt" << "\"";
		return;
	}
	while(inputFile.good()){
		getline(inputFile,line);
		if(line.empty()) {
			command = "";
			continue; 
		}// in case the line has nothing in it

		stringstream linestream(line);

		linestream >> line_noStr;
		linestream >> command;         // get the command

		if(!command.compare("IP")){
			linestream >> numberStr;
			LongInt x, y;
			x = LongInt(numberStr);
			linestream >> numberStr;
			y = LongInt(numberStr);

			processIP(x, y);
		}
		else if(!command.compare("DY")){
			// TODO: Implement delay functionality
			linestream >> numberStr;
			cout << "TODO: Implement delay: " << numberStr << endl;
		}
		else if(!command.compare("CD")){
			processCD();
		}
		else{
			cerr << "Exception: Wrong input command: " << command << endl;
		}
	}

}

void writeFile()
{
	ofstream outputFile("savefile.txt",ios::out);
	if(outputFile.fail()){
		cerr << "Error: Cannot create output file \"" << "output.txt" << "\"";
		return;
	}

	int lineNbr = 0;
	// Add all points to output file
	for (int i = 1; i <= points.noPt(); i++)
	{
		string prefix = (lineNbr < 10) ? "000" :
			(lineNbr < 100) ? "00" :
			(lineNbr < 1000) ? "0" : "";

		LongInt x, y;
		int res = points.getPoint(i, x, y);
		if (res != 1)
			cout << "Error, wrong point index" << endl;
		else
			outputFile << prefix << lineNbr++ << ": IP " << x.toString() << " " << y.toString() << endl;
	}

	// Add CD command at the end
	string prefix = (lineNbr < 10) ? "000" :
		(lineNbr < 100) ? "00" :
		(lineNbr < 1000) ? "0" : "";
	outputFile << prefix << lineNbr++ << ": CD" << endl;
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'r':
		case 'R':
			readFile();
			break;

		case 'w':
		case 'W':
			writeFile();
			break;

		case 'Q':
		case 'q':
			exit(0);
			break;

		case 'c':
		case 'C':
			processCD();
			break;
			
		// DEBUG!
		case 'e':
		case 'E':
			setupDataStructures();
			break;

		case '+':
			scaleVal += (float)0.1;
			break;

		case '-':
			if( scaleVal > 0.1 ) 
				scaleVal -= (float)0.1;
			break;

		case '.':
			scaleVal = (float)1.0;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}


void mouse(int button, int state, int x, int y)
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_RIGHT_BUTTON)&&(state == GLUT_UP))
	{
#if T2C
		x -= HalfWinWidth;
		y -= HalfWinHeight;
		x /= scaleVal;
		y /= scaleVal;
#endif
		processIP(LongInt(x), LongInt(y));
		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	cout << "CS5237 Phase III"<< endl<< endl;
	cout << "Right mouse click: Insert point"<<endl;
	cout << "R: Read in control points from \"input.txt\"" <<endl;
	cout << "C: Compute Delaunay triangulation" << endl;
	cout << "E: Erase all points" << endl;
	cout << "W: Write control points to \"savefile.txt\"" <<endl;
	cout << "Q: Quit" <<endl;
	setupDataStructures();

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (WinWidth, WinHeight);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS5237 Phase III");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
