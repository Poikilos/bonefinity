#ifndef BASE_H
#define BASE_H

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <memory>
#include <cmath>
#include <fstream>
#include <string>
#include <exception>
#include <dotnetfake.h>
//#include "C:\My Documents\Projects-cpp\Base\dotnetfake.h"
#include <SDL/SDL.h>

using namespace std;

#ifndef byte
#define byte unsigned char //#define byte unsigned __int8
#endif
#ifndef HANDLE
#define HANDLE void*
#endif

#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
#define BIT1		1
#define BIT2		2
#define BIT3		4
#define BIT4		8
#define BIT5		16
#define BIT6		32
#define BIT7		64
#define BIT8		128
#define BIT9		256
#define BIT10	 512
#define BIT11	 1024
#define BIT12	 2048
#define BIT13	 4096
#define BIT14	 8192
#define BIT15	 16384
#define BIT16	 32768
#define BIT17	 65536
#define BIT18	 131072
#define BIT19	 262144
#define BIT20	 524288
#define BIT21	 1048576
#define BIT22	 2097152
#define BIT23	 4194304
#define BIT24	 8388608
#define BIT25	 16777216
#define BIT26	 33554432
#define BIT27	 67108864
#define BIT28	 134217728
#define BIT29	 268435456
#define BIT30	 536870912
#define BIT31	 1073741824
#define BIT32	 2147483648

#define LOWNIBBLEBITS	 15
#define HIGHNIBBLEBITS	240
#define LOWNIBBLE(by) ((by)&LOWNIBBLEBITS)
#define HIGHNIBBLE(by) ((by)>>4) //or (by/16)
#define SETLOWNIBBLE(by,by1) (((by)&HIGHNIBBLEBITS) | (by1)) //assumes by1<16
#define SETHIGHNIBBLE(by,by1) (((by)&LOWNIBBLEBITS) | ((by1)<<4)) //assumes by1<16

#define APPROACH(start,toward,factor) (  (start)  -  ((start)-(toward)) * (factor)  )
#define BYROUNDF(f1) ((byte)((f1)+.5f))
#define BYROUNDLD(ld) ((byte)((ld) + .5L))
#define IROUNDF(f1) (int((f1)+.5f))
#define FFLOOR(f1) ( (float)((int)(f1)) )
#define IFLOOR(f1) ( (int)(f1) )
#define ICEILING(f1) (  ( (int)(FMOD((f1),1.0f)>0.0f) ) ? ((int)(f1))+1 : (int)(f1) )
#define IROUNDD(d) (int((d)+.5))
#define IROUNDLD(ld) (int((ld)+.5L))
#define BYTEOFHUE(fH360) ( ((fH360)<360.0f)? BYROUNDF(float((fH360)*256.0f/360.0f)) : BYROUNDF(float((fH360-360.0f)*256.0f/360.0f)) ) //assumes fH360<720 //division using 256 allows 256 levels -- 255<360 is okay since 0=0
#define BYTEOFSL(fSatOrLum) ( byte( float((fSatOrLum)*255.0f)+.5f ) )


#define F360_TIMES_256 (92160.0f)
#define F256_DIV_360 (0.711111111111111111111111111111111f)
#define LD256_DIV_360 (0.711111111111111111111111111111111L)
#define BYTEOF360LD(a) (byte(( (long double)(a) * 256.0L/360.0L	)+.5L)) //#define LDBYTEOF360(a) (byte(( (long double)(a) * LD256_DIV_360 )+.5L))
#define BYTEOF360F(fA) (byte((	(fA) * F256_DIV_360	)+.5f) ) //.5f is to change truncation to rounding
#define ROFXY(x,y) ( sqrt( (x) * (x) + (y) * (y) ) )

#define FMOD(f1,f2) (((f1)>(f2)) ? ( (f1) - float(int((f1)/(f2)))*(f2)) : 0 )
#define DMOD(f1,f2) (((f1)>(f2)) ? ( (f1) - double(int((f1)/(f2)))*(f2)) : 0 )

