#ifndef RMATH_H
#define RMATH_H

#include <frameworkdummy.h>
#include <RTypes.h>
#include <string>


using namespace std;
using namespace System;
using namespace ExpertMultimediaBase;

namespace RMath {
	extern const int IntersectionError;//=-1;
	extern const int IntersectionNo;//=0;
	extern const int IntersectionYes;//=1;
	extern const int IntersectionBeyondSegment;//=2;
	extern const int LineRelationshipNone;//=0;
	extern const int LineRelationshipParallelDifferentLine;//=1;
	extern const int LineRelationshipParallelSameLine;//=2;
	extern const int LineRelationshipIntersectionNotTouchingEndsOfLineB;//=3;
	extern const int LineRelationshipLineBPoint1IsOnLineA;//=4;
	extern const int LineRelationshipLineBPoint2IsOnLineA;//=5;
	extern const int LineRelationshipThisOrHigherNotCrossingInRange;  // =100;
	extern const int LineRelationshipIntersectionNotCrossingInRangeCouldNotFinish;//=100;
	extern const int LineRelationshipIntersectionNotCrossingInRangeWouldIntersect;//=101;
	extern const int LineRelationshipIntersectionNotCrossingInRangeWouldntIntersect;//=102;
	extern const float FPI;//=3.1415926535897932384626433832795f;
	extern const double DPI;//=3.1415926535897932384626433832795d;
	extern const decimal MPI;//=3.1415926535897932384626433832795m;
	//extern const IPoint ipZero;
	///#region can't be const, since passed by ref
	extern int i10;//=10;
	extern long l10;//=10L;
	extern float f10;//=10.0F;
	extern double d10;//=10.0D;
	extern decimal m10;//=(decimal)10.0;
	///#endregion can't be const, since passed by ref
	///#region fixfloat
	//extern const float MaxFloat=;
	//extern const float MinFloatAbsVal=;
	//extern const double MaxDouble;
	///#endregion fixfloat
	///#region bits
	/*
	extern const uint BIT1		;//=1;
	extern const uint BIT2		;//=2;
	extern const uint BIT3		;//=4;
	extern const uint BIT4		;//=8;
	extern const uint BIT5		;//=16;
	extern const uint BIT6		;//=32;
	extern const uint BIT7		;//=64;
	extern const uint BIT8		;//=128;
	extern const uint BIT9		;//=256;
	extern const uint BIT10		;//=512;
	extern const uint BIT11		;//=1024;
	extern const uint BIT12		;//=2048;
	extern const uint BIT13		;//=4096;
	extern const uint BIT14		;//=8192;
	extern const uint BIT15		;//=16384;
	extern const uint BIT16		;//=32768;
	extern const uint BIT17		;//=65536;
	extern const uint BIT18		;//=131072;
	extern const uint BIT19		;//=262144;
	extern const uint BIT20		;//=524288;
	extern const uint BIT21		;//=1048576;
	extern const uint BIT22		;//=2097152;
	extern const uint BIT23		;//=4194304;
	extern const uint BIT24		;//=8388608;
	extern const uint BIT25		;//=16777216;
	extern const uint BIT26		;//=33554432;
	extern const uint BIT27		;//=67108864;
	extern const uint BIT28		;//=134217728;
	extern const uint BIT29		;//=268435456;
	extern const uint BIT30		;//=536870912;
	extern const uint BIT31		;//=1073741824;
	extern const uint BIT32		;//=2147483648;
	*/
	///#endregion bits
	///#region IRound
	extern const float f_int_MaxValue;//=(float)int.MaxValue;
	extern const float f_int_MaxValue_minus_point5;//=(float)int.MaxValue-.5f;
	extern const float f_int_MaxValue_minus_1;//=(float)int.MaxValue-1.0f;
	extern const float f_int_MinValue;//=(float)int.MinValue;
	extern const float f_int_MinValue_plus_point5;//=(float)int.MinValue+.5f;
	extern const float f_int_MinValue_plus_1;//=(float)int.MinValue+1.0f;
	extern const double d_int_MaxValue;//=(double)int.MaxValue;
	extern const double d_int_MaxValue_minus_point5;//=(double)int.MaxValue-.5d;
	extern const double d_int_MaxValue_minus_1;//=(double)int.MaxValue-1.0d;
	extern const double d_long_MaxValue_minus_1;//=(double)long.MaxValue-1.0d;
	extern const double d_int_MinValue;//=(double)int.MinValue;
	extern const double d_int_MinValue_plus_point5;//=(double)int.MinValue+.5d;
	extern const double d_int_MinValue_plus_1;//=(double)int.MinValue+1.0d;
	extern const double d_long_MinValue_plus_1;//=(double)long.MinValue+1.0d;
	extern const decimal m_int_MaxValue;//=(decimal)int.MaxValue;
	extern const decimal m_int_MaxValue_minus_point5;//=(decimal)int.MaxValue-M_0_5;
	extern const decimal m_int_MaxValue_minus_1;//=(decimal)int.MaxValue-1.0m;
	extern const decimal m_long_MaxValue_minus_1;//=(decimal)long.MaxValue-1.0m;
	extern const decimal m_int_MinValue;//=(decimal)int.MinValue;
	extern const decimal m_int_MinValue_plus_point5;//=(decimal)int.MinValue+M_0_5;
	extern const decimal m_int_MinValue_plus_1;//=(decimal)int.MinValue+1.0m;
	extern const decimal m_long_MinValue_plus_1;//=(decimal)long.MinValue+1.0m;
	///#endregion IRound


