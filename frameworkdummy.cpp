#ifndef DOTNETFAKE_CPP
#define DOTNETFAKE_CPP

#include <dotnetfake.h>

using namespace std;

namespace ExpertMultimediaBase {
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
	bool FILE_FAKENAMESPACE_CLASS::Exists(string sFile) {
		bool bReturn=false;
		static bool bFirstRun=true;
		if (bFirstRun) Console.Write("checking File.Exists(\""+sFile+"\")...");
		string sMsg="opening";
		if (bFirstRun) Console.Write(sMsg+"...");
		try {
			std::fstream fsNow;
			fsNow.open(sFile.c_str(),std::ios::in);
			sMsg="check if open";
			if (bFirstRun) Console.Write(sMsg+"...");
			bReturn=fsNow.is_open();
			sMsg=((bReturn)?"(yes) ":"(no) ")+ToString("closing");
			if (bFirstRun) Console.Write(sMsg+"...");
			fsNow.close();
			if (bFirstRun) Console.Write("done File.Exists...");
		}
		catch (char* carrExn) {
			bReturn=false;
			ShowAndDeleteException(carrExn,"File.Exists",sMsg);
		}
		catch (...) {
			bReturn=false;
			ShowUnknownException("File.Exists",sMsg);
		}
		bFirstRun=false;
		return bReturn;
	}//end File.Exists
	void CONSOLE_FAKENAMESPACE_CLASS::Write(string val) {
		try {
			if (iConsoleLinesWritten==iConsoleLinesMax) {
				cout<<"Too many console messages ("<<iConsoleLinesWritten<<")"<<endl
					<<"--this is the last message that will be shown:"<<endl;
			}
			if (iConsoleLinesWritten<=iConsoleLinesMax) {
				cout<<val<<flush;
			}
		}
		catch (char* carrExn) {
			ShowAndDeleteException(carrExn,"Console::Write");
		}
		catch (...) {
			ShowUnknownException("Console::Write");
		}
		iConsoleLinesWritten++;
	}//end Console::Write
	void CONSOLE_FAKENAMESPACE_CLASS::WriteLine(string val) {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Write
			Console.Write(val);
			Console.WriteLine();
		}
	}//end Console::WriteLine
	void CONSOLE_FAKENAMESPACE_CLASS::WriteLine() {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) {
			cout<<endl;
		}
	}//end Console::WriteLine
	void ERROR_FAKENAMESPACE_CLASS::WriteLine(string val) {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Write
			Console.Error.Write(val);
			Console.Error.WriteLine();
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Write
			cerr<<"Too console messages.  This is the last error that will be shown:"<<endl;
			Console.Error.Write(val);
			Console.Error.WriteLine();
		}
	}
	void ERROR_FAKENAMESPACE_CLASS::WriteLine() {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Write
			cerr<<endl;
			iConsoleLinesMax++;
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) {
			cerr<<endl;
			cerr<<"Too many errors--no more console errors will be shown."<<endl;
		}
	}
	void ERROR_FAKENAMESPACE_CLASS::Write(string val) {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console.Write
			cerr<<val;
		}
	}
}//end namespace
#endif