#define LD180_DIV_PI						57.2957795130823208767981548141052L
#define D180_DIV_PI						 57.2957795130823208767981548141052
#define F180_DIV_PI				57.2957795130823208767981548141052f // 180/PI
#define THETAOFXY(x,y) ( ((y)!=0 || (x)!=0) ? (atan2((y),(x))*F180_DIV_PI) : 0 ) //*F180_DIV_PI converts radians to degrees
#define DTHETAOFXY(x,y) ( ((y)!=0 || (x)!=0) ? (atan2((y),(x))*D180_DIV_PI) : 0 ) //*F180_DIV_PI converts radians to degrees
#define LDTHETAOFXY(x,y) ( ((y)!=0 || (x)!=0) ? (atan2((y),(x))*LD180_DIV_PI) : 0 ) //*F180_DIV_PI converts radians to degrees
#define XOFRTHETA(r,theta) ((r)*cos((theta)/D180_DIV_PI)) //divide since cos takes radians
#define YOFRTHETA(r,theta) ((r)*sin((theta)/D180_DIV_PI)) //divide since cos takes radians
#define FXOFRTHETA(r,theta) ((r)*cos((theta)/F180_DIV_PI)) //divide since cos takes radians
#define FYOFRTHETA(r,theta) ((r)*sin((theta)/F180_DIV_PI)) //divide since cos takes radians
//debugging lines
#define HASATTRIB(word,bit) ( ((word)&(bit))!=0 )
#define REMOVEATTRIB(word,bit) ( (word)&=((bit)^0xFFFFFFFF) )
/////////////////////////  STRUCTS & CLASSES  //////////////////////////////////


