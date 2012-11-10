#include "math.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>
#include <queue>
#include <search.h>
#include "VoronoiDiagramGenerator.h"
#include "GL\glut.h"
#include "basicsP2\pointSetArray.h"
#include "basicsP2\Trist.h"
using namespace std;

struct animateState
{
	int loop;
	float scale;
	int j, i;
	bool start;
	bool bDone;
	int state;//Zoom-in, flip, zoom-out
	int fromTri[2][3];
	int toTri[2][3];
	std::vector<TriRecord> trivec;
	unsigned int startTm, stopTm;
};

struct line_typ
{
	double x1, y1, x2, y2;
};

#define T2C				1		//Tranform to center
#define INF 10000000

PointSetArray points; // The points added by the user.
Trist triangles; // The current triangles

int weightDefault = 10;	// default weight, pls change accordingly
int weightDisplay = weightDefault;		
int weightIncr = 1;	// weight increment, pls change accordingly
int weightMax = 20;		// max weight
int weightMin = 1;		// min weight

double windowWidth = 1200;
double windowHeight = 700;
double HalfWinWidth	 = (windowWidth/2);
double HalfWinHeight = (windowHeight/2);
const double messageBoxWidth = 250;
const double gap = 5;
// window position -- or use glutGet(GLUT_WINDOW_X) to get X position ...
const int windowX = 50, windowY = 50;  
int base = 0;  // for display lists of bitmaps
GLfloat angle = 0, vPos = 0, hPos = 0, zPos = 0;
int parentWindow, playWindow, topWindow, dataWindow;
int idleTimer = 1, timerDelay = 10, frameCount = 0, intervalTimerCalls = 0;
int dataWindowHeight, dataWindowWidth;

typedef std::vector<line_typ> LINE_VEC;


// strings for information data display
const int nMsg = 60, messageLength = 80;
char message[nMsg][messageLength];
double ip_x = 0, ip_y = 0;
float delay = 10;
bool bAnimate = false;
animateState aniState;
float scaleVal = 1.0;
bool bCompDel = false;
unsigned int compCDTime = 0;

//Function for computation
void processIP(LongInt x, LongInt y);
void makeEdgesLD(queue<int>& idxArr, int pIdx);
void processCD();
void animateCD();

//Init function
void initTextList();
void initWindow(float r, float g, float b, float a);

//Draw function
void drawPlay(void);
void drawData(void);
void drawAPoint(double x,double y);
void drawALine(double x1,double y1, double x2, double y2);
void drawALineRed(double x1,double y1, double x2, double y2);
void drawALineGreen(double x1,double y1, double x2, double y2);
void drawATriangle(double x1,double y1, double x2, double y2, double x3, double y3, bool randomColor);
void showText(int x, int y, int z);


// callback, responds to window resize events
void resizeParent(int width, int height) 
{
	// don't let window resize -- restore postion and size
	//if (width != windowWidth && height != windowHeight) {
	windowWidth = width;
	windowHeight = height;
	glutPositionWindow(windowX, windowY);
	glutReshapeWindow(windowWidth, windowHeight);
	//}
	/* Reset the viewport, if window resizes were handled */
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width/2, width, -height/2, height, -1000, 1000.0);
}

// Respond to window resize events
void resizePlay(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,height,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void resizeTop(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width / (float)height, 1.0, 2000.0);
}   

void resizeData(int width, int height) 
{
	dataWindowHeight = height;
	dataWindowWidth = width;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width/2, width, -height/2, height, -100, 1000.0);
}

// set and redraw all the subWindows
void redisplayAll() 
{
	glutSetWindow(parentWindow);  // drawn only when needed
	glutPostRedisplay();
	glutSetWindow(dataWindow);   // drawn only when needed
	glutPostRedisplay(); 
	glutSetWindow(playWindow);
	glutPostRedisplay();
}