	byte SafeAverage(byte by1, byte by2, byte by3);
	float SafeAdd(float var1, float var2);
	double SafeSubtract(double var, double subtract);
	double SafeAdd(double var1, double var2);
	int SafeSubtract(int var, int subtract);
	int SafeAdd(int var1, int var2);
	byte SafeSubtract(byte var, byte subtract);
	byte SafeSubtract_Math(byte var1, byte var2);
	byte SafeAdd(byte var1, byte var2);
	byte SafeAdd_Math(byte var1, byte var2);
	ushort SafeSubtract(ushort var1, ushort var2);
	ushort SafeAdd(ushort var1, ushort var2);
	uint SafeSubtract(uint var1, uint var2);
	uint SafeAdd(uint var1, uint var2);
	ulong SafeSubtract(ulong var1, ulong var2);
	ulong SafeAdd(ulong var1, ulong var2);
	byte SafeAddWrapped(byte by1, byte by2);
	byte SafeSubtractWrapped(byte by1, byte by2);
	ulong SafeAddWrapped(ulong var1, ulong var2);
	int SafeAddWrappedTowardZero(int var1, int var2);
	float SafeSubtract(float var, float subtract);
	void SafeSignChangeByRef(ref_int val);
	void SafeSignChangeByRef(ref_long val);
	void SafeSignChangeByRef(ref_float val);
	void SafeSignChangeByRef(ref_double val);
	void SafeSignChangeByRef(ref_decimal val);
	byte SafeMultiply(byte val1, byte val2);
	byte SafeMultiply_Math(byte val1, byte val2);
	int SafeMultiply(int val1, int val2);
	float SafeMultiply(float val1, float val2);
	double SafeMultiply(double val1, double val2);
	//float SafeAngle360(float valNow);
	//double SafeAngle360(double valNow);
	decimal SafeAngle360(decimal valNow);
	string IntersectionToString(int IntersectionA);
	string LineRelationshipToString(int LineRelationshipA);
	void ToRectPoints(ref_int x1, ref_int y1, ref_int x2, ref_int y2);
	void ToRectPoints(ref_float x1, ref_float y1, ref_float x2, ref_float y2);
	void ToRectPoints(ref_double x1, ref_double y1, ref_double x2, ref_double y2);
	bool IsInBox(int x, int y, int x1, int y1, int x2, int y2);
	bool IsInBox(float x, float y, float x1, float y1, float x2, float y2);
	bool IsInBox(double x, double y, double x1, double y1, double x2, double y2);
	bool IsInBoxEx(int x, int y, int x1, int y1, int x2, int y2);
	bool IsInBoxEx(float x, float y,float x1, float y1, float x2, float y2);
	bool IsInBoxEx(double x, double y, double x1, double y1, double x2, double y2);
	void OrderPointsLR(ref_int x1, ref_int y1, ref_int x2, ref_int y2);
	void OrderPointsLR(ref_float x1, ref_float y1, ref_float x2, ref_float y2);
	void OrderPointsLR(ref_double x1, ref_double y1, ref_double x2, ref_double y2);
	bool PointIsOnLine(int x, int y, int x1, int y1, int x2, int y2, bool bLinePointOrderHasBeenFixedAlready);
	bool PointIsOnLine(int x, int y, int x1, int y1, int x2, int y2, bool bLinePointOrderHasBeenFixedAlready, bool bOnlyTrueIfOnSegment);
	bool PointIsOnLine(int x, int y, int x1, int y1, int x2, int y2, bool bLinePointOrderHasBeenFixedAlready, bool bOnlyTrueIfOnSegment, int line_r, int line_theta);
	int Intersection(out_int x, out_int y, ILine line1, ILine line2);
	int Intersection(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2);
	int Intersection(out_float x,out_float y, float Line1_x1, float Line1_y1, float Line1_x2, float Line1_y2, float Line2_x1, float Line2_y1, float Line2_x2, float Line2_y2);
	int Intersection(out_double x,out_double y, double Line1_x1, double Line1_y1, double Line1_x2, double Line1_y2, double Line2_x1, double Line2_y1, double Line2_x2, double Line2_y2);
	int Intersection(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2, bool bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1);
	int Intersection(out_float x, out_float y, float Line1_x1, float Line1_y1, float Line1_x2, float Line1_y2, float Line2_x1, float Line2_y1, float Line2_x2, float Line2_y2, bool bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1);
	int Intersection(out_double x, out_double y, double Line1_x1, double Line1_y1, double Line1_x2, double Line1_y2, double Line2_x1, double Line2_y1, double Line2_x2, double Line2_y2, bool bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1);
	int SafeIntersection(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2);
	int SafeIntersection(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2, bool bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1);
	int IntersectionAndRelationship(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2);
	int IntersectionAndRelationship(out_float x, out_float y, float Line1_x1, float Line1_y1, float Line1_x2, float Line1_y2, float Line2_x1, float Line2_y1, float Line2_x2, float Line2_y2);
	int IntersectionAndRelationship(out_double x, out_double y, double Line1_x1, double Line1_y1, double Line1_x2, double Line1_y2, double Line2_x1, double Line2_y1, double Line2_x2, double Line2_y2);
	float SafeAtan2Radians(float y, float x);
	//REAL SafeAtan2Radians(REAL y, REAL x);
	double SafeAtan2Radians(double y, double x);
	int AlignC(int widthOuter, int widthInner);//formerly Centered
	float AlignC(float widthOuter, float widthInner);
	double AlignC(double widthOuter, double widthInner);
	decimal AlignC(decimal widthOuter, decimal widthInner);
	int Length(int iStart, int iInclusiveEnder);
	int LengthEx(int iStart, int iExclusiveEnder);
	int Wrap(int iWrap, int length);
	float Wrap(float val, float start, float endexclusive);
	double Wrap(double val, double start, double endexclusive);
	decimal Wrap(decimal val, decimal start, decimal endexclusive);
	decimal Floor(decimal val);//debug performance VERY SLOW
	int GetSignedCropped(uint uiNow);
	ushort GetUnsignedLossless(short val);
	int GetCircleIntersectionPoints(ref_DPoint p1, ref_DPoint p2, double c1_X, double c1_Y, double c1_Radius, double c2_X, double c2_Y, double c2_Radius); //from "int Circle::getIntersectionPoints( Circle c, VecPosition *p1, VecPosition *p2);" <http://staff.science.uva.nl/~jellekok/robocup/2003/html/Geometry_8cpp-source.html> 2009-08-14
	void GetPointOnLineByRatio(ref_DPoint dpReturn, DPoint p1, DPoint p2, double dRatioTo1); //VecPosition VecPosition::getVecPositionOnLineFraction( VecPosition &p,
	double GetAreaOfCirclesIntersection(double c1_X, double c1_Y, double c1_Radius, double c2_X, double c2_Y, double c2_Radius); //from "double Circle::getIntersectionArea( Circle c );//" <http://staff.science.uva.nl/~jellekok/robocup/2003/html/Geometry_8cpp-source.html> 2009-08-14
	byte Dist(byte by1, byte by2);
	double Dist(double x1, double y1, double x2, double y2);
	float Dist(float x1, float y1, float x2, float y2);
	int Dist1D(int x1, int x2);
	int Dist(int x1, int y1, int x2, int y2);
	float Dist(ref_FPoint p1, ref_FPoint p2);
	double Dist(ref_DPoint p1, ref_DPoint p2);
	decimal Dist(ref_MPoint p1, ref_MPoint p2);
	float SafeDivide(float val, float valDivisor, float valMax);
	double SafeDivide(double val, double valDivisor, double valMax);
	decimal SafeDivide(decimal val, decimal valDivisor, decimal valMax);
	float SafeDivide(float val, float valDivisor, float valMax, float valMin);
	double SafeDivide(double val, double valDivisor, double valMax, double valMin);
	decimal SafeDivide(decimal val, decimal valDivisor, decimal valMax, decimal valMin);
	int SafeAbs(int val);
	void SafeAbsByRef(ref_int val);
	float SafeAbs(float val);
	void SafeAbsByRef(ref_float val);
	int Negative(int val);
	void NegativeByRef(ref_int val);
	int SafeDivideRound(int val, int valDivisor, int valMax);
	int SafeDivideRound(int val, int valDivisor, int valMax, int valMin);
	int SafeDivide(int val, int valDivisor, int valMax);
	int SafeDivide(int val, int valDivisor, int valMax, int valMin);
	float FixFloat(float var);
	double FixDouble(double var);
	uint Bit(int iBit);
	int SafeE10I(int exp);
	long SafeE10L(int exp);
	float SafeE10F(ref_int exp);
	double SafeE10D(ref_int exp);
	decimal SafeE10M(ref_int exp);
	int SafePow(int basenum, int exp);
	//int SafePow(ref_int basenum, ref_int exp);
	uint SafePow(uint basenum, int exp);
	//uint SafePow(ref_uint basenum, ref_int exp);
	long SafePow(long basenum, int exp);
	//long SafePow(ref_long basenum, ref_int exp);
	float SafePow(float basenum, int exp);
	//float SafePow(ref_float basenum, ref_int exp);
	double SafePow(double basenum, int exp);
	//double SafePow(ref_double basenum, ref_int exp);
	decimal SafePow(decimal basenum, int exp);
	//decimal SafePow(ref_decimal basenum, ref_int exp);
	void Rotate(ref_float xToMove, ref_float yToMove, float xCenter, float yCenter, float rRotate);
	void Rotate(ref_double xToMove, ref_double yToMove, double xCenter, double yCenter, double rRotate);
	void Rotate(ref_float xToMove, ref_float yToMove, float rRotate);
	void Rotate(ref_double xToMove, ref_double yToMove, double rRotate);
	float Round(float val);
	byte Approach(byte start, byte toward, byte factor);
	byte Approach(byte start, byte toward, float factor);
	byte Approach(byte start, byte toward, double factor);
	float Approach(float start, float toward, float factor);
	double Approach(double start, double toward, double factor);
	//float Mod(float num, float div);
	float Mod(float val, float divisor); //formerly FMOD
	double Mod(double val, double divisor); //formerly DMOD
	/*
	void Ceiling(ref_float val);
	float CeilingRefToNonRef(ref_float val);
	float Ceiling(float val);
	void Ceiling(ref_decimal val);
	void Ceiling(ref_double val);
	double CeilingRefToNonRef(ref_double val);
	float Floor(float val);
	double Floor(double num);//use System_Math_Floor, which has double and decimal
	float FloorRefToNonRef(ref_float num);
	void Floor(ref_float num);
	void Floor(ref_double num);
	double FloorRefToNonRef(ref_double num);
	*/
	byte LOWNIBBLE(byte by);
	byte HIGHNIBBLE(byte by);
	void SETLOWNIBBLE(ref_byte byTarget, byte byNibbleValueMustBeLessthan16);
	void SETHIGHNIBBLE(ref_byte byTarget, byte byNibbleValueMustBeLessthan16);
	byte ByRound(float val);
	byte ByRound(double val);
	int IRound(float val);
	int IRound(double val);
	int IRound(decimal val);
	int ICeiling(float val);
	int ICeiling(double val);
	long LCeiling(double val);
	void CropAbsoluteValueToPosMax(ref_int val);//formerly PrepareToBePos
	void CropAbsoluteValueToPosMax(ref_long val);
	void CropAbsoluteValueToPosMax(ref_float val);
	void CropAbsoluteValueToPosMax(ref_double val);
	void CropAbsoluteValueToPosMax(ref_decimal val);
	float SafeSqrt(float val);
	double SafeSqrt(double val);
	float Sqrt(float val);
	decimal Sqrt(decimal val);
	decimal SafeSqrt(decimal val);
	//REAL SafeSqrt(REAL val);
	int SafeSqrt(int val);
	long SafeSqrt(long val);
	//ulong SafeSqrt(ulong val);
	//int DaysAbsoluteValueOfDifferenceIgnoringHours(DateTime datetime1, DateTime datetime2); //formerly DaysAbsoluteDifferenceIgnoringHours
	//int DaysDifferenceIgnoringHours(DateTime datetime1, DateTime datetime2);
	string ToString(IPoint& point);
	string ToString(FPoint& point);
	string ToString(DPoint& point);
	//int IndexOf(int arrVal[], int valX);
	//string RemoveExpNotation(string sNum);
	void RemoveExpNotation(ref_string sNum);
	// 		int LocationToFuzzyMaximum(int iCurrentMax, int iLoc);
	float FractionPartOf(float val);
	double FractionPartOf(double val);
	/*
	int MinVal(int arrVal[]);
	int MaxVal(int arrVal[]);
	int MinPosVal(int arrVal[]);
	int MaxPosVal(int arrVal[]);
	*/
	//int CropAndGetLossEx(ref_int iToChange, int iMin, int iExclusiveMax);
	void CropEx(ref_int iToChange, int iMin, int iExclusiveMax);
	void CropZone(ref_int zone_Left, ref_int zone_Top, ref_int zone_Right, ref_int zone_Bottom, int Boundary_X, int Boundary_Y, int Boundary_Width, int Boundary_Height);
	// 		void CropRect(ref_int rect_X, ref_int rect_Y, ref_int rect_Width, ref_int rect_Height, int Boundary_X, int Boundary_Y, int Boundary_Width, int Boundary_Height);
	///<summary>
	///Crops the RectToModify to the Boundary rect INCLUSIVELY
	///Returns false if there is nothing left of the RectToModify after cropping.
	///</summary>	bool CropRect(ref_int RectToModify_X, ref_int RectToModify_Y, ref_int RectToModify_Width, ref_int RectToModify_Height, int Boundary_X, int Boundary_Y, int Boundary_Width, int Boundary_Height);
	bool CropRect(ref_float RectToModify_X, ref_float RectToModify_Y, ref_float RectToModify_Width, ref_float RectToModify_Height, float Boundary_X, float Boundary_Y, float Boundary_Width, float Boundary_Height);
	bool CropZone(ref_float ZoneToModify_X, ref_float ZoneToModify_Y, ref_float ZoneToModify_Right, ref_float ZoneToModify_Bottom, float Boundary_X, float Boundary_Y, float Boundary_Right, float Boundary_Bottom);
	bool CropRect(ref_double RectToModify_X, ref_double RectToModify_Y, ref_double RectToModify_Width, ref_double RectToModify_Height, double Boundary_X, double Boundary_Y, double Boundary_Width, double Boundary_Height);
	bool CropRect(ref_decimal RectToModify_X, ref_decimal RectToModify_Y, ref_decimal RectToModify_Width, ref_decimal RectToModify_Height, decimal Boundary_X, decimal Boundary_Y, decimal Boundary_Width, decimal Boundary_Height);
	//void CropRect(ref_int rect_X, ref_int rect_Y, ref_int rect_Width, ref_int rect_Height, int Boundary_X, int Boundary_Y, int Boundary_Width, int Boundary_Height);
	void AutoSize(out_int iWidthInner, out_int iHeightInner, int iWidthOuter, int iHeightOuter, float fPercent);
	int AutoInnerPosition(int iSizeOuter, float fPercent);
}//end RMath

#endif