namespace ExpertMultimediaBase {
	int iLastErr=0; //each call of every function must reset this to track errors.
	int iMaxErrors=8000;
	int iErrors=0;
	byte by3dAlphaLookup[256][256][256];
	string sLastErr="";
	string sLastFunc="";
	char carrHex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	char carrAlphabetUpper[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	char carrAlphabetLower[26]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	Uint32 dwBit[33]={0,1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648};
	char cDigit[10]={'0','1','2','3','4','5','6','7','8','9'};
	char sDigit[10][2]={"0","1","2","3","4","5","6","7","8","9"};
	char slash[2]={'\\','\0'};

	int iLogNow=0;
	//end debugging

	typedef struct IRECT_STRUCT {
		int top;
		int left;
		int bottom;
		int right;
	} IRECT, *LPIRECT;

	typedef struct FRECT_STRUCT {
		float top;
		float left;
		float bottom;
		float right;
	} FRECT, *LPFRECT;

	typedef struct IPOINT_STRUCT {
		int x;
		int y;
	} IPOINT, *LPIPOINT;
	typedef struct FPOINT_STRUCT {
		float x;
		float y;
	}FPOINT, *LPFPOINT;

	typedef struct DPOINT_STRUCT {
		double x;
		double y;
	}DPOINT, *LPDPOINT;

	typedef struct FPOINT3D_STRUCT {
		float x;
		float y;
		float z;
	}FPOINT3D, *LPFPOINT3D;

	typedef struct DPOINT3D_STRUCT {
		double x;
		double y;
		double z;
	}DPOINT3D, *LPDPOINT3D;

	typedef struct PIXEL32BGRA_STRUCT {
		byte b;
		byte g;
		byte r;
		byte a;
	}PIXEL32BGRA, *LPPIXEL32BGRA;

	typedef struct FPIXELHSL_STRUCT {
		float h; //degrees
		float s; //percent
		float l; //percent
	}FPIXELHSL, *LPFPIXELHSL;

	typedef struct PIXELHSL_STRUCT {
		byte h;
		byte s;
		byte l;
	}BYPIXELHSL, *LPBYPIXELHSL;

	typedef struct PIXEL24BGR_STRUCT {
		byte b;
		byte g;
		byte r;
	}PIXEL24BGR, *LPPIXEL24BGR;
	using namespace std;

	class IPoint {
	public:
		int x;
		int y;
		IPoint();
		string ToString();
	};

	class FPoint {
	public:
		float x;
		float y;
		FPoint();
		string ToString();
	};

	class DPoint {
	public:
		double x;
		double y;
		DPoint();
		string ToString();
	};

	class IRect {
	public:
		int top;
		int left;
		int bottom;
		int right;
		IRect();
		string ToString();
	};

	class Pixel {
	public:
		byte b;
		byte g;
		byte r;
		byte a;
		Pixel();
		Pixel(byte red, byte green, byte blue, byte alpha);
		void Set(byte red, byte green, byte blue, byte alpha);
		void Set(Uint32& dwPixel);
		//void Set(Uint32* dwPixel);
	};

	class Variable {
	private:
		string sVal;
		string sName;
		int iMaxChars;
		int iMaxNameChars;
		void Activate();
		void Activate(int iStart, int iLen);
		void Deactivate();
		bool bActive;
		int iBlankReturnStringLen;
		void SetActiveState(bool bSetActiveStatus);
		void Init(int iMaxNameCharsNow, int iMaxCharsNow);
		void Init();
	public:
		//int MAX_VARSTRLEN;
		//int MAX_VARNAMELEN;
		bool bDebug;
		bool IsActive();
		void SetActive();
		void SetInactive();
		int MaxChars();
		int MaxNameChars();
		Variable();
		~Variable();
		Variable(int val);
		Variable(float val);
		Variable(string val);
		void Set(int val);
		void Set(float val);
		void Set(string val);
		bool Get(int &val);
		bool Get(float &val);
		int Indeces();
		int Indeces(int i1stDimension);
		bool Get(string &val);
		bool Get(string &val, int i1stDimension);
		bool Get(string &val, int i1stDimension, int i2ndDimension);
		bool SetName(string val);
		bool GetName(string &val);
		bool IsNamed(string val);
		string GetForcedString();
		bool FromIniLine(string val);
		void ToIniLine(string &val);
		string ToIniLine();
		int IniLineLen();
	};
	#define MAX_VARS    	1000
	 #define MAX_FILENAME    1024
	class Variables {
	private:
		//const int MAX_VARS=1000;
		//const int MAX_FILENAME=1024;
		int iVars;
		Variable varr[MAX_VARS];
		void Init();
		string sFileName;
	public:
		bool bSaveEveryChange;
		bool bDebug;
		int iBlankReturnStringLen;
		Variables();
		~Variables();
		Variables(string sFile);
		bool Load(string sFile);
		bool Save(string sFile);
		bool Save();
		int IndexOf(string sName);
		Variable* PointerOf(string sName);
		bool Exists(string sName);
		void CreateOrIgnore(string sName, int val);
		void CreateOrIgnore(string sName, float val);
		void CreateOrIgnore(string sName, string val);
		void SetOrCreate(string sName, int val);
	    void SetOrCreate(string sName, float val);
		void SetOrCreate(string sName, string val);
		void GetOrCreate(int &val, string sName);
		void GetOrCreate(float &val, string sName);
		void GetOrCreate(string &val, string sName);
		bool Get(int &val, string sName);
		bool Get(float &val, string sName);
		bool Get(string &val, string sName);
		int GetForcedInt(string sName);
		float GetForcedFloat(string sName);
		string GetForcedString(string sName);
		bool GetForcedBool(string sName);
		int Indeces(string sName);
		int Indeces(string sName, int i1stDimension);
	};
	class Mass3d {
	public:
		float x,y,z;
		float xMin,yMin,zMin;
		float xMax,yMax,zMax;
		float xVel,yVel,zVel;
		float xRot,yRot,zRot;
		float xRotMin,yRotMin,zRotMin;
		float xRotMax,yRotMax,zRotMax;
		float xRotVel,yRotVel,zRotVel;
		float xRotDest,yRotDest,zRotDest;
		float xSize,ySize,zSize;
		Mass3d();
		void RotateTowardDest(int iMillisecondsSinceLastCall);
		void SafeDestAngles();
		void SafeAngles();
		void AngleDestToLimits();
		void AngleToLimits();
		void LocationToLimits();
		void HardRotation(float xRotTo, float yRotTo, float zRotTo);
		void HardLocation(float xTo, float yTo, float zTo);
		void SetRotMaxSpeed(float xSpeed, float ySpeed, float zSpeed);
	};
	class Mass2d {
	private:
	   	int xCenterRelNonScaled;
		int yCenterRelNonScaled;
	public:
		IRECT rectRender;
		IRECT rectOriginal;
		FRECT rectHitDetectPixelRatios;
		FRECT rectHitDetectPixelBased;
		//int xSize,ySize;
		float fScale;
		float fPixelsPerMeter;
		int CenterXAbsScaled();
		int CenterYAbsScaled();
		float FCenterXAbsScaled();
		float FCenterYAbsScaled();
		float FCenterXRelScaled();
		float FCenterYRelScaled();
		float BottomOffsetRatio();
		float HitRectHeightRatio();
		Mass2d();
		void SetPixCenter(int xPixNonScaled, int yPixNonScaled);
		void Init(int Width, int Height, float fPixPerMeter);
		void Init(int Width, int Height);
		void Init(int Width, int Meight, bool bWithSamePixPerMeterAsAPreviousInitialization);
		void SetHitRect(float Top, float Left, float Bottom, float Right);
	};
	class Gradient {
	private:
		Uint32 dwLevels;
		byte* lpbyShade;
		bool Init(Uint32 dwSetLevels);
		bool InitNull();
	public:
		Gradient();
		bool Shade(byte* byarrDest, uint dwDestLoc, byte bySrcValue);
		bool Shade(byte* byarrDest, uint dwDestLoc, Uint32 dwSrcValue);
	};
	//end classes

	////////////////////////////////////// PROTOTYPES //////////////////////////////
	int GetCSVRow(string *sarrReturn, string val);
	int GetCSVRow(string *sarrReturn, string val, bool bCountOnlyAndDontTouchArray);
	int GetCSVRow(string *sarrReturn, string val, bool bCountOnlyAndDontTouchArray, int iExactCols_OrZeroForAuto, bool bAllowCurleyBracesRecursively_FalseIgnoresThem);
	int GetCSVRow(string *sarrReturn, string val, bool bCountOnlyAndDontTouchArray, int iExactCols_OrZeroForAuto);
	int CountCArrayNotationFields(string val);
	bool GetForcedCSVNotationSubstring(string &valReturn, string val, int iIndexer);
	bool GetForcedCSVNotationSubstring(string &valReturn, string val, int iIndexer, bool bAllowCurleyBracesRecursively_FalseIgnoresThem);
	bool GetForcedCArrayNotationSubstring(string &valReturn, string val, int iIndexer);
	bool CopySafe(byte* byarrDest, byte* byarrSrc, int iDestStart, int iSrcStart, int iBytes);
	byte NibbleValueOfHexChar(char cNow);
	byte ByteFromHexChars(string sHexPair);
	char LowercaseElseZeroIfNotUppercase(char cNow);
	string ToUpper(string val);
	bool StartsWith(string val, string sPart);
	bool EndsWith(string val, string sPart);
	string SafeSubstring(string val, int iStart, int iLen);
	string SafeSubstring(string val, int iStart);
	int SafeLength(string val);
	float MirrorOnYLine(float fAngle);
	float AngleToward(float xDest, float yDest, float xSrc, float ySrc);
	int FileSize(string sFile);
	int FileSize(const char* sFile);
	inline void Crop(float &fToModify, float fMin, float fMax);
	inline LPIPOINT IPOINTFROM(float xNow, float yNow);
	inline float SafeAngle360(float fToLimitBetweenZeroAnd360);
	inline void SafeAngle360ByRef(float &fToLimitBetweenZeroAnd360);
	inline void FPOLAROFRECT(float &r,float &theta, float x, float y);
	inline void DPOLAROFRECT(double &r,double &theta, double x, double y);
	inline float FANGLEDIFF(float f1, float f2); //returns 0 to 180
	inline float FANGLEDIFFPOSITIVE(float f1, float f2); //returns 0 to 180
	inline float FDist(FPoint &point1, FPoint &point2);
	inline float FPDIST(float x1, float y1, float x2,  float y2);
	double DDist(DPoint &point1, DPoint &point2);
	inline double DPDIST(double x1, double y1, double x2, double y2);
	inline long double LDANGLE360(long double d1);
	inline long double LDANGLEDIFF(long double d1, long double d2); //returns 0 to 180
	inline float DIST3D(Mass3d &pointA, Mass3d &pointB);
	inline float DIST3D(FPOINT3D &pointA, FPOINT3D &pointB);
	inline float DIST3D(FPOINT3D &pointA, Mass3d &pointB);
	inline float DIST3D(Mass3d &pointA, FPOINT3D &pointB);
	inline float DIST3D(DPOINT3D &pointA, DPOINT3D &pointB);
	inline bool Travel3d(Mass3d &pointToSlide, float fPitch, float fYaw, float fDistanceTravel);
	inline bool Travel3d(float &xToMove, float &yToMove, float &zToMove, float fPitch, float fYaw, float fDistanceTravel);
	inline bool Travel3dAbs(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravel);
	inline bool Travel3d(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravelRatio);
	void strcatfromsiverrno(char *sErr, int iNum);
	void strcpyfromsiverrno(char *s, int i);
	inline void Rotate(float &xToMove, float &yToMove, float xCenter, float yCenter, float fRotate);
	void Rotate(float &xToMove, float &yToMove, float fRotate);
	void FakeException(string sExn);
	bool ShowErr();
	bool ShowError();
	void ShowError(string sMsg);
	void ShowError(string sMsg, string sCurrentFunction);
	void ShowError(int iSivErrNo, string sCurrentFunction);
	void ShowError(int iSivErrNo, string sMsg, string sCurrentFunction);
	void ShowAndDeleteException(char* &sExn, string sFuncNow, string sVerbNow);
	void ShowAndDeleteException(char* &sExn, string sFuncNow);
	void ShowException(exception& exn, string sFuncNow, string sVerbNow);
	void ShowException(exception& exn, string sFuncNow);
	void ShowUnknownException(string sFuncNow, string sVerbNow);
	void ShowUnknownException(string sFuncNow);
	bool Compare(char* s1, char* s2);
	string ToString(int val);
	string ToString(long val);
	string ToString(Uint32 val);
	string ToString(float val);
	string ToString(double val);
	string ToString(string val);
	string ToString(char* val);
	string ToString(bool val);
	////////////////////////////////////// SIMPLE FUNCTIONS USING INTRINSIC TYPES ///
	inline void SafeFree(byte* &refToFreeAndSetToNull) {
		try {
			if (refToFreeAndSetToNull!=NULL) free(refToFreeAndSetToNull);
			refToFreeAndSetToNull=NULL;
		}
		catch (exception& exn) {//!!!!!!!!!!TODO: finish this: copy this to ALL catch statements EVERYWHERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
		    ShowException(exn, "SafeFree(byte*)");
		}
		catch (...) {
			ShowUnknownException("SafeFree(byte*)");
		}
	}
	inline void SafeFree(char* &refToFreeAndSetToNull) {
		try {
			if (refToFreeAndSetToNull!=NULL) free(refToFreeAndSetToNull);
			refToFreeAndSetToNull=NULL;
		}
		catch (exception& exn) {//!!!!!!!!!!TODO: finish this: copy this to ALL catch statements EVERYWHERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
		    ShowException(exn, "SafeFree(char*)");
		}
		catch (...) {
			ShowUnknownException("SafeFree(char*)");
		}
	}
	inline void SafeFree(int* &refToFreeAndSetToNull) {
		try {
			if (refToFreeAndSetToNull!=NULL) free(refToFreeAndSetToNull);
			refToFreeAndSetToNull=NULL;
		}
		catch (exception& exn) {//!!!!!!!!!!TODO: finish this: copy this to ALL catch statements EVERYWHERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
		    ShowException(exn, "SafeFree(int*)");
		}
		catch (...) {
			ShowUnknownException("SafeFree(int*)");
		}
	}
	inline void SafeFree(int** &refToFreeAndSetToNull, int iIndeces) {
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
		    ShowException(exn, "SafeFree(int**,"+ToString(iIndeces)+")");
		}
		catch (...) {
			ShowUnknownException("SafeFree(int**,"+ToString(iIndeces)+")");
		}
	}
	inline bool stringerrors(char *carr, Uint32 dwMaxBuff) {
		if (carr==NULL) {
			//error_txt<<"STRING ERROR: "<<"carr is NULL!!!"<<endl;
			return(true);
		}
		if (strlen(carr)<1 || strlen(carr)>=dwMaxBuff) {
			//error_txt<<"STRING ERROR: "<<strlen(carr)<<" is strlen(carr)! (MaxBuff="<<dwMaxBuff<<") carr="<<carr<<endl;
			return(true);
		}
		return(false);
	}
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
	void DetectSlashType(char *sFullPath) { //always run this before using directory carr
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
		//error_txt<<"iToMatch="<<iToMatch<<endl;
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
		//error_txt<<"iMatches="<<iMatches<<endl<<endl;
		return((iMatches==iToMatch)?true:false);
	} // end IsLikeMask