// Estimate FPS, use for fixed interval timer driven animation
void intervalTimer (int i) 
{ 
	glutTimerFunc(timerDelay, intervalTimer, 1);
	// compute frames / second
	intervalTimerCalls++;
	if ( (intervalTimerCalls % 10) == 0) { 
		sprintf(message[21],"IP[x,y]: %d, %d", (int)ip_x, (int)ip_y);
		sprintf(message[22],"DY[in msec]: %d", (int)delay);
		sprintf(message[23],"Zoom[%%]: %d %%", (int)((scaleVal/1.0)*100));
		sprintf(message[24],"CD[On|Off]: %s", bCompDel? "On": "Off");
		sprintf(message[25],"Animation[On|Off]: %s", bAnimate? "On": "Off");
		sprintf(message[26],"Computation Time[in msec]: %d", compCDTime);

		sprintf(message[41],"From: Point[a,b,c] and Point[a,b,d]");
		sprintf(message[42],"To: Point[b,c,d] and Point[a,c,d]");
		sprintf(message[41],"From: Point[%d,%d,%d] and Point[%d,%d,%d]", aniState.fromTri[0][0], aniState.fromTri[0][1], aniState.fromTri[0][2]
																		, aniState.fromTri[1][0], aniState.fromTri[1][1], aniState.fromTri[1][2]);
		sprintf(message[42],"To: Point[%d,%d,%d] and Point[%d,%d,%d]",aniState.toTri[0][0], aniState.toTri[0][1], aniState.toTri[0][2]
																	,aniState.toTri[1][0], aniState.toTri[1][1], aniState.toTri[1][2]);
		sprintf(message[43],"Last point Weight [%d~%d]: %d ", weightMin, weightMax, weightDisplay);
		glutSetWindow(dataWindow);
		glutPostRedisplay();
	}

	const int delayDYCnt = (int)(delay / timerDelay);
	if ( ((intervalTimerCalls % delayDYCnt) == 0) && bAnimate && aniState.start)
	{
		//Start animation here
		animateCD();
		glutSetWindow(dataWindow);
		glutPostRedisplay();
	}
}

// render parentWindow    
void drawParent(void) 
{
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}      


LINE_VEC computeVoronoi()
{
	int i, pos;
	LINE_VEC lineVec;
	float* xValues;
    float* yValues;
	const int nSize = points.noPt();
	xValues = new float[nSize];
    yValues = new float[nSize];
	for( i=1, pos=0; i<nSize+1; ++i)
	{
		LongInt x1, y1;
		int res = points.getPoint(i, x1, y1);
		if (res == 1)
		{
			if(y1 < -(INF-1) || y1 > (INF-1))
				continue;
			xValues[pos] = static_cast<float>(x1.doubleValue());
			yValues[pos] = static_cast<float>(y1.doubleValue());
			pos++;
		}
	}
    VoronoiDiagramGenerator vdg;
    vdg.generateVoronoi(xValues,yValues,pos, -1000,1000,-1000,1000,3);
    vdg.resetIterator();

    float x1,y1,x2,y2;
    while(vdg.getNext(x1,y1,x2,y2))
    {
		line_typ lp;
		lp.x1 = x1;
		lp.y1 = y1;
		lp.x2 = x2;
		lp.y2 = y2;
		lineVec.push_back(lp);
    }
	return lineVec;
}

