#ifndef BASE_H
#define BASE_H
#pragma once
#include <limits>
// #include <type_traits>
// #include <algorithm>

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <memory>
#include <cmath>
#include <fstream>
#include <string>
#include <exception>
#include <frameworkdummy.h>
#include <SDL2/SDL.h>
#include <RTypes.h>
// for cpp file:
#include <cstring>  // memcpy etc
// #include <cstdlib>
// #include <iostream>
// #include <iomanip>
// #include <fstream>
// #include <memory>
#include <algorithm>  // transform etc
// #include <string>
// #include <frameworkdummy.h>
#include <float.h>
#include <limits.h>
#include <RMath.h>

//using namespace std;
//using namespace ProtoArmor;

//#ifndef byte
//typedef unsigned char byte; //#define byte unsigned __int8
//#endif
#ifndef HANDLE
#define HANDLE void*
#endif
#ifndef MAX_PATH
#define MAX_PATH 255
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
#define MAX_VARS		1000
#define MAX_FILENAME	1024





//#define LOWNIBBLE(by) ((by)&LOWNIBBLEBITS)
//#define HIGHNIBBLE(by) ((by)>>4) //or (by/16)
//#define SETLOWNIBBLE(by,by1) (((by)&HIGHNIBBLEBITS) | (by1)) //assumes by1<16
//#define SETHIGHNIBBLE(by,by1) (((by)&LOWNIBBLEBITS) | ((by1)<<4)) //assumes by1<16

#define APPROACH(start,toward,factor) (  (start)  -  ((start)-(toward)) * (factor)  )
#define BYROUNDF(f1) ((byte)((f1)+.5f))
#define BYROUNDLD(ld) ((byte)((ld) + .5L))
#define IROUNDF(f1) (int((f1)+.5f))
#define FFLOOR(f1) ( (float)((int)(f1)) )
#define DFLOOR(f1) ( (double)((int)(f1)) )
#define IFLOOR(f1) ( (int)(f1) )
#define ICEILING(f1) (  ( (int)(FMOD((f1),1.0f)>0.0f) ) ? ((int)(f1))+1 : (int)(f1) )
#define IROUNDD(d) (int((d)+.5))
#define IROUNDLD(ld) (int((ld)+.5L))
#define BYTEOFHUE(fH360) ( ((fH360)<360.0f)? BYROUNDF(float((fH360)*256.0f/360.0f)) : BYROUNDF(float((fH360-360.0f)*256.0f/360.0f)) ) //assumes fH360<720 //division using 256 allows 256 levels -- 255<360 is okay since 0=0
#define BYTEOFSL(fSatOrLum) ( byte( float((fSatOrLum)*255.0f)+.5f ) )

#define F360_TIMES_256 (92160.0f)
#define F256_DIV_360 (0.711111111111111111111111111111111f)
#define LD256_DIV_360 (0.711111111111111111111111111111111L)
//BYTEOF360 intentionally divides by 256 to allow storage of 256 degree variations since 360 would be redundant if both 0 and 255 where 360!!!
#define BYTEOF360LD(a) (byte(( (long double)(a) * 256.0L/360.0L	)+.5L)) //#define LDBYTEOF360(a) (byte(( (long double)(a) * LD256_DIV_360 )+.5L))
#define BYTEOF360F(fA) (byte((	(fA) * F256_DIV_360	)+.5f) ) //.5f is to change truncation to rounding
#define ROFXY(X,Y) ( sqrt( (X) * (X) + (Y) * (Y) ) )

#define FMOD(f1,f2) (((f1)>(f2)) ? ( (f1) - float(int((f1)/(f2)))*(f2)) : 0 )
#define DMOD(f1,f2) (((f1)>(f2)) ? ( (f1) - double(int((f1)/(f2)))*(f2)) : 0 )