	void strcatfrom(char *carr, int i) {
		if (i==0) {
			strcat(carr, "0");
			return;
		}
		else if (i<0) { //if negative
			strcat(carr, "-");
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
				strcat(carr, sDigit[iTemp/iDivisor]);
				iTemp-=iDivisor*int(iTemp/iDivisor);
			}
			else {
			}
			iExpOfTen--;
		}
		return;
	}
	void strcatfrom(char *carr, long val) {
		if (val==0) {
			strcat(carr, "0");
			return;
		}
		else if (val<0) { //if negative
			strcat(carr, "-");
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
				strcat(carr, sDigit[iTemp/iDivisor]);
				iTemp-=iDivisor*long(iTemp/iDivisor);
			}
			else {
			}
			iExpOfTen--;
		}
		return;
	}
	void strcatfrom(char *carr, Uint32 val) {
		if (val==0) {
			strcat(carr, "0");
			return;
		}
		//else if (val<0) { //if negative
		//	strcat(carr, "-");
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
				strcat(carr, sDigit[iTemp/divisor]);
				iTemp-=divisor*(Uint32)(iTemp/divisor);
			}
			else {
			}
			iExpOfTen--;
		}
		return;
	}//end strcatfrom Uint32

	void strcpyfrom(char *carr, int i) {
		carr[0]='\0';
		strcatfrom(carr, i);
	}
	void strcpyfrom(char *carr, long val) {
		carr[0]='\0';
		strcatfrom(carr, val);
	}
	void strcpyfrom(char *carr, Uint32 val) {
		carr[0]='\0';
		strcatfrom(carr, val);
	}

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
		while (d<pow((long double)10,(long double)iExp10)) iExp10--;
		////error_txt<<"float iExp10's = ";
		if (iExp10<0) iExp10=0; //make sure leading zeros get copied
		const static double DMINEXP=-15;
		while ( (d>0 && iExp10>=DMINEXP && iExp10>iExp10Max) || iExp10>=-1 ) {
			if ( (Uint32)( d/pow((long double)10,(long double)iExp10) ) < 10 ) {
				//error_txt<<"["<<(Uint32)( d/pow(10,iExp10) )<<"](e="<<iExp10<<")"<<flush;
				strcat(sNum, sDigit[(Uint32)( d/pow((long double)10,(long double)iExp10) )]);
				if (iExp10==0) strcat(sNum, ".");
			}
			else {
				//error_txt<<"ERROR: strcatfromdouble()'s (Uint32)( d/pow(10,iExp10) ) isn't < 10! =("<<(Uint32)( d/pow((long double)10,(long double)iExp10) )<<")"<<endl;
			}
			d-=double((Uint32)( d/pow((long double)10,(long double)iExp10) ))*pow((long double)10,(long double)iExp10);
			iExp10--;
		}
		return;
	}
	void strcatfrom(char *sNum, register double d1) {
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

	int IntOfChar(char *cDigitX) {
		static int iErrLevel=0;
		for (register int i=0; i<10; i++) {
			if (*cDigitX==cDigit[i]) return(i);
		}
		//return before now usually
		//if (iErrLevel<10)error_txt<<"IntOfChar error: "<<*cDigitX<<" is not an integer!";//error if didn't return yet;
		//else if (iErrLevel<11)error_txt<<"LAST WARNING -- IntOfChar error: "<<*cDigitX<<" is not an integer!";//error if didn't return yet;
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
				cerr<<"Exception in substrcpy(\""<<sDest<<"\",\""<<sSrc<<"\","<<iStart<<","<<iLen<<")";
			}
			catch (...) {
	            cerr<<"Exception:"<<endl;
				try {
					cerr<<"  substrcpy Dest:"<<sDest<<endl;
				}
				catch (...) {
					cerr<<"  substrcpy(bad Dest carr sent)"<<endl;
				}
				try {
					cerr<<"  substrcpy Src:"<<sSrc<<endl;
				}
				catch (...) {
					cerr<<"  substrcpy(bad Source carr sent)"<<endl;
				}
				cerr<<"  substrcpy iStart:"<<iStart<<endl;
				cerr<<"  substrcpy iLen:"<<iLen<<endl;
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
			cerr<<"Exception in Crop char*"<<endl;
			bGood=false;
		}
		return bGood;
	}
	int IntOfString(char *sNum) {
		register Uint32 dwStrlen=strlen(sNum);
		register Uint32 dw=0;//loop var
		register Uint32 dwIsNeg=0;
		if (sNum[0]=='-') dwIsNeg=1;
		register int iExp10=strlen(sNum)-1-int(dwIsNeg);
		register int iReturn=0;
		for (dw=dwIsNeg; dw<dwStrlen; dw++) {
			if (sNum[dw]!='.') {
				iReturn+=(int)( powl((long double)10,(long double)iExp10)*(long double)IntOfChar(&sNum[dw]) );//math.h: long powl(10,int)
				iExp10--;
			}
		}
		if (dwIsNeg==1) iReturn*=-1;
		return(iReturn);
	}//end IntOfString
	double DoubleOfString(char *sNum) {
		register Uint32 dwStrlen=strlen(sNum);
		register Uint32 dw=0;//loop var
		register int iExp10=-1;
		register Uint32 dwIsNeg=0;
		if (sNum[0]=='-') {
			dwIsNeg=1;
		}
		for (dw=dwIsNeg; dw<dwStrlen && sNum[dw]!='.'; dw++) {
			iExp10++;
		}
		register double dReturn=0;
		for (dw=dwIsNeg; dw<dwStrlen; dw++) {
			if (sNum[dw]!='.') {
				dReturn+=pow((long double)10,(long double)iExp10)*(double)IntOfChar(&sNum[dw]);//math.h: double pow(10,int)
				iExp10--;
			}
		}
		if (dwIsNeg==1) dReturn*=-1;
		return(dReturn);
	}

	float FloatOfString(char *sNum) {
		register Uint32 dwStrlen=strlen(sNum);
		register Uint32 dw=0;//loop var
		register int iExp10=-1;
		register Uint32 dwIsNeg=0;
		if (sNum[0]=='-') {
			dwIsNeg=1;
		}
		for (dw=dwIsNeg; dw<dwStrlen && sNum[dw]!='.'; dw++) {
			iExp10++;
		}
		register float fReturn=0;
		for (dw=dwIsNeg; dw<dwStrlen; dw++) {
			if (sNum[dw]!='.') {
				fReturn+=powl(10,iExp10)*(long double)IntOfChar(&sNum[dw]);//math.h: long double powl(10,int)
				iExp10--;
			}
		}
		if (dwIsNeg==1) fReturn*=-1;
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
	inline float FSQUARED(float val) {
		return val*val;
	}
	inline float DSQUARED(double val) {
		return val*val;
	}
	bool ExistsAt(char* sHaystack, char* sNeedle, int iAtHaystack) {
		bool bMatch=false;
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
		catch (...) {
			ShowError("Exception in ExistsAt");
		}
	}
	inline int IndexOf(char* sHaystack, char* sNeedle) {
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
			ShowException(exn,"IndexOf");
		}
		catch (char* carrExn) {
			ShowAndDeleteException(carrExn,"IndexOf");
		}
		catch (...) {
			ShowUnknownException("IndexOf");
		}
		return iFound;
	}//end IndexOf
	
	
	inline int SafePow(int basenum, int exp) {
		int result;
		bool bNeg;
		if (exp<0) {
			bNeg=true;
			exp*=-1;
		}
		if (exp==0) return 1;
		else {
			bNeg=false;
			result=basenum;
			for (int iCount=1; iCount<exp; iCount++) {
				if (result<int_MaxValue-basenum) result*=basenum;
				else return int_MaxValue;
			}
		}
		if (bNeg) {
			result=1/result;
			exp*=-1;//leaves it the way we found it
		}
		return result;
	}
	inline long SafePow(long basenum, int exp) {
		long result;
		bool bNeg;
		if (exp<0) {
			bNeg=true;
			exp*=-1;
		}
		if (exp==0) return 1;
		else {
			bNeg=false;
			result=basenum;
			for (int iCount=1; iCount<exp; iCount++) {
				if (result<long_MaxValue-basenum) result*=basenum;
				else return long_MaxValue;
			}
		}
		if (bNeg) {
			result=1L/result;
			exp*=-1;//leaves it the way we found it
		}
		return result;
	}
	inline float SafePow(float basenum, int exp) {
		float result;
		bool bNeg;
		if (exp<0) {
			bNeg=true;
			exp*=-1;
		}
		if (exp==0) return 1;
		else {
			bNeg=false;
			result=basenum;
			for (int iCount=1; iCount<exp; iCount++) {
				if (result<float_MaxValue-basenum) result*=basenum;
				else return float_MaxValue;
			}
		}
		if (bNeg) {
			result=1.0f/result;
			exp*=-1;//leaves it the way we found it
		}
		return result;
	}
	inline double SafePow(double basenum, int exp) {
		double result;
		bool bNeg;
		if (exp<0) {
			bNeg=true;
			exp*=-1;
		}
		if (exp==0) return 1;
		else {
			bNeg=false;
			result=basenum;
			for (int iCount=1; iCount<exp; iCount++) {
				if (result<double_MaxValue-basenum) result*=basenum;
				else return double_MaxValue;
			}
		}
		if (bNeg) {
			result=1.0/result;
			exp*=-1; //leaves it the way we found it
		}
		return result;
	}
	const int i10=10;
	const long l10=10L;
	const float f10=10.0f;
	const double d10=10;
	inline int SafeE10I(int exp) {
		return SafePow(i10, exp);
	}
	inline long SafeE10L(int exp) {
		return SafePow(l10, exp);
	}
	inline float SafeE10F(int exp) {
		return SafePow(f10, exp);
	}
	inline double SafeE10D(int exp) {
		return SafePow(d10, exp);
	}
	inline byte SafeByRoundF(float val) {
		val+=.5f;
		if (val<0.0f) return 0;
		else if (val>255.0f) return 255;
		else return (byte)val;
	}
	inline byte SafeByte(float val) {
	    return (val>255.0f)?255:((val<0.0f)?0:(byte)val);
	}
	inline int SafeAddWrappedPositiveOnly(int val1, int val2) {
		if (val1<0) val1*=-1;
		if (val2<0) val2*=-1;
	    int max=2147483647;
	    int val3=0;
	    int maxdiff=max-val1;
	    if (maxdiff<val2) val3=val1+val2;
	    else val3=val2-maxdiff;
	    return val3;
	}
} //end namespace
#endif