void drawTrist() 
{
	static int s_counter = 0;
	LINE_VEC lv = computeVoronoi();

	glPushMatrix();

	if(bAnimate && aniState.state == 1)
	{
		LongInt x1, y1, x2, y2, x3, y3;
		int res1 = points.getPoint(aniState.toTri[0][0], x1, y1);
		int res2 = points.getPoint(aniState.toTri[0][1], x2, y2);
		int res3 = points.getPoint(aniState.toTri[0][2], x3, y3);
		drawATriangle(x1.doubleValue(), y1.doubleValue(),
					x2.doubleValue(), y2.doubleValue(),
					x3.doubleValue(), y3.doubleValue(), aniState.start);
		res1 = points.getPoint(aniState.toTri[1][0], x1, y1);
		res2 = points.getPoint(aniState.toTri[1][1], x2, y2);
		res3 = points.getPoint(aniState.toTri[1][2], x3, y3);
		drawATriangle(x1.doubleValue(), y1.doubleValue(),
					x2.doubleValue(), y2.doubleValue(),
					x3.doubleValue(), y3.doubleValue(), aniState.start);
		aniState.state=0;
		aniState.loop++;
	}

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
				cerr << "Error, wrong triangle point index" << endl;
			else
			{
				drawATriangle(x1.doubleValue(), y1.doubleValue(),
					x2.doubleValue(), y2.doubleValue(),
					x3.doubleValue(), y3.doubleValue(), aniState.start);

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
	//Draw Voronoi diagram
	for(int i=0; i<lv.size(); ++i)
	{
		drawALine( lv[i].x1, lv[i].y1, lv[i].x2, lv[i].y2);
	}
	glPopMatrix();
	//glutSwapBuffers();
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
			linestream >> numberStr;
			LongInt dy;
			dy = LongInt(numberStr);
			delay = (int)(dy.doubleValue()) * 1000;
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
	if( !aniState.start)
		processCD();
	break;

	// DEBUG!
case 'e':
case 'E':
	setupDataStructures();
	break;

case 'a':
case 'A':
	bAnimate = !bAnimate;
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

case 'm':
case 'M':
	if (weightDisplay < weightMax) {
		weightDisplay += weightIncr;
		points.setLastPointWeight(weightDisplay);
	}
	break;

case 'n':
case 'N':
	if (weightDisplay > weightMin) {
		weightDisplay -= weightIncr;
		points.setLastPointWeight(weightDisplay);
	}
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

int main(int  argc, char *argv[])  
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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	parentWindow = glutCreateWindow("COMPUTATIONAL GEOMETRY AND APPLICATIONS");
	glutPositionWindow(windowX, windowY);
	// set glut callback functions
	glutReshapeFunc(resizeParent);
	glutDisplayFunc(drawParent);
	glutKeyboardFunc(keyboard);

	// make play window
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	double playWindowWidth = windowWidth - messageBoxWidth - gap*2;
	double playWindowHeight = windowHeight -  - gap*2;
	playWindow = glutCreateSubWindow(parentWindow, gap, gap, playWindowWidth, playWindowHeight);
	// set glut callback functions for playWindow
	glutReshapeFunc(resizePlay);
	glutDisplayFunc(drawPlay); 
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(redisplayAll);  

	// call intervalTimer every second, last arg not used
	glutTimerFunc(timerDelay, intervalTimer, 1); 
	// make data window
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	double dataWindowX = windowWidth - messageBoxWidth;
	double dataWindowWidth = messageBoxWidth - gap;
	double dataWindowHeight = windowHeight -  - gap*2;
	dataWindow = glutCreateSubWindow(parentWindow, dataWindowX, gap, dataWindowWidth, dataWindowHeight);
	glutReshapeFunc(resizeData);
	glutDisplayFunc(drawData); 

	initTextList();  // create character display list
	glutSetWindow(playWindow);
	glutMainLoop();
	return (0);
}

// Create character display lists for text display.
void initTextList() 
{
	for(int i = 0; i < 256; i++) 
	{  // fill ASCII char lists
		glNewList(base + i, GL_COMPILE);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, i);
		glEndList(); 
	}
	glListBase(base);  // set list base
	sprintf(message[0],"CS5237 Phase III");
	sprintf(message[1],"[Information]------------------------------------------");
	sprintf(message[2],"Right mouse click: Insert point");
	sprintf(message[3],"R|r: Read in control points from \"input.txt\"");
	sprintf(message[4],"C|c: Compute Delaunay triangulation");
	sprintf(message[5],"E|e: Erase all points");
	sprintf(message[6],"W|w: Write control points to \"savefile.txt\"");
	sprintf(message[7],"+|-: Zoom In | Zoom Out");
	sprintf(message[8],"A|a: Animation On or Off");
	sprintf(message[9],"M|m: Increase last point weight");
	sprintf(message[10],"N|n: Decrease last point weight");
	sprintf(message[11],"                                                   ");
	// Amend the number showText() if you want to add msg here
	sprintf(message[20],"[Command]-----------------------------------------");
	sprintf(message[21],"IP[x,y]: 0, 0");
	sprintf(message[22],"DY[in sec]: 0");
	sprintf(message[23],"Zoom[%%]: 100%%");
	sprintf(message[24],"CD[On|Off]: Off");
	sprintf(message[25],"Animation[On|Off]: Off");
	sprintf(message[26],"Computation Time[in msec]: 0");
	sprintf(message[27],"                                                   ");
	// Amend the number showText() if you want to add msg here
	sprintf(message[40],"[Working]-----------------------------------------");
	sprintf(message[41],"From: Point[a,b,c] and Point[a,b,d]");
	sprintf(message[42],"To: Point[b,c,d] and Point[a,c,d]");
	sprintf(message[43],"Last point Weight [%d~%d]: 10", weightMin, weightMax);
}
// initialize a window 
void initWindow(float r, float g, float b, float a) 
{
	// light values
	static GLfloat diffuse[4] = { 0.0f, 0.3f, 0.3f, 1.0f };
	static GLfloat ambient[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
	static GLfloat lightPos[4] = {-50.0f, 50.0f, -10.0f, 1.0f };  // ambient light
	// Clear the color and depth buffers
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);               
	// set light materials
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}

