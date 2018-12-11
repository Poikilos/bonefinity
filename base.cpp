#ifndef BASE_CPP
#define BASE_CPP
//#pragma once

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <string>
#include <base.h> //#include "E:\Projects-cpp\Base\base.h"
#include <frameworkdummy.h>
#include <float.h>
#include <limits.h>
#include <RMath.h>

using namespace std;

namespace ExpertMultimediaBase {
	const REAL Base_r0=(REAL)0.0;
	const REAL Base_r1=(REAL)1.0;
	const REAL Base_r1000=(REAL)1000.0;
	int RString_iDecimalPlacesForToString=-1;
	Uint32 dwTicksAcquiredOutsideOfGameState_Run=0;

	int iLastErr=0; //each call of every function must reset this to track errors.
	string sLastErr="";
	string sLastFunc="";
	char szHex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	char szAlphabetUpper[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	char szAlphabetLower[26]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	Uint32 u32Bit[33]={0,1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648};
	char cDigit[10]={'0','1','2','3','4','5','6','7','8','9'};
	char sDigit[10][2]={"0","1","2","3","4","5","6","7","8","9"};
	char slash[2]={'\\','\0'};
	int iLogNow=0;
	//end debugging
	const int i10=10;
	const long l10=10L;
	const float f10=10.0f;
	const double d10=10;
	bool Mass3d_ToString_bFirstRun=true;

	///#region globals defined in base.h
	int iErrors=0;
	int iMaxErrors=8000;
	byte by3dAlphaLookup[256][256][256];
	bool bDebug=false;//CHANGED during init to the value of the debugmode script variable
	bool bMegaDebug=false; //true for debug only!
	//IPoint ipZero;

	///#endregion globals defined in base.h

	///#region methods
	Pixel::Pixel() {
		Set(0,0,0,0);
	}
	Pixel::Pixel(byte red, byte green, byte blue, byte alpha) {
		Set(red,green,blue,alpha);
	}
	void Pixel::Set(byte red, byte green, byte blue, byte alpha) {
		b=blue;
		g=green;
		r=red;
		a=alpha;
	}
	void Pixel::Set(Uint32& u32Pixel) {
		byte* byPixel=(byte*)&u32Pixel;
		b=byPixel[0]; //u32Pixel>>3;
		g=byPixel[1]; //(u32Pixel>>2)&&0x000000ff;
		r=byPixel[2]; //(u32Pixel>>1)&&0x000000ff;
		a=byPixel[3]; //(u32Pixel)&&0x000000ff;
	}
	//void Pixel::Set(Uint32* u32Pixel) {
	//	byte* byPixel=(byte*)u32Pixel;
	//	b=byPixel[0]; //u32Pixel>>3;
	//	g=byPixel[1]; //(u32Pixel>>2)&&0x000000ff;
	//	r=byPixel[2]; //(u32Pixel>>1)&&0x000000ff;
	//	a=byPixel[3]; //(u32Pixel)&&0x000000ff;
	//}


	bool Variable::IsActive() {
		return bActive;
	}
	void Variable::SetActiveState(bool bSetActiveStatus) {
		if (bSetActiveStatus && !bActive) Activate();
		else if (!bSetActiveStatus && bActive) Deactivate();
	}
	void Variable::SetActive() {
		SetActiveState(true);
	}
	void Variable::SetInactive() {
		SetActiveState(false);
	}
	int Variable::MaxChars() {
		return iMaxChars;
	}
	int Variable::MaxNameChars() {
		return iMaxNameChars;
	}
	void Variable::Init() {
		Init(128,256);
	}
	void Variable::Init(int iMaxNameCharsNow, int iMaxCharsNow) {
		bActive=false;
		iMaxNameChars=iMaxNameCharsNow;
		iMaxChars=iMaxCharsNow;
		sName="";
		sVal="";
		iBlankReturnStringLen=100;
	}
	void Variable::Activate() {
		Activate(128,256);
	}
	void Variable::Activate(int iMaxNameCharsNow, int iMaxCharsNow) {
		bActive=true;
	}
	void Variable::Deactivate() {
		sVal="";
		sName="";
		bActive=false;
	}
	Variable::Variable() {
		Init();
	}
	Variable::Variable(int val) {
		Init();
		Set(val);
	}
	Variable::Variable(float val) {
		Init();
		Set(val);
	}
	Variable::Variable(string val) {
		Init();
		Set(val);
	}
	void Variable::Set(int val) {
		try { SetActive();
			sVal=RString_ToString(val);
		}
		catch (exception& exn) { ShowExn(exn,"Variable::Set(int)");
		}
		catch (...) { ShowUnknownExn("Variable::Set(int)","value=\""+RString_ToString(val)+"\"");
		}
	}
	void Variable::Set(float val) {
		try { SetActive();
			sVal=RString_ToString(val); }
		catch (exception& exn) { ShowExn(exn,"Variable::Set(float)");
		}
		catch (...) { ShowUnknownExn("Variable::Set(float)","value=\""+RString_ToString(val)+"\"");
		}
	}
	void Variable::Set(string val) {
		try {
			SetActive();
			sVal=val;
		}
		catch (exception& exn) { ShowExn(exn,"Variable::Set(string)");
		}
		catch (...) { ShowUnknownExn("Variable::Set(string)","value=\""+val+"\"");
		}
	}
	bool Variable::Get(int &val) {
		bool bGood=false;
		try {
			if (bActive) {
				//stringstream ssboth(s3, ios_base::in | ios_base::out);
				//ssboth+sVal;
				//ssboth>>val;
				//char szTemp[2048];
				val=atoi(sVal.c_str());
				bGood=true;
			}
			else bGood=false;
		}
		catch (exception& exn) { ShowExn(exn,"Variable::Get(int)");
		}
		catch (...) { ShowUnknownExn("Variable::Get(int)");
		}
		return bGood;
	}
	bool Variable::Get(float &val) {
		bool bGood=false;
		try {
			if (bActive) { val=atof(sVal.c_str()); bGood=true; }
			else bGood=false;
		}
		catch (exception& exn) { ShowExn(exn,"Variable::Get(float)");
		}
		catch (...) { ShowUnknownExn("Variable::Get(float)");
		}
		return bGood;
	}
	bool Variable::Get(string &val) {
		bool bGood=false;
		try {
			if (bActive) { val=sVal; bGood=true; }
			else bGood=false;
		}
		catch (exception& exn) { ShowExn(exn,"Variable::Get(string)");
		}
		catch (...) { ShowUnknownExn("Variable::Get(string)");
		}
		return bGood;
	}
	int Variable::Indeces() {//returns 0 if non-array.  Otherwise returns 1 or more.
		return CountCArrayNotationFields(sVal);
	}
	int Variable::Indeces(int i1stDimension) {//returns 0 if non-array.  Otherwise returns 1 or more.
		//string *psIgnored;
		//Variable vTemp;
		string sTemp;
		bool bGood=GetForcedCArrayNotationSubstring(sTemp,sVal,i1stDimension);
		Console::Error.Write("Checking "+sTemp+"...GetForcedCArrayNotationSubstring..."+(bGood?"OK":"FAILED")+"...");
		int iReturn=0;
		iReturn=CountCArrayNotationFields(sTemp);
		Console::Error.Write( "Found "+RString_ToString(iReturn)+RString_ToString("...") );
		return iReturn;
	}
	bool Variable::Get(string &val, int i1stDimension) {
		bool bGood=false;
		try {
			if (bActive) { bGood=Get(val,i1stDimension,0); }
			else bGood=false;
		}
		catch (exception& exn) { ShowExn(exn,"Variable::Get(string)");
		}
		catch (...) { ShowUnknownExn("Variable::Get(string,i1stDim)");
		}
		return bGood;
	}
	bool Variable::Get(string &val, int i1stDimension, int i2ndDimension) {
		bool bGood=false;
		try {
			if (bActive) {
				Variable vInA1stDim;
				string s1stDimBracesArea, s2ndDimVal;
				GetForcedCSVNotationSubstring(s1stDimBracesArea,sVal,i1stDimension);
				GetForcedCSVNotationSubstring(s2ndDimVal,s1stDimBracesArea,i2ndDimension);
				Variable vInA2ndDim;
				vInA2ndDim.Set(s2ndDimVal);
				bGood=vInA2ndDim.Get(val);
			}
			else bGood=false;
		}
		catch (exception& exn) { ShowExn(exn,"Variable::Get(string)");
		}
		catch (...) { ShowUnknownExn("Variable::Get(string,i1stDim,i2ndDim)");
		}
		return bGood;
	}
	bool Variable::SetName(string val) {
		SetActive();
		bool bGood=false;
		try {
			if (bActive) { sName=val; bGood=true; }
			else bGood=false;
		}
		catch (exception& exn) { ShowExn(exn,"Variable::SetName");
		}
		catch (...) { ShowUnknownExn("Variable::SetName");
		}
		return bGood;
	}
	bool Variable::GetName(string &val) {
		bool bGood=false;
		try {
			if (bActive) { val=sName; bGood=true; }
			else bGood=false;
		}
		catch (exception& exn) { ShowExn(exn,"Variable::GetName");
		}
		catch (...) { ShowUnknownExn("Variable::GetName");
		}
		return bGood;
	}
	bool Variable::IsNamed(string val) {
		bool bMatch=false;
		try {
			if (bActive) bMatch=(sName==val);
		}
		catch (exception& exn) { ShowExn(exn,"Variable::IsNamed");
		}
		catch (...) { ShowUnknownExn("Variable::IsNamed");
		}
		return bMatch;
	}
	Variable::~Variable() {
		try {
			SetActiveState(false); //frees strings too
		}
		catch (exception& exn) { ShowExn(exn,"Variable deconstructor");
		}
		catch (...) { ShowUnknownExn("Variable deconstructor");
		}
	}
	string Variable::GetForcedString() {
		string sReturn="";
		if (bActive) {
			sReturn=sVal;
		}
		return sReturn;
	}
	bool Variable::FromIniLine(string val) {
		static bool bFirstRun=true;
		bool bGood=true;
		int iNowPrev=-1;//is set later
		SetActive();
		string sFuncNow="FromIniLine";
		try {
			if (bFirstRun&&bMegaDebug) Console::Error.Write(sFuncNow);
			if (bFirstRun&&bMegaDebug) Console::Error.WriteLine("(\""+val+"\")");
			sFuncNow+="(\""+val+"\")";
			if (bFirstRun&&bMegaDebug) Console::Error.WriteLine("  -about to analyze \""+val+"\"");
			int iNow=val.find_first_of("=");
			if (iNow>=0) {
				if (bFirstRun&&bMegaDebug) Console::Error.WriteLine("  -about to get first part from 0 to "+RString_ToString(iNow));
				sName=val.substr(0,iNow);
				if (sName=="") ShowError("  -var name string is blank \"\"!",sFuncNow);
				if (bGood && (iNow<(int)val.length()-1)) {
					iNowPrev=iNow;
					iNow++;
					if (bFirstRun&&bMegaDebug) Console::Error.WriteLine("	-done get first part: "+sName);
					if (bFirstRun&&bMegaDebug) Console::Error.WriteLine("	-about to get next part from "+RString_ToString(iNow)+" length "+RString_ToString(val.length()-iNow));
					sVal=val.substr(iNow,val.length()-iNow);
					if (sVal=="") ShowError("	  -\"\" VALUE part is blank !",sFuncNow);
					if (bFirstRun&&bMegaDebug) Console::Error.WriteLine("	  -done getting next part");
					//if (!bGood) ShowError("	  -get VALUE part failed!",sFuncNow);
				}
				else if (!bGood) ShowError("	-substr name failed!",sFuncNow);
				else ShowError("	-(no second part found)",sFuncNow);
			}
		}
		catch (exception& exn) { ShowExn(exn,"Variable::FromIniLine");
		}
		catch (...) { ShowUnknownExn("Variable::FromIniLine");
		}
		return bGood;
	}//end Variable::FromIniLine()
	string Variable::ToIniLine() {
		string val="";
		static bool bFirstRun=false;
		if (!bActive) {
			ShowError("tried to use inactive var--returning blank non-null string.","ToIniLine");
		}
		else { //if active
			try {
				if (bFirstRun) Console::Error.WriteLine("Trying to save ini line:");
				val=sName;
				if (bFirstRun) Console::Error.WriteLine(" 1."+val+" (after adding "+sName+")");
				if (sVal.length()>0) {
					if (sName.length()>1) {
						val+="=";
						if (bFirstRun) Console::Error.WriteLine(" 3."+val+" (after adding '=' sign)");
						val+=sVal;
						if (bFirstRun) Console::Error.WriteLine(" 3."+val+" (after adding "+sVal+")");
					}
				}
			}
			catch (exception& exn) { ShowExn(exn,"Variable::ToIniLine");
			}
			catch (...) { ShowUnknownExn("Variable::ToIniLine");
			}
		}//end else active
		return val;
	}//ToIniLine
	int Variable::IniLineLen() {
		if (bActive) return sVal.length()+sName.length()+2;//2 for '=' AND '\0'
		else return iMaxChars+iMaxNameChars+2;//2 for '=' AND '\0'
	}
	void Variables::Init() {
		iVars=0;
		sFileName="";
		iBlankReturnStringLen=1;
		bSaveEveryChange=false;
	}
	Variables::Variables() {
		Init();
	}
	Variables::~Variables() {
		sFileName="";
	}
	Variables::Variables(string sFile) {
		Init();
		Load(sFile);
	}
	bool Variables::Load(string sFile) {
		static bool bFirstRun=true;
		bool bGood=false;
		try {
			if (bFirstRun) Console::Error.WriteLine("About to load \""+sFile+"\"");//debug only
			sFileName=sFile;
			ifstream ifNow(sFile.c_str());
			if (bFirstRun) Console::Error.WriteLine("Done trying to load \""+sFile+"\"");//debug only
			int iChars=1;
			bool bCheck;
			//int iMaxLine=varr[iVars].MaxChars()+varr[iVars].MaxNameChars()+2; //+2 not 1, to include '=' AND '\0'

			char sPtr[2048];
			int iVarsNow=0;
			int iLines=0;
			string sLine;
			bool bNew;//whether a new or existing variable
			while ( ifNow.getline(sPtr,varr[iVars].MaxChars()+varr[iVars].MaxNameChars()+1,'\n') ) {
				sLine.assign(sPtr);
				if ((sLine.length()>0) && sLine[sLine.length()-1]=='\r') sLine=sLine.substr(0,sLine.length()-1);
				if ((sLine.length()>0) && sLine[0]=='\r') sLine=sLine.substr(1,sLine.length()-1);
				iChars=sLine.length();
				if (iChars>0) {
					int iEnder=sLine.find_first_of("=");
					if (iEnder<0) iEnder=sLine.length();
					string sNameX=SafeSubstring(sLine,0,iEnder);
					int iAt=IndexOf(sNameX);
					if (iAt<0) {
						iAt=iVars;
						bNew=true;
					}
					else bNew=false;
					if (bFirstRun&&bMegaDebug) Console::Error.WriteLine("  -about to get FromIniLine: "+sLine);
					bCheck=varr[iAt].FromIniLine(sLine);
					if (bFirstRun&&bMegaDebug) Console::Error.WriteLine("  -done get FromIniLine");
					if (bCheck&&bNew) {
						iVars++;
						iVarsNow++;
					}
				}//end if
				iLines++;
			}//end while sPtr line
			if (bFirstRun) Console::Error.WriteLine("Done: Read "+RString_ToString(iVars)+" variables from "+RString_ToString(iLines)+" lines in "+sFile);
			ifNow.close();
			bGood=true;
		}
		catch (exception& exn) { bGood=false; ShowExn(exn,"Variables::Load");
		}
		catch (...) { bGood=false; ShowUnknownExn("Variables::Load");
		}
		bFirstRun=false;
		return bGood;
	}//end Variables::Load
	bool Variables::Save(string sFile) {
		bool bGood=false;
		try {
			sFileName=sFile;
			ofstream ofNow(sFile.c_str());
			//int iMaxLine=varr[0].MaxChars()+varr[0].MaxNameChars()+2; //+2 not 1, to include '=' AND '\0'
			string sLine="";
			for (int iNow=0; iNow<iVars; iNow++) {
				sLine=varr[iNow].ToIniLine();
				ofNow<<sLine<<endl;
			}
			ofNow.close();
			bGood=true;
		}
		catch (exception& exn) { bGood=false; ShowExn(exn,"Variables::Save(...)");
		}
		catch (...) { bGood=false; ShowUnknownExn("Variables::Save(...)");
		}
		return bGood;
	}//end Variables::Save
	bool Variables::Save() {
		bool bGood=false;
		if (sFileName!="") bGood=Save(sFileName);
		return bGood;
	}
	int Variables::IndexOf(string sName) {
		int iMatch=-1;
		for (int iNow=0; iNow<iVars; iNow++) {
			if (varr[iNow].IsNamed(sName)) {
				iMatch=iNow;
				break;
			}
		}
		return iMatch;
	}
	Variable* Variables::PointerOf(string sName) {
		int iIndex=IndexOf(sName);
		Variable* pvReturn=null;
		try {
			if (iIndex>=0) pvReturn=&varr[iIndex];
			else pvReturn=null;
		}
		catch (...) {
			pvReturn=null;
			ShowUnknownExn( "variables.PointerOf", "accessing varr index "+RString_ToString(iIndex) );
		}
		return pvReturn;
	}
	bool Variables::Exists(string sName) {
		int iReturn=(IndexOf(sName)>=0);
		return iReturn;
	}
	void Variables::CreateOrIgnore(string sName, int val) {
		try {
			if (!Exists(sName)) SetOrCreate(sName,val);
		}
		catch (exception& exn) { ShowExn(exn,"Variables::CreateOrIgnore(...,int)");
		}
		catch (...) {
			ShowUnknownExn("Variables::CreateOrIgnore(...,int)");
		}
	}
	void Variables::CreateOrIgnore(string sName, float val) {
		try {
			if (!Exists(sName)) SetOrCreate(sName,val);
		}
		catch (exception& exn) { ShowExn(exn,"Variables::CreateOrIgnore(...,float)");
		}
		catch (...) { ShowUnknownExn("Variables::CreateOrIgnore(...,float)");
		}
	}
	void Variables::CreateOrIgnore(string sName, string val) {
		try {
			if (!Exists(sName)) SetOrCreate(sName,val);
		}
		catch (exception& exn) { ShowExn(exn,"Variables::CreateOrIgnore(...,string)");
		}
		catch (...) { ShowUnknownExn("Variables::CreateOrIgnore(...,string)");
		}
	}
	void Variables::SetOrCreate(string sName, int val) {
		int iAt=IndexOf(sName);
		bool bFound=(iAt>=0);
		if (iAt<0) iAt=iVars;
		try {
			varr[iAt].Set(val);
			if (!bFound) {
				varr[iAt].SetName(sName);
				iVars++;
			}
			if (bSaveEveryChange) Save();
		}
		catch (exception& exn) { ShowExn(exn,"Variables::SetOrCreate(...,int)");
		}
		catch (...) { ShowUnknownExn("Variables::SetOrCreate(...,int)");
		}
	}//end Variables::SetOrCreate(...,int)
	void Variables::SetOrCreate(string sName, float val) {
		int iAt=IndexOf(sName);
		bool bFound=(iAt>=0);
		if (iAt<0) iAt=iVars;
		try {
			varr[iAt].Set(val);
			if (!bFound) {
				varr[iAt].SetName(sName);
				iVars++;
			}
			if (bSaveEveryChange) Save();
		}
		catch (exception& exn) { ShowExn(exn,"Variables::SetOrCreate(...,float)");
		}
		catch (...) { ShowUnknownExn("Variables::SetOrCreate(...,float)");
		}
	}//end Variables::SetOrCreate(...,float)
	void Variables::SetOrCreate(string sName, string val) {
		int iAt=IndexOf(sName);
		bool bFound=(iAt>=0);
		if (iAt<0) iAt=iVars;
		try {
			varr[iAt].Set(val);
			if (iAt<0) iVars++;
			if (!bFound) {
				varr[iAt].SetName(sName);
				iVars++;
			}
			if (bSaveEveryChange) Save();
		}
		catch (exception& exn) { ShowExn(exn,"Variables::SetOrCreate(...,string)");
		}
		catch (...) { ShowUnknownExn("Variables::SetOrCreate(...,string)");
		}
	}//end Variables::SetOrCreate(...,string)
	void Variables::GetOrCreate(int &val, string sName) {
		int iAt=IndexOf(sName);
		bool bFound=(iAt>=0);
		try {
			if (bFound) varr[iAt].Get(val);
			else {
				iAt=iVars;
				varr[iAt].Set(val);
				varr[iAt].SetName(sName);
				iVars++;
				if (bSaveEveryChange) Save();
			}
		}
		catch (exception& exn) { ShowExn(exn,"Variables::GetOrCreate(int,...)");
		}
		catch (...) { ShowUnknownExn("Variables::GetOrCreate(int,...)");
		}
	}//end Variables::GetOrCreate(int,...)
	void Variables::GetOrCreate(float &val, string sName) {
		int iAt=IndexOf(sName);
		bool bFound=(iAt>=0);
		try {
			if (bFound) varr[iAt].Get(val);
			else {
				iAt=iVars;
				varr[iAt].Set(val);
				varr[iAt].SetName(sName);
				iVars++;
				if (bSaveEveryChange) Save();
			}
		}
		catch (exception& exn) { ShowExn(exn,"Variables::GetOrCreate(float,...)");
		}
		catch (...) { ShowUnknownExn("Variables::GetOrCreate(float,...)");
		}
	}//end GetOrCreate(float,...
	void Variables::GetOrCreate(REAL &val, string sName) {
		int iAt=IndexOf(sName);
		bool bFound=(iAt>=0);
		float vTemp;
		try {
			if (bFound) {
				varr[iAt].Get(vTemp);
				val=vTemp;
			}
			else {
				iAt=iVars;
				vTemp=val;
				varr[iAt].Set(vTemp);
				varr[iAt].SetName(sName);
				iVars++;
				if (bSaveEveryChange) Save();
			}
		}
		catch (exception& exn) { ShowExn(exn,"Variables::GetOrCreate(REAL,...)");
		}
		catch (...) { ShowUnknownExn("Variables::GetOrCreate(REAL,...)");
		}
	}//end GetOrCreate(REAL,...
	void Variables::GetOrCreate(string &val, string sName) {
		int iAt=IndexOf(sName);
		bool bFound=(iAt>=0);
		try {
			if (bFound) varr[iAt].Get(val);
			else {
				iAt=iVars;
				varr[iAt].Set(val);
				varr[iAt].SetName(sName);
				iVars++;
				if (bSaveEveryChange) Save();
			}
		}
		catch (exception& exn) { ShowExn(exn,"Variables::GetOrCreate(string,...)");
		}
		catch (...) { ShowUnknownExn("Variables::GetOrCreate(string,...)");
		}
	}//end Variables::GetOrCreate(string,...)
	bool Variables::Get(int &valReturn, string sName) {
		valReturn=0;
		bool bFound=false;
		int iAt=IndexOf(sName);
		if (iAt>=0) {
			try {
				varr[iAt].Get(valReturn);
				bFound=true;
			}
			catch (exception& exn) { ShowExn(exn,"Variables::Get(int,...)");
			}
			catch (...) { ShowUnknownExn("Variables::Get(int,...)");
			}
		}
		return bFound;
	}
	bool Variables::Get(float &valReturn, string sName) {
		valReturn=0.0f;
		bool bFound=false;
		int iAt=IndexOf(sName);
		if (iAt>=0) {
			try {
				varr[iAt].Get(valReturn);
				bFound=true;
			}
			catch (exception& exn) { ShowExn(exn,"Variables::Get(float,...)");
			}
			catch (...) { ShowUnknownExn("Variables::Get(float,...)");
			}
		}
		return bFound;
	}
	bool Variables::Get(string &valReturn, string sName) {
		valReturn="";
		bool bFound=false;
		int iAt=IndexOf(sName);
		if (iAt>=0) {
			try {
				varr[iAt].Get(valReturn);
				bFound=true;
			}
			catch (exception& exn) { ShowExn(exn,"Variables::Get(string,...)");
			}
			catch (...) { ShowUnknownExn("Variables::Get(string,...)");
			}
		}
		return bFound;
	}
	int Variables::GetForcedInt(string sName) {
		int valReturn=0;
		int iAt=IndexOf(sName);
		try { if (iAt>=0) varr[iAt].Get(valReturn);
		}
		catch (exception& exn) { ShowExn(exn,"Variables::GetForcedInt(...)");
		}
		catch (...) { ShowUnknownExn("Variables::GetForcedInt(...)");
		}
		return valReturn;
	}
	float Variables::GetForcedFloat(string sName) {
		float valReturn=0.0f;
		int iAt=IndexOf(sName);
		try { if (iAt>=0) varr[iAt].Get(valReturn);
		}
		catch (exception& exn) { ShowExn(exn,"Variables::GetForcedFloat(...)");
		}
		catch (...) { ShowUnknownExn("Variables::GetForcedFloat(...)");
		}
		return valReturn;
	}
	string Variables::GetForcedString(string sName) {
		string val="";
		int iAt=IndexOf(sName);
		try { if (iAt>=0) val=varr[iAt].GetForcedString();
		}
		catch (exception& exn) { ShowExn(exn,"Variables::GetForcedString(...)");
		}
		catch (...) { ShowUnknownExn("Variables::GetForcedString(...)");
		}
		return val;
	}
	bool Variables::GetForcedBool(string sName) {
		bool bTest=false;
		try {
			string sTemp=GetForcedString(sName); //int iX=GetForcedInt(sName);
			bTest=( (sTemp!="no") && (sTemp!="false") ) || (GetForcedInt(sName)==1);
		}
		catch (exception& exn) {
			bTest=false;
			ShowExn(exn,"Variables::GetForcedBool(...)");
		}
		catch (...) {
			bTest=false;
			ShowUnknownExn("Variables::GetForcedBool(...)");
		}
		return bTest;
	}
	int Variables::Indeces(string sName) {
		int iReturn=0;
		int iIndex=IndexOf(sName);
		if (iIndex>=0) {
			try {
				iReturn=varr[iIndex].Indeces();
			}
			catch (...) { ShowUnknownExn("variables.Indeces(...)", "accessing varr index "+RString_ToString(iIndex)+RString_ToString(".")); }
		}
		return iReturn;
	}
	int Variables::Indeces(string sName, int i1stDimension) {
		int iReturn=0;
		int iIndex=IndexOf(sName);
		if (iIndex>=0) {
			try {
				iReturn=varr[iIndex].Indeces(i1stDimension);
			}
			catch (...) { ShowUnknownExn("variables.Indeces(...,"+RString_ToString(i1stDimension)+")", "accessing varr index "+RString_ToString(iIndex)+"."); }
		}
		return iReturn;
	}
	//end class Variables

	////////////////////////////////// MASS3D //////////////////////////////////////

	//float Mass3d::BottomOffsetRatio() {
		//return (zSize*.912);
	//}
	Mass3d::Mass3d() {
		X=0;Y=0;Z=0;
		xMin=-8;yMin=-8;zMin=0;
		xMax=8;yMax=8;zMax=16;
		xVel=0;yVel=0;zVel=0;
		xRot=0;yRot=0;zRot=0;
		xRotMin=0;yRotMin=0;zRotMin=0;
		xRotMax=360;yRotMax=360;zRotMax=360;
		xRotVelDegreesPerSec=90;yRotVelDegreesPerSec=90;zRotVelDegreesPerSec=90;
		xRotDest=xRot;yRotDest=yRot;zRotDest=zRot;
		xSize=1;ySize=1;zSize=1;
	}
	void Mass3d::AngleToLimits() {
		SafeAngles();
		if (xRot<xRotMin) xRot=xRotMin;
		else if (xRot>xRotMax) xRot=xRotMax;
		if (yRot<yRotMin) yRot=yRotMin;
		else if (yRot>yRotMax) yRot=yRotMax;
		if (zRot<zRotMin) zRot=zRotMin;
		else if (zRot>zRotMax) zRot=zRotMax;
	}
	void Mass3d::LocationToLimits() {
		if		(X<xMin) X=xMin;
		else if (X>xMax) X=xMax;
		if		(Y<yMin) Y=yMin;
		else if (Y>yMax) Y=yMax;
		if		(Z<zMin) Z=zMin;
		else if (Z>zMax) Z=zMax;
	}
	void Mass3d::AngleDestToLimits() {
		SafeDestAngles();
		if (xRotDest<xRotMin) xRotDest=xRotMin;
		else if (xRotDest>xRotMax) xRotDest=xRotMax;
		if (yRotDest<yRotMin) yRotDest=yRotMin;
		else if (yRotDest>yRotMax) yRotDest=yRotMax;
		if (zRotDest<zRotMin) zRotDest=zRotMin;
		else if (zRotDest>zRotMax) zRotDest=zRotMax;
	}
	void Mass3d::SafeAngles() {
		SafeAngle360ByRef(xRot);
		SafeAngle360ByRef(yRot);
		SafeAngle360ByRef(zRot);
	}
	void Mass3d::SafeDestAngles() {
		SafeAngle360ByRef(xRotDest);
		SafeAngle360ByRef(yRotDest);
		SafeAngle360ByRef(zRotDest);
	}
	void Mass3d::RotateTowardDestByDegreesByRef(float& degToChange, float degDest, float degOperand) {
		if (degOperand<0.0f) {
			ShowErr("Error in RotateTowardDestByDegreesByRef: degOperand should not be negative (absolute value will be used instead)! {degOperand:"+RString_ToString(degOperand)+"}");
			degOperand*=-1.0f;
		}
		if (degDest<degToChange) {
			degToChange-=degOperand;
			if (degToChange<degDest) degToChange=degDest;
		}
		else if (degDest>degToChange) {
			degToChange+=degOperand;
			if (degToChange>degDest) degToChange=degDest;
		}
	}//end RotateTowardDestByDegreesByRef float overload
	void Mass3d::RotateTowardDestByDegreesByRef(double& degToChange, double degDest, double degOperand) {
		if (degOperand<0.0) {
			ShowErr("Error in RotateTowardDestByDegreesByRef: degOperand should not be negative (absolute value will be used instead)! {degOperand:"+RString_ToString(degOperand)+"}");
			degOperand*=-1.0;
		}
		if (degDest<degToChange) {
			degToChange-=degOperand;
			if (degToChange<degDest) degToChange=degDest;
		}
		else if (degDest>degToChange) {
			degToChange+=degOperand;
			if (degToChange>degDest) degToChange=degDest;
		}
	}//end RotateTowardDestByDegreesByRef double overload
	void Mass3d::RotateTowardDest(int iMillisecondsSinceLastCall) {
		AngleToLimits();
		AngleDestToLimits();
		//xRot=xRotDest;yRot=yRotDest;zRot=zRotDest;
		float xDegMove=(float)( (double)xRotVelDegreesPerSec * ((double)iMillisecondsSinceLastCall / 1000.0) );
		float yDegMove=(float)( (double)yRotVelDegreesPerSec * ((double)iMillisecondsSinceLastCall / 1000.0) );
		float zDegMove=(float)( (double)zRotVelDegreesPerSec * ((double)iMillisecondsSinceLastCall / 1000.0) );
		RotateTowardDestByDegreesByRef(xRot, xRotDest, xDegMove);
		RotateTowardDestByDegreesByRef(yRot, yRotDest, yDegMove);
		RotateTowardDestByDegreesByRef(zRot, zRotDest, zDegMove);
		//if (xDegMove>=ANGLEDIFFPOSITIVE(xRot,xRotDest)) xRot=xRotDest;
		//else xRot=APPROACH(xRot,xRotDest,xDegMove/(xRotDest-xRot));
		//if (yDegMove>=ANGLEDIFFPOSITIVE(yRot,yRotDest)) yRot=yRotDest;
		//else yRot=APPROACH(yRot,yRotDest,yDegMove/(yRotDest-yRot));
		//if (zDegMove>=ANGLEDIFFPOSITIVE(zRot,zRotDest)) zRot=zRotDest;
		//else zRot=APPROACH(zRot,zRotDest,zDegMove/(zRotDest-zRot));
	}//end RotateTowardDest
	void Mass3d::RotateTowardDest(float rSecondsSinceLastCall) {
		AngleToLimits();
		AngleDestToLimits();
		//xRot=xRotDest;yRot=yRotDest;zRot=zRotDest;return;
		float xDegMove=xRotVelDegreesPerSec*rSecondsSinceLastCall;
		float yDegMove=yRotVelDegreesPerSec*rSecondsSinceLastCall;
		float zDegMove=zRotVelDegreesPerSec*rSecondsSinceLastCall;
		float rApproach;
		float xToDest=ANGLEDIFFPOSITIVE(xRot,xRotDest);
		float yToDest=ANGLEDIFFPOSITIVE(yRot,yRotDest);
		float zToDest=ANGLEDIFFPOSITIVE(zRot,zRotDest);

		if (xDegMove>=xToDest) xRot=xRotDest;
		else {
			rApproach=xDegMove/xToDest;//xDegMove/360.0f;
			xRot=APPROACH(xRot,xRotDest,rApproach);//commented for debug only//,xDegMove/(xRotDest-xRot));
		}
		if (yDegMove>=yToDest) yRot=yRotDest;
		else {
			rApproach=yDegMove/yToDest;//yDegMove/360.0f;
			yRot=APPROACH(yRot,yRotDest,rApproach);//debug only
		}
		if (zDegMove>=zToDest) zRot=zRotDest;
		else {
			rApproach=zDegMove/zToDest;//zDegMove/360.0f;
			zRot=APPROACH(zRot,zRotDest,rApproach);//debug only
		}
	}//end RotateTowardDest
	void Mass3d::RotateTowardDest(double rSecondsSinceLastCall) {
		AngleToLimits();
		AngleDestToLimits();
		//xRot=xRotDest;yRot=yRotDest;zRot=zRotDest;return;//debug only
		double xDegMove=xRotVelDegreesPerSec*rSecondsSinceLastCall;
		double yDegMove=yRotVelDegreesPerSec*rSecondsSinceLastCall;
		double zDegMove=zRotVelDegreesPerSec*rSecondsSinceLastCall;
		double rApproach;
		double xToDest=ANGLEDIFFPOSITIVE(xRot,xRotDest);
		double yToDest=ANGLEDIFFPOSITIVE(yRot,yRotDest);
		double zToDest=ANGLEDIFFPOSITIVE(zRot,zRotDest);

		if (xDegMove>=xToDest) xRot=xRotDest;
		else {
			rApproach=xDegMove/xToDest;//xDegMove/360.0f;
			xRot=APPROACH(xRot,xRotDest,rApproach);//commented for debug only//,xDegMove/(xRotDest-xRot));
		}
		if (yDegMove>=yToDest) yRot=yRotDest;
		else {
			rApproach=yDegMove/yToDest;//yDegMove/360.0f;
			yRot=APPROACH(yRot,yRotDest,rApproach);//debug only
		}
		if (zDegMove>=zToDest) zRot=zRotDest;
		else {
			rApproach=zDegMove/zToDest;//zDegMove/360.0f;
			zRot=APPROACH(zRot,zRotDest,rApproach);//debug only
		}
	}//end RotateTowardDest
	void Mass3d::HardRotation(float xRotTo, float yRotTo, float zRotTo) {
		xRotDest=xRotTo;xRot=xRotTo;
		yRotDest=yRotTo;yRot=yRotTo;
		zRotDest=zRotTo;zRot=zRotTo;
	}
	void Mass3d::HardLocation(float xTo, float yTo, float zTo) {
		X=xTo;//xDest=xTo;
		Y=yTo;//yDest=yTo;
		Z=zTo;//zDest=zTo;
	}
	void Mass3d::CopyLocationOnlyTo(Mass3d& m3dTo) {
		m3dTo.X=X;
		m3dTo.Y=Y;
		m3dTo.Z=Z;
	}
	void Mass3d::CopyTo(Mass3d& m3dTo) {
		m3dTo.X=X;
		m3dTo.Y=Y;
		m3dTo.Z=Z;
		m3dTo.xMin=xMin;m3dTo.yMin=yMin;m3dTo.zMin=zMin;
		m3dTo.xMax=xMax;m3dTo.yMax=yMax;m3dTo.zMax=zMax;
		m3dTo.xVel=xVel;m3dTo.yVel=yVel;m3dTo.zVel=zVel;
		m3dTo.xRot=xRot;m3dTo.yRot=yRot;m3dTo.zRot=zRot;
		m3dTo.xRotMin=xRotMin;m3dTo.yRotMin=yRotMin;m3dTo.zRotMin=zRotMin;
		m3dTo.xRotMax=xRotMax;m3dTo.yRotMax=yRotMax;m3dTo.zRotMax=zRotMax;
		m3dTo.xRotVelDegreesPerSec=xRotVelDegreesPerSec;m3dTo.yRotVelDegreesPerSec=yRotVelDegreesPerSec;m3dTo.zRotVelDegreesPerSec=zRotVelDegreesPerSec;
		m3dTo.xRotDest=xRotDest;m3dTo.yRotDest=yRotDest;m3dTo.zRotDest=zRotDest;
		m3dTo.zSize=xSize;m3dTo.zSize=ySize;m3dTo.zSize=zSize;
	}

	void Mass3d::OffsetSomethingByMyLocation(Mass3d& m3dSomethingToMove) {
		m3dSomethingToMove.X+=X;
		m3dSomethingToMove.Y+=Y;
		m3dSomethingToMove.Z+=Z;
	}

	//void Mass3d::SetRotMaxSpeed(float xSpeed, float ySpeed, float zSpeed) {
	//	xVel=xSpeed;
	//	yVel=ySpeed;
	//	zVel=zSpeed;
	//}
	string Mass3d::ToString() {
		return ToString(false);
	}
	string Mass3d::ToString(bool bShowAll) {
		string sReturn="";
		int iDecimalPlacesPrev=RString_iDecimalPlacesForToString;
		RString_iDecimalPlacesForToString=1;
		if (Mass3d_ToString_bFirstRun) {
			Console::Error.Write("Mass3d::ToString(bShowAll="+RString_ToString(bShowAll)+")...");
			Console::Error.Flush();
		}
		if (bShowAll) {
			bool bWholeNumbers=true;
			if (bWholeNumbers) {
				sReturn=RString_ToString("{At(")+RString_ToString((int)X)+RString_ToString(",")+RString_ToString((int)Y)+RString_ToString(",")+RString_ToString((int)Z)+RString_ToString(")")
				+RString_ToString(" ~ Velocity")+RString_ToString("(")+RString_ToString((int)xVel)+RString_ToString(",")+RString_ToString((int)yVel)+RString_ToString(",")+RString_ToString((int)zVel)+RString_ToString(")")
				+RString_ToString("\n Rot")+RString_ToString("(")+RString_ToString((int)xRot)+RString_ToString(",")+RString_ToString((int)yRot)+RString_ToString(",")+RString_ToString((int)zRot)+RString_ToString(")")
				+RString_ToString(" ~ RotDest")+RString_ToString("(")+RString_ToString((int)xRotDest)+RString_ToString(",")+RString_ToString((int)yRotDest)+RString_ToString(",")+RString_ToString((int)zRotDest)+RString_ToString(")")
				+"}";
			}
			else {
				sReturn=RString_ToString("{At(")+RString_ToString(X)+RString_ToString(",")+RString_ToString(Y)+RString_ToString(",")+RString_ToString(Z)+RString_ToString(")")
				+RString_ToString(" ~ Velocity")+RString_ToString("(")+RString_ToString(xVel)+RString_ToString(",")+RString_ToString(yVel)+RString_ToString(",")+RString_ToString(zVel)+RString_ToString(")")
				+RString_ToString("\n Rot")+RString_ToString("(")+RString_ToString(xRot)+RString_ToString(",")+RString_ToString(yRot)+RString_ToString(",")+RString_ToString(zRot)+RString_ToString(")")
				+RString_ToString(" ~ RotDest")+RString_ToString("(")+RString_ToString(xRotDest)+RString_ToString(",")+RString_ToString(yRotDest)+RString_ToString(",")+RString_ToString(zRotDest)+RString_ToString(")")
				+"}";
			}
		}
		else {
			sReturn=RString_ToString("(")+RString_ToString(X)+RString_ToString(",")+RString_ToString(Y)+RString_ToString(",")+RString_ToString(Z)+RString_ToString(")");
		}
		RString_iDecimalPlacesForToString=iDecimalPlacesPrev;
		if (Mass3d_ToString_bFirstRun) {
			Console::Error.Write("returning...");
			Console::Error.Flush();
			Mass3d_ToString_bFirstRun=false;
		}
		return sReturn;
	}//end RString_ToString

	///Mass2d:
	int Mass2d::CenterXAbsScaled() {
		return IROUNDF(FCenterXAbsScaled());
	}
	int Mass2d::CenterYAbsScaled() {
		return IROUNDF(FCenterYAbsScaled());
	}
	float Mass2d::FCenterXAbsScaled() {
		return (float)rectRender.left+FCenterXRelScaled();
		//return (float)rectRender.left + ((float)rectRender.right-(float)rectRender.left) / 2.0f;
	}
	float Mass2d::FCenterYAbsScaled() {
		return (float)rectRender.top+FCenterYRelScaled();
		//return (float)rectRender.top + ((float)rectRender.bottom-(float)rectRender.top) / 2.0f;
	}
	float Mass2d::FCenterXRelScaled() {
		return (float)xCenterRelNonScaled*fScale;
		//return (float)rectRender.left + ((float)rectRender.right-(float)rectRender.left) / 2.0f;
	}
	float Mass2d::FCenterYRelScaled() {
		return (float)yCenterRelNonScaled*fScale;
		//return (float)rectRender.top + ((float)rectRender.bottom-(float)rectRender.top) / 2.0f;
	}
	float Mass2d::BottomOffsetRatio() {
		return rectHitDetectPixelRatios.bottom;
	}
	float Mass2d::HitRectHeightRatio() {
		return rectHitDetectPixelRatios.bottom-rectHitDetectPixelRatios.top;
	}
	void Mass2d::Init(int Width, int Height) {
		Init(Height,Width,100.0f);
	}
	void Mass2d::Init(int Width, int Height, float fPixPerMeter) {
		SetPixCenter(IROUNDF((float)Width/2.0f),IROUNDF((float)Height/2.0f));
		rectOriginal.left=0;
		rectOriginal.top=0;
		rectOriginal.bottom=Height;
		rectOriginal.right=Width;
		rectRender.left=0;
		rectRender.top=0;
		rectRender.bottom=Height;
		rectRender.right=Width;
		rectHitDetectPixelBased.left=0;
		rectHitDetectPixelBased.top=0;
		rectHitDetectPixelBased.bottom=Height;
		rectHitDetectPixelBased.right=Width;
		rectHitDetectPixelRatios.left=0;
		rectHitDetectPixelRatios.top=0;
		rectHitDetectPixelRatios.bottom=1.0f;
		rectHitDetectPixelRatios.right=1.0f;
		//xSize=Width;
		//ySize=Height;
		fPixelsPerMeter=fPixPerMeter;
	}
	void Mass2d::Init(int Width, int Height, bool bWithSamePixPerMeterAsAPreviousInitialization) {
		if (bWithSamePixPerMeterAsAPreviousInitialization) Init(Height,Width,fPixelsPerMeter);
		else Init(Width,Height);
	}
	void Mass2d::SetPixCenter(int xPixNonScaled, int yPixNonScaled) {
		xCenterRelNonScaled=xPixNonScaled;
		yCenterRelNonScaled=yPixNonScaled;
	}
	void Mass2d::SetHitRect(float Top, float Left, float Bottom, float Right) {
		rectHitDetectPixelBased.left=Left;
		rectHitDetectPixelBased.top=Top;
		rectHitDetectPixelBased.bottom=Bottom;
		rectHitDetectPixelBased.right=Right;
		rectHitDetectPixelRatios.top=Top/(float)rectOriginal.bottom;
		rectHitDetectPixelRatios.left=Left/(float)rectOriginal.right;
		rectHitDetectPixelRatios.bottom=Bottom/(float)rectOriginal.bottom;
		rectHitDetectPixelRatios.right=Right/(float)rectOriginal.right;
	}
	Mass2d::Mass2d() {
		Init(0,0,100.0f);
		fScale=1.0f;
	}
	//bool Gradient::Shade(byte* arrbyDest, Uint32 u32DestLoc, byte bySrcValue) {
	//	return Shade(arrbyDest, u32DestLoc, (Uint32)bySrcValue);
	//}
	//bool Gradient::Shade(byte* arrbyDest, Uint32 u32DestLoc, Uint32 u32SrcValue) {
	//	try {
	//		if (lpbyShade!=null) {
	//			arrbyDest[u32DestLoc]=lpbyShade[u32SrcValue];
	//		}
	//	}
	//	catch (exception& exn) {
	//		ShowExn(exn,"Gradient::Shade");
	//	}
	//	catch (...) {
	//		ShowUnknownExn("Gradient::Shade");
	//	}
	//}//end Gradient::Shade
	//Gradient::Gradient() {
	//	InitNull();
	//	Init(256);
	//}
	//bool Gradient::InitNull() {
	//	u32Levels=0;
	//	lpbyShade=NULL;
	//}
	//bool Gradient::Init(Uint32 u32SetLevels) {
	//	try {
	//		SafeFree(lpbyShade);
	//		lpbyShade=(byte*)malloc(sizeof(byte)*u32SetLevels);
	//		u32Levels=u32SetLevels;
	//	}
	//	catch (exception& exn) {
	//		ShowExn(exn,"Gradient::Init");
	//	}
	//	catch (...) {
	//		ShowUnknownExn("Gradient::Init");
	//	}
	//}

	///#endregion methods

	///#region memory
	/*inline*/ void SafeFree(byte* &refToFreeAndSetToNull) {
		try {
			if (refToFreeAndSetToNull!=NULL) free(refToFreeAndSetToNull);
			refToFreeAndSetToNull=NULL;
		}
		catch (exception& exn) {//!!!!!!!!!!TODO: finish this: copy this to ALL catch statements EVERYWHERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
		    ShowExn(exn, "SafeFree(byte*)");
		}
		catch (...) {
			ShowUnknownExn("SafeFree(byte*)");
		}
	}
	/*inline*/ void SafeFree(char* &refToFreeAndSetToNull) {
		try {
			if (refToFreeAndSetToNull!=NULL) free(refToFreeAndSetToNull);
			refToFreeAndSetToNull=NULL;
		}
		catch (exception& exn) {//!!!!!!!!!!TODO: finish this: copy this to ALL catch statements EVERYWHERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
		    ShowExn(exn, "SafeFree(char*)");
		}
		catch (...) {
			ShowUnknownExn("SafeFree(char*)");
		}
	}
	/*inline*/ void SafeFree(int* &refToFreeAndSetToNull) {
		try {
			if (refToFreeAndSetToNull!=NULL) free(refToFreeAndSetToNull);
			refToFreeAndSetToNull=NULL;
		}
		catch (exception& exn) {//!!!!!!!!!!TODO: finish this: copy this to ALL catch statements EVERYWHERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
		    ShowExn(exn, "SafeFree(int*)");
		}
		catch (...) {
			ShowUnknownExn("SafeFree(int*)");
		}
	}
	/*inline*/ void SafeFree(int** &refToFreeAndSetToNull, int iIndeces) {
		try {
			for (int index=0; index<iIndeces; index++) {
                if (refToFreeAndSetToNull[index]!=NULL) {
					free(refToFreeAndSetToNull[index]);
					refToFreeAndSetToNull[index]=NULL;
				}
			}
			if (refToFreeAndSetToNull!=NULL) free(refToFreeAndSetToNull);
			refToFreeAndSetToNull=NULL;
		}
		catch (exception& exn) {//!!!!!!!!!!TODO: finish this: copy this to ALL catch statements EVERYWHERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
		    ShowExn(exn, "SafeFree(int**,"+RString_ToString(iIndeces)+")");
		}
		catch (...) {
			ShowUnknownExn("SafeFree(int**,"+RString_ToString(iIndeces)+")");
		}
	}
   bool CopySafe(byte* arrbyDest, byte* arrbySrc, int iDestStart, int iSrcStart, int iBytes) {
		bool bGood=true;
		try {
			memcpy(&arrbyDest[iDestStart],&arrbySrc[iSrcStart],iBytes);
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"CopySafe");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("CopySafe");
		}
		return bGood;
	}
	///#endregion memory

