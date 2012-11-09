#include <windows.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

const int leng = 500;

void print4DigitLN(int n)
{
	string str = "";
	int nD = 1 + (n ? log10((float)n) : 0);
	for (int i=0;i<4-nD;i++)
		str += "0";
	cout << str << n << ": ";
}

int main() {

	int lineNo = 0;

	int i,j,k;
	char ten[]= "9876543210";
	char tenRev[]= "1234567890";

	lineNo++;
	print4DigitLN(lineNo);
	cout << "LX 0" << endl;

	for(j=1;j<leng;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "AD ";
		for(i=0;i<j;i++)
			cout << tenRev;
		cout << endl;

		lineNo++;
		print4DigitLN(lineNo);
		cout << "AD ";
		for(i=0;i<j;i++)
			cout << ten;
		cout << endl;
	}

	for(j=1;j<leng;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "SB ";
		for(i=0;i<j;i++)
			cout << tenRev;
		cout << endl;

		lineNo++;
		print4DigitLN(lineNo);
		cout << "SB ";
		for(i=0;i<j;i++)
			cout << ten;
		cout << endl;
	}
	
	lineNo++;
	print4DigitLN(lineNo);
	cout << "LX 0" << endl;

	for(j=1;j<leng;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "AD ";
		for(i=0;i<j;i++)
			cout << tenRev;
		cout << endl;

		lineNo++;
		print4DigitLN(lineNo);
		cout << "AD ";
		for(i=0;i<j;i++)
			cout << ten;
		cout << endl;
	}

	lineNo++;
	print4DigitLN(lineNo);
	cout << "NG" << endl;

	for(j=1;j<leng;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "AD ";
		for(i=0;i<j;i++)
			cout << tenRev;
		cout << endl;

		lineNo++;
		print4DigitLN(lineNo);
		cout << "AD ";
		for(i=0;i<j;i++)
			cout << ten;
		cout << endl;
	}

	lineNo++;
	print4DigitLN(lineNo);
	cout << "LX 1" << endl;

	for(j=1;j<leng/2;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "MU ";
		for(i=0;i<j;i++)
			cout << tenRev;
		cout << endl;
	}

	for(j=1;j<leng;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "AP ";
		for(i=0;i<j;i++)
			cout << ten;
		cout << " ";
		for(i=0;i<j;i++)
			cout << ten;
		cout << endl;
	}

	for(j=1;j<leng;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "AP ";
		for(i=0;i<j;i++)
			cout << tenRev;
		cout << " ";

		for(i=0;i<j;i++)
			cout << ten;
		cout << endl;
	}

	for(j=1;j<leng;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "AP ";
		for(i=0;i<j;i++)
			cout << ten;
		cout << " ";


		for(i=0;i<j;i++)
			cout << tenRev;
		cout << endl;
	}

	for(j=1;j<leng-4;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "IT ";
		cout << j+1 << " " << j+2 << " " << j+3 << " " << j+4;
		cout << endl;
	}

	for(j=1;j<leng-4;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "IT ";
		cout << leng + j+1 << " " << j+2 << " " << leng + j+3 << " " << j+4;
		cout << endl;
	}
	for(j=1;j<leng-4;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "IT ";
		cout << j+1 << " " << j+2 << " " << leng + j+3 << " " << leng + j+4;
		cout << endl;
	}
	
	for(j=1;j<leng-4;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "IT ";
		cout << j+1 << " " << leng + j+2 << " " << leng + j+3 << " " <<  j+4;
		cout << endl;
	}

	for(j=1;j<leng-4;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "IC  ";
		cout << j+1 << " " << j+2 << " " << j+3 << " " << j+4;
		cout << endl;
	}

	for(j=1;j<leng-4;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "IC ";
		cout << leng + j+1 << " " << j+2 << " " << leng + j+3 << " " << j+4;
		cout << endl;
	}
	for(j=1;j<leng-4;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "IC ";
		cout << j+1 << " " << j+2 << " " << leng + j+3 << " " << leng + j+4;
		cout << endl;
	}
	
	for(j=1;j<leng-4;j++)
	{
		lineNo++;
		print4DigitLN(lineNo);
		cout << "IC ";
		cout << j+1 << " " << leng + j+2 << " " << leng + j+3 << " " <<  j+4;
		cout << endl;
	}
}