void processIP(LongInt x, LongInt y)
{
	int ptIdx = points.checkPointExist(x, y);
	if( ptIdx < 1 ) // only add the point if it does not exist already
		int PtSz = points.addPoint(x, y, weightDefault); // add the point to the list of points
	ip_x = x.doubleValue();
	ip_y = y.doubleValue();
	weightDisplay = weightDefault;
}

void buildTriangulation(int curPoint)
{
	// Iterator thru' all triangles to check for intri
	for ( int i = 0; i < triangles.noTri(); i++)
	{
		int v1, v2, v3;
		if( triangles.getVertexIdx(i << 3, v1, v2, v3) )
		{
			//Click inside one of the triangles
			if( PointSet::inTri(v1, v2, v3, curPoint) == 1 )
			{
				int idxArr[3]; // Indices to the new triangles
				triangles.makeTri3(i, curPoint, &idxArr[0]);

				//Flip edges if necessary
				queue<int> adjTri;
				adjTri.push(idxArr[0]); adjTri.push(idxArr[1]); adjTri.push(idxArr[2]); 
				makeEdgesLD(adjTri, curPoint);
				break;
			}
		}
	}
}

void animateCD()
{
	if(bAnimate && aniState.start)
	{
		if(aniState.j <= points.noPt())
		{
			buildTriangulation(aniState.j);
			aniState.j++;
		}
		else
		{
			SYSTEMTIME st;
			aniState.start = false;
			aniState.bDone = true;
			aniState.loop = 0;
			GetLocalTime(&st);
			aniState.stopTm = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds; 
			cerr << "End: " << aniState.stopTm << endl;
			cerr << "Elapsed Time(ms): " << aniState.stopTm-aniState.startTm << endl;
			compCDTime = (aniState.stopTm-aniState.startTm);
			scaleVal = aniState.scale;
			return;
		}
	}
}

void processCD()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	int start = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds; cerr << "Start: " << start << endl;

	if( !bAnimate )
	{
		triangles.clear();
		// Add "dummy" big triangle
		triangles.makeTri(1, 2, 3);
		for (int j = 4; j <= points.noPt(); j++)
			buildTriangulation(j);

		GetLocalTime(&st);
		int end = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds; cerr << "End: " << end << endl;
		cerr << "Elapsed Time(ms): " << end-start << endl;
		compCDTime = (end-start);
	}
	else
	{
		aniState.startTm = start;
		aniState.start = true;
		aniState.i = 0;
		aniState.j = 4;
		aniState.state = 0;
		aniState.bDone = false;
		aniState.scale = scaleVal;
		aniState.trivec = triangles.getTriangles();

		if (triangles.noTri() == 0) {
			triangles.clear();
			// Add "dummy" big triangle
			triangles.makeTri(1, 2, 3);
		}
		animateCD();
	}
}


// Flip all edges in the link of pIdx until they are
// all LD. pIdx is a vertex of all triangles given
// by the three indices i idxArr
void makeEdgesLD(queue<int>& idxArr, int pIdx)
{
	while(!idxArr.empty())
	{
		int curAdjTri = idxArr.front();
		idxArr.pop();
		int noTri;
		int w[3];
		triangles.getVertexIdx(curAdjTri << 3, w[0], w[1], w[2]);

		int adjTri[6];
		triangles.adjacentTriangles(curAdjTri, noTri, &adjTri[0]);

		int v[3];
		// noTri is (probably) never more than 3
		for (int j = 0; j < noTri; j++)
		{
			triangles.getVertexIdx(adjTri[j] << 3, v[0], v[1], v[2]);

			if (v[0] == pIdx || v[1] == pIdx || v[2] == pIdx)
				continue;  // consider only triangles that do not have pIdx as a vertex
			
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

				if(bAnimate)
				{
					aniState.fromTri[0][0] = v[0];
					aniState.fromTri[0][1] = v[1];
					aniState.fromTri[0][2] = v[2];
					aniState.fromTri[1][0] = w[0];
					aniState.fromTri[1][1] = w[1];
					aniState.fromTri[1][2] = w[2];
					aniState.toTri[0][0] = pIdx;
					aniState.toTri[0][1] = v[connectToP];
					aniState.toTri[0][2] = v[(connectToP+1)%3];
					aniState.toTri[1][0] = pIdx;
					aniState.toTri[1][1] = v[connectToP];
					aniState.toTri[1][2] = v[(connectToP+2)%3];
					aniState.state = 1;

					LongInt x1, y1, x2, y2;
					int res1 = points.getPoint(v[(connectToP+1)%3], x1, y1);
					int res2 = points.getPoint(v[(connectToP+2)%3], x2, y2);
					if (res1 == 1 && res2 == 1
						&& !(y1 == INF || y2 == INF) && !(y1 == -INF || y2 == -INF)) {
						drawALineRed(x1.doubleValue(), y1.doubleValue(), x2.doubleValue(), y2.doubleValue());
						glutSwapBuffers();
						glPopMatrix();
						glutPostRedisplay();
						Sleep(1000);
					}
					res1 = points.getPoint(pIdx, x1, y1);
					res2 = points.getPoint(v[connectToP], x2, y2);
					if (res1 == 1 && res2 == 1
						&& !(y1 == INF || y2 == INF) && !(y1 == -INF || y2 == -INF)) {
						drawALineGreen(x1.doubleValue(), y1.doubleValue(), x2.doubleValue(), y2.doubleValue());
						glutSwapBuffers();
						glPopMatrix();
						glutPostRedisplay();
						Sleep(1000);
					}

				}
				// Remove the old triangles
				triangles.delTri(adjTri[j] << 3);
				triangles.delTri(curAdjTri << 3);

				// Add the new triangles
				idxArr.push(triangles.makeTri(pIdx, v[connectToP], v[(connectToP+1)%3], true));
				idxArr.push(triangles.makeTri(pIdx, v[connectToP], v[(connectToP+2)%3], true));
			}
		}
	}
}

