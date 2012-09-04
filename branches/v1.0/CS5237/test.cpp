/*

	For this file, it should be sufficient to modify the function "processFile()" only.

	*/


#include <windows.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <strstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#include "li.h"
#include "lmath.h"
#include "pointSet.h"
#include "stopWatch.h"

static StopWatch globalSW;
static string groupName = "(Your one-line group name here)";

void processFile(char* ifilename, char* ofilename){

	LongInt li;

	string line_noStr;
	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation
	string outputAns = "Answer of your computation"; // the answer you computed

	ifstream inputFile(ifilename,ios::in);
	ofstream outputFile(ofilename,ios::out);

	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << ifilename << "\"";
		exit(1);
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

		if(!command.compare("LX")){
			linestream >> numberStr;
			//Perform the load operation here
			li = numberStr;

			globalSW.pause();
			outputFile << line_noStr  << " " << li.toString() << endl;
			globalSW.resume();
		}
		else if(!command.compare("AD")){
			linestream >> numberStr;
			//Perform addition
			li = li + numberStr;
			globalSW.pause();
			outputFile << line_noStr  << " " << li.toString() << endl;
			globalSW.resume();
		}
		else if(!command.compare("SB")){
			linestream >> numberStr;
			//Perform substraction here
			li = li - numberStr;
			globalSW.pause();
			outputFile << line_noStr  << " " << li.toString() << endl;
			globalSW.resume();
		}
		else if(!command.compare("MU")){
			linestream >> numberStr;
			li = li * numberStr;
			globalSW.pause();
			outputFile << line_noStr  << " " << li.toString() << endl;
			globalSW.resume();
		}
		else if(!command.compare("NG")){
			//linestream >> numberStr;
			li = -li;
			globalSW.pause();
			outputFile << line_noStr  << " " << li.toString() << endl;
			globalSW.resume();
		}
		else if(!command.compare("SD")){
			// you can assume that the three rows of matrix are in three seperate lines in the file
			linestream >> numberStr;
			getline(inputFile,line);		
			getline(inputFile,line);		
			getline(inputFile,line);

			globalSW.pause();
			outputFile << line_noStr  << " " << li.toString() << endl;
			globalSW.resume();
		}
		else if(!command.compare("AP")){
			linestream >> numberStr;

			globalSW.pause();
			outputFile << line_noStr  << " " << li.toString() << endl;
			globalSW.resume();
		}
		else if(!command.compare("IT")){
			linestream >> numberStr;

			linestream >> numberStr;

			linestream >> numberStr;

			linestream >> numberStr;

			globalSW.pause();
			outputFile << line_noStr  << " " << li.toString() << endl;
			globalSW.resume();
		}
		else if(!command.compare("IC")){
			linestream >> numberStr;

			linestream >> numberStr;

			linestream >> numberStr;

			linestream >> numberStr;

			globalSW.pause();
			outputFile << line_noStr  << " " << li.toString() << endl;
			globalSW.resume();
		}
		else{
			cerr << "Exception: Wrong input command" << endl;
		}
	}

}

void runningExperiment()
{
	globalSW.reset();
	globalSW.resume();
	processFile("input6.txt", "output.txt");
	globalSW.pause();
}

int main()
{
    SYSTEMTIME st;
	const int no_exp = 5;
	ULONGLONG t[no_exp];
	ULONGLONG total_t = 0;
    
    GetLocalTime(&st);
	int start = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds;

	cerr << groupName <<endl;
	cerr << "Start Time:  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond + (st.wMilliseconds/1000.0) << endl;

	for(int i=0;i<no_exp;i++)
	{
		runningExperiment();
		t[i] = globalSW.ms();
		total_t += t[i];
	}

    GetLocalTime(&st);
	int end = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds;
	cerr << "Ending Time: " << st.wHour << ":" << st.wMinute << ":" << st.wSecond + (st.wMilliseconds/1000.0) << endl;
	cerr << "Overall Length in millisecond(including file output): " << end-start << endl;
	cerr << "Excluding file output time:"<< endl;
		cerr << "         Total Stopwatch Length(ms): " << total_t << endl;

	for(int i=0;i<no_exp;i++)
		cerr << "         Stopwatch" << (i+1) << "Length(ms): " << t[i] << endl;
	cerr << "         Average Stopwatch Length(ms): " << total_t/no_exp << endl;
   
	system("pause");
}