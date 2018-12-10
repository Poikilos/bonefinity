#ifndef BASE_CPP
#define BASE_CPP

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <string>
#include <base.h>
//#include "C:\My Documents\Projects-cpp\Base\base.h"

using namespace std;

namespace ExpertMultimediaBase {
   	bool CopySafe(byte* byarrDest, byte* byarrSrc, int iDestStart, int iSrcStart, int iBytes) {
		bool bGood=true;
		try {
			memcpy(&byarrDest[iDestStart],&byarrSrc[iSrcStart],iBytes);
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"CopySafe");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("CopySafe");
		}
		return bGood;
	}
	byte NibbleValueOfHexChar(char cUppercaseLetter) {
		byte byReturn=0;
		for (byte iNow=0; iNow<16; iNow++) {
			if (cUppercaseLetter==carrHex[(int)iNow]) {
				byReturn=iNow;
				break;
			}
		}
		return byReturn;
	}
    byte ByteFromHexChars(string sHexPair) {
		byte byReturn=0;
		const char* carrNow=sHexPair.c_str();
		byReturn=NibbleValueOfHexChar(carrNow[0]);
		byReturn+=(byte)16*NibbleValueOfHexChar(carrNow[1]);
		return byReturn;
	}
	char LowercaseElseZeroIfNotUppercase(char cNow) {
		char cReturn=(char)0;
		for (int iNow=0; iNow<26; iNow++) {
			if (cNow==carrAlphabetUpper[iNow]) {
				cReturn=carrAlphabetLower[iNow];
				break;
			}
		}
		return cReturn;
	}//LowercaseElseZeroIfNotUppercase
	string ToUpper(string val) {
		const char* carrNow=val.c_str();
		char* carrTemp=(char*)malloc(sizeof(char)*val.length());
		for (int iNow=0; iNow<val.length(); iNow++) {
			//TODO: fix SLOWness
			char cNow=LowercaseElseZeroIfNotUppercase(carrNow[iNow]);
			if ((int)cNow>0) carrTemp[iNow]=cNow;
			else carrTemp[iNow]=carrNow[iNow];
		}
		val.assign(carrTemp);
		SafeFree(carrTemp);
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
	//TODO uncomment: #region inline functions
	float MirrorOnYLine(float fAngle) {
		SafeAngle360ByRef(fAngle);
		if (fAngle<90.0f) fAngle+=180.0f-fAngle;
		else if (fAngle<180.0f) fAngle=180.0f-fAngle;
		else if (fAngle<270.0f) fAngle=360.0f-(fAngle-180.0f);
		else fAngle=360.0f-(fAngle-180.0f);
	}
	float AngleToward(float xDest, float yDest, float xSrc, float ySrc) {
		register float xRel=xDest-xSrc, yRel=yDest-xSrc;
		float fReturn=THETAOFXY(xRel,yRel);
		if (fReturn<0.0f) fReturn+=360.0f;
		return fReturn;
	}
	int FileSize(const char* carrFile) {
		static bool bFirstRun=true;
		if (bFirstRun) Console.Write("FileSize--");
		int iBytes=0;
		std::ifstream ifData;
		string sOpNoun="start";
		try {
			sOpNoun="open";
			if (bFirstRun) Console.Write(sOpNoun+",");
			ifData.open(carrFile, std::ios_base::binary | std::ios_base::in);
			sOpNoun="status";
			if (bFirstRun) Console.Write(sOpNoun+",");
			if (!ifData.good() || ifData.eof() || !ifData.is_open()) {
				if (bFirstRun) Console.WriteLine("found zero bytes...");
				return 0;
			}
			sOpNoun="init";
			if (bFirstRun) Console.Write(sOpNoun+",");
			ifData.seekg(0, std::ios_base::beg);
			sOpNoun="traverse";
			if (bFirstRun) Console.Write(sOpNoun+",");
			std::ifstream::pos_type posNow = ifData.tellg();
			sOpNoun="seek";
			if (bFirstRun) Console.Write(sOpNoun+",");
			ifData.seekg(0, std::ios_base::end);
			sOpNoun="convert";
			if (bFirstRun) Console.Write(sOpNoun+",");
			iBytes=static_cast<int>(ifData.tellg() - posNow);
			//thanks http://www.codeproject.com/file/filesize.asp
		}
		catch (exception& exn) {
			ShowException(exn,"ExpertMultimediaBase::FileSize");
		}
		catch (...) {
			ShowUnknownException("ExpertMultimediaBase::FileSize",sOpNoun);
		}
		try {
	  		sOpNoun="close...";
			if (bFirstRun) Console.Write(sOpNoun);
			if (ifData!=NULL) ifData.close();
		}
		catch (exception& exn) {
			ShowException(exn,"ExpertMultimediaBase::FileSize");
		}
		catch (...) {
			ShowUnknownException("ExpertMultimediaBase::FileSize",sOpNoun);
		}
	 	sOpNoun="done ("+ToString(iBytes)+" bytes)...";
		if (bFirstRun) Console.Write(sOpNoun);
	 	bFirstRun=false;
		return iBytes;
	}//end inline int FileSize(char* carrFile)
	int FileSize(string sFile) {
		int iBytes=0;
		//int iBlockSize=1024;
		//int iCount=10;
		//byte byarrTest[10240];
		//int iLastRead=1;//1 is unused but required initial value
		//FILE * pFile;
		try {
			iBytes=FileSize(sFile.c_str());
			//thanks http://www.codeproject.com/file/filesize.asp
		}
		catch (exception& exn) {
			ShowException(exn,"ExpertMultimediaBase::FileSize");
		}
		catch (...) {
			ShowUnknownException("ExpertMultimediaBase::FileSize string overload");
		}
		return iBytes;
	}
	inline void Crop(float &fToModify, float fMin, float fMax) {
		if (fToModify>fMax) fToModify=fMax;
		else if (fToModify<fMin) fToModify=fMin;
	}
	inline float SafeAngle360(float fToLimitBetweenZeroAnd360) {
		SafeAngle360ByRef(fToLimitBetweenZeroAnd360);
		return fToLimitBetweenZeroAnd360;
	}
	inline void SafeAngle360ByRef(float &fToLimitBetweenZeroAnd360) {
		//float fAngleTest=fToLimitBetweenZeroAnd360;//debug only
		fToLimitBetweenZeroAnd360-=( FFLOOR(fToLimitBetweenZeroAnd360/360.0f) * 360.0f );
		if (fToLimitBetweenZeroAnd360<0.0f) fToLimitBetweenZeroAnd360+=360.0f;
		//static int iTest=0;//debug only
		//if (iTest<1000) {
		//	Console.WriteLine("SafeAngle360 of "+ToString(fAngleTest)+" is "+ToString(fToLimitBetweenZeroAnd360)+".");
		//}
		//iTest++;
	}
	inline LPIPOINT IPOINTFROM(float xNow, float yNow) {
		LPIPOINT lpiPoint=NULL;
		try {
			lpiPoint=(LPIPOINT)malloc(sizeof(IPOINT));
			lpiPoint->x=(int)xNow;
			lpiPoint->y=(int)yNow;
		}
		catch (exception& exn) {
			lpiPoint=NULL;
			ShowException(exn,"ExpertMultimediaBase::IPOINTFROM");
		}
		catch (...) {
			lpiPoint=NULL;
			ShowUnknownException("ExpertMultimediaBase::IPOINTFROM");
		}
		return lpiPoint;
	}
	inline void FPOLAROFRECT(float &r,float &theta, float x, float y) {
		//uses double since trig functions are double
		r=(float)ROFXY(x,y);
		float x1=x, y1=y;
		theta=DTHETAOFXY(x1,y1);
	}

	inline void DPOLAROFRECT(double &r,double &theta, double x, double y) {
		r=ROFXY(x,y);
		double x1=x, y1=y;
		theta=DTHETAOFXY(x1,y1);
	}


	//BYTEOF360 intentionally divides by 256 to allow storage of 256 degree variations!!!


	inline float FANGLEDIFFPOSITIVE(float f1, float f2) { //returns 0 to 180
		SafeAngle360ByRef(f1);//f1-=FFLOOR(f1/360.0f)*360.0f;
		SafeAngle360ByRef(f2);//f2-=FFLOOR(f2/360.0f)*360.0f;
		if (f1<f2) {
			float fTemp=f1;
			f1=f2;
			f2=fTemp;
		}
		register float fDiff=fabs(f1-f2);
		if (fDiff>180.0f) fDiff=(360.0f-fDiff);
		return(fDiff);
	}
	inline float FANGLEDIFF(float f1, float f2) { //returns 0 to 180
		SafeAngle360ByRef(f1);//f1-=FFLOOR(f1/360.0f)*360.0f;
		SafeAngle360ByRef(f2);//f2-=FFLOOR(f2/360.0f)*360.0f;
		if (f1<f2) {
			float fTemp=f1;
			f1=f2;
			f2=fTemp;
		}
		register float fDiff=fabs(f1-f2);
		if (fDiff>180.0f) fDiff=(360.0f-fDiff);
		return(fDiff);
	}

	inline float FDist(FPoint &point1, FPoint &point2) {
		register float xSquaring=(point1.x-point2.x);
		register float ySquaring=(point1.y-point2.y);
		register float fSumOfSquares=xSquaring*xSquaring+ySquaring*ySquaring;
		return ((fSumOfSquares>0)?sqrt(fSumOfSquares):0);
	}
	inline float FPDIST(float x1, float y1, float x2,  float y2) {
		register float xSquaring=(x1-x2);
		register float ySquaring=(y1-y2);
		register float fSumOfSquares=xSquaring*xSquaring+ySquaring*ySquaring;
		return ((fSumOfSquares>0)?sqrt(fSumOfSquares):0);
	}
	double DDist(DPoint &point1, DPoint &point2) {
		register double xSquaring=(point1.x-point2.x);
		register double ySquaring=(point1.y-point2.y);
		register double dSumOfSquares=xSquaring*xSquaring+ySquaring*ySquaring;
		return ((dSumOfSquares>0)?sqrt(dSumOfSquares):0);
	}
	inline double DPDIST(double x1, double y1, double x2, double y2) {
		register double xSquaring=(x1-x2);
		register double ySquaring=(y1-y2);
		register double dSumOfSquares=xSquaring*xSquaring+ySquaring*ySquaring;
		return ((dSumOfSquares>0)?sqrt(dSumOfSquares):0);
	}

	inline long double LDANGLE360(long double d1) {
		while (d1>360.0L) d1-=360.0L;
		while (d1<-360.0L) d1+=360.0L;
		if (d1<0) d1=360.0L+d1;
		return(d1);
	}

	inline long double LDANGLEDIFF(long double d1, long double d2) { //returns 0 to 180
		while (d1>360.0L) d1-=360.0L;
		while (d1<-360.0L) d1+=360.0L;
		while (d2>360.0L) d2-=360.0L;
		while (d2<-360.0L) d2+=360.0L;
	 	register long double dDiff=abs(d1-d2);
		if (dDiff>180.0L)dDiff=(360.0L-dDiff);
		return(dDiff);
	}

	inline float DIST3D(Mass3d &pointA, Mass3d &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.x-pointA.x) + DSQUARED(pointB.y-pointA.y) + DSQUARED(pointB.z-pointA.z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	inline float DIST3D(FPOINT3D &pointA, FPOINT3D &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.x-pointA.x) + DSQUARED(pointB.y-pointA.y) + DSQUARED(pointB.z-pointA.z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	inline float DIST3D(FPOINT3D &pointA, Mass3d &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.x-pointA.x) + DSQUARED(pointB.y-pointA.y) + DSQUARED(pointB.z-pointA.z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	inline float DIST3D(Mass3d &pointA, FPOINT3D &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.x-pointA.x) + DSQUARED(pointB.y-pointA.y) + DSQUARED(pointB.z-pointA.z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	inline float DIST3D(DPOINT3D &pointA, DPOINT3D &pointB) {
		register float fSquare;
		fSquare=DSQUARED(pointB.x-pointA.x) + DSQUARED(pointB.y-pointA.y) + DSQUARED(pointB.z-pointA.z);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	inline float DIST3D(float x1, float y1, float z1, float x2, float y2, float z2) {
		register float fSquare;
		fSquare=DSQUARED(x2-x1) + DSQUARED(y2-y1) + DSQUARED(z2-z1);
		return (fSquare>0)?sqrt(fSquare):0;
	}
	//inline bool Point3dFromPolar(Mass3d &pointToSet, float fAltitude, float fAzimuth, float fDistance) {
	//	//TODO: copy from Travel3dAbs but set instead of move
	//}
	inline bool Travel3d(Mass3d &pointToSlide, float fPitch, float fYaw, float fDistanceTravel) {
		register float xRel,yRel,zRel;
		//notes:
		//--Roll, though not used here, would be done first (x rotation)
		//--setting pitch first DOES matter (?)
		//--y-rotation (fPitch==Altitude) is on the x-z plane (SINCE zrot of zero makes it point toward +x!!!!)
		//  --(an azimuth of zero touches this plane)
		//--z-rotation (fYaw==Azimuth) is on the x-y plane; (an altitude of zero touches this plane)
		yRel=0;
		xRel=FXOFRTHETA(fDistanceTravel,fPitch);
		zRel=FYOFRTHETA(fDistanceTravel,fPitch);
		if (xRel!=0.0f) { //if rotation would modify location from this view
		    Rotate(xRel,yRel,fYaw);
		}
		//else do nothing since xRel and yRel are zero
		pointToSlide.x+=xRel;
		pointToSlide.y+=yRel;
		pointToSlide.z+=zRel;
	}
	inline bool Travel3dAbs(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravel) {
		return Travel3d(pointToSlide,pointDest,fDistanceTravel/DIST3D(pointToSlide,pointDest));
	}
	inline bool Travel3d(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravelRatio) {
		pointToSlide.x=APPROACH(pointToSlide.x,pointDest.x,fDistanceTravelRatio);
		pointToSlide.y=APPROACH(pointToSlide.y,pointDest.y,fDistanceTravelRatio);
		pointToSlide.z=APPROACH(pointToSlide.z,pointDest.z,fDistanceTravelRatio);
	}
	inline bool Travel3d(float &xToMove, float &yToMove, float &zToMove, float fPitch, float fYaw, float fDistanceTravel) {
		register float xRel,yRel,zRel;
		yRel=0;
		xRel=FXOFRTHETA(fDistanceTravel,fPitch);
		zRel=FYOFRTHETA(fDistanceTravel,fPitch);
		if (xRel!=0.0f) { //if rotation would modify location from this view
		    Rotate(xRel,yRel,fYaw);
		}
		//else do nothing since xRel and yRel are zero
		xToMove+=xRel;
		yToMove+=yRel;
		zToMove+=zRel;
	}
	/*
	class IPoint {
		int x;
		int y;
		IPoint();
	};
	IPoint::IPoint() {
		x=0;
		y=0;
	}
	class IRect() {
		int top;
		int left;
		int bottom;
		int right;
		IRect();
	};
	IRect::IRect() {
		top=0;
		left=0;
		bottom=0;
		right=0;
	}
	*/
	//void strcatfromsiverrno(char*, int);
	//void strcpyfromsiverrno(char*, int);



	inline void Rotate(float &xToMove, float &yToMove, float xCenter, float yCenter, float fRotate) {
		xToMove-=xCenter;
		yToMove-=yCenter;
		float rTemp=ROFXY(xToMove,yToMove), thetaTemp=THETAOFXY(xToMove,yToMove);
		thetaTemp+=fRotate;
		xToMove=XOFRTHETA(rTemp,thetaTemp);
		yToMove=YOFRTHETA(rTemp,thetaTemp);
		xToMove+=xCenter;
		yToMove+=yCenter;
	}
	void Rotate(float &xToMove, float &yToMove, float fRotate) {
		float rTemp=ROFXY(xToMove,yToMove), thetaTemp=THETAOFXY(xToMove,yToMove);
		thetaTemp+=fRotate;
		xToMove=XOFRTHETA(rTemp,thetaTemp);
		yToMove=YOFRTHETA(rTemp,thetaTemp);
	}
	//TODO uncomment: #endregion
	
	//TODO uncomment: #region error functions
	bool ShowErr() {
		return ShowError();
	}
	bool ShowError() {
		bool bPermission=false;
		if (iErrors<iMaxErrors) {
			bPermission=true;
		}
		else if (iErrors==iMaxErrors) {
			Console.Error.WriteLine("Too many errors ("+ToString(iErrors)+")--this is the last one that will be shown:");
			bPermission=true;
		}
		iErrors++;
	}
	void ShowAndDeleteException(char* &sExn, string sFuncNow) {
		ShowAndDeleteException(sExn,sFuncNow,"");
	}
	void ShowAndDeleteException(char* &sExn, string sFuncNow, string sVerbNow) {
		if (sVerbNow.length()>0) sVerbNow=" "+sVerbNow;
		if (ShowErr()) cerr<<"Exception in "<<sFuncNow<<sVerbNow<<": "<<sExn<<endl;
		SafeFree(sExn);
	}
	void ShowException(exception& exn, string sFuncNow) {
		ShowException(exn,sFuncNow,"");
	}
	void ShowException(exception& exn, string sFuncNow, string sVerbNow) {
		if (sVerbNow.length()>0) sVerbNow=" "+sVerbNow;
		if (ShowErr()) cerr<<"Exception in "<<sFuncNow<<sVerbNow<<": "<<exn.what();
	}
	void ShowUnknownException(string sFuncNow, string sVerbNow) {
		if (sVerbNow.length()>0) sVerbNow=" "+sVerbNow;
		if (ShowErr()) cerr<<"Unknown Exception in "<<sFuncNow<<sVerbNow<<"."<<endl;
	}
	void ShowUnknownException(string sFuncNow) {
		ShowUnknownException(sFuncNow,"");
	}
	void ShowError(string sMsg, string sCurrentFunction) {
		if (ShowError()) cerr<<sCurrentFunction<<": "<<sMsg<<endl;
	}
	void ShowError(int iSivErrNo, string sMsg, string sCurrentFunction) {
		char sTemp[2048];//=(char*)malloc(10240*sizeof(char));
		if (iSivErrNo!=0) {
			if (iErrors<iMaxErrors) {
				try {
	    			sTemp[0]='\0';
					strcatfromsiverrno(sTemp, iSivErrNo);
					cerr<<"In "<<sCurrentFunction<<" -- "<<sTemp<<": "<<sMsg<<endl;
				}
				catch (exception& exn) { cerr<<"ShowError (self-exception)"<<exn.what()<<endl; }
				catch (...) { cerr<<"Default self-exception during exception output."<<endl; }
			}
			else if (iErrors==iMaxErrors) { //if > do nothing!
		        cerr<< "Too many errors ("<<iErrors<<")--this is the last that will be shown: \n\t"<<"In "<<sCurrentFunction<<" -- "<<sTemp<<": "<<sMsg<<endl;
			}
			iErrors++;
		}//end if iSivErrNo!=0
	}//end ShowError
	void ShowError(int iSivErrNo, string sCurrentFunction) {
		if (ShowError()) {
			char sTemp[2048];
			strcpyfromsiverrno(sTemp,iSivErrNo);
			cerr<<sCurrentFunction<<": "<<sTemp<<endl;
		}
	}
	void ShowError(string sMsg) {
		if (iErrors<iMaxErrors) {
			try {
				cerr<<sMsg<<endl;
			}
			catch (exception& exn) { cerr<<"ShowError (self-exception)"<<exn.what()<<endl; }
			catch (...) { cerr<<"Default self-exception during exception output."<<endl; }
		}
		else if (iErrors==iMaxErrors) { //if > do nothing!
	        cerr<< "Too many errors ("<<iErrors<<")--this is the last that will be shown: \n\t" <<sMsg<<endl;
		}
		iErrors++;
	}//end ShowError
	void FakeException(string sExn) { //TODO: remove this and all calls to it
		if (iErrors<iMaxErrors) {
			try {
				cerr<<"Exception error: "<<sExn<<endl;
			}
			catch (exception& exn) { cerr<<"FakeException (self-exception)"<<exn.what()<<endl; }
			catch (...) { cerr<<"Default self-exception during exception output."<<endl; }
		}
		else if (iErrors==iMaxErrors) { //if > do nothing!
	        cerr<< "Too many errors ("<<iErrors<<")--this is the last that will be shown: \n\t" << sExn << endl;
		}
		iErrors++;
	}//end FakeException
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
	string ToString(int val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console.Write("ToString(int)--");
		string sReturn="";
		char sTemp[2048];
		strcpyfrom(sTemp,val);
		sReturn.assign(sTemp);
		if (bFirstRun) Console.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string ToString(long val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console.Write("ToString(long)--");
		string sReturn="";
		char sTemp[2048];
		if (bFirstRun) Console.Write("copy...");
		strcpyfrom(sTemp,val);
		if (bFirstRun) Console.Write("assign...");
		sReturn.assign(sTemp);
		if (bFirstRun) Console.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string ToString(Uint32 val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console.Write("ToString(Uint32)--");
		string sReturn="";
		char sTemp[2048];
		if (bFirstRun) Console.Write("copy...");
		strcpyfrom(sTemp,val);
		if (bFirstRun) Console.Write("assign...");
		sReturn.assign(sTemp);
		if (bFirstRun) Console.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string ToString(float val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console.Write("ToString(float)--");
		string sReturn="";
		char sTemp[2048];
		if (bFirstRun) Console.Write("copy...");
		strcpyfrom(sTemp,(double)val);
		if (bFirstRun) Console.Write("assign...");
		sReturn.assign(sTemp);
		if (bFirstRun) Console.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string ToString(double val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console.Write("ToString(double)--");
		string sReturn="";
		char sTemp[2048];
		if (bFirstRun) Console.Write("copy...");
		strcpyfrom(sTemp,val);
		if (bFirstRun) Console.Write("assign...");
		sReturn.assign(sTemp);
		if (bFirstRun) Console.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string ToString(string val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console.Write("ToString(string)--");
		if (bFirstRun) Console.Write("done...");
		bFirstRun=false;
		return val;
	}
	string ToString(char* val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console.Write("ToString(char*)--");
		string sReturn="";
		sReturn.assign(val);
		if (bFirstRun) Console.Write("done...");
		bFirstRun=false;
		return sReturn;
	}
	string ToString(bool val) {
		static bool bFirstRun=false;//false to prevent crap from showing inside other debugs
		if (bFirstRun) Console.Write("ToString(bool)--");
		if (bFirstRun) Console.Write("done...");
		bFirstRun=false;
		return val?"true":"false";
	}
	//TODO uncomment: #endregion

	//TODO uncomment: #region simple classes
	IPoint::IPoint() {
		x=0;
		y=0;
	}
	string IPoint::ToString() {
		return "("+ExpertMultimediaBase::ToString(x)+","+ExpertMultimediaBase::ToString(y)+")";
	}

	FPoint::FPoint() {
		x=0;
		y=0;
	}
	string FPoint::ToString() {
		return "("+ExpertMultimediaBase::ToString(x)+","+ExpertMultimediaBase::ToString(y)+")";
	}

	DPoint::DPoint() {
		x=0;
		y=0;
	}
	string DPoint::ToString() {
		return "("+ExpertMultimediaBase::ToString(x)+","+ExpertMultimediaBase::ToString(y)+")";
	}

	IRect::IRect() {
		top=0;
		left=0;
		bottom=0;
		right=0;
	}
	string IRect::ToString() {
		return "("+ExpertMultimediaBase::ToString(left)+","+ExpertMultimediaBase::ToString(top)+")to("+ExpertMultimediaBase::ToString(right)+","+ExpertMultimediaBase::ToString(bottom)+")";
	}
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
	void Pixel::Set(Uint32& dwPixel) {
		byte* byPixel=(byte*)&dwPixel;
		b=byPixel[0]; //dwPixel>>3;
		g=byPixel[1]; //(dwPixel>>2)&&0x000000ff;
		r=byPixel[2]; //(dwPixel>>1)&&0x000000ff;
		a=byPixel[3]; //(dwPixel)&&0x000000ff;
	}
	//void Pixel::Set(Uint32* dwPixel) {
	//	byte* byPixel=(byte*)dwPixel;
	//	b=byPixel[0]; //dwPixel>>3;
	//	g=byPixel[1]; //(dwPixel>>2)&&0x000000ff;
	//	r=byPixel[2]; //(dwPixel>>1)&&0x000000ff;
	//	a=byPixel[3]; //(dwPixel)&&0x000000ff;
	//}
	//TODO uncomment: #endregion simple classes
	
	//TODO uncomment: #region scripting
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
		        sVal=ToString(val);
		}
		catch (exception& exn) { ShowException(exn,"Variable::Set(int)");
		}
		catch (...) { ShowUnknownException("Variable::Set(int)","value=\""+ToString(val)+"\"");
		}
	}
	void Variable::Set(float val) {
		try { SetActive();
			sVal=ToString(val); }
		catch (exception& exn) { ShowException(exn,"Variable::Set(float)");
		}
		catch (...) { ShowUnknownException("Variable::Set(float)","value=\""+ToString(val)+"\"");
		}
	}
	void Variable::Set(string val) {
		try {
	        SetActive();
			sVal=val;
		}
		catch (exception& exn) { ShowException(exn,"Variable::Set(string)");
		}
		catch (...) { ShowUnknownException("Variable::Set(string)","value=\""+val+"\"");
		}
	}
	bool Variable::Get(int &val) {
		bool bGood=false;
		try {
			if (bActive) {
	            //stringstream ssboth(s3, ios_base::in | ios_base::out);
	            //ssboth<<sVal;
	            //ssboth>>val;
	            char sTemp[2048];
				val=atoi(sVal.c_str());
				bGood=true;
			}
			else bGood=false;
		}
		catch (exception& exn) { ShowException(exn,"Variable::Get(int)");
		}
		catch (...) { ShowUnknownException("Variable::Get(int)");
		}
		return bGood;
	}
	bool Variable::Get(float &val) {
		bool bGood=false;
		try {
			if (bActive) { val=atof(sVal.c_str()); bGood=true; }
			else bGood=false;
		}
		catch (exception& exn) { ShowException(exn,"Variable::Get(float)");
		}
		catch (...) { ShowUnknownException("Variable::Get(float)");
		}
		return bGood;
	}
	bool Variable::Get(string &val) {
		bool bGood=false;
		try {
			if (bActive) { val=sVal; bGood=true; }
			else bGood=false;
		}
		catch (exception& exn) { ShowException(exn,"Variable::Get(string)");
		}
		catch (...) { ShowUnknownException("Variable::Get(string)");
		}
		return bGood;
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
			Console.Write("Parsing "+ToString(val.length())+ToString(" chars..."));
			for (int iNow=0; iNow<val.length(); iNow++) {
				sCharNow=val.substr(iNow,1);
				Console.Write(" '"+sCharNow+ToString("'"));
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
                   		Console.Write("("+ToString(iStartNow)+ToString(",")+ToString(iLenNow)+ToString(")"));
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
			if (bCountOnlyAndDontTouchArray) Console.Write("done parsing "+ToString(iCols)+ToString(" cols..."));
		}
		catch (exception& exn) { ShowException(exn,"GetCSVRow");
		}
		catch (...) { ShowUnknownException("GetCSVRow");
		}
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
			int iStartThatOne;
			int iLenThatOne;
			int iStartNow=0;
			int iLenNow=0;
			int iCol=0;
			string sCharNow;
			for (int iNow=0; iNow<val.length(); iNow++) {
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
		catch (exception& exn) { ShowException(exn,"GetForcedCArrayNotationSubstring");
		}
		catch (...) { ShowUnknownException("GetForcedCArrayNotationSubstring");
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
		catch (exception& exn) { ShowException(exn,"GetForcedCArrayNotationSubstring");
		}
		catch (...) { ShowUnknownException("GetForcedCArrayNotationSubstring");
		}
		return bGood;
	}//end GetForcedCArrayNotationSubstring
	int Variable::Indeces() {//returns 0 if non-array.  Otherwise returns 1 or more.
		return CountCArrayNotationFields(sVal);
	}
	int Variable::Indeces(int i1stDimension) {//returns 0 if non-array.  Otherwise returns 1 or more.
		string *psIgnored;
		//Variable vTemp;
		string sTemp;
		bool bGood=GetForcedCArrayNotationSubstring(sTemp,sVal,i1stDimension);
		Console.Write("Checking "+sTemp+"...");
		int iReturn=0;
		iReturn=CountCArrayNotationFields(sTemp);
		Console.Write( "Found "+ToString(iReturn)+ToString("...") );
		return iReturn;
	}
	bool Variable::Get(string &val, int i1stDimension) {
		bool bGood=false;
		try {
			if (bActive) { bGood=Get(val,i1stDimension,0); }
			else bGood=false;
		}
		catch (exception& exn) { ShowException(exn,"Variable::Get(string)");
		}
		catch (...) { ShowUnknownException("Variable::Get(string,i1stDim)");
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
		catch (exception& exn) { ShowException(exn,"Variable::Get(string)");
		}
		catch (...) { ShowUnknownException("Variable::Get(string,i1stDim,i2ndDim)");
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
		catch (exception& exn) { ShowException(exn,"Variable::SetName");
		}
		catch (...) { ShowUnknownException("Variable::SetName");
		}
		return bGood;
	}
	bool Variable::GetName(string &val) {
		bool bGood=false;
		try {
			if (bActive) { val=sName; bGood=true; }
			else bGood=false;
		}
		catch (exception& exn) { ShowException(exn,"Variable::GetName");
		}
		catch (...) { ShowUnknownException("Variable::GetName");
		}
		return bGood;
	}
	bool Variable::IsNamed(string val) {
		bool bMatch=false;
		try {
			if (bActive) bMatch=(sName==val);
		}
		catch (exception& exn) { ShowException(exn,"Variable::IsNamed");
		}
		catch (...) { ShowUnknownException("Variable::IsNamed");
		}
		return bMatch;
	}
	Variable::~Variable() {
		try {
			SetActiveState(false); //frees strings too
		}
		catch (exception& exn) { ShowException(exn,"Variable deconstructor");
		}
		catch (...) { ShowUnknownException("Variable deconstructor");
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
		int iNowPrev;
		SetActive();
		string sFuncNow="FromIniLine";
		try {
			if (bFirstRun) Console.Write(sFuncNow);
			if (bFirstRun) Console.WriteLine("(\""+val+"\")");
			sFuncNow+="(\""+val+"\")";
			if (bFirstRun) Console.WriteLine("  -about to analyze \""+val+"\"");
			int iNow=val.find_first_of("=");
			if (iNow>=0) {
				if (bFirstRun) Console.WriteLine("  -about to get first part from 0 to "+ToString(iNow));
				sName=val.substr(0,iNow);
				if (sName=="") ShowError("  -var name string is blank \"\"!",sFuncNow);
				if (bGood && (iNow<val.length()-1)) {
					iNowPrev=iNow;
					iNow++;
					if (bFirstRun) Console.WriteLine("    -done get first part: "+sName);
					if (bFirstRun) Console.WriteLine("    -about to get next part from "+ToString(iNow)+" length "+ToString(val.length()-iNow));
					sVal=val.substr(iNow,val.length()-iNow);
					if (sVal=="") ShowError("      -\"\" VALUE part is blank !",sFuncNow);
					if (bFirstRun) Console.WriteLine("      -done getting next part");
					//if (!bGood) ShowError("      -get VALUE part failed!",sFuncNow);
				}
				else if (!bGood) ShowError("    -substr name failed!",sFuncNow);
				else ShowError("    -(no second part found)",sFuncNow);
			}
		}
		catch (exception& exn) { ShowException(exn,"Variable::FromIniLine");
		}
		catch (...) { ShowUnknownException("Variable::FromIniLine");
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
				if (bFirstRun) cout<<"Trying to save ini line:"<<endl;
				val=sName;
				if (bFirstRun) cout<<" 1."<<val<<" (after adding "<<sName<<")"<<endl;
				if (sVal.length()>0) {
					if (sName.length()>1) {
						val+="=";
						if (bFirstRun) cout<<" 3."<<val<<" (after adding "<<"="<<")"<<endl;
						val+=sVal;
						if (bFirstRun) cout<<" 3."<<val<<" (after adding "<<sVal<<")"<<endl;
					}
				}
			}
			catch (exception& exn) { ShowException(exn,"Variable::ToIniLine");
			}
			catch (...) { ShowUnknownException("Variable::ToIniLine");
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
		try {
			if (bFirstRun) cout<<"About to load \""<<sFile<<"\""<<endl;//debug only
			sFileName=sFile;
			ifstream ifNow(sFile.c_str());
			if (bFirstRun) cout<<"Done trying to load \""<<sFile<<"\""<<endl;//debug only
			int iChars=1;
			bool bCheck;
			int iMaxLine=varr[iVars].MaxChars()+varr[iVars].MaxNameChars()+2; //+2 not 1, to include '=' AND '\0'

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
					if (bFirstRun) cout<<"  -about to get FromIniLine: "<<sLine<<endl;
					bCheck=varr[iAt].FromIniLine(sLine);
					if (bFirstRun) cout<<"  -done get FromIniLine"<<endl;
					if (bCheck&&bNew) {
						iVars++;
						iVarsNow++;
					}
				}//end if
				iLines++;
			}//end while sPtr line
			if (bFirstRun) cout<<"Done: Read "<<iVars<<" variables from "<<iLines<<" lines in "<<sFile<<endl;
			ifNow.close();
		}
		catch (exception& exn) { ShowException(exn,"Variables::Load");
		}
		catch (...) { ShowUnknownException("Variables::Load");
		}
		bFirstRun=false;
	}//end Variables::Load
	bool Variables::Save(string sFile) {
		try {
			sFileName=sFile;
			ofstream ofNow(sFile.c_str());
			int iMaxLine=varr[0].MaxChars()+varr[0].MaxNameChars()+2; //+2 not 1, to include '=' AND '\0'
			string sLine="";
	  		for (int iNow=0; iNow<iVars; iNow++) {
				sLine=varr[iNow].ToIniLine();
				ofNow<<sLine<<endl;
			}
			ofNow.close();
		}
		catch (exception& exn) { ShowException(exn,"Variables::Save(...)");
		}
		catch (...) { ShowUnknownException("Variables::Save(...)");
		}
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
			ShowUnknownException( "variables.PointerOf", "accessing varr index "+ToString(iIndex) );
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
		catch (exception& exn) { ShowException(exn,"Variables::CreateOrIgnore(...,int)");
		}
		catch (...) { ShowUnknownException("Variables::CreateOrIgnore(...,int)");
		}
	}
	void Variables::CreateOrIgnore(string sName, float val) {
		try {
			if (!Exists(sName)) SetOrCreate(sName,val);
		}
		catch (exception& exn) { ShowException(exn,"Variables::CreateOrIgnore(...,float)");
		}
		catch (...) { ShowUnknownException("Variables::CreateOrIgnore(...,float)");
		}
	}
	void Variables::CreateOrIgnore(string sName, string val) {
		try {
			if (!Exists(sName)) SetOrCreate(sName,val);
		}
		catch (exception& exn) { ShowException(exn,"Variables::CreateOrIgnore(...,string)");
		}
		catch (...) { ShowUnknownException("Variables::CreateOrIgnore(...,string)");
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
		catch (exception& exn) { ShowException(exn,"Variables::SetOrCreate(...,int)");
		}
		catch (...) { ShowUnknownException("Variables::SetOrCreate(...,int)");
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
		catch (exception& exn) { ShowException(exn,"Variables::SetOrCreate(...,float)");
		}
		catch (...) { ShowUnknownException("Variables::SetOrCreate(...,float)");
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
		catch (exception& exn) { ShowException(exn,"Variables::SetOrCreate(...,string)");
		}
		catch (...) { ShowUnknownException("Variables::SetOrCreate(...,string)");
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
		catch (exception& exn) { ShowException(exn,"Variables::GetOrCreate(int,...)");
		}
		catch (...) { ShowUnknownException("Variables::GetOrCreate(int,...)");
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
		catch (exception& exn) { ShowException(exn,"Variables::GetOrCreate(float,...)");
		}
		catch (...) { ShowUnknownException("Variables::GetOrCreate(float,...)");
		}
	}//end GetOrCreate(float,...
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
		catch (exception& exn) { ShowException(exn,"Variables::GetOrCreate(string,...)");
		}
		catch (...) { ShowUnknownException("Variables::GetOrCreate(string,...)");
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
			catch (exception& exn) { ShowException(exn,"Variables::Get(int,...)");
			}
			catch (...) { ShowUnknownException("Variables::Get(int,...)");
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
			catch (exception& exn) { ShowException(exn,"Variables::Get(float,...)");
			}
			catch (...) { ShowUnknownException("Variables::Get(float,...)");
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
			catch (exception& exn) { ShowException(exn,"Variables::Get(string,...)");
			}
			catch (...) { ShowUnknownException("Variables::Get(string,...)");
			}
		}
		return bFound;
	}
	int Variables::GetForcedInt(string sName) {
		int valReturn=0;
		int iAt=IndexOf(sName);
		try { if (iAt>=0) varr[iAt].Get(valReturn);
		}
		catch (exception& exn) { ShowException(exn,"Variables::GetForcedInt(...)");
		}
		catch (...) { ShowUnknownException("Variables::GetForcedInt(...)");
		}
		return valReturn;
	}
	float Variables::GetForcedFloat(string sName) {
		float valReturn=0.0f;
		int iAt=IndexOf(sName);
		try { if (iAt>=0) varr[iAt].Get(valReturn);
		}
		catch (exception& exn) { ShowException(exn,"Variables::GetForcedFloat(...)");
		}
		catch (...) { ShowUnknownException("Variables::GetForcedFloat(...)");
		}
		return valReturn;
	}
	string Variables::GetForcedString(string sName) {
		string val="";
		int iAt=IndexOf(sName);
		try { if (iAt>=0) val=varr[iAt].GetForcedString();
		}
		catch (exception& exn) { ShowException(exn,"Variables::GetForcedString(...)");
		}
		catch (...) { ShowUnknownException("Variables::GetForcedString(...)");
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
			ShowException(exn,"Variables::GetForcedBool(...)");
		}
		catch (...) {
			bTest=false;
			ShowUnknownException("Variables::GetForcedBool(...)");
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
			catch (...) { ShowUnknownException("variables.Indeces(...)", "accessing varr index "+ToString(iIndex)+ToString(".")); }
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
			catch (...) { ShowUnknownException("variables.Indeces(...,"+ToString(i1stDimension)+")", "accessing varr index "+ToString(iIndex)+"."); }
		}
		return iReturn;
	}
	//end class Variables
	//TODO uncomment: #endregion scripting

	//TODO uncomment: #region mass3d
	//float Mass3d::BottomOffsetRatio() {
		//return (zSize*.912);
	//}
	Mass3d::Mass3d() {
		x=0;y=0;z=0;
		xMin=-8;yMin=-8;zMin=0;
		xMax=8;yMax=8;zMax=16;
		xVel=0;yVel=0;zVel=0;
		xRot=0;yRot=0;zRot=0;
		xRotMin=0;yRotMin=0;zRotMin=0;
		xRotMax=360;yRotMax=360;zRotMax=360;
		xRotVel=90;yRotVel=90;zRotVel=90;
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
		if 		(x<xMin) x=xMin;
		else if (x>xMax) x=xMax;
		if 		(y<yMin) y=yMin;
		else if (y>yMax) y=yMax;
		if 		(z<zMin) z=zMin;
		else if (z>zMax) z=zMax;
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
	void Mass3d::RotateTowardDest(int iMillisecondsSinceLastCall) {
		AngleToLimits();
		AngleDestToLimits();
		//xRot=xRotDest;yRot=yRotDest;zRot=zRotDest;return;//debug only
		float xDegMove=((double)xRotVel*(double)iMillisecondsSinceLastCall) / 1000.0;
		float yDegMove=((double)yRotVel*(double)iMillisecondsSinceLastCall) / 1000.0;
		float zDegMove=((double)zRotVel*(double)iMillisecondsSinceLastCall) / 1000.0;
		float fApproach=0;
		if (xDegMove>=FANGLEDIFFPOSITIVE(xRot,xRotDest)) xRot=xRotDest;
		else {
	        fApproach=FANGLEDIFFPOSITIVE(xRotDest,xRot)/xDegMove;//xDegMove/360.0f;
	        if (fApproach>1.0f) fApproach=1.0f;
	        else if (fApproach<0.0f) fApproach=0.0f;//else if (fApproach<-1.0f) fApproach=-1.0f;
	        //xRot=APPROACH(xRot,xRotDest,1.0f);//debug only
	        //fApproach=.5;//debug only
			xRot=APPROACH(xRot,xRotDest,fApproach);//commented for debug only//,xDegMove/(xRotDest-xRot));
		}
		if (yDegMove>=FANGLEDIFFPOSITIVE(yRot,yRotDest)) yRot=yRotDest;
		else {
	        fApproach=FANGLEDIFFPOSITIVE(yRotDest,yRot)/yDegMove;//yDegMove/360.0f;
	        if (fApproach>1.0f) fApproach=1.0f;
	        else if (fApproach<0.0f) fApproach=0.0f;//else if (fApproach<-1.0f) fApproach=-1.0f;
			//yRot=APPROACH(yRot,yRotDest,fApproach);//commented for debug only//,yDegMove/(yRotDest-yRot));
	        //fApproach=.5;//debug only
	        yRot=APPROACH(yRot,yRotDest,fApproach);//debug only
		}
		if (zDegMove>=FANGLEDIFFPOSITIVE(zRot,zRotDest)) zRot=zRotDest;
		else {
	        fApproach=FANGLEDIFFPOSITIVE(zRotDest,zRot)/zDegMove;//zDegMove/360.0f;
	        if (fApproach>1.0f) fApproach=1.0f;
	        else if (fApproach<0.0f) fApproach=0.0f;//else if (fApproach<-1.0f) fApproach=-1.0f;
			//zRot=APPROACH(zRot,zzRotDest,fApproach);//commented for debug only//,zDegMove/(zRotDest-zRot));
	        //fApproach=.5;//debug only
	        zRot=APPROACH(zRot,zRotDest,fApproach);//debug only
		}
	}
	void Mass3d::HardRotation(float xRotTo, float yRotTo, float zRotTo) {
		xRotDest=xRotTo;xRot=xRotTo;
		yRotDest=yRotTo;yRot=yRotTo;
		zRotDest=zRotTo;zRot=zRotTo;
	}
	void Mass3d::HardLocation(float xTo, float yTo, float zTo) {
		x=xTo;//xDest=xTo;
		y=yTo;//yDest=yTo;
		z=zTo;//zDest=zTo;
	}
	void Mass3d::SetRotMaxSpeed(float xSpeed, float ySpeed, float zSpeed) {
		xVel=xSpeed;
		yVel=ySpeed;
		zVel=zSpeed;
	}
	//TODO uncomment: #endregion mass3d
	
	//TODO uncomment: #region mass2d

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
	bool Gradient::Shade(byte* byarrDest, Uint32 dwDestLoc, byte bySrcValue) {
		return Shade(byarrDest, dwDestLoc, (Uint32)bySrcValue);
	}
	bool Gradient::Shade(byte* byarrDest, Uint32 dwDestLoc, Uint32 dwSrcValue) {
		try {
			if (lpbyShade!=null) {
				byarrDest[dwDestLoc]=lpbyShade[dwSrcValue];
			}
		}
		catch (exception& exn) {
			ShowException(exn,"Gradient::Shade");
		}
		catch (...) {
			ShowUnknownException("Gradient::Shade");
		}
	}//end Gradient::Shade
	Gradient::Gradient() {
		InitNull();
		Init(256);
	}
	bool Gradient::InitNull() {
		dwLevels=0;
		lpbyShade=NULL;
	}
	bool Gradient::Init(Uint32 dwSetLevels) {
		try {
			SafeFree(lpbyShade);
			lpbyShade=(byte*)malloc(sizeof(byte)*dwSetLevels);
			dwLevels=dwSetLevels;
		}
		catch (exception& exn) {
			ShowException(exn,"Gradient::Init");
		}
		catch (...) {
			ShowUnknownException("Gradient::Init");
		}
	}
	//end classes' methods
	//TODO uncomment: #endregion
}//end namespace
#endif
