#ifndef FRAMEWORKDUMMY_CPP
#define FRAMEWORKDUMMY_CPP

#include <frameworkdummy.h>
#include <fstream>
//#include <cstdio>
#include <iostream>
//#include <float.h> //maximums i.e. LONG_MAX
//#include <limits.h> //maximums i.e. INT_MAX
//#include "E:\Projects-cpp\Base\frameworkdummy.h"

using namespace std;

namespace ExpertMultimediaBase {
	int iConsoleLinesMax=3000;
	int iConsoleLinesWritten=0;
	//const float float_MaxValue=FLT_MAX;
	//const float double_MaxValue=DBL_MAX;
	//const int int_MaxValue=INT_MAX;//2147483647;
	//const long long_MaxValue=LONG_MAX;//9223372036854775807
	FILE_FAKESTATIC_CLASS File;
	CONSOLE_FAKESTATIC_CLASS Console;
	void Rectangle::Set(int x, int y, int width, int height) {
		X=x;
		Y=y;
		Width=width;
		Height=height;
		if (Height<0) Height=0;
		if (Width<0) Width=0;
	}
	void RectangleF::Set(float x, float y, float width, float height) {
		X=x;
		Y=y;
		Width=width;
		Height=height;
		if (Height<0) Height=0;
		if (Width<0) Width=0;
	}
	bool FILE_FAKESTATIC_CLASS::Exists(string sFile) {
		bool bReturn=false;
		static bool bFirstRun=true;
		if (bFirstRun) Console.Error.Write("checking File.Exists(\""+sFile+"\")...");
		string sMsg="opening";
		if (bFirstRun) Console.Error.Write(sMsg+"...");
		try {
			std::fstream fsNow;
			fsNow.open(sFile.c_str(),std::ios::in);
			sMsg="check if open";
			if (bFirstRun) Console.Error.Write(sMsg+"...");
			bReturn=fsNow.is_open();
			sMsg=((bReturn)?"(yes) ":"(no) ")+(string)("closing");
			if (bFirstRun) Console.Error.Write(sMsg+"...");
			fsNow.close();
			if (bFirstRun) Console.Error.Write("done File.Exists...");
		}
		catch (exception& exn) {
			bReturn=false;
			Console.Error.WriteLine("Exception error in File.Exists check");//avoids base.cpp -- ShowExn(exn,"File.Exists",sMsg);
		}
		catch (...) {
			bReturn=false;
			Console.Error.WriteLine("Unknown exception error in File.Exists check");//avoids base.cpp -- ShowUnknownExn("File.Exists",sMsg);
		}
		bFirstRun=false;
		return bReturn;
	}//end File.Exists
	void CONSOLE_FAKESTATIC_CLASS::Write(string val) {
		bool bGood=true;
		try {
			if (iConsoleLinesWritten==iConsoleLinesMax) {
				cout<<"Too many console messages ("<<iConsoleLinesWritten<<")"<<endl; //MUST BE COUT OR WILL BE INFINITE RECURSION!
				cout<<"--this is the last message that will be shown:"; //MUST BE COUT OR WILL BE INFINITE RECURSION!
			}
			if (iConsoleLinesWritten<=iConsoleLinesMax)
				cout<<(val); //MUST BE COUT OR WILL BE INFINITE RECURSION!
				cout<<flush; //MUST BE COUT OR WILL BE INFINITE RECURSION!
		}
		catch (exception& exn) {
			bGood=false;
			cerr<<("Exception error in Console.Error.Write"); //MUST BE COUT OR WILL BE INFINITE RECURSION! //avoids base.cpp -- ShowExn(exn,"File.Exists",sMsg);
		}
		catch (...) {
			bGood=false;
			cerr<<("Unknown exception error in Console.Error.Write"); //MUST BE COUT OR WILL BE INFINITE RECURSION!//avoids base.cpp -- ShowUnknownExn("File.Exists",sMsg);
		}
		iConsoleLinesWritten++;
	}//end Console::Write
	void CONSOLE_FAKESTATIC_CLASS::WriteLine(string val) {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Write
			cerr<<(val); //MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<endl; //MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
	}//end Console::WriteLine
	void CONSOLE_FAKESTATIC_CLASS::WriteLine() {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) {
			cout<<endl;
		}
	}//end Console::WriteLine
	void CONSOLE_TEXTWRITER::Flush() {
		cerr<<flush;
	}
	void ERROR_TEXTWRITER::WriteLine(string val) {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Error.Write
			cerr<<(val); //MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Error.Write
			cerr<<"Too console messages.  This is the last error that will be shown:"<<endl;
			cerr<<(val);//MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
	}
	void ERROR_TEXTWRITER::WriteLine() {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Error.Write
			cerr<<endl;
			iConsoleLinesMax++;
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) {
			cerr<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<"Too many errors--no more console errors will be shown."<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
	}
	void ERROR_TEXTWRITER::Write(string val) {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Error.Write
			cerr<<val;
		}
	}
	void ERROR_TEXTWRITER::Flush() {
		cerr<<flush;
	}

	void ref_ERROR_TEXTWRITER::WriteLine(string val) {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Error.Write
			cerr<<(val); //MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Error.Write
			cerr<<"Too console messages.  This is the last error that will be shown:"<<endl;
			cerr<<(val);//MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
	}
	void ref_ERROR_TEXTWRITER::WriteLine() {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Error.Write
			cerr<<endl;
			iConsoleLinesMax++;
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) {
			cerr<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<"Too many errors--no more console errors will be shown."<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
	}
	void ref_ERROR_TEXTWRITER::Write(string val) {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Error.Write
			cerr<<val;
		}
	}
	void ref_ERROR_TEXTWRITER::Flush() {
		cerr<<flush;
	}
}//end namespace
#endif