	///#region string
	byte NibbleValueOfHexChar(char cUppercaseLetter) {
		byte byReturn=0;
		for (byte iNow=0; iNow<16; iNow++) {
			if (cUppercaseLetter==szHex[(int)iNow]) {
				byReturn=iNow;
				break;
			}
		}
		return byReturn;
	}
	byte HexCharsToByte(string sHexPair) {
		byte byReturn=0;
		const char* szNow=sHexPair.c_str();
		byReturn=NibbleValueOfHexChar(szNow[0]);
		byReturn+=(byte)16*NibbleValueOfHexChar(szNow[1]);
		return byReturn;
	}
	char LowercaseElseZeroIfNotUppercase(char cNow) {
		char cReturn=(char)0;
		for (int iNow=0; iNow<26; iNow++) {
			if (cNow==szAlphabetUpper[iNow]) {
				cReturn=szAlphabetLower[iNow];
				break;
			}
		}
		return cReturn;
	}//LowercaseElseZeroIfNotUppercase
	string ToUpper(string val) {
		const char* szNow=val.c_str();
		char* szTemp=(char*)malloc(sizeof(char)*val.length());
		for (size_t iNow=0; iNow<val.length(); iNow++) {
			//TODO: fix SLOWness
			char cNow=LowercaseElseZeroIfNotUppercase(szNow[iNow]);
			if ((int)cNow>0) szTemp[iNow]=cNow;
			else szTemp[iNow]=szNow[iNow];
		}
		val.assign(szTemp);
		SafeFree(szTemp);
		return val;
	}//ToUpper
	bool StartsWith(string val, string sPart) {
		return (val.length()>=sPart.length() && sPart.length()>0 && val.substr(0,sPart.length())==sPart);
	}
	bool EndsWith(string val, string sPart) {
		return (val.length()>=sPart.length() && sPart.length()>0 && val.substr(val.length()-sPart.length(),sPart.length())==sPart);
	}
	int SafeLength(string val) {
		int iLen=0;
		try {
			iLen=val.length();
		}
		catch (...) {iLen=0;}
		return iLen;
	}
	string SafeSubstring(string val, int iStart) {
		int iSrcLen=SafeLength(val);
		int iLen=iSrcLen-iStart;
		return SafeSubstring(val,iStart,iLen);
	}
	string SafeSubstring(string val, int iStart, int iLen) {
		string sReturn="";
		int iSrcLen=SafeLength(val);
		try {
			if (iSrcLen>0 && iLen>0) {
				if (iStart<iSrcLen-1) {
					if (iStart+iLen>iSrcLen)
						iLen=iSrcLen-iStart;
					if (iLen>0) {
						sReturn=val.substr(iStart,iLen);
					}
				}
			}
		}
		catch (...) {sReturn="";}
		return sReturn;
	}
	bool Compare(char* s1, char* s2) {
		bool bTest=false;
		try {
			bTest=(0==strcmp(s1, s2));
		}
		catch (...) {
			ShowError("unknown exception","Compare");
			bTest=false;
		}
		return bTest;
	}
	string RString_ToString(int val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(int)--");
		string sReturn="";
		char szTemp[2048];
		strcpyfrom(szTemp,val);
		sReturn.assign(szTemp);
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string RString_ToString(long val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(long)--");
		string sReturn="";
		char szTemp[2048];
		if (bFirstRun) Console::Error.Write("copy...");
		strcpyfrom(szTemp,val);
		if (bFirstRun) Console::Error.Write("assign...");
		sReturn.assign(szTemp);
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string RString_ToString(Uint32 val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(Uint32)--");
		string sReturn="";
		char szTemp[2048];
		if (bFirstRun) Console::Error.Write("copy...");
		strcpyfrom(szTemp,val);
		if (bFirstRun) Console::Error.Write("assign...");
		sReturn.assign(szTemp);
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string RString_ToString(float val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(float)--");
		string sReturn="";
		char szTemp[2048];
		if (bFirstRun) Console::Error.Write("copy...");
		if (RString_iDecimalPlacesForToString>=0) strcpyfrom(szTemp,val,RString_iDecimalPlacesForToString);
		else strcpyfrom(szTemp,val);
		if (bFirstRun) Console::Error.Write("assign...");
		sReturn.assign(szTemp);
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string RString_ToString(double val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(double)--");
		string sReturn="";
		char szTemp[2048];
		if (bFirstRun) Console::Error.Write("copy...");
		if (RString_iDecimalPlacesForToString>=0) strcpyfrom(szTemp,val,RString_iDecimalPlacesForToString);
		else strcpyfrom(szTemp,val);
		if (bFirstRun) Console::Error.Write("assign...");
		sReturn.assign(szTemp);
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string RString_ToString(long double val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(double)--");
		string sReturn="";
		char szTemp[2048];
		if (bFirstRun) Console::Error.Write("copy...");
		if (RString_iDecimalPlacesForToString>=0) strcpyfrom(szTemp,val,RString_iDecimalPlacesForToString);
		else strcpyfrom(szTemp,val);
		if (bFirstRun) Console::Error.Write("assign...");
		sReturn.assign(szTemp);
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string RString_ToString(string val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(string)--");
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return val;
	}
	string RString_ToString(char* val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(char*)--");
		string sReturn="";
		sReturn.assign(val);
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string RString_ToString(const char* val) {
		return RString_ToString((char*)val);
	}
	string RString_ToString(char val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(char)--");
		string sReturn="";
		char szTemp[2];
		szTemp[0]=val;
		szTemp[1]='\0';
		sReturn.assign(szTemp);
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string RString_ToString(bool val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console::Error.Write("RString_ToString(bool)--");
		if (bFirstRun) Console::Error.Write("done...");
		bFirstRun=false;
		return val?"true":"false";
	}
	int GetCSVRow(string *sarrReturn, string val) {
		return GetCSVRow(sarrReturn, val, false,0);
	}
	int GetCSVRow(string *sarrReturn, string val, bool bCountOnlyAndDontTouchArray) {
		return GetCSVRow(sarrReturn,val,bCountOnlyAndDontTouchArray,0) > 0;
	}
	int GetCSVRow(string *sarrReturn, string val, bool bCountOnlyAndDontTouchArray, int iExactCols_OrZeroForAuto) {
		return GetCSVRow(sarrReturn, val, bCountOnlyAndDontTouchArray, iExactCols_OrZeroForAuto, true);
	}
	int GetCSVRow(string *sarrReturn, string val, bool bCountOnlyAndDontTouchArray, int iExactCols_OrZeroForAuto, bool bAllowCurleyBracesRecursively_FalseIgnoresThem) {
		//DOES return all doublequote marks
		bool bGood=true;
		int iCols=0;
		try {
			int iDim=0;
			if (!bCountOnlyAndDontTouchArray) {
				if (iExactCols_OrZeroForAuto>0) {
					sarrReturn=new string[iDim];
				}
				else {
					iDim=GetCSVRow(sarrReturn, val, true, 0);
					if (iDim>0) sarrReturn=new string[iDim];
				}
			}
			bool bInQuotes=false;
			int iInCurleyBraces=0;
			int iStartNow=0;
			int iLenNow=0;
			string sCharNow;
			Console::Error.Write("Parsing "+RString_ToString(val.length())+RString_ToString(" chars..."));
			for (size_t iNow=0; iNow<val.length(); iNow++) {
				sCharNow=val.substr(iNow,1);
				Console::Error.Write(" '"+sCharNow+RString_ToString("'"));
				if (sCharNow=="\"") { bInQuotes=!bInQuotes; iLenNow++; }
				else if (bAllowCurleyBracesRecursively_FalseIgnoresThem && !bInQuotes && sCharNow=="{") { iInCurleyBraces++; iLenNow++;}
				else if (bAllowCurleyBracesRecursively_FalseIgnoresThem && !bInQuotes && sCharNow=="}") { iInCurleyBraces--; iLenNow++;}
				else if (sCharNow=="," && !bInQuotes && iInCurleyBraces<1) {
					if (!bCountOnlyAndDontTouchArray) {
						if (iExactCols_OrZeroForAuto<1 || iCols<iExactCols_OrZeroForAuto) {
							if (iLenNow>0) {
								sarrReturn[iCols]=val.substr(iStartNow,iLenNow);
							}
							else sarrReturn[iCols]="";
						}
					}
					else {
						Console::Error.Write("("+RString_ToString(iStartNow)+RString_ToString(",")+RString_ToString(iLenNow)+RString_ToString(")"));
					}
					iLenNow=0;
					iStartNow=iNow+1;
					iCols++;
				}
				else iLenNow++;
			}//end for character iNow
			//Now do the last col:
			if (!bCountOnlyAndDontTouchArray) {
				if (iExactCols_OrZeroForAuto<1 || iCols<iExactCols_OrZeroForAuto) {
					if (iLenNow>0) {
						sarrReturn[iCols]=val.substr(iStartNow,iLenNow);
					}
					else sarrReturn[iCols]="";
				}
			}
			iCols++;
			if (bCountOnlyAndDontTouchArray) Console::Error.Write("done parsing "+RString_ToString(iCols)+RString_ToString(" cols..."));
		}
		catch (exception& exn) { bGood=false; ShowExn(exn,"GetCSVRow");
		}
		catch (...) { bGood=false; ShowUnknownExn("GetCSVRow");
		}
		if (!bGood) Console::Error.WriteLine("GetCSVRow: Failed, but caller didn't check on that problem.  Fix this code so it returns negative when went wrong instead of having an unused bGood var set to false.  Then program the caller to check on that.");
		return iCols;
	}//end GetCSVRow
	int CountCArrayNotationFields(string val) { //return 0 for non-array
		string* sarrIgnored=null;
		int iReturn=0;
		if ( StartsWith(val,"{") && EndsWith(val,"}") ) {
			if (val.length()>2) iReturn=GetCSVRow(sarrIgnored,SafeSubstring(val,1,val.length()-2),true);
			else iReturn=1;
		}
		return iReturn;
	}
	bool GetForcedCSVNotationSubstring(string &valReturn, string val, int iIndexer) {
		return GetForcedCSVNotationSubstring(valReturn, val, iIndexer, true);
	}
	bool GetForcedCSVNotationSubstring(string &valReturn, string val, int iIndexer, bool bAllowCurleyBracesRecursively_FalseIgnoresThem) {
		//DOES return all doublequote marks
		bool bGood=false;
		try {
			bool bInQuotes=false;
			int iInCurleyBraces=0;
			size_t iStartThatOne;
			size_t iLenThatOne;
			size_t iStartNow=0;
			size_t iLenNow=0;
			int iCol=0;
			string sCharNow;
			for (size_t iNow=0; iNow<val.length(); iNow++) {
				sCharNow=val.substr(iNow,1);
				if (sCharNow=="\"") { bInQuotes=!bInQuotes; iLenNow++; }
				else if (bAllowCurleyBracesRecursively_FalseIgnoresThem && !bInQuotes && sCharNow=="{") { iInCurleyBraces++; iLenNow++; }
				else if (bAllowCurleyBracesRecursively_FalseIgnoresThem && !bInQuotes && sCharNow=="}") { iInCurleyBraces--; iLenNow++; }
				else if (sCharNow=="," && !bInQuotes && iInCurleyBraces<1) {
					if (iCol==iIndexer) {
						iStartThatOne=iStartNow;
						iLenThatOne=iLenNow;
						bGood=true;
						break;
					}
					iLenNow=0;
					iStartNow=iNow+1;
					iCol++;
				}
				else iLenNow++;
			}
			//Now deal with the last column (or col 0 if only 1 column)
			if (bGood) {
				if (iLenThatOne>0 && iStartThatOne<val.length()) valReturn=val.substr(iStartThatOne,iLenThatOne);
				else valReturn="";
			}
			else if (iCol==iIndexer) { //either 1 col, or otherwise last col
				if (iIndexer==0) valReturn=val;
				else if (iLenNow>0 && iStartNow<val.length()) valReturn=val.substr(iStartNow,iLenNow);
				else valReturn="";
				bGood=true;
			}
			else valReturn="";
		}
		catch (exception& exn) { ShowExn(exn,"GetForcedCArrayNotationSubstring");
		}
		catch (...) { ShowUnknownExn("GetForcedCArrayNotationSubstring");
		}
		return bGood;
	}//end GetForcedCSVNotationSubstring
	bool GetForcedCArrayNotationSubstring(string &valReturn, string val, int iIndexer) {
		//return false if not array OR error
		bool bGood=true;
		try {
			if (StartsWith(val,"{")) {
				if (EndsWith(val,"}")) {
					bGood=GetForcedCSVNotationSubstring(valReturn, val.substr(1,val.length()-2), iIndexer);
				}
				else bGood=false;
			}
			else bGood=false;
			if (bGood==false) valReturn=val;
		}
		catch (exception& exn) { ShowExn(exn,"GetForcedCArrayNotationSubstring");
		}
		catch (...) { ShowUnknownExn("GetForcedCArrayNotationSubstring");
		}
		return bGood;
	}//end GetForcedCArrayNotationSubstring


	/// SIMPLE FUNCTIONS USING INTRINSIC TYPES ///


	void trunc(char *s, int i) {
		s[i]='\0';
	}

	void Upperstring(char *s) {
		static int iAt;
		iAt=strlen(s)-1;
		while (iAt>=0) {
			if (islower(s[iAt])) s[iAt]=toupper(s[iAt]);
			iAt--;
		}
		return;
	}
	void Lowerstring(char *s) {
		static int iAt;
		iAt=strlen(s)-1;
		while (iAt>=0) {
			if (isupper(s[iAt])) s[iAt]=tolower(s[iAt]);
			iAt--;
		}
		return;
	}
	void DetectSlashType(char *sFullPath) { //always run this before using directory sz
		if (slash[0]!='/' && sFullPath[0]=='/') slash[0]='/';
		return;
	}
	void ValidateSlash(char *sDir) { //detects slash type and adds slash if needed
		DetectSlashType(sDir);
		if (sDir[ strlen(sDir)-1 ] != slash[0]) {
			if (strlen(sDir)+1<MAX_PATH) strcat(sDir,slash);
		}
		return;
	}
	bool IsLikeMask(char *sFullPath1, char *sMask1) {
	//limitations: gets confused if mask has 2 asterisks directly in a row
		static char sDir[MAX_PATH], sMask[MAX_PATH];
		strcpy(sDir,sFullPath1);//necessary to recopy because of Lowerstring, AND for removal of slash
		strcpy(sMask,sMask1);
		Lowerstring(sDir);
		Lowerstring(sMask);
		DetectSlashType(sDir);
		int iDirLen=strlen(sDir);
		int iDirPlace=iDirLen-1;
		if (sDir[iDirPlace]==slash[0]) sDir[iDirPlace]='\0';
		int iMaskLen=strlen(sMask);
		if ((sDir[iDirLen-1]!=sMask[iMaskLen-1]) && (sMask[iMaskLen-1]!='*')) return(false);
		while (iDirPlace>=0 && sMask[iDirPlace]!=slash[0]) iDirPlace--;
		int iToMatch=0;//characters to find (not the asterisks)
		for (int i=0; i<iMaskLen; i++) {
						if (sMask[i]!='*') iToMatch++;
		}
		//error_txt+"iToMatch="+RString_ToString(iToMatch));
		int iMatches=0;
		int iMaskPlace;
		for (iMaskPlace=0; iMaskPlace<iMaskLen && iDirPlace<iDirLen; iMaskPlace++) {
			if (sMask[iMaskPlace]==sDir[iDirPlace]) {
				iDirPlace++;
				iMatches++;
			}
			else if (sMask[iMaskPlace]=='*') {
				if (iMaskPlace+1<iMaskLen) {
					iMaskPlace++;
					while (iDirPlace<iDirLen && sMask[iMaskPlace]!=sDir[iDirPlace]) {
						iDirPlace++;
					}
					if (iDirPlace<iDirLen && sMask[iMaskPlace]==sDir[iDirPlace]) {
						iDirPlace++;
						iMatches++;
					}
				}
			}
		}
		//error_txt+"iMatches="+RString_ToString(iMatches));
		return((iMatches==iToMatch)?true:false);
	} // end IsLikeMask

	void strcatfrom(char *sz, int i) {
		if (i==0) {
			strcat(sz, "0");
			return;
		}
		else if (i<0) { //if negative
			strcat(sz, "-");
			i=abs(i);
		}
		char sDigit[10][2] = {"0","1","2","3","4","5","6","7","8","9"};
		int iExpOfTen = 0;
		int iTemp = i;
		while (iTemp>9) {
			iTemp/=10;
			iExpOfTen++;
		}
		iTemp=i;
		int iDivisor;
		while (iExpOfTen>=0) {
			iDivisor=int(pow((long double)10, (long double)iExpOfTen));
			if (iDivisor) {
				strcat(sz, sDigit[iTemp/iDivisor]);
				iTemp-=iDivisor*int(iTemp/iDivisor);
			}
			else {
			}
			iExpOfTen--;
		}
		return;
	}
	void strcatfrom(char *sz, long val) {
		if (val==0) {
			strcat(sz, "0");
			return;
		}
		else if (val<0) { //if negative
			strcat(sz, "-");
			val=abs(val);
		}
		char sDigit[10][2] = {"0","1","2","3","4","5","6","7","8","9"};
		long iExpOfTen = 0;
		long iTemp = val;
		while (iTemp>9) {
			iTemp/=10;
			iExpOfTen++;
		}
		iTemp=val;
		long iDivisor;
		while (iExpOfTen>=0) {
			iDivisor=long(pow((long double)10, (long double)iExpOfTen));
			if (iDivisor) {
				strcat(sz, sDigit[iTemp/iDivisor]);
				iTemp-=iDivisor*long(iTemp/iDivisor);
			}
			else {
			}
			iExpOfTen--;
		}
		return;
	}
	void strcatfrom(char *sz, Uint32 val) {
		if (val==0) {
			strcat(sz, "0");
			return;
		}
		//else if (val<0) { //if negative
		//	strcat(sz, "-");
		//	val=abs(val);
		//}
		char sDigit[10][2] = {"0","1","2","3","4","5","6","7","8","9"};
		int iExpOfTen = 0;
		Uint32 iTemp = val;
		while (iTemp>9) {
			iTemp/=10;
			iExpOfTen++;
		}
		iTemp=val;
		unsigned long divisor;
		while (iExpOfTen>=0) {
			divisor=int(pow((long double)10, (long double)iExpOfTen));
			if (divisor) {
				strcat(sz, sDigit[iTemp/divisor]);
				iTemp-=divisor*(Uint32)(iTemp/divisor);
			}
			else {
			}
			iExpOfTen--;
		}
		return;
	}//end strcatfrom Uint32

	void strcpyfrom(char *sz, int i) {
		sz[0]='\0';
		strcatfrom(sz, i);
	}
	void strcpyfrom(char *sz, long val) {
		sz[0]='\0';
		strcatfrom(sz, val);
	}
	void strcpyfrom(char *sz, Uint32 val) {
		sz[0]='\0';
		strcatfrom(sz, val);
	}

	void strcatfrom(char *sNum, register float d1, int iMaxDecimals) {
		int iExp10Max=-iMaxDecimals;
		if (d1==0.0f) {
			strcat(sNum,"0.0");
			return;
		}
		//sNum[0]='\0';
		if (d1<0.0f) { //if negative
			strcat(sNum, "-");
			d1=fabs(d1);//set abs
		}
		register float d=d1;//d is the destructible version
		int iExp10=15;//DMAXEXP;
		while (d<pow(10.0f,iExp10)) iExp10--;
		////error_txt+"float iExp10's = ";
		if (iExp10<0) iExp10=0; //make sure leading zeros get copied
		const static float DMINEXP=-15;
		while ( (d>0 && iExp10>=DMINEXP && iExp10>iExp10Max) || iExp10>=-1 ) {
			if ( (Uint32)( d/pow(10.0f,iExp10) ) < 10 ) {
				//error_txt+"["+(Uint32)( d/pow(10,iExp10) )+"](e="+RString_ToString(iExp10)+")"); Console::Error.Flush();
				strcat(sNum, sDigit[(Uint32)( d/pow(10.0f,iExp10) )]);
				if (iExp10==0) strcat(sNum, ".");
			}
			//else {
				//error_txt+"ERROR: strcatfromdouble()'s (Uint32)( d/pow(10,iExp10) ) isn't < 10! =("+(Uint32)( d/pow(10.0f,iExp10) )+")");
			//}
			d-=(float)((Uint32)( d/pow(10.0f,iExp10) ))*pow(10.0f,iExp10);//Uint32 to truncate
			iExp10--;
		}
		return;
	}//end strcatfrom(char *sNum, register float d1, int iMaxDecimals) {
	void strcatfrom(char *sNum, register double d1, int iMaxDecimals) {
		int iExp10Max=-iMaxDecimals;
		if (d1==0.0) {
			strcat(sNum,"0.0");
			return;
		}
		//sNum[0]='\0';
		if (d1<0.0) { //if negative
			strcat(sNum, "-");
			d1=fabs(d1);//set abs
		}
		register double d=d1;//d is the destructible version
		int iExp10=15;//DMAXEXP;
		while (d<pow(10.0/*d*/,iExp10)) iExp10--;
		////error_txt+"float iExp10's = ";
		if (iExp10<0) iExp10=0; //make sure leading zeros get copied
		const static double DMINEXP=-15;
		while ( (d>0 && iExp10>=DMINEXP && iExp10>iExp10Max) || iExp10>=-1 ) {
			if ( (Uint32)( d/pow(10.0/*d*/,iExp10) ) < 10 ) {
				//error_txt+"["+(Uint32)( d/pow(10.0/*d*/,iExp10) )+"](e="+RString_ToString(iExp10)+")"); Console::Error.Flush();
				strcat(sNum, sDigit[(Uint32)( d/pow(10.0/*d*/,iExp10) )]);
				if (iExp10==0) strcat(sNum, ".");
			}
			else {
				//error_txt+"ERROR: strcatfromdouble()'s (Uint32)( d/pow(10.0/*d*/,iExp10) ) isn't < 10! =("+(Uint32)( d/pow(10.0/*d*/,iExp10) )+")");
			}
			d-=double((Uint32)( d/pow(10.0/*d*/,iExp10) ))*pow(10.0/*d*/,iExp10);
			iExp10--;
		}
		return;
	}
	void strcatfrom(char *sNum, register long double d1, int iMaxDecimals) {
		int iExp10Max=-iMaxDecimals;
		if (d1==0.0) {
			strcat(sNum,"0.0");
			return;
		}
		//sNum[0]='\0';
		if (d1<0.0) { //if negative
						strcat(sNum, "-");
						d1=fabs(d1);//set abs
		}
		register double d=d1;//d is the destructible version
		int iExp10=15;//DMAXEXP;
		while (d<pow(10.0L,iExp10)) iExp10--; //L is long double
		////error_txt+"float iExp10's = ";
		if (iExp10<0) iExp10=0; //make sure leading zeros get copied
		const static double DMINEXP=-15;
		while ( (d>0 && iExp10>=DMINEXP && iExp10>iExp10Max) || iExp10>=-1 ) {
			if ( (Uint32)( d/pow(10.0L,iExp10) ) < 10 ) {
				//error_txt+"["+(Uint32)( d/pow(10.0L,iExp10) )+"](e="+RString_ToString(iExp10)+")"); Console::Error.Flush();
				strcat(sNum, sDigit[(Uint32)( d/pow(10.0L,iExp10) )]);
				if (iExp10==0) strcat(sNum, ".");
			}
			else {
				//error_txt+"ERROR: strcatfromdouble()'s (Uint32)( d/pow(10.0L,iExp10) ) isn't < 10! =("+(Uint32)( d/pow(10.0L,iExp10) )+")");
			}
			d-=double((Uint32)( d/pow(10.0L,iExp10) ))*pow(10.0L,iExp10);
			iExp10--;
		}
		return;
	}
	void strcatfrom(char *sNum, register float d1) {
		strcatfrom(sNum, d1, 10);//-(DMINEXP));
	}
	void strcatfrom(char *sNum, register double d1) {
		strcatfrom(sNum, d1, 10);//-(DMINEXP));
	}
	void strcatfrom(char *sNum, register long double d1) {
		strcatfrom(sNum, d1, 10);//-(DMINEXP));
	}

	void strcpyfrom(char *sNum, register float d1, int iMaxDecimals) {
		sNum[0]='\0';//effectively, strcpy
		strcatfrom(sNum, d1, iMaxDecimals);
	}
	//overload it without Max Decimals, so set MaxDecimals to 10:
	void strcpyfrom(char *sNum, register float d1) {
		sNum[0]='\0';//effectively, strcpy
		strcatfrom(sNum, d1, 10);//-(DMINEXP));
	}
	void strcpyfrom(char *sNum, register double d1, int iMaxDecimals) {
		sNum[0]='\0';//effectively, strcpy
		strcatfrom(sNum, d1, iMaxDecimals);
	}
	//overload it without Max Decimals, so set MaxDecimals to 10:
	void strcpyfrom(char *sNum, register double d1) {
		sNum[0]='\0';//effectively, strcpy
		strcatfrom(sNum, d1, 10);//-(DMINEXP));
	}
	void strcpyfrom(char *sNum, register long double d1) {
		sNum[0]='\0';//effectively, strcpy
		strcatfrom(sNum, d1, 10);//-(DMINEXP));
	}
	void strcpyfrom(char *sNum, register long double d1, int iMaxDecimals) {
		sNum[0]='\0';//effectively, strcpy
		strcatfrom(sNum, d1, 10);//-(DMINEXP));
	}

	int IntOfChar(char *cDigitX) {
		static int iErrLevel=0;
		for (register int i=0; i<10; i++) {
			if (*cDigitX==cDigit[i]) return(i);
		}
		//return before now usually
		//if (iErrLevel<10)error_txt+"IntOfChar error: "+*cDigitX+" is not an integer!";//error if didn't return yet;
		//else if (iErrLevel<11)error_txt+"LAST WARNING -- IntOfChar error: "+*cDigitX+" is not an integer!";//error if didn't return yet;
		iErrLevel++;
		return(0);
	}
	bool substrcpy(char* sDest, char* sSrc, int iStart, int iLen) {
		bool bGood=false;
		try {
			//memcpy( (byte*)sDest, ( ((byte*)sSrc)+iStart*sizeof(char) ), iLen*sizeof(char) );
			for (int iNow=0; iNow<iLen; iNow++) {
				sDest[iNow]=sSrc[iStart+iNow];
			}
			sDest[iLen]='\0';
			bGood=true;
		}
		catch(...) {
			try {
				Console::Error.WriteLine("Exception in substrcpy(\""+RString_ToString(sDest)+"\",\""+RString_ToString(sSrc)+"\","+RString_ToString(iStart)+","+RString_ToString(iLen)+")");
			}
			catch (...) {
	            Console::Error.WriteLine("Exception:");
				try {
					Console::Error.WriteLine("  substrcpy Dest:"+RString_ToString(sDest));
				}
				catch (...) {
					Console::Error.WriteLine("  substrcpy(bad Dest sz sent)");
				}
				try {
					Console::Error.WriteLine("  substrcpy Src:"+RString_ToString(sSrc));
				}
				catch (...) {
					Console::Error.WriteLine("  substrcpy(bad Source sz sent)");
				}
				Console::Error.WriteLine("  substrcpy iStart:"+RString_ToString(iStart));
				Console::Error.WriteLine("  substrcpy iLen:"+RString_ToString(iLen));
			}
		}
		if (!bGood) {
			try {
				sDest[0]='\0';
			}
			catch(...) {
				//don't report this
			}
		}
		return bGood;
	}//substrcpy
	bool Crop(char* &sVal, int iStart) {
		bool bGood=false;
		try {
			char* sTemp=(char*)malloc(sizeof(char)*strlen(sVal));
			strcpy(sTemp,(char*)(sVal+iStart));
			SafeFree(sVal);
			sVal=sTemp;
			sTemp=NULL;
			bGood=true;
		}
		catch (...){
			Console::Error.WriteLine("Exception in Crop char*");
			bGood=false;
		}
		return bGood;
	}
	int IntOfString(char *sNum) {
		register Uint32 u32Strlen=strlen(sNum);
		register Uint32 u32=0;//loop var
		register Uint32 u32IsNeg=0;
		if (sNum[0]=='-') u32IsNeg=1;
		register int iExp10=strlen(sNum)-1-int(u32IsNeg);
		register int iReturn=0;
		for (u32=u32IsNeg; u32<u32Strlen; u32++) {
			if (sNum[u32]!='.') {
				iReturn+=(int)( powl((long double)10,(long double)iExp10)*(long double)IntOfChar(&sNum[u32]) );//math.h: long powl(10,int)
				iExp10--;
			}
		}
		if (u32IsNeg==1) iReturn*=-1;
		return(iReturn);
	}//end IntOfString
	double DoubleOfString(char *sNum) {
		register Uint32 u32Strlen=strlen(sNum);
		register Uint32 u32=0;//loop var
		register int iExp10=-1;
		register Uint32 u32IsNeg=0;
		if (sNum[0]=='-') {
			u32IsNeg=1;
		}
		for (u32=u32IsNeg; u32<u32Strlen && sNum[u32]!='.'; u32++) {
			iExp10++;
		}
		register double dReturn=0;
		for (u32=u32IsNeg; u32<u32Strlen; u32++) {
			if (sNum[u32]!='.') {
				dReturn+=pow(10.0/*d*/,iExp10)*(double)IntOfChar(&sNum[u32]);//math.h: double pow(10.0/*d*/,int)
				iExp10--;
			}
		}
		if (u32IsNeg==1) dReturn*=-1;
		return(dReturn);
	}

	float FloatOfString(char *sNum) {
		register Uint32 u32Strlen=strlen(sNum);
		register Uint32 u32=0;//loop var
		register int iExp10=-1;
		register Uint32 u32IsNeg=0;
		if (sNum[0]=='-') {
			u32IsNeg=1;
		}
		for (u32=u32IsNeg; u32<u32Strlen && sNum[u32]!='.'; u32++) {
			iExp10++;
		}
		register float fReturn=0;
		for (u32=u32IsNeg; u32<u32Strlen; u32++) {
			if (sNum[u32]!='.') {
				fReturn+=powl(10,iExp10)*(long double)IntOfChar(&sNum[u32]);//math.h: long double powl(10,int)
				iExp10--;
			}
		}
		if (u32IsNeg==1) fReturn*=-1;
		return(fReturn);
	}

	bool IsExt(char *sFull1, char *sExt1) {
		static char sFull[MAX_PATH];
		static char sExt[MAX_PATH];
		strcpy(sFull, sFull1);
		strcpy(sExt, sExt1);
		Upperstring(sFull);
		Upperstring(sExt);

		int iFullLen=strlen(sFull);
		int iExtLen=strlen(sExt);
		bool bTrue=true;
		if (sFull[iFullLen-iExtLen-1]!='.') bTrue=false;
		else {
			int iExtNow=0;
			for (int iFullNow=iFullLen-iExtLen; iFullNow<iFullLen; iFullNow++, iExtNow++) {
				if (sFull[iFullNow]!=sExt[iExtNow]) {
					bTrue=false;
					break;
				}
			}
		}
		return(bTrue);
	}
	bool ExistsAt(char* sHaystack, char* sNeedle, int iAtHaystack) {
		bool bMatch=false;
		//bool bGood=false;
		try {
			int iMatchingChars=0;
			int iLen=strlen(sNeedle);
			int iHaystack=iAtHaystack;
			for (int iNow=0; iNow<iLen; iNow++, iHaystack++) {
				if (sHaystack[iHaystack]==sNeedle[iNow]) {
					iMatchingChars++;
				}
				else break;
			}
			if (iMatchingChars==iLen) bMatch=true;
		}
		catch (exception& exn) {
			ShowExn(exn,"ExistsAt(char* sHaystack, char* sNeedle, int iAtHaystack)");
		}
		catch (...) {
			ShowUnknownExn("ExistsAt(char* sHaystack, char* sNeedle, int iAtHaystack)");
		}
		//catch (...) {
		//	ShowError("Exception in ExistsAt");
		//}
		return bMatch;
	}

	/*inline*/ int IndexOf(char* sHaystack, char* sNeedle) {
		int iMaxLoops=1000000;
		int iNow=0;
		int iFound=-1;
		try {
			while (sHaystack[iNow]!='\0') {
				if ( ExistsAt(sHaystack,sNeedle,iNow) ) {
					iFound=iNow;
					break;
				}
				iNow++;
				if (iNow>iMaxLoops) break;
			}
		}
		catch (exception& exn) {
			ShowExn(exn,"IndexOf");
		}
		//catch (char* szExn) {
		//	ShowAndDeleteException(szExn,"IndexOf");
		//}
		catch (...) {
			ShowUnknownExn("IndexOf");
		}
		return iFound;
	}//end IndexOf

	///#endregion string

	///#region reporting
	bool ShowErr() {
		return ShowError();
	}
	//bool ShowErr(string sMsg, string sCurrentFunction) {
	//	return ShowError(sMsg,sCurrentFunction);
	//}
	bool ShowError() {
		bool bPermission=false;
		if (iErrors<iMaxErrors) {
			bPermission=true;
		}
		else if (iErrors==iMaxErrors) {
			Console::Error.WriteLine("Too many errors ("+RString_ToString(iErrors)+")--this is the last one that will be shown:");
			bPermission=true;//TODO: should this be false???
		}
		else bPermission=false;
		iErrors++;
		return bPermission;//TODO: should this return bGood???
	}
	bool ShowErr(string sMsg) {
		return ShowError(sMsg,"");
	}
	bool ShowError(string sMsg) {
		bool bGood=true;
		if (iErrors<iMaxErrors) {
			try {
				Console::Error.WriteLine(sMsg);
			}
			catch (exception& exn) { Console::Error.WriteLine("ShowError (self-exception)"+RString_ToString(exn.what())); }
			catch (...) { Console::Error.WriteLine("Default self-exception during exception output."); }
		}
		else if (iErrors==iMaxErrors) { //if > do nothing!
			Console::Error.WriteLine("Too many errors ("+RString_ToString(iErrors)+")--this is the last that will be shown: \n\t"+sMsg);
		}
		iErrors++;
		return bGood;
	}//end ShowError
	bool ShowErr(string sMsg, string sCurrentFunction) {
		return ShowError(sMsg,sCurrentFunction);
	}
	bool ShowError(string sMsg, string sCurrentFunction) {
		bool bGood=true;
		if (ShowError()) Console::Error.WriteLine(sCurrentFunction+": "+sMsg);
		return bGood;
	}
	bool ShowError(int iSivErrNo, string sMsg, string sCurrentFunction) {
		bool bGood=true;
		char szTemp[2048];//=(char*)malloc(10240*sizeof(char));
		if (iSivErrNo!=0) {
			if (iErrors<iMaxErrors) {
				try {
					szTemp[0]='\0';
					strcatfromsiverrno(szTemp, iSivErrNo);
					Console::Error.WriteLine(sCurrentFunction+": "+RString_ToString(szTemp)+": "+sMsg);
				}
				catch (exception& exn) { Console::Error.WriteLine("ShowError (self-exception)"+RString_ToString(exn.what())); }
				catch (...) { Console::Error.WriteLine("Default self-exception during exception output."); }
			}
			else if (iErrors==iMaxErrors) { //if > do nothing!
				Console::Error.WriteLine("Too many errors ("+RString_ToString(iErrors)+")--this is the last that will be shown: \n\t"+sCurrentFunction+" {szTemp:"+RString_ToString(szTemp)+"}: "+sMsg);
			}
			iErrors++;
		}//end if iSivErrNo!=0
		return bGood;
	}//end ShowError
	bool ShowError(int iSivErrNo, string sCurrentFunction) {
		bool bGood=true;
		if (ShowError()) {
			char szTemp[2048];
			strcpyfromsiverrno(szTemp,iSivErrNo);
			Console::Error.WriteLine(sCurrentFunction+": "+szTemp);
		}
		return bGood;
	}
	//void ShowAndDeleteException(char* &sExn, string sFuncNow) {
	//	ShowAndDeleteException(sExn,sFuncNow,"");
	//}
	//void ShowAndDeleteException(char* &sExn, string sFuncNow, string sVerbNow) {
	//	if (sVerbNow.length()>0) sVerbNow=" "+sVerbNow;
	//	if (ShowErr()) Console::Error.WriteLine("Could not finish "+sFuncNow+sVerbNow+": "+sExn);
	//	SafeFree(sExn);
	//}
	void ShowExn(exception& exn, string sFuncNow) {
		ShowExn(exn,sFuncNow,"");
	}
	void ShowExn(exception& exn, string sFuncNow, string sVerbNow) {
		if (sVerbNow.length()>0) sVerbNow=" "+sVerbNow;
		if (ShowErr()) Console::Error.WriteLine("Could not finish "+sFuncNow+sVerbNow+": "+RString_ToString(exn.what()));
	}
	void ShowUnknownExn(string sFuncNow, string sVerbNow) {
		if (sVerbNow.length()>0) sVerbNow=" "+sVerbNow;
		if (ShowErr()) Console::Error.WriteLine("Could not finish "+sFuncNow+sVerbNow+".");
	}
	void ShowUnknownExn(string sFuncNow) {
		ShowUnknownExn(sFuncNow,"");
	}
	void FakeException(string sExn) { //TODO: remove this and all calls to it
		if (iErrors<iMaxErrors) {
			try {
				Console::Error.WriteLine("Could not finish: "+sExn);
			}
			catch (exception& exn) { Console::Error.WriteLine("Could not finish while showing exception: "+RString_ToString(exn.what())); }
			catch (...) { Console::Error.WriteLine("Could not finish while showing exception."); }
		}
		else if (iErrors==iMaxErrors) { //if > do nothing!
			Console::Error.WriteLine("Too many errors ("+RString_ToString(iErrors)+")--this is the last that will be shown: \n\t"+sExn);
		}
		iErrors++;
	}//end FakeException
	//void strcatfromsiverrno(char*, int);
	//void strcpyfromsiverrno(char*, int);
	void strcpyfromsiverrno(char* s, int i) {
		if (s==NULL) {// sErr=(char*)malloc(255*sizeof(char));
			//ShowError(NULL,"s==NULL!","strcatfromsiverrno",0);
			//ShowError("s==NULL!","strcatfromsiverrno",0);
		}
		else {
			s[0]='\0';
			strcatfromsiverrno(s, i);
		}
		return;
	}//end strcpyfromsiverrno
	void strcatfromsiverrno(char* sErr, int iNum) {
		//0-99 - general
		//100's - Memory
		//200's - DirectX
		//300's - File
		//400's - Data (also used when checking data at file load time)
		//500's - DirectX, 550-599 are fatal
		if (sErr==NULL) {// sErr=(char*)malloc(255*sizeof(char));
			//errlog.Add(NULL,"sErr==NULL!","strcatfromsiverrno",0);
			//errlog.Add("sErr==NULL!","strcatfromsiverrno",0);
			return;//debug
		}
		if (iNum) {
			strcat(sErr, "Fail, error #");
			strcatfrom(sErr, iNum);
			strcat(sErr, " ");
		}
		if (iNum==0) strcat(sErr,"Success.");
		else if (iNum==1) strcat(sErr,"General Error");
		else if (iNum==2) strcat(sErr,"Dimensions are too big (greater than 1048576 pixels) for Compression");
		else if (iNum==3) strcat(sErr,"Too many blocks!	May need to increase compression or decrease image size.");
		else if (iNum==4) strcat(sErr,"Image dimensions must both be divisible by 4 for this Compression method.");
		else if (iNum==5) strcat(sErr,"User cancelled compression.");
		else if (iNum==6) strcat(sErr,"Fast copy cannot transfer dissimilar bitdepths");
		else if (iNum==7) strcat(sErr,"Null compressed structure pointer.");
		else if (iNum==8) strcat(sErr,"_LOADED flag is not present.");
		else if (iNum==9) strcat(sErr,"Null targa pointer.");
		else if (iNum==10) strcat(sErr,"Function not implemented.");
		else if (iNum==11) strcat(sErr,"Compression header structure is NULL!");
		else if (iNum==12) strcat(sErr,"Wrong function chosen for this compression type");
		else if (iNum==13) strcat(sErr,"Null compression caps pointer");

		else if (iNum==20) strcat(sErr,"This version of the decompressor cannot load this type of compression, or the file header is corrupt.");

		else if (iNum==40) strcat(sErr,"This version of the compressor cannot encode this type of pixel");
		else if (iNum==41) strcat(sErr,"This version of the compressor cannot encode use this set of compression options.");
		else if (iNum==42) strcat(sErr,"This version of the compressor cannot encode this compression method.");
		else if (iNum==43) strcat(sErr,"To many pattern instances.	Image must be too large.");
		else if (iNum==50) strcat(sErr,"Compressed file structure is already unloaded.");
		else if (iNum==51) strcat(sErr,"Targa is already unloaded.");

		else if (iNum==60) strcat(sErr,"alphablender is full");

		//Memory errors
		else if (iNum==100) strcat(sErr,"General error allocating memory");
		else if (iNum==101) strcat(sErr,"Couldn't create compression block pixel buffer");
		else if (iNum==102) strcat(sErr,"Couldn't create block flags buffer");
		else if (iNum==103) strcat(sErr,"Couldn't create compression map buffer");
		else if (iNum==104) strcat(sErr,"Couldn't create temp block pixel buffer");
		else if (iNum==105) strcat(sErr,"Couldn't create temp map buffer");
		else if (iNum==106) strcat(sErr,"Couldn't allocate compression header structure");
		else if (iNum==107) strcat(sErr,"Couldn't allocate targa buffer");
		else if (iNum==108) strcat(sErr,"Couldn't create this type of targa -- try uncompressed non-zero-length image");
		else if (iNum==109) strcat(sErr,"Couldn't create targa with odd bit depth -- try 8/16/24/32");
		else if (iNum==110) strcat(sErr,"General error allocating Targa memory");
		else if (iNum==111) strcat(sErr,"Intermediate targa bitdepth error");
		//memory read errors:
		else if (iNum==150) strcat(sErr,"General typesafe error (bad data)");
		else if (iNum==151) strcat(sErr,"Uncompress buffer is NULL");
		//Siv DirectX pattern surface - see 500's for regular directX surface
		else if (iNum==200) strcat(sErr,"General error in Compressor surface pattern buffer");
		//File
			//Save
		else if (iNum==300) strcat(sErr,"General error saving compressed file");
		else if (iNum==301) strcat(sErr,"Error saving compressed file header");
		else if (iNum==302) strcat(sErr,"Error saving compressed file integer buffer");
		else if (iNum==303) strcat(sErr,"Error saving compressed file map buffer");
		else if (iNum==304) strcat(sErr,"Error saving compressed file block pixel buffer");
		else if (iNum==305) strcat(sErr,"Error saving compressed file pattern buffer");
		else if (iNum==306) strcat(sErr,"Error saving compressed file because this graphics type (color depth etc) is not supported");
		else if (iNum==307) strcat(sErr,"Error saving Targa, corrupt data or disk error");
			//Load
		else if (iNum==310) strcat(sErr,"General error loading compressed file");
		else if (iNum==311) strcat(sErr,"File not found");
		else if (iNum==312) strcat(sErr,"Not all data from compressed file was used, check header and integer data");
		else if (iNum==313) strcat(sErr,"Couldn't open the source targa file");
		else if (iNum==314) strcat(sErr,"Couldn't load source targa into non-NULL buffer");
		else if (iNum==315) strcat(sErr,"Couldn't load source targa because it couldn't allocate buffer memory");
		else if (iNum==316) strcat(sErr,"Couldn't load source targa into NULL pointer");
		else if (iNum==317) strcat(sErr,"Compresed file has unknown bit depth or compression type");
		else if (iNum==318) strcat(sErr,"Compressed file is too big and probably unusable!");
		else if (iNum==319) strcat(sErr,"Loading source targa file to NULL pointer is impossible");
		else if (iNum==320) strcat(sErr,"Failed to find the expected data in this file.");
		//Data
			//General
		else if (iNum==400) strcat(sErr,"General compression data error (appears to contain bad data)");
		else if (iNum==401) strcat(sErr,"Compression header appears to be bad");
		else if (iNum==402) strcat(sErr,"Compressed file is missing or incomplete (read less than size of header)");
		else if (iNum==403) strcat(sErr,"Compression header's specified bit depth is not usable");
		else if (iNum==404) strcat(sErr,"Compression header's bit depth is a negative number");
		else if (iNum==405) strcat(sErr,"Compression header says compressed file is not loaded!");
		else if (iNum==406) strcat(sErr,"Cannot draw source targa to surface.");
		else if (iNum==450) strcat(sErr,"General graphics effects error");
		else if (iNum==451) strcat(sErr,"Couldn't interpret compressed targa");
			//integer buffer
		else if (iNum==410) strcat(sErr,"Data in compressed file integer buffer is bad");
		else if (iNum==411) strcat(sErr,"Compression Integer buffer doesn't exist");
		else if (iNum==412) strcat(sErr,"Data in Compression integer buffer is incomplete");
		else if (iNum==413) strcat(sErr,"Data in Compression integer buffer is beyond normal range");
		else if (iNum==414) strcat(sErr,"Data in Compression integer buffer is bad since negative");
		else if (iNum==415) strcat(sErr,"Data in Compression integer buffer is bad since map size is zero");
		else if (iNum==416) strcat(sErr,"Data in Compression integer buffer is bad since zero");
		else if (iNum==417) strcat(sErr,"Values in Compression integer buffer are incongruous");
			//map buffer
		else if (iNum==420) strcat(sErr,"Data in Compression map is bad");
		else if (iNum==421) strcat(sErr,"Compression map doesn't exist");
		else if (iNum==422) strcat(sErr,"Data in Compression map is incomplete");
		else if (iNum==423) strcat(sErr,"Data in Compression map is beyond range");
		else if (iNum==424) strcat(sErr,"Data in Compression map is negative");
		else if (iNum==425) strcat(sErr,"Compression map contains a misplaced zero");
			//block buffer
		else if (iNum==430) strcat(sErr,"Data in Compression block pixel buffer is bad");
		else if (iNum==431) strcat(sErr,"Compression block pixel buffer doesn't exist");
		else if (iNum==432) strcat(sErr,"Data in Compression block pixel buffer is incomplete");
		else if (iNum==433) strcat(sErr,"Compression block pixel buffer isn't correct size");
		else if (iNum==434) strcat(sErr,"Compression block pixel buffer contains a zero block");
		else if (iNum==435) strcat(sErr,"Compression block pixel buffer contains multiple zero blocks");
			//animation
		else if (iNum==440) strcat(sErr,"Compression animation data error");
		else if (iNum==441) strcat(sErr,"(User canceled the animation/Compression)");
		//DirectX
		else if (iNum==500) strcat(sErr,"General Compression DirectX Error");
		else if (iNum==502) strcat(sErr,"Couldn't release DirectDraw Lock");
		else if (iNum==503) strcat(sErr,"Couldn't set DirectX cooperative level");
		else if (iNum==504) strcat(sErr,"Couldn't set DirectX display mode");
		else if (iNum==505) strcat(sErr,"Couldn't lock DirectX surface");
		else if (iNum==506) strcat(sErr,"Couldn't unlock DirectX surface");
			//directX fatal errors
		else if (iNum==550) strcat(sErr,"Serious Compression DirectX Error!");
		else if (iNum==551) strcat(sErr,"Compression couldn't create DirectX surface (low on memory?)!");
		else if (iNum==552) strcat(sErr,"Compression couldn't find directX 7 or higher on your system!");
		else if (iNum==553) strcat(sErr,"Compression couldn't attach backbuffer!");
		else if (iNum==1000) strcat(sErr,"Exception");
		else //Unknown
			strcat(sErr,"No Description");
		return;
	}//end strcatfromsiverrno
	/*inline*/ bool stringerrors(char *sz, Uint32 u32MaxBuff) {
		if (sz==NULL) {
			//error_txt+"STRING ERROR: "+"sz is NULL!!!");
			return(true);
		}
		if (strlen(sz)<1 || strlen(sz)>=u32MaxBuff) {
			//error_txt+"STRING ERROR: "+strlen(sz)+" is strlen(sz)! (MaxBuff="+u32MaxBuff+") sz="+sz);
			return(true);
		}
		return(false);
	}
	///#endregion reporting

	///#region math
	///NEXT TWO METHODS SHOULD BE THE ONLY CALLS TO SDL_GetTicks in the WHOLE PROGRAM
	/*inline*/ int Base_GetTicks_Absolute() {
		return SDL_GetTicks();
	}
	/*inline*/Uint32 Base_GetTicks_Relative() {
		return SDL_GetTicks()-dwTicksAcquiredOutsideOfGameState_Run;
	}
	REAL Base_GetSeconds_Relative() {
		return ((REAL)Base_GetTicks_Relative())/Base_r1000;
	}
	UInt32 Base_TicksSince_Relative(int iTickCount) {
		return  (UInt32) (Base_GetTicks_Relative()-iTickCount);
	}

	REAL Base_SecondsSince_Relative(REAL rSecondCount) {
		return  (REAL) (Base_GetSeconds_Relative()-rSecondCount);
	}
	REAL Base_GetSeconds_Absolute() {
		return ((REAL)Base_GetTicks_Absolute())/Base_r1000;
	}
	REAL Base_SecondsSince_Absolute(REAL rSecondCount) {
		return  (REAL) (Base_GetSeconds_Absolute()-rSecondCount);
	}
	float MirrorOnYLine(float fAngle) {
		SafeAngle360ByRef(fAngle);
		if (fAngle<90.0f) fAngle+=180.0f-fAngle;
		else if (fAngle<180.0f) fAngle=180.0f-fAngle;
		else if (fAngle<270.0f) fAngle=360.0f-(fAngle-180.0f);
		else fAngle=360.0f-(fAngle-180.0f);
		return fAngle;
	}
	float AngleToward(float xDest, float yDest, float xSrc, float ySrc) {
		register float xRel=xDest-xSrc, yRel=yDest-xSrc;
		float fReturn=FTHETAOFXY_DEG(xRel,yRel);
		if (fReturn<0.0f) fReturn+=360.0f;
		return fReturn;
	}
	///*inline*/ void Rotate(float &xToMove, float &yToMove, float fRotate) {
	//	float rTemp=ROFXY(xToMove,yToMove), thetaTemp=FTHETAOFXY_DEG(xToMove,yToMove);
	//	thetaTemp+=fRotate;
	//	xToMove=DXOFRTHETA_DEG(rTemp,thetaTemp);
	//	yToMove=DYOFRTHETA_DEG(rTemp,thetaTemp);
	//}
	///*inline*/ void Rotate(float &xToMove, float &yToMove, float xCenter, float yCenter, float fRotate) {
	//	xToMove-=xCenter;
	//	yToMove-=yCenter;
	//	float rTemp=ROFXY(xToMove,yToMove), thetaTemp=FTHETAOFXY_DEG(xToMove,yToMove);
	//	thetaTemp+=fRotate;
	//	xToMove=DXOFRTHETA_DEG(rTemp,thetaTemp);
	//	yToMove=DYOFRTHETA_DEG(rTemp,thetaTemp);
	//	xToMove+=xCenter;
	//	yToMove+=yCenter;
	//}
	/*inline*/ void Crop(float &fToModify, float fMin, float fMax) {
		if (fToModify>fMax) fToModify=fMax;
		else if (fToModify<fMin) fToModify=fMin;
	}
	///*inline*/ float SafeAngle360(float fToLimitBetweenZeroAnd360) {
	//	SafeAngle360ByRef(fToLimitBetweenZeroAnd360);
	//	return fToLimitBetweenZeroAnd360;
	//}
	///*inline*/ double SafeAngle360(double fToLimitBetweenZeroAnd360) {
	//	SafeAngle360ByRef(fToLimitBetweenZeroAnd360);
	//	return fToLimitBetweenZeroAnd360;
	//}
	/*inline*/ void SafeAngle360ByRef(float &fToLimitBetweenZeroAnd360) {
		//float fAngleTest=fToLimitBetweenZeroAnd360;//debug only
		fToLimitBetweenZeroAnd360-=( FFLOOR(fToLimitBetweenZeroAnd360/360.0f) * 360.0f );
		if (fToLimitBetweenZeroAnd360<0.0f) fToLimitBetweenZeroAnd360+=360.0f;
		if (fToLimitBetweenZeroAnd360<0.0f) Console::Error.WriteLine("Programmer error: SafeAngle360ByRef returning negative!");
		//static int iTest=0;//debug only
		//if (iTest<1000) {
		//	Console::Error.WriteLine("SafeAngle360 of "+RString_ToString(fAngleTest)+" is "+RString_ToString(fToLimitBetweenZeroAnd360)+".");
		//}
		//iTest++;
	}
	/*inline*/ void SafeAngle360ByRef(double &fToLimitBetweenZeroAnd360) {
		//double fAngleTest=fToLimitBetweenZeroAnd360;//debug only
		fToLimitBetweenZeroAnd360-=( DFLOOR(fToLimitBetweenZeroAnd360/360.0) * 360.0 );
		if (fToLimitBetweenZeroAnd360<0.0) fToLimitBetweenZeroAnd360+=360.0;
		if (fToLimitBetweenZeroAnd360<0.0) Console::Error.WriteLine("Programmer error: SafeAngle360ByRef returning negative!");
		//static int iTest=0;//debug only
		//if (iTest<1000) {
		//	Console::Error.WriteLine("SafeAngle360 of "+RString_ToString(fAngleTest)+" is "+RString_ToString(fToLimitBetweenZeroAnd360)+".");
		//}
		//iTest++;
	}
	/*inline*/ LPIPOINT IPOINTFROM(float xNow, float yNow) {
		LPIPOINT lpiPoint=NULL;
		try {
			lpiPoint=(LPIPOINT)malloc(sizeof(IPOINT));
			lpiPoint->X=(int)xNow;
			lpiPoint->Y=(int)yNow;
		}
		catch (exception& exn) {
			lpiPoint=NULL;
			ShowExn(exn,"ExpertMultimediaBase::IPOINTFROM");
		}
		catch (...) {
			lpiPoint=NULL;
			ShowUnknownExn("ExpertMultimediaBase::IPOINTFROM");
		}
		return lpiPoint;
	}
	/*inline*/ void RConvert_RectToPolar(int &r,int &theta, int X, int Y) {
		r=(int)(sqrt((double)(X*X+Y*Y))+.5); //+.5 to round // ROFXY is ( sqrt( (X) * (X) + (Y) * (Y) ) )
		float x1=X, y1=Y;
		theta=(int)(DTHETAOFXY_DEG((double)x1,(double)y1)+.5); //+.5 to round
	}
	/*inline*/ void RConvert_RectToPolar(float &r,float &theta, float X, float Y) {
		r=(float)ROFXY(X,Y);
		float x1=X, y1=Y;
		theta=FTHETAOFXY_DEG(x1,y1);
	}

	/*inline*/ void RConvert_RectToPolar(double &r,double &theta, double X, double Y) {
		r=ROFXY(X,Y);
		double x1=X, y1=Y;
		theta=DTHETAOFXY_DEG(x1,y1);
	}
	float RConvert_ToFloat(float val) {
		return val;
	}
	float RConvert_ToFloat(double val) {
		try { return (float)val; }
		catch (exception& exn) { return (val<0.0)?float_MinValue:float_MaxValue; }
	}
	float RConvert_ToFloat(decimal val) {
		try { return (float)val; }
		catch (exception& exn) { return (val<M_0)?float_MinValue:float_MaxValue; }
	}
	float RConvert_THETAOFXY_RAD(float x, float y) {
		return THETAOFXY_RAD(x,y);
	}
	double RConvert_THETAOFXY_RAD(double x, double y) {
		return THETAOFXY_RAD(x,y);
	}
	decimal RConvert_THETAOFXY_RAD(decimal x, decimal y) {
		return THETAOFXY_RAD(((long double)x),((long double)y));//TODO: fix truncation of value where LD is less than 128-bit
	}
	float RConvert_THETAOFXY_DEG(float x, float y) {
		return FTHETAOFXY_DEG(x,y);
	}
	double RConvert_THETAOFXY_DEG(double x, double y) {
		return DTHETAOFXY_DEG(x,y);
	}
	decimal RConvert_THETAOFXY_DEG(decimal x, decimal y) {
		return LDTHETAOFXY_DEG(((long double)x),((long double)y));//TODO: fix truncation of value where LD is less than 128-bit
	}
	float RConvert_ROFXY(float x, float y) {
		return ROFXY(x,y);
	}
	double RConvert_ROFXY(double x, double y) {
		return ROFXY(x,y);
	}
	decimal RConvert_ROFXY(decimal x, decimal y) {
		return ROFXY(((long double)x),((long double)y));//TODO: fix truncation of value where LD is less than 128-bit
	}

	/*inline*/ float ANGLEDIFFPOSITIVE(float f1, float f2) { //returns 0 to 180
		SafeAngle360ByRef(f1);
		SafeAngle360ByRef(f2);
		if (f1<f2) {
			float fTemp=f1;
			f1=f2;
			f2=fTemp;
		}
		register float fDiff=fabs(f1-f2);
		if (fDiff>180.0f) fDiff=(360.0f-fDiff);
		return(fDiff);
	}
	/*inline*/ double ANGLEDIFFPOSITIVE(double f1, double f2) { //returns 0 to 180
		SafeAngle360ByRef(f1);
		SafeAngle360ByRef(f2);
		if (f1<f2) {
			double fTemp=f1;
			f1=f2;
			f2=fTemp;
		}
		register double fDiff=fabs(f1-f2);
		if (fDiff>180.0) fDiff=(360.0-fDiff);
		return(fDiff);
	}
	/*inline*/ float FANGLEDIFF(float end, float start) { //returns 0 to 180
		SafeAngle360ByRef(end);//end-=FFLOOR(end/360.0f)*360.0f;
		SafeAngle360ByRef(start);//start-=FFLOOR(start/360.0f)*360.0f;
		//if (end<start) {
		//	float fTemp=end;
		//	end=start;
		//	start=fTemp;
		//}
		//i.e. if end==0 and start==180, then end-start=-180
		register float fDiff=end-start; //fabs(end-start);
		if (fDiff>180.0f) fDiff=(360.0f-fDiff);
		else if (fDiff<-180.0f) fDiff=(360.0f+fDiff);
		return(fDiff);
	}

	/*inline*/ float FPDIST(float x1, float y1, float x2,  float y2) {
		register float xSquaring=(x1-x2);
		register float ySquaring=(y1-y2);
		register float fSumOfSquares=xSquaring*xSquaring+ySquaring*ySquaring;
		return ((fSumOfSquares>0)?sqrt(fSumOfSquares):0);
	}
	/*inline*/ double DPDIST(double x1, double y1, double x2, double y2) {
		register double xSquaring=(x1-x2);
		register double ySquaring=(y1-y2);
		register double dSumOfSquares=xSquaring*xSquaring+ySquaring*ySquaring;
		return ((dSumOfSquares>0)?sqrt(dSumOfSquares):0);
	}

	/*inline*/ long double LDANGLE360(long double d1) {
		while (d1>360.0L) d1-=360.0L;
		while (d1<-360.0L) d1+=360.0L;
		if (d1<0) d1=360.0L+d1;
		return(d1);
	}

	/*inline*/ long double LDANGLEDIFF(long double d1, long double d2) { //returns 0 to 180
		while (d1>360.0L) d1-=360.0L;
		while (d1<-360.0L) d1+=360.0L;
		while (d2>360.0L) d2-=360.0L;
		while (d2<-360.0L) d2+=360.0L;
		register long double dDiff=abs(d1-d2);
		if (dDiff>180.0L)dDiff=(360.0L-dDiff);
		return(dDiff);
	}

	/*inline*/ float DIST3D(Mass3d &pointA, Mass3d &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.X-pointA.X) + DSQUARED(pointB.Y-pointA.Y) + DSQUARED(pointB.Z-pointA.Z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	/*inline*/ float DIST3D(FPOINT3D &pointA, FPOINT3D &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.X-pointA.X) + DSQUARED(pointB.Y-pointA.Y) + DSQUARED(pointB.Z-pointA.Z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	/*inline*/ float DIST3D(FPOINT3D &pointA, Mass3d &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.X-pointA.X) + DSQUARED(pointB.Y-pointA.Y) + DSQUARED(pointB.Z-pointA.Z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	/*inline*/ float DIST3D(Mass3d &pointA, FPOINT3D &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.X-pointA.X) + DSQUARED(pointB.Y-pointA.Y) + DSQUARED(pointB.Z-pointA.Z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	/*inline*/ float DIST3D(DPOINT3D &pointA, DPOINT3D &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.X-pointA.X) + DSQUARED(pointB.Y-pointA.Y) + DSQUARED(pointB.Z-pointA.Z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	/*inline*/ float DIST3D(float x1, float y1, float z1, float x2, float y2, float z2) {
		register float fSquare;
		fSquare=DSQUARED(x2-x1) + DSQUARED(y2-y1) + DSQUARED(z2-z1);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	//bool Point3dFromPolar(Mass3d &pointToSet, float fAltitude, float fAzimuth, float fDistance) {
	//	//TODO: copy from Travel3dAbs but set instead of move
	//}
	/*inline*/ bool Travel3d(Mass3d &pointToSlide, float fPitch, float fYaw, float fDistanceTravel) {
		register float xRel,yRel,zRel;
		bool bGood=false;
		try {
			//notes:
			//--Roll, though not used here, would be done first (X rotation)
			//--setting pitch first DOES matter (?)
			//--Y-rotation (fPitch==Altitude) is on the X-Z plane (SINCE zrot of zero makes it point toward +X!!!!)
			//  --(an azimuth of zero touches this plane)
			//--Z-rotation (fYaw==Azimuth) is on the X-Y plane; (an altitude of zero touches this plane)
			yRel=0;
			xRel=FXOFRTHETA_DEG(fDistanceTravel,fPitch);
			zRel=FYOFRTHETA_DEG(fDistanceTravel,fPitch);
			if (xRel!=0.0f) { //if rotation would modify location from this view
				RMath::Rotate(xRel,yRel,fYaw);
			}
			//else do nothing since xRel and yRel are zero
			pointToSlide.X+=xRel;
			pointToSlide.Y+=yRel;
			pointToSlide.Z+=zRel;
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "Travel3d(Mass3d &pointToSlide, float fPitch, float fYaw, float fDistanceTravel)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Travel3d(Mass3d &pointToSlide, float fPitch, float fYaw, float fDistanceTravel)");
		}
		return bGood;

	}//end Travel3d(Mass3d &pointToSlide, float fPitch, float fYaw, float fDistanceTravel)
	/*inline*/ bool Travel3dAbs(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravel) {
		return Travel3d(pointToSlide,pointDest,fDistanceTravel/DIST3D(pointToSlide,pointDest));
	}
	/*inline*/ bool Travel3d(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravelRatio) {
		bool bGood=false;
		try {
			pointToSlide.X=APPROACH(pointToSlide.X,pointDest.X,fDistanceTravelRatio);
			pointToSlide.Y=APPROACH(pointToSlide.Y,pointDest.Y,fDistanceTravelRatio);
			pointToSlide.Z=APPROACH(pointToSlide.Z,pointDest.Z,fDistanceTravelRatio);
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "Travel3d(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravelRatio)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Travel3d(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravelRatio)");
		}
		return bGood;
	}//end Travel3d(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravelRatio)
	/*inline*/ bool Travel3d(float &xToMove, float &yToMove, float &zToMove, float fPitch, float fYaw, float fDistanceTravel) {
		bool bGood=false;
		register float xRel,yRel,zRel;
		try {
			yRel=0.0f;
			xRel=FXOFRTHETA_DEG(fDistanceTravel,fPitch);
			zRel=FYOFRTHETA_DEG(fDistanceTravel,fPitch);
			if (xRel!=0.0f) { //if rotation would modify location from this view
				RMath::Rotate(xRel,yRel,fYaw);
			}
			//else do nothing since xRel and yRel are zero
			xToMove+=xRel;
			yToMove+=yRel;
			zToMove+=zRel;
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Travel3d(float &xToMove, float &yToMove, float &zToMove, float fPitch, float fYaw, float fDistanceTravel)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Travel3d(float &xToMove, float &yToMove, float &zToMove, float fPitch, float fYaw, float fDistanceTravel)");
		}
		return bGood;
	}//end Travel3d(float &xToMove, float &yToMove, float &zToMove, float fPitch, float fYaw, float fDistanceTravel)
	/*inline*/ bool Travel3d(double &xToMove, double &yToMove, double &zToMove, double fPitch, double fYaw, double fDistanceTravel) {
		bool bGood=false;
		register double xRel,yRel,zRel;
		try {
			yRel=0.0d;
			xRel=DXOFRTHETA_DEG(fDistanceTravel,fPitch);
			zRel=DYOFRTHETA_DEG(fDistanceTravel,fPitch);
			if (xRel!=0.0d) { //if rotation would modify location from this view
				RMath::Rotate(xRel,yRel,fYaw);
			}
			//else do nothing since xRel and yRel are zero
			xToMove+=xRel;
			yToMove+=yRel;
			zToMove+=zRel;
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Travel3d(double &xToMove, double &yToMove, double &zToMove, double fPitch, double fYaw, double fDistanceTravel)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Travel3d(double &xToMove, double &yToMove, double &zToMove, double fPitch, double fYaw, double fDistanceTravel)");
		}
		return bGood;
	}//end Travel3d(double &xToMove, double &yToMove, double &zToMove, double fPitch, double fYaw, double fDistanceTravel)
	/*inline*/ float FSQUARED(float val) {
		return val*val;
	}
	/*inline*/ float DSQUARED(double val) {
		return val*val;
	}

	///*inline*/ int SafePow(int basenum, int exp) {
	//	int result;
	//	bool bNeg;
	//	if (exp<0) {
	//		bNeg=true;
	//		exp*=-1;
	//	}
	//	if (exp==0) return 1;
	//	else {
	//		bNeg=false;
	//		result=basenum;
	//		for (int iCount=1; iCount<exp; iCount++) {
	//			if (result<INT_MAX-basenum) result*=basenum;
	//			else return INT_MAX;
	//		}
	//	}
	//	if (bNeg) {
	//		result=1/result;
	//		exp*=-1;//leaves it the way we found it
	//	}
	//	return result;
	//}
	///*inline*/ long SafePow(long basenum, int exp) {
	//	long result;
	//	bool bNeg;
	//	if (exp<0) {
	//		bNeg=true;
	//		exp*=-1;
	//	}
	//	if (exp==0) return 1;
	//	else {
	//		bNeg=false;
	//		result=basenum;
	//		for (int iCount=1; iCount<exp; iCount++) {
	//			if (result<LONG_MAX-basenum) result*=basenum;
	//			else return LONG_MAX;
	//		}
	//	}
	//	if (bNeg) {
	//		result=1L/result;
	//		exp*=-1;//leaves it the way we found it
	//	}
	//	return result;
	//}
	///*inline*/ float SafePow(float basenum, int exp) {
	//	float result;
	//	bool bNeg;
	//	if (exp<0) {
	//		bNeg=true;
	//		exp*=-1;
	//	}
	//	if (exp==0) return 1;
	//	else {
	//		bNeg=false;
	//		result=basenum;
	//		for (int iCount=1; iCount<exp; iCount++) {
	//			if (result<FLT_MAX-basenum) result*=basenum;
	//			else return FLT_MAX;
	//		}
	//	}
	//	if (bNeg) {
	//		result=1.0f/result;
	//		exp*=-1;//leaves it the way we found it
	//	}
	//	return result;
	//}
	///*inline*/ double SafePow(double basenum, int exp) {
	//	double result;
	//	bool bNeg;
	//	if (exp<0) {
	//		bNeg=true;
	//		exp*=-1;
	//	}
	//	if (exp==0) return 1;
	//	else {
	//		bNeg=false;
	//		result=basenum;
	//		for (int iCount=1; iCount<exp; iCount++) {
	//			if (result<DBL_MAX-basenum) result*=basenum;
	//			else return DBL_MAX;
	//		}
	//	}
	//	if (bNeg) {
	//		result=1.0/result;
	//		exp*=-1; //leaves it the way we found it
	//	}
	//	return result;
	//}
	///*inline*/ int SafeE10I(int exp) {
	//	return SafePow(i10, exp);
	//}
	///*inline*/ long SafeE10L(int exp) {
	//	return SafePow(l10, exp);
	//}
	///*inline*/ float SafeE10F(int exp) {
	//	return SafePow(f10, exp);
	//}
	///*inline*/ double SafeE10D(int exp) {
	//	return SafePow(d10, exp);
	//}
	/*inline*/ byte SafeByRoundF(float val) {
		val+=.5f;
		if (val<0.0f) return 0;
		else if (val>255.0f) return 255;
		else return (byte)val;
	}
	/*inline*/ byte SafeByte(float val) {
		return (val>255.0f)?255:((val<0.0f)?0:(byte)val);
	}
	const int INT_MAX_TIMES_NEG1=INT_MAX*-1;
	/*inline*/ int SafeAddWrappedPositiveOnly(int val1, int val2) { //formerly SafeAddWrapped
		///REMEMBER: the absolute value of MIN_INT (0x80000000==-2147483648) is 1 GREATER THAN MAX_INT (0x7fffffff (2147483647))
		if (val1<0) {
			ShowErr("WARNING: Caller attempted SafeAddWrappedPositiveOnly using a negative operand 1 (will be changed to absolute value)");
			if (val1<INT_MAX_TIMES_NEG1) val1=(val1+INT_MAX)*-1;//do solo wrap before wrapped add, starting at zero using the range of the positive
			else val1*=-1;
		}
		if (val2<0) {
			ShowErr("WARNING: Caller attempted SafeAddWrappedPositiveOnly using a negative operand 2 (will be changed to absolute value)");
			if (val2<INT_MAX_TIMES_NEG1) val2=(val2+INT_MAX)*-1;//do solo wrap before wrapped add, starting at zero using the range of the positive
			else val2*=-1;
		}
		static const int max=INT_MAX;//2147483647 //this var exists only to ease overloading this method
		int val3=0;
		int roomleft=max-val1;
		if (val2<=roomleft) val3=val1+val2; //roomleft<val2
		else val3=val2-(max-val1); //e.g. on a scale of 5, "4+3 == 3-(5-4) == 2"
		//else val3=val2-roomleft+val1;///NOTE: won't work this is an UNSIGNED overload //=val2-roomleft;
		if (val3<0) {
			ShowErr("Programmer error: SafeAddWrappedPositiveOnly returning a negative");
		}
		return val3;
	}
	float MetersToMoveThisManyMS(float fMetersPerSecondX, int iForThisManyMilliseconds) {
		return (float)(  (double)fMetersPerSecondX * ((double)(iForThisManyMilliseconds)) / 1000.0  );
	}
	double MetersToMoveThisManyMS(double fMetersPerSecondX, int iForThisManyMilliseconds) {
		return (  (double)fMetersPerSecondX * ((double)(iForThisManyMilliseconds)) / 1000.0  );
	}
	float DegreesToMoveThisManyMS(float fDegreesPerSecondX, int iForThisManyMilliseconds) {//TODO: finish this--check this asdf
		return (float)(  (double)fDegreesPerSecondX * ((double)(iForThisManyMilliseconds)) / 1000.0  );
	}
	double DegreesToMoveThisManyMS(double fDegreesPerSecondX, int iForThisManyMilliseconds) {//TODO: finish this--check this asdf
		return (  (double)fDegreesPerSecondX * ((double)(iForThisManyMilliseconds)) / 1000.0  );
	}
	float MetersToMoveThisManyS(float fMetersPerSecondX, float rSeconds) {
		return fMetersPerSecondX*rSeconds;
	}
	double MetersToMoveThisManyS(double fMetersPerSecondX, double rSeconds) {
		return fMetersPerSecondX*rSeconds;
	}
	float DegreesToMoveThisManyS(float fDegreesPerSecondX, float rSeconds) {
		return fDegreesPerSecondX*rSeconds;
	}
	double DegreesToMoveThisManyS(double fDegreesPerSecondX, double rSeconds) {
		return fDegreesPerSecondX*rSeconds;
	}
	///#endregion math

	///#region file operations
	int FileSize(const char* szFile) {
		static bool bFirstRun=true;
		if (bFirstRun) Console::Error.Write("FileSize--");
		int iBytes=0;
		std::ifstream ifData;
		string sOpNoun="start";
		try {
			sOpNoun="open";
			if (bFirstRun) Console::Error.Write(sOpNoun+",");
			ifData.open(szFile, std::ios_base::binary | std::ios_base::in);
			sOpNoun="status";
			if (bFirstRun) Console::Error.Write(sOpNoun+",");
			if (!ifData.good() || ifData.eof() || !ifData.is_open()) {
				if (bFirstRun) Console::Error.WriteLine("found zero bytes...");
				return 0;
			}
			sOpNoun="init";
			if (bFirstRun) Console::Error.Write(sOpNoun+",");
			ifData.seekg(0, std::ios_base::beg);
			sOpNoun="traverse";
			if (bFirstRun) Console::Error.Write(sOpNoun+",");
			std::ifstream::pos_type posNow = ifData.tellg();
			sOpNoun="seek";
			if (bFirstRun) Console::Error.Write(sOpNoun+",");
			ifData.seekg(0, std::ios_base::end);
			sOpNoun="convert";
			if (bFirstRun) Console::Error.Write(sOpNoun+",");
			iBytes=static_cast<int>(ifData.tellg() - posNow);
			//thanks http://www.codeproject.com/file/filesize.asp
		}
		catch (exception& exn) {
			ShowExn(exn,"ExpertMultimediaBase::FileSize");
		}
		catch (...) {
			ShowUnknownExn("ExpertMultimediaBase::FileSize",sOpNoun);
		}
		try {
			sOpNoun="close...";
			if (bFirstRun) Console::Error.Write(sOpNoun);
			if (ifData!=NULL) ifData.close();
		}
		catch (exception& exn) {
			ShowExn(exn,"ExpertMultimediaBase::FileSize");
		}
		catch (...) {
			ShowUnknownExn("ExpertMultimediaBase::FileSize",sOpNoun);
		}
		sOpNoun="done ("+RString_ToString(iBytes)+" bytes)...";
		if (bFirstRun) Console::Error.Write(sOpNoun);
		bFirstRun=false;
		return iBytes;
	}//end FileSize(char* szFile)
	int FileSize(string sFile) {
		int iBytes=0;
		//int iBlockSize=1024;
		//int iCount=10;
		//byte arrbyTest[10240];
		//int iLastRead=1;//1 is unused but required initial value
		//FILE * pFile;
		try {
			iBytes=FileSize(sFile.c_str());
			//thanks http://www.codeproject.com/file/filesize.asp
		}
		catch (exception& exn) {
			ShowExn(exn,"ExpertMultimediaBase::FileSize");
		}
		catch (...) {
			ShowUnknownExn("ExpertMultimediaBase::FileSize string overload");
		}
		return iBytes;
	}
	///#endregion file operations
}//end namespace
#endif