//formerly all caps (see 1.DXManSDL and comparison to dxman-kdevelop.txt for what they used to be called):
//moved to RMath as function: #define LD180_DIV_PI						57.2957795130823208767981548141052L
//moved to RMath as function: #define D180_DIV_PI						 57.2957795130823208767981548141052
//moved to RMath as function: #define F180_DIV_PI				57.2957795130823208767981548141052f // 180/PI
//moved to RMath as function: #define FTHETAOFXY_DEG(X,Y) ( ((Y)!=0 || (X)!=0) ? (atan2((Y),(X))*180.0f/3.141592653589793238462643383279502884f) : 0 )
//moved to RMath as function: #define DTHETAOFXY_DEG(X,Y) ( ((Y)!=0 || (X)!=0) ? (atan2((Y),(X))*180.0/3.141592653589793238462643383279502884) : 0 )
//moved to RMath as function: #define LDTHETAOFXY_DEG(X,Y) ( ((Y)!=0 || (X)!=0) ? (atan2((Y),(X))*180.0L/3.141592653589793238462643383279502884L) : 0 )
//moved to RMath as function: #define FTHETAOFXY_DEG(X,Y) ( ((Y)!=0 || (X)!=0) ? (atan2((Y),(X))*F180_DIV_PI) : 0 ) //*F180_DIV_PI converts radians to degrees (no RADTODEG method exists here)
//moved to RMath as function: #define DTHETAOFXY_DEG(X,Y) ( ((Y)!=0 || (X)!=0) ? (atan2((Y),(X))*D180_DIV_PI) : 0 ) //*F180_DIV_PI converts radians to degrees (no RADTODEG method exists here)
//moved to RMath as function: #define LDTHETAOFXY_DEG(X,Y) ( ((Y)!=0 || (X)!=0) ? (atan2((Y),(X))*LD180_DIV_PI) : 0 ) //*F180_DIV_PI converts radians to degrees (no RADTODEG method exists here)

//moved to RMath as function: #define THETAOFXY_RAD(X,Y) ( ((Y)!=0 || (X)!=0) ? (atan2((Y),(X))) : 0 )
//moved to RMath as function: #define FXOFRTHETA_DEG(r,theta) ((r)*cos((theta)/F180_DIV_PI)) //divide since cos takes radians
//moved to RMath as function: #define FYOFRTHETA_DEG(r,theta) ((r)*sin((theta)/F180_DIV_PI)) //divide since cos takes radians
//moved to RMath as function: #define DXOFRTHETA_DEG(r,theta) ((r)*cos((theta)/D180_DIV_PI)) //divide since cos takes radians
//moved to RMath as function: #define DYOFRTHETA_DEG(r,theta) ((r)*sin((theta)/D180_DIV_PI)) //divide since cos takes radians
//moved to RMath as function: #define LDXOFRTHETA_DEG(r,theta) ((r)*cos((theta)/LD180_DIV_PI)) //divide since cos takes radians
//moved to RMath as function: #define LDYOFRTHETA_DEG(r,theta) ((r)*sin((theta)/LD180_DIV_PI)) //divide since cos takes radians
//moved to RMath as function: #define XOFRTHETA_RAD(r,theta) ((r)*cos((theta))) //cos takes radians
//moved to RMath as function: #define YOFRTHETA_RAD(r,theta) ((r)*sin((theta))) //cos takes radians
//#define FXOFRTHETA_RAD(r,theta) ((r)*cos((theta))) //cos takes radians
//#define FYOFRTHETA_RAD(r,theta) ((r)*sin((theta))) //cos takes radians
#define HASATTRIB(word,bit) ( ((word)&(bit))!=0 )
#define REMOVEATTRIB(word,bit) ( (word)&=((bit)^0xFFFFFFFF) )
/////////////////////////  STRUCTS & CLASSES  //////////////////////////////////


namespace ExpertMultimediaBase {