// Render the main "play" scene  
void drawPlay(void)
{   
	glutSetWindow(playWindow);
	initWindow(0.2f, 0.2f, 0.2f, 0.2f);
	frameCount++;  // for fps estimation
	// set and initialize model view 
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();
	// set camera with gluLookAt
	//gluLookAt( 0, 0, 300, 0, 0, 0, 0, 1,   0);    // up vector
	drawTrist();
	glutSwapBuffers();
}

// present text in a viewport
void drawData(void) 
{
	glutSetWindow(dataWindow);
	initWindow(0.2f, 0.2f, 0.2f, 0.2f);  
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();
	// left border = dataWindowWidth / 2 or 185/3 = 92.5 ... so -80 gives a margin
	showText(-120, 40, 0);  
	glutSwapBuffers();
	//printf("drawData\n");
}

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
	glColor3f(1,1,1);
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

void drawALineRed(double x1,double y1, double x2, double y2)
{
	glPointSize(1);
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0,0,0);
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
}

void drawALineGreen(double x1,double y1, double x2, double y2)
{
	glPointSize(1);
	glBegin(GL_LINE_LOOP);
	glColor3f(0,1.0,0);
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
}

void drawATriangle(double x1,double y1, double x2, double y2, double x3, double y3, bool randomColor)
{
	glBegin(GL_POLYGON);

	randomColor = false;
	if(randomColor)
	{
		//to set random color for all triangle
		double col1, col2, col3;
		col1 = (double)rand() /RAND_MAX;
		col2 = (double)rand() /RAND_MAX; 
		col3 = (double)rand() /RAND_MAX; 
		glColor3f( col1, col2, col3);
	}
	else
	{
		// Only one colour
		glColor3f( 0.3, 0.3, 0.7);
	}
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



// Show text information.  Lighting, color, position tricky...
void showText(int x, int y, int z)
{
	// Lighting interacts with the glColor... disable, enable
	glDisable(GL_LIGHTING);
	glColor3f(1.0f,1.0f,0.0f);  // color must be changed before glRasterPos
	int yOffset = dataWindowHeight;  // about 380
	for(int i = 0; i < nMsg; i++) {
		yOffset -= 30;
		glRasterPos3i(x, yOffset, z);  
		char* cstr = message[i];	
		while (*cstr) {
			switch(i)
			{
			case 0:
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *cstr++);
				glColor3f(1.0f,1.0f,1.0f); 
				break;
			case 11:	// amend if new msg is added
				i = 19;
				break;
			case 27:	// amend if new msg is added
				i = 39;
				break;
			case 19:
			case 39:
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *cstr++);
				glColor3f(1.0f,1.0f,1.0f);
				break;
			default:
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *cstr++);
				glColor3f(0.7f,0.7f,0.7f);
			}
			//glutStrokeCharacter(GLUT_STROKE_ROMAN,*cstr++);
		}
	}
	glEnable(GL_LIGHTING);
}