	typedef double REAL;
	extern const REAL Base_r0;//=(REAL)0.0;
	extern const REAL Base_r1;//=(REAL)1.0;
	extern const REAL Base_r1000;//=(REAL)1000.0;
	extern Uint32 dwTicksAcquiredOutsideOfGameState_Run;

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
		int X;
		int Y;
	} IPOINT, *LPIPOINT;
	/*
	typedef struct FPOINT_STRUCT {
		float X;
		float Y;
	}FPOINT, *LPFPOINT;
*/
/*
	typedef struct DPOINT_STRUCT {
		double X;
		double Y;
	}DPOINT, *LPDPOINT;
*/
	typedef struct FPOINT3D_STRUCT {
		float X;
		float Y;
		float Z;
	}FPOINT3D, *LPFPOINT3D;

	typedef struct DPOINT3D_STRUCT {
		double X;
		double Y;
		double Z;
	}DPOINT3D, *LPDPOINT3D;

	typedef struct PIXEL_STRUCT {
		byte b;
		byte g;
		byte r;
		byte a;
	}PIXEL, *LPPIXEL;

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


	class Pixel {
	public:
		byte b;
		byte g;
		byte r;
		byte a;
		Pixel();
		Pixel(byte red, byte green, byte blue, byte alpha);
		void Set(byte red, byte green, byte blue, byte alpha);
		void Set(Uint32& u32Pixel);
		string ToString();
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
		//bool bDebug;
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
		//bool bDebug;
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
		void GetOrCreate(REAL &val, string sName);
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
		string name;
		float X,Y,Z;
		float xMin,yMin,zMin;
		float xMax,yMax,zMax;
		float xVel,yVel,zVel;
		float xRot,yRot,zRot;
		float xRotMin,yRotMin,zRotMin;
		float xRotMax,yRotMax,zRotMax;
		float xRotVelDegreesPerSec,yRotVelDegreesPerSec,zRotVelDegreesPerSec;
		float xRotDest,yRotDest,zRotDest;
		float xSize,ySize,zSize;
		Mass3d();
		void RotateTowardDestByDegreesByRef(float& degToChange, float degDest, float degOperand, bool IsToTakeMostEfficientRoute);
		void RotateTowardDestByDegreesByRef(double& degToChange, double degDest, double degOperand, bool IsToTakeMostEfficientRoute);
		void RotateTowardDest(int iMillisecondsSinceLastCall);
		void RotateTowardDest(float rSecondsSinceLastCall);
		void RotateTowardDest(double rSecondsSinceLastCall);
		void SafeDestAngles();
		void SafeAngles();
		void AngleDestToLimits();
		void AngleToLimits();
		void LocationToLimits();
		void HardRotation(float xRotTo, float yRotTo, float zRotTo);
		void HardLocation(float xTo, float yTo, float zTo);
		void CopyLocationOnlyTo(Mass3d& m3dTo);
		void CopyTo(Mass3d& m3dTo);
		///<summary>
		///orientation is a six-long array defining two 3d points: At then Up
		///</summary>
		void CopyTo(float setOrientation[], float setPosition[], float setVelocity[], bool make_orientation_Z_UP, bool make_orientation_X_FORWARD);
		void OffsetSomethingByMyLocation(Mass3d& m3dSomethingToMove);

		//void SetRotMaxSpeed(float xSpeed, float ySpeed, float zSpeed);
		string ToString();
		string ToString(bool bShowAll);
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
		float get_width_scaled_f();
		float get_height_scaled_f();
	};
	//class Gradient {
	//private:
	//	Uint32 u32Levels;
	//	byte* lpbyShade;
	//	bool Init(Uint32 u32SetLevels);
	//	bool InitNull();
	//public:
	//	Gradient();
	//	bool Shade(byte* arrbyDest, uint u32DestLoc, byte bySrcValue);
	//	bool Shade(byte* arrbyDest, uint u32DestLoc, Uint32 u32SrcValue);
	//};
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
	bool CopySafe(byte* arrbyDest, byte* arrbySrc, int iDestStart, int iSrcStart, int iBytes);
	byte NibbleValueOfHexChar(char cNow);
	byte HexCharsToByte(string sHexPair);
	char LowercaseElseZeroIfNotUppercase(char cNow);
	string ToUpper(string val);
	bool StartsWith(string val, string sPart);
	bool EndsWith(string val, string sPart);
	string SafeSubstring(string val, int iStart, int iLen);
	string SafeSubstring(string val, int iStart);
	int SafeLength(string val);
	float MirrorOnYLine(float fAngle);
	float AngleTowardDestFromSrc_Deg(float xDest, float yDest, float xSrc, float ySrc, bool InvertY);
	int FileSize(string sFile);
	int FileSize(const char* sFile);
	void strcatfromsiverrno(char *sErr, int iNum);
	void strcpyfromsiverrno(char *s, int i);
	void Rotate(float &xToMove, float &yToMove, float fRotate);
	void Rotate(double &xToMove, double &yToMove, double fRotate);
	void FakeException(string sExn);
	bool ShowErr();
	bool ShowError();
	bool ShowErr(string sMsg);
	bool ShowError(string sMsg);
	bool ShowErr(string sMsg, string sCurrentFunction);
	bool ShowError(string sMsg, string sCurrentFunction);
	bool ShowError(int iSivErrNo, string sCurrentFunction);
	bool ShowError(int iSivErrNo, string sMsg, string sCurrentFunction);
	//void ShowAndDeleteException(char* &sExn, string sFuncNow, string sVerbNow);
	//void ShowAndDeleteException(char* &sExn, string sFuncNow);
	void ShowExn(exception& exn, string sFuncNow, string sVerbNow);
	void ShowExn(exception& exn, string sFuncNow);
	void ShowUnknownExn(string sFuncNow, string sVerbNow);
	void ShowUnknownExn(string sFuncNow);
	bool Compare(char* s1, char* s2);
	string RString_ToString(int val);
	string RString_ToString(long val);
	string RString_ToString(Uint32 val);
	string RString_ToString(float val);
	string RString_ToString(double val);
	string RString_ToString(long double val);
	string RString_ToString(string val);
	string RString_ToString(char* val);
	string RString_ToString(const char* val);
	//string RString_ToString(char val[]); //same as const char*
	string RString_ToString(char val);
	string RString_ToString(bool val);
	////////////////////////////////////// SIMPLE FUNCTIONS USING INTRINSIC TYPES ///
	void trunc(char *s, int i);
	void Upperstring(char *s);
	void Lowerstring(char *s);
	void DetectSlashType(char *sFullPath);
	void ValidateSlash(char *sDir);
	bool IsLikeMask(char *sFullPath1, char *sMask1);

	void strcatfrom(char *sz, int i);
	void strcatfrom(char *sz, long val);
	void strcatfrom(char *sz, Uint32 val);

	void strcpyfrom(char *sz, int i);
	void strcpyfrom(char *sz, long val);
	void strcpyfrom(char *sz, Uint32 val);

	void strcatfrom(char *sNum, register float d1, int iMaxDecimals);
	void strcatfrom(char *sNum, register float d1);
	void strcatfrom(char *sNum, register double d1, int iMaxDecimals);
	void strcatfrom(char *sNum, register double d1);
	void strcatfrom(char *sNum, register long double d1, int iMaxDecimals);
	void strcatfrom(char *sNum, register long double d1);

	//overload it without Max Decimals, so set MaxDecimals to 10:
	void strcpyfrom(char *sNum, register float d1);
	void strcpyfrom(char *sNum, register float d1, int iMaxDecimals);
	//overload it without Max Decimals, so set MaxDecimals to 10:
	void strcpyfrom(char *sNum, register double d1);
	void strcpyfrom(char *sNum, register double d1, int iMaxDecimals);
	//overload it without Max Decimals, so set MaxDecimals to 10:
	void strcpyfrom(char *sNum, register long double d1);
	void strcpyfrom(char *sNum, register long double d1, int iMaxDecimals);

	int IntOfChar(char *cDigitX);
	bool substrcpy(char* sDest, char* sSrc, int iStart, int iLen);
	bool Crop(char* &sVal, int iStart);
	int IntOfString(char *sNum);
	double DoubleOfString(char *sNum);

	float FloatOfString(char *sNum);

	bool IsExt(char *sFull1, char *sExt1);
	bool ExistsAt(char* sHaystack, char* sNeedle, int iAtHaystack);

	///#region (these functions were de-inlined to avoid the gcc 3.4 "undefined reference" bug)
	void Crop(float &fToModify, float fMin, float fMax);
	LPIPOINT IPOINTFROM(float xNow, float yNow);
	float SafeAngle360(float fToLimitBetweenZeroAnd360);
	double SafeAngle360(double fToLimitBetweenZeroAnd360);
	decimal SafeAngle360(decimal fToLimitBetweenZeroAnd360);
	void SafeAngle360ByRef(float &fToLimitBetweenZeroAnd360);
	void SafeAngle360ByRef(double &fToLimitBetweenZeroAnd360);
	void RConvert_RectToPolar(int &r,int &theta, int X, int Y);
	void RConvert_RectToPolar(float &r,float &theta, float X, float Y);
	void RConvert_RectToPolar(double &r,double &theta, double X, double Y);
	float RConvert_ToFloat(float val);
	float RConvert_ToFloat(double val);
	float RConvert_ToFloat(decimal val);
	float RConvert_THETAOFXY_RAD(float x, float y);
	double RConvert_THETAOFXY_RAD(double x, double y);
	decimal RConvert_THETAOFXY_RAD(decimal x, decimal y);
	float RConvert_THETAOFXY_DEG(float x, float y);
	double RConvert_THETAOFXY_DEG(double x, double y);
	decimal RConvert_THETAOFXY_DEG(decimal x, decimal y);
	float RConvert_ROFXY(float x, float y);
	double RConvert_ROFXY(double x, double y);
	decimal RConvert_ROFXY(decimal x, decimal y);
	float FANGLEDIFF(float f1, float f2); //returns >=0 <360 formerly -180 to 180
	double DANGLEDIFF(double f1, double f2); //returns >=0 <360 formerly -180 to 180
	float ANGLEDIFFPOSITIVE(float f1, float f2); //returns 0 to 180
	double ANGLEDIFFPOSITIVE(double f1, double f2); //returns 0 to 180
	float FPDIST(float x1, float y1, float x2,  float y2);
	double DPDIST(double x1, double y1, double x2, double y2);
	long double LDANGLE360(long double d1);
	long double LDANGLEDIFF(long double d1, long double d2); //returns 0 to 180
	float DIST3D(Mass3d &pointA, Mass3d &pointB);
	float DIST3D(FPOINT3D &pointA, FPOINT3D &pointB);
	float DIST3D(FPOINT3D &pointA, Mass3d &pointB);
	float DIST3D(Mass3d &pointA, FPOINT3D &pointB);
	float DIST3D(DPOINT3D &pointA, DPOINT3D &pointB);
	float DIST3D(float x1, float y1, float z1, float x2, float y2, float z2);
	///#region 3d movement
	bool Travel3d(float &xToMove, float &yToMove, float &zToMove, float fPitch, float fYaw, float fDistanceTravel, bool make_orientation_Z_UP, bool make_orientation_X_FORWARD);
	bool Travel3d(Mass3d &pointToSlide, float fPitch, float fYaw, float fDistanceTravel, bool make_orientation_Z_UP, bool make_orientation_X_FORWARD);
	bool Travel3dTowardDest_UsingDistance(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravel);
	bool Travel3dTowardDest_UsingRatio(Mass3d &pointToSlide, Mass3d &pointDest, float fDistanceTravelRatio);
	//bool Travel3d(double &xToMove, double &yToMove, double &zToMove, double fPitch, double fYaw, double fDistanceTravel);
	///#endregion 3d movement

	void Travel2d_Polar_Deg(float &changeThisX, float &changeThisY, float r, float theta, bool InvertY);
	void Travel2d_Polar_Rad(float &changeThisX, float &changeThisY, float r, float theta, bool InvertY);
	void Rotate(float &xToMove, float &yToMove, float xCenter, float yCenter, float fRotate);
	void SafeFree(byte* &refToFreeAndSetToNull, string DebugNote);
	void SafeFree(char* &refToFreeAndSetToNull, string DebugNote);
	void SafeFree(int* &refToFreeAndSetToNull, string DebugNote);
	void SafeFree(int** &refToFreeAndSetToNull, int iIndeces, string DebugNote);
	bool stringerrors(char *sz, Uint32 u32MaxBuff);
	float FSQUARED(float val);
	float DSQUARED(double val);
	int IndexOf(char* sHaystack, char* sNeedle);
	//int SafePow(int basenum, int exp);
	//long SafePow(long basenum, int exp);
	//float SafePow(float basenum, int exp);
	//double SafePow(double basenum, int exp);
	//int SafeE10I(int exp);
	//long SafeE10L(int exp);
	//float SafeE10F(int exp);
	//double SafeE10D(int exp);
	byte SafeByRoundF(float val);
	byte SafeByte(float val);
	int SafeAddWrappedPositiveOnly(int val1, int val2);
	///#endregion (these functions were de-inlined to avoid the gcc 3.4 "undefined reference" bug)
	int Base_GetTicks_Absolute();
	UInt32 Base_GetTicks_Relative();
	UInt32 Base_TicksSince_Relative(int iTickCount);
	REAL Base_GetSeconds_Absolute();
	REAL Base_SecondsSince_Absolute(REAL rSecondCount);
	REAL Base_GetSeconds_Relative();
	REAL Base_SecondsSince_Relative(REAL rSecondCount);
	float MetersToMoveThisManyMS(float fMetersPerSecondX, int iForThisManyMilliseconds);
	double MetersToMoveThisManyMS(double fMetersPerSecondX, int iForThisManyMilliseconds);
	float DegreesToMoveThisManyMS(float fDegreesPerSecondX, int iForThisManyMilliseconds);
	double DegreesToMoveThisManyMS(double fDegreesPerSecondX, int iForThisManyMilliseconds);
	float MetersToMoveThisManyS(float fMetersPerSecondX, float rSeconds);
	double MetersToMoveThisManyS(double fMetersPerSecondX, double rSeconds);
	float DegreesToMoveThisManyS(float fDegreesPerSecondX, float rSeconds);
	double DegreesToMoveThisManyS(double fDegreesPerSecondX, double rSeconds);
	float get_scale_by_percentage_of_smallest_screen_axis(int original_w, int original_h, float smallest_axis_size_hint, int screen_w, int screen_h);

	///<summary>
	/// the machine epsilon has to be scaled to the magnitude of the values used
	/// and multiplied by the desired precision in ULPs (units in the last place)
	/// unless the result is subnormal
	/// (this description and function is based on the c++-11 example page for almost_equal from http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon )
	///</summary>
	template<class T>
	bool //typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
		almost_equal(T x, T y, int ulp)
	{
		return std::abs(x-y) < std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
		// unless the result is subnormal
			   || std::abs(x-y) < std::numeric_limits<T>::min();
	}
	template<class T>
	bool //typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
		almost_equal(T x, T y)
	{
		return almost_equal(x,y,2); /// 2 so something like 1.000000000000001 and 1.000000000000003 are equal
	}

	///#region globals defined in base.h
	extern int iErrors;
	extern int iMaxErrors;
	extern byte by3dAlphaLookup[256][256][256];
	extern bool bDebug;//CHANGED during init to the value of the debugmode script variable
	extern bool bMegaDebug; //true for debug only!
	extern int RString_iDecimalPlacesForToString;
	extern bool VisualDebugEnable; //formerly IsRealtimeDebugScreenOn
	extern string global_msg;
	extern float epsilon_tweaked_float;
	///#endregion globals defined in base.h
} //end namespace
#endif

