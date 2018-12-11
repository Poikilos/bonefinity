#ifndef RMATH_CPP
#define RMATH_CPP

#include <frameworkdummy.h>
#include <RMath.h>
#include <preporting.h>

using namespace std;
using namespace ExpertMultimediaBase;

//////#region RMath
namespace RMath {
	const int IntersectionError=-1;
	const int IntersectionNo=0;
	const int IntersectionYes=1;
	const int IntersectionBeyondSegment=2;
	const int LineRelationshipNone=0;
	const int LineRelationshipParallelDifferentLine=1;
	const int LineRelationshipParallelSameLine=2;
	const int LineRelationshipIntersectionNotTouchingEndsOfLineB=3;
	const int LineRelationshipLineBPoint1IsOnLineA=4;
	const int LineRelationshipLineBPoint2IsOnLineA=5;
	const int LineRelationshipThisOrHigherNotCrossingInRange=100;
	const int LineRelationshipIntersectionNotCrossingInRangeCouldNotFinish=100;
	const int LineRelationshipIntersectionNotCrossingInRangeWouldIntersect=101;
	const int LineRelationshipIntersectionNotCrossingInRangeWouldntIntersect=102;
	const float FPI=3.1415926535897932384626433832795f;
	const double DPI=3.1415926535897932384626433832795d;
	const decimal MPI=(decimal)3.1415926535897932384626433832795;
	//const IPoint ipZero;
	///#region can't be const, since passed by ref
	int i10=10;
	long l10=10L;
	float f10=10.0F;
	double d10=10.0D;
	decimal m10=(decimal)10.0;
	///#endregion can't be const, since passed by ref
	///#region fixfloat
	//const float MaxFloat=;
	//const float MinFloatAbsVal=;
	//const double MaxDouble;
	///#endregion fixfloat
	///#region bits
	/*
	const uint BIT1		=1;
	const uint BIT2		=2;
	const uint BIT3		=4;
	const uint BIT4		=8;
	const uint BIT5		=16;
	const uint BIT6		=32;
	const uint BIT7		=64;
	const uint BIT8		=128;
	const uint BIT9		=256;
	const uint BIT10		=512;
	const uint BIT11		=1024;
	const uint BIT12		=2048;
	const uint BIT13		=4096;
	const uint BIT14		=8192;
	const uint BIT15		=16384;
	const uint BIT16		=32768;
	const uint BIT17		=65536;
	const uint BIT18		=131072;
	const uint BIT19		=262144;
	const uint BIT20		=524288;
	const uint BIT21		=1048576;
	const uint BIT22		=2097152;
	const uint BIT23		=4194304;
	const uint BIT24		=8388608;
	const uint BIT25		=16777216;
	const uint BIT26		=33554432;
	const uint BIT27		=67108864;
	const uint BIT28		=134217728;
	const uint BIT29		=268435456;
	const uint BIT30		=536870912;
	const uint BIT31		=1073741824;
	const uint BIT32		=2147483648;
	///#endregion bits
	*/
	///#region IRound
	const float f_int_MaxValue=(float)int_MaxValue;
	const float f_int_MaxValue_minus_point5=(float)int_MaxValue-.5f;
	const float f_int_MaxValue_minus_1=(float)int_MaxValue-1.0f;
	const float f_int_MinValue=(float)int_MinValue;
	const float f_int_MinValue_plus_point5=(float)int_MinValue+.5f;
	const float f_int_MinValue_plus_1=(float)int_MinValue+1.0f;
	const double d_int_MaxValue=(double)int_MaxValue;
	const double d_int_MaxValue_minus_point5=(double)int_MaxValue-.5d;
	const double d_int_MaxValue_minus_1=(double)int_MaxValue-1.0d;
	const double d_long_MaxValue_minus_1=(double)long_MaxValue-1.0d;
	const double d_int_MinValue=(double)int_MinValue;
	const double d_int_MinValue_plus_point5=(double)int_MinValue+.5d;
	const double d_int_MinValue_plus_1=(double)int_MinValue+1.0d;
	const double d_long_MinValue_plus_1=(double)long_MinValue+1.0d;
	const decimal m_int_MaxValue=(decimal)int_MaxValue;
	const decimal m_int_MaxValue_minus_point5=(decimal)int_MaxValue-M_0_5;
	const decimal m_int_MaxValue_minus_1=(decimal)int_MaxValue-M_1;
	const decimal m_long_MaxValue_minus_1=(decimal)long_MaxValue-M_1;
	const decimal m_int_MinValue=(decimal)int_MinValue;
	const decimal m_int_MinValue_plus_point5=(decimal)int_MinValue+M_0_5;
	const decimal m_int_MinValue_plus_1=(decimal)int_MinValue+M_1;
	const decimal m_long_MinValue_plus_1=(decimal)long_MinValue+M_1;
	///#endregion IRound
	byte SafeAverage(byte by1, byte by2, byte by3) {
		return (byte)SafeDivideRound((int)by1+(int)by2+(int)by3,3,255);
	}
	float SafeAdd(float var1, float var2) {
		if (var1<0) {
			if (var2<0) { // -v1 + -v2
				if (var2>=float_MinValue-var1) return var1+var2; //less confusing than: if (float_MinValue-var1<=var2) return var1+var2;
					//--i.e. if MinValue were -1,000,000
					// and v1==-999,999 and v2==-1.1
					// ( -1,000,000 - -999,9999 ) == -1.1
					// if ( -1.1 >= -1 ) (would be fine if v2 were -1 -- would result in -999,999+-1 = -1,000,000
					// but truth is ( -1.1 < -1 ) which would cause overflow so return MinValue instead
				else return float_MinValue;
			}
			else { // -v1 + +v2 (var2>=0)
				return var1+var2; //impossible to overflow since one is negative
			}
		}
		else { // +v1 (var1>=0)
			if (var2<0) { // +v1 + -v2
				return var1+var2;
			}
			else { // +v1 + +v2 (var2>=0)
				if (float_MaxValue-var1>var2) return var1+var2;
				else return float_MaxValue;
			}
		}
	}//end SafeAdd
	double SafeSubtract(double var, double subtract) {
		CropAbsoluteValueToPosMax(subtract);
		return SafeAdd(var, -1*subtract);
	}
	double SafeAdd(double var1, double var2) {
		if (var1<0) {
			if (var2<0) { // -v1 + -v2
				if (var2>=double_MinValue-var1) return var1+var2; //less confusing than: if (float_MinValue-var1<=var2) return var1+var2;
					//--i.e. if MinValue were -1,000,000
					// and v1==-999,999 and v2==-1.1
					// ( -1,000,000 - -999,9999 ) == -1.1
					// if ( -1.1 >= -1 ) (would be fine if v2 were -1 -- would result in -999,999+-1 = -1,000,000
					// but truth is ( -1.1 < -1 ) which would cause overflow so return MinValue instead
				else return double_MinValue;
			}
			else {  // -v1 + +v2 (var2>=0)
				return var1+var2; //impossible to overflow since one is negative
			}
		}
		else { // +v1 (var1>=0)
			if (var2<0) { // +v1 + -v2
				return var1+var2;
			}
			else { // +v1 + +v2 (var2>=0)
				if (double_MaxValue-var1>var2) return var1+var2;
				else return double_MaxValue;
			}
		}
	}//end SafeAdd
	int SafeSubtract(int var, int subtract) {
		CropAbsoluteValueToPosMax(subtract);
		return SafeAdd(var, -1*subtract);
	}
	int SafeAdd(int var1, int var2) {
		if (var1<0) {
			if (var2<0) {
				if (var2>=int_MinValue-var1) return var1+var2; //less confusing than: if (float_MinValue-var1<=var2) return var1+var2;
					//--i.e. if MinValue were -1,000,000
					// and v1==-999,999 and v2==-1.1
					// ( -1,000,000 - -999,9999 ) == -1.1
					// if ( -1.1 >= -1 ) (would be fine if v2 were -1 -- would result in -999,999+-1 = -1,000,000
					// but truth is ( -1.1 < -1 ) which would cause overflow so return MinValue instead
				else return int_MinValue;
			}
			else { //var2>=0
				return var1+var2; //impossible to overflow since one is negative
			}
		}
		else { //var1>=0
			if (var2<0) {
				return var1+var2;
			}
			else { //var2>=0
				if (int_MaxValue-var1>var2) return var1+var2;
				else return int_MaxValue;
			}
		}
	}//end SafeAdd

	byte SafeSubtract(byte var, byte subtract) {
		return SafeSubtract_Math(var,subtract);//SubtractBytes[(int)var][(int)subtract];
	}
	byte SafeSubtract_Math(byte var1, byte var2) {
		if (var1>var2) return (byte)(var1-var2);
		else return 0;
	}
	byte SafeAdd(byte var1, byte var2) {
		return SafeAdd_Math(var1,var2);//AddBytes[var1][var2];
	}
	byte SafeAdd_Math(byte var1, byte var2) {
		if (byte_MaxValue-var1>var2) return (byte)(var1+var2);
		else return byte_MaxValue;
	}//end SafeAdd byte
	ushort SafeSubtract(ushort var1, ushort var2) {
		if (var1>var2) return (ushort)(var1-var2);
		else return 0;
	}
	ushort SafeAdd(ushort var1, ushort var2) {
		if (ushort_MaxValue-var1>var2) return (ushort)(var1+var2);
		else return ushort_MaxValue;
	}//end SafeAdd ushort
	uint SafeSubtract(uint var1, uint var2) {
		if (var1>var2) return var1-var2;
		else return 0;
	}
	uint SafeAdd(uint var1, uint var2) {
		if (uint_MaxValue-var1>var2) return var1+var2;
		else return uint_MaxValue;
	}//end SafeAdd uint
	ulong SafeSubtract(ulong var1, ulong var2) {
		if (var1>var2) return var1-var2;
		else return 0;
	}
	ulong SafeAdd(ulong var1, ulong var2) {
		if (ulong_MaxValue-var1>var2) return var1+var2;
		//i.e. if MaxValue was 10000
		//i.e. 9999 + 2
		// 10000-9999=1 > 2 NOT TRUE so don't do 9999+2 (return 10000)
		//i.e. 9999 + 1
		// 10000-9999=1 > 1 NOT TRUE but that's ok return MaxValue anyway
		//i.e. 9999 + 0
		// 10000-9999=1 > 0 IS TRUE so do 9999+0
		//i.e. 2 + 9999
		// 10000-2=9998 > 9998 NOT TRUE so don't do 2+9998 (return 10000)
		//i.e. 1 + 9998
		// 10000-1=9999 > 9999 NOT TRUE so don't do 1+9999 (return 10000)
		//i.e. 0 + 9999
		// 10000-0=10000 > 9999 IS TRUE so do 0+9999
		else return ulong_MaxValue;
	}//end SafeAdd ulong
	byte SafeAddWrapped(byte by1, byte by2) {
		int iReturn=(int)by1+(int)by2;
		if (iReturn>255) iReturn-=256; //i.e. 256 - 256 = 0
		return (byte)iReturn;
	}
	byte SafeSubtractWrapped(byte by1, byte by2) {
		int iReturn=(int)by1-(int)by2;
		if (iReturn<0) iReturn+=256; //i.e. -1 + 256 = 255
		return (byte)iReturn;
	}
	ulong SafeAddWrapped(ulong var1, ulong var2) {
		if (ulong_MaxValue-var1>var2) return var1+var2;
		else if (var1>var2) {
			//i.e. 9999+2
			//9999>2 so do [10000-9999==1] 2-1
			//i.e. 9998+3
			//9998>3 so do [10000-9998=2] 3-2`
			return var2-(ulong_MaxValue-var1);
		}
		else { //else var1<=var2
			//i.e. 2+9999
			//9999>2 NOT TRUE so do [10000-9999==1] 2-1
			//i.e. 9998+3
			//9998>3 NOT TRUE so do [10000-9998=2] 3-2
			return var1-(ulong_MaxValue-var2);
		}
	}
	int SafeAddWrappedTowardZero(int var1, int var2) {
		if (var1<0) {
			if (var2<0) {
				//if (int_MinValue-var1>var2) return var1+var2;
				if (var2>=int_MinValue-var1) return var1+var2; //less confusing than: if (float_MinValue-var1<=var2) return var1+var2;
					//--i.e. if MinValue were -1,000,000
					// and v1==-999,999 and v2==-1.1
					// ( -1,000,000 - -999,9999 ) == -1.1
					// if ( -1.1 >= -1 ) (would be fine if v2 were -1 -- would result in -999,999+-1 = -1,000,000
					// but truth is ( -1.1 < -1 ) which would cause overflow so return MinValue instead
				else return (var1-int_MinValue)+var2;//wrap toward zero
			}
			else { //var2>=0
				return var1+var2;
			}
		}
		else { //var1>=0
			if (var2<0) {
				return var1+var2;
			}
			else { //var2>=0
				if (int_MaxValue-var1>var2) return var1+var2;
				else {
					return (var1-int_MaxValue)+var2;//(var1+var2)-(int_MaxValue);
				}
			}
		}
	}//end SafeAddWrappedTowardZero
	float SafeSubtract(float var, float subtract) {
		CropAbsoluteValueToPosMax(subtract);
		return SafeAdd(var, -1*subtract);
	}
	void SafeSignChangeByRef(ref_int val) {
		if (val==int_MinValue) val=int_MaxValue;
		else val*=-1;
	}
	void SafeSignChangeByRef(ref_long val) {
		if (val==long_MinValue) val=long_MaxValue;
		else val*=-1;
	}
	void SafeSignChangeByRef(ref_float val) {
		if (val==float_MinValue) val=float_MaxValue;
		else val*=-1;
	}
	void SafeSignChangeByRef(ref_double val) {
		if (val==double_MinValue) val=double_MaxValue;
		else val*=-1;
	}
	void SafeSignChangeByRef(ref_decimal val) {
		if (val==decimal_MinValue) val=decimal_MaxValue;
		else val*=-1;
	}
	byte SafeMultiply(byte val1, byte val2) {
		return SafeMultiply_Math(val1,val2);//MultiplyBytes[val1][val2];
	}
	byte SafeMultiply_Math(byte val1, byte val2) {
		//byte valReturn=0;
		int iVal2=(int)val2;
		if (SafeDivide(255,val2,255)>=val1) {//== allowed since result floored i.e. if SafeMultiply(127,2), 255/127 results in 2 so allow 127*2 (254)
			return SafeByte(val1*val2);//RConvert_ToByte(val1*val2);
		}
		else {
			return 255;
			//for (int iNow=0; iNow<iVal2; iNow++) {
			//	valReturn=SafeAdd_Math(valReturn,val1);//i.e. if val2 is 1, this happens once
			//}
		}
		//return valReturn;
	}//end int SafeMultiply_Math
	int SafeMultiply(int val1, int val2) {
		int valReturn=0;
		bool bNeg=false;
		if (val1<0) {
			SafeSignChangeByRef(val1);
			bNeg=!bNeg;
		}
		if (val2<0) {
			SafeSignChangeByRef(val2);
			bNeg=!bNeg;
		}
		if (SafeDivide(int_MaxValue,val2,int_MaxValue)>=val1) {//== allowed since result floored
			valReturn=val1*val2;
		}
		else {//TODO: finish this -- just needs to be tested for overflow
			if (bNeg) {
				valReturn=int_MinValue;
				bNeg=false;
			}
			else valReturn=int_MaxValue;
			//for (int iNow=0; iNow<val2; iNow++) {
			//	valReturn=SafeAdd(valReturn, val1);//i.e. if val2 is 1, this happens once
			//}
		}
		if (bNeg) SafeSignChangeByRef(valReturn);
		return valReturn;
	}//end int SafeMultiply
	float SafeMultiply(float val1, float val2) {
		float valReturn=0;
		bool bNeg=false;
		if (val1<0) {
			SafeSignChangeByRef(val1);
			bNeg=!bNeg;
		}
		if (val2<0) {
			SafeSignChangeByRef(val2);
			bNeg=!bNeg;
		}
		if (SafeDivide(float_MaxValue,val2,float_MaxValue)>val1) {
			valReturn=val1*val2;
		}
		else {
			if (bNeg) {
				valReturn=float_MinValue;
				bNeg=false;
			}
			else valReturn=float_MaxValue;
			//int iLimiter=(int)val2;
			//for (int iNow=0; iNow<iLimiter; iNow++) {
			//	valReturn=SafeAdd(valReturn, val1);//i.e. if val2 is one, this happens once
			//}
			//valReturn=SafeAdd(valReturn, val1*(val2-(float)iLimiter));//remainder
		}
		if (bNeg) SafeSignChangeByRef(valReturn);
		return valReturn;
	}//end float SafeMultiply
	double SafeMultiply(double val1, double val2) {
		double valReturn=0;
		bool bNeg=false;
		if (val1<0) {
			SafeSignChangeByRef(val1);
			bNeg=!bNeg;
		}
		if (val2<0) {
			SafeSignChangeByRef(val2);
			bNeg=!bNeg;
		}
		if (SafeDivide(double_MaxValue,val2,double_MaxValue)>val1) {
			valReturn=val1*val2;
		}
		else {
			if (bNeg) {
				valReturn=double_MinValue;
				bNeg=false;
			}
			else valReturn=double_MaxValue;
			//int iLimiter=(int)val2;
			//for (int iNow=0; iNow<iLimiter; iNow++) {
			//	valReturn=SafeAdd(valReturn, val1);//i.e. if val2 is one, this happens once
			//}
			//valReturn=SafeAdd(valReturn, val1*(val2-(double)iLimiter));//remainder
		}
		if (bNeg) SafeSignChangeByRef(valReturn);
		return valReturn;
	}//end double SafeMultiply



	float SafeAngle360(float valNow) {
		return Wrap(valNow, 0, 360);
	}
	double SafeAngle360(double valNow) {
		return Wrap(valNow, 0, 360);
	}
	decimal SafeAngle360(decimal valNow) {
		return Wrap(valNow, 0, 360);
	}
	string IntersectionToString(int IntersectionA) {
		if (IntersectionA==IntersectionYes) return "Intersection Found";
		else if (IntersectionA==IntersectionNo) return "No Intersection";
		else if (IntersectionA==IntersectionBeyondSegment) return "Lines Only Intersect Beyond Segment";
		else if (IntersectionA==IntersectionError) return "Intersection Error";
		else return "Unknown Intersection Type #"+RString_ToString(IntersectionA);
	}//IntersectionToString
	string LineRelationshipToString(int LineRelationshipA) {
		if (LineRelationshipA==LineRelationshipNone) return "None";
		else if (LineRelationshipA==LineRelationshipParallelDifferentLine) return "Parallel Different Line";
		else if (LineRelationshipA==LineRelationshipParallelSameLine) return "Parallel Same Line";
		else if (LineRelationshipA==LineRelationshipIntersectionNotTouchingEndsOfLineB) return "IntersectionNotTouchingEndsOfLineB";
		else if (LineRelationshipA==LineRelationshipLineBPoint1IsOnLineA) return "LineBPoint1IsOnLineA";
		else if (LineRelationshipA==LineRelationshipLineBPoint2IsOnLineA) return "LineBPoint2IsOnLineA";
		else if (LineRelationshipA==LineRelationshipIntersectionNotCrossingInRangeWouldIntersect) return "NotCrossingInRangeWouldIntersect";
		else if (LineRelationshipA==LineRelationshipIntersectionNotCrossingInRangeWouldntIntersect) return "NotCrossingInRangeWouldntIntersect";
		else if (LineRelationshipA==LineRelationshipIntersectionNotCrossingInRangeCouldNotFinish) return "NotCrossingInRangeCouldNotFinish";

		else return "Unknown Line Relationship Type #"+RString_ToString(LineRelationshipA);
	}


	///#region geometry
	void ToRectPoints(ref_int x1, ref_int y1, ref_int x2, ref_int y2) {
		if (y2<y1) {int temp=y1; y1=y2; y2=temp;}//RMemory_Swap(y1, y2);
		if (x2<x1) {int temp=x1; x1=x2; x2=temp;}//RMemory_Swap(x1, x2);
	}
	void ToRectPoints(ref_float x1, ref_float y1, ref_float x2, ref_float y2) {
		if (y2<y1) {float temp=y1; y1=y2; y2=temp;}//RMemory_Swap(y1, y2);
		if (x2<x1) {float temp=x1; x1=x2; x2=temp;}//RMemory_Swap(x1, x2);
	}
	void ToRectPoints(ref_double x1, ref_double y1, ref_double x2, ref_double y2) {
		if (y2<y1) {double temp=y1; y1=y2; y2=temp;}//RMemory_Swap(y1, y2);
		if (x2<x1) {double temp=x1; x1=x2; x2=temp;}//RMemory_Swap(x1, x2);
	}
	bool IsInBox(int x, int y, int x1, int y1, int x2, int y2) {
		ToRectPoints(x1, y1, x2, y2);
		return (x>=x1 && x<=x2 && y>=y1 && y<=y2);
	}
	bool IsInBox(float x, float y, float x1, float y1, float x2, float y2) {
		ToRectPoints(x1, y1, x2, y2);
		return (x>=x1 && x<=x2 && y>=y1 && y<=y2);
	}
	bool IsInBox(double x, double y, double x1, double y1, double x2, double y2) {
		ToRectPoints(x1, y1, x2, y2);
		return (x>=x1 && x<=x2 && y>=y1 && y<=y2);
	}
	bool IsInBoxEx(int x, int y, int x1, int y1, int x2, int y2) {
		ToRectPoints(x1, y1, x2, y2);
		return (x>=x1 && x<x2 && y>=y1 && y<y2);
	}
	bool IsInBoxEx(float x, float y, float x1, float y1, float x2, float y2) {
		ToRectPoints(x1, y1, x2, y2);
		return (x>=x1 && x<x2 && y>=y1 && y<y2);
	}
	bool IsInBoxEx(double x, double y, double x1, double y1, double x2, double y2) {
		ToRectPoints(x1, y1, x2, y2);
		return (x>=x1 && x<x2 && y>=y1 && y<y2);
	}
	///<summary>
	///makes point 2 to the right (may remain above or below point 1)
	///</summary>
	void OrderPointsLR(ref_int x1, ref_int y1, ref_int x2, ref_int y2) {
		//if (y1==y2) {
		//	if (x2<x1) RMemory.Swap(x1, x2);
		//}
		//else if (x1==x2) {
		//	if (y2<y1) RMemory.Swap(y1, y2);
		//}
		//else
		if (x2<x1) {
			int temp=y1; y1=y2; y2=temp;//RMemory.Swap(y1, y2);
			temp=x1; x1=x2; x2=temp;//RMemory.Swap(x1, x2);
		}
		//else if (y2<y1) {
		//	RMemory.Swap(y1, y2);
		//	RMemory.Swap(x1, x2);
		//}
	}//end OrderPointsLR
		///<summary>
	///makes point 2 to the right (may remain above or below point 1)
	///</summary>
	void OrderPointsLR(ref_float x1, ref_float y1, ref_float x2, ref_float y2) {
		//if (y1==y2) {
		//	if (x2<x1) RMemory.Swap(x1, x2);
		//}
		//else if (x1==x2) {
		//	if (y2<y1) RMemory.Swap(y1, y2);
		//}
		//else
		if (x2<x1) {
			float temp=y1; y1=y2; y2=temp;//RMemory.Swap(y1, y2);
			temp=x1; x1=x2; x2=temp;//RMemory.Swap(x1, x2);
		}
		//else if (y2<y1) {
		//	RMemory.Swap(y1, y2);
		//	RMemory.Swap(x1, x2);
		//}
	}//end OrderPointsLR
		///<summary>
	///makes point 2 to the right (may remain above or below point 1)
	///</summary>
	void OrderPointsLR(ref_double x1, ref_double y1, ref_double x2, ref_double y2) {
		//if (y1==y2) {
		//	if (x2<x1) RMemory.Swap(x1, x2);
		//}
		//else if (x1==x2) {
		//	if (y2<y1) RMemory.Swap(y1, y2);
		//}
		//else
		if (x2<x1) {
			double temp=y1; y1=y2; y2=temp;//RMemory.Swap(y1, y2);
			temp=x1; x1=x2; x2=temp;//RMemory.Swap(x1, x2);
		}
		//else if (y2<y1) {
		//	RMemory.Swap(y1, y2);
		//	RMemory.Swap(x1, x2);
		//}
	}//end OrderPointsLR
	bool PointIsOnLine(int x, int y, int x1, int y1, int x2, int y2, bool bLinePointOrderHasBeenFixedAlready) {
		return PointIsOnLine(x,y,x1,y1,x2,y2,bLinePointOrderHasBeenFixedAlready,true);
	}

	bool PointIsOnLine(int x, int y, int x1, int y1, int x2, int y2, bool bLinePointOrderHasBeenFixedAlready, bool bOnlyTrueIfOnSegment) {
		return PointIsOnLine(x, y, x1, y1, x2, y2, bLinePointOrderHasBeenFixedAlready, bOnlyTrueIfOnSegment, 0, 0);
	}
	///<summary>
	///send line_r of zero in order to calculate r and theta for relative line polar coords.
	///</summary>
	bool PointIsOnLine(int x, int y, int x1, int y1, int x2, int y2, bool bLinePointOrderHasBeenFixedAlready, bool bOnlyTrueIfOnSegment, int line_r, int line_theta) {
		if (!bLinePointOrderHasBeenFixedAlready) OrderPointsLR(x1, y1, x2, y2);
		int relative_r,relative_theta;
		bool bReturn=false;
		if (bOnlyTrueIfOnSegment) {
			if (!IsInBox(x,y,x1,x2,y1,y2))
				return false;//MUST force return now for this to work
		}
		if (line_r==0) RConvert_RectToPolar(line_r, line_theta, x2-x1, y2-y1);
		OrderPointsLR(x1, y1, x, y);//makes point 2 to the right (may be above or below)
		RConvert_RectToPolar(relative_r, relative_theta, x-x1, y-y1);//this is right --subtraction is right since x was placed AFTER x1
		if (relative_theta==line_theta) bReturn=true;
		return bReturn;
	}//end PointIsOnLine
	int Intersection(out_int x, out_int y, ILine line1, ILine line2) {
		return Intersection(x, y, line1.X1, line1.Y1, line1.X2, line1.Y2,
													 line2.X1, line2.Y1, line2.X2, line2.Y2);
	}
	int Intersection(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2) {
		return Intersection(x, y, Line1_x1, Line1_y1, Line1_x2, Line1_y2, Line2_x1, Line2_y1, Line2_x2, Line2_y2, true);
	}
	int Intersection(out_float x, out_float y, float Line1_x1, float Line1_y1, float Line1_x2, float Line1_y2, float Line2_x1, float Line2_y1, float Line2_x2, float Line2_y2) {
		return Intersection(x, y, Line1_x1, Line1_y1, Line1_x2, Line1_y2, Line2_x1, Line2_y1, Line2_x2, Line2_y2, true);
	}
	int Intersection(out_double x, out_double y, double Line1_x1, double Line1_y1, double Line1_x2, double Line1_y2, double Line2_x1, double Line2_y1, double Line2_x2, double Line2_y2) {
		return Intersection(x, y, Line1_x1, Line1_y1, Line1_x2, Line1_y2, Line2_x1, Line2_y1, Line2_x2, Line2_y2, true);
	}
	int Intersection(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2, bool bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1) {
		x=0;
		y=0;
		int iReturn=IntersectionError;
		try {

			int Line1_A, Line1_B, Line1_C, Line2_A, Line2_B, Line2_C;//line in format Ax+Bx==C
			Line1_A = Line1_y2-Line1_y1;
			Line1_B = Line1_x1-Line1_x2;
			Line1_C = Line1_A*Line1_x1+Line1_B*Line1_y1;
			Line2_A = Line2_y2-Line2_y1;
			Line2_B = Line2_x1-Line2_x2;
			Line2_C = Line2_A*Line2_x1+Line2_B*Line2_y1;
			int iDeterminant = Line1_A*Line2_B - Line2_A*Line1_B;
			if (iDeterminant==0) {
				iReturn=IntersectionNo;
			}
			else {
				x=(Line2_B*Line1_C - Line1_B*Line2_C)/iDeterminant;
				y=(Line1_A*Line2_C - Line2_A*Line1_C)/iDeterminant;
				if (bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1) {
					if (  IsInBox(x,y,Line1_x1,Line1_y1,Line1_x2,Line1_y2)
						&& IsInBox(x,y,Line2_x1,Line2_y1,Line2_x2,Line2_y2) ) iReturn=IntersectionYes;
					else iReturn=IntersectionBeyondSegment;
				}
				else iReturn=IntersectionYes;
			}
		}
		catch (exception& exn) {
			iReturn=IntersectionError;
			PReporting::ShowExn(exn,"Base Intersection","checking line intersection {"
				+(string)"Line1:("+RString_ToString( Line1_x1)+","+RString_ToString( Line1_y1)+")to("+RString_ToString( Line1_x2)+","+RString_ToString( Line1_y2)+"); "
				+(string)"Line2:("+RString_ToString( Line2_x1)+","+RString_ToString( Line2_y1)+")to("+RString_ToString( Line2_x2)+","+RString_ToString( Line2_y2)+"); "
				+(string)"}");
		}
		return iReturn;
	}//end Intersection
	int Intersection(out_float x, out_float y, float Line1_x1, float Line1_y1, float Line1_x2, float Line1_y2, float Line2_x1, float Line2_y1, float Line2_x2, float Line2_y2, bool bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1) {
		x=0;
		y=0;
		int iReturn=IntersectionError;
		try {

			float Line1_A, Line1_B, Line1_C, Line2_A, Line2_B, Line2_C;//line in format Ax+Bx==C
			Line1_A = Line1_y2-Line1_y1;
			Line1_B = Line1_x1-Line1_x2;
			Line1_C = Line1_A*Line1_x1+Line1_B*Line1_y1;
			Line2_A = Line2_y2-Line2_y1;
			Line2_B = Line2_x1-Line2_x2;
			Line2_C = Line2_A*Line2_x1+Line2_B*Line2_y1;
			float iDeterminant = Line1_A*Line2_B - Line2_A*Line1_B;
			if (iDeterminant==0.0d) {
				iReturn=IntersectionNo;
			}
			else {
				x=(Line2_B*Line1_C - Line1_B*Line2_C)/iDeterminant;
				y=(Line1_A*Line2_C - Line2_A*Line1_C)/iDeterminant;
				if (bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1) {
					if (  IsInBox(x,y,Line1_x1,Line1_y1,Line1_x2,Line1_y2)
						&& IsInBox(x,y,Line2_x1,Line2_y1,Line2_x2,Line2_y2) ) iReturn=IntersectionYes;
					else iReturn=IntersectionBeyondSegment;
				}
				else iReturn=IntersectionYes;
			}
		}
		catch (exception& exn) {
			iReturn=IntersectionError;
			PReporting::ShowExn(exn,"Base Intersection","checking line intersection {"
				+(string)"Line1:("+RString_ToString( Line1_x1)+","+RString_ToString( Line1_y1)+")to("+RString_ToString( Line1_x2)+","+RString_ToString( Line1_y2)+"); "
				+(string)"Line2:("+RString_ToString( Line2_x1)+","+RString_ToString( Line2_y1)+")to("+RString_ToString( Line2_x2)+","+RString_ToString( Line2_y2)+"); "
				+(string)"}");
		}
		return iReturn;
	}//end Intersection(float,...)
	int Intersection(out_double x, out_double y, double Line1_x1, double Line1_y1, double Line1_x2, double Line1_y2, double Line2_x1, double Line2_y1, double Line2_x2, double Line2_y2, bool bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1) {
		x=0;
		y=0;
		int iReturn=IntersectionError;
		try {

			double Line1_A, Line1_B, Line1_C, Line2_A, Line2_B, Line2_C;//line in format Ax+Bx==C
			Line1_A = Line1_y2-Line1_y1;
			Line1_B = Line1_x1-Line1_x2;
			Line1_C = Line1_A*Line1_x1+Line1_B*Line1_y1;
			Line2_A = Line2_y2-Line2_y1;
			Line2_B = Line2_x1-Line2_x2;
			Line2_C = Line2_A*Line2_x1+Line2_B*Line2_y1;
			double iDeterminant = Line1_A*Line2_B - Line2_A*Line1_B;
			if (iDeterminant==0.0d) {
				iReturn=IntersectionNo;
			}
			else {
				x=(Line2_B*Line1_C - Line1_B*Line2_C)/iDeterminant;
				y=(Line1_A*Line2_C - Line2_A*Line1_C)/iDeterminant;
				if (bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1) {
					if (  IsInBox(x,y,Line1_x1,Line1_y1,Line1_x2,Line1_y2)
						&& IsInBox(x,y,Line2_x1,Line2_y1,Line2_x2,Line2_y2) ) iReturn=IntersectionYes;
					else iReturn=IntersectionBeyondSegment;
				}
				else iReturn=IntersectionYes;
			}
		}
		catch (exception& exn) {
			iReturn=IntersectionError;
			PReporting::ShowExn(exn,"Base Intersection","checking line intersection {"
				+(string)"Line1:("+RString_ToString( Line1_x1)+","+RString_ToString( Line1_y1)+")to("+RString_ToString( Line1_x2)+","+RString_ToString( Line1_y2)+"); "
				+(string)"Line2:("+RString_ToString( Line2_x1)+","+RString_ToString( Line2_y1)+")to("+RString_ToString( Line2_x2)+","+RString_ToString( Line2_y2)+"); "
				+(string)"}");
		}
		return iReturn;
	}//end Intersection(double,...)
	int SafeIntersection(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2) {
		return SafeIntersection(x, y, Line1_x1, Line1_y1, Line1_x2, Line1_y2, Line2_x1, Line2_y1, Line2_x2, Line2_y2, true);
	}
	int SafeIntersection(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2, bool bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1) {
		x=0;
		y=0;
		int iReturn=IntersectionError;//should NEVER happen!
		int Line1_A, Line1_B, Line1_C, Line2_A, Line2_B, Line2_C;//line in format Ax+Bx==C
		Line1_A = SafeSubtract(Line1_y2,Line1_y1);
		Line1_B = SafeSubtract(Line1_x1,Line1_x2);
		Line1_C = SafeAdd(SafeMultiply(Line1_A,Line1_x1),SafeMultiply(Line1_B,Line1_y1));
		Line2_A = SafeSubtract(Line2_y2,Line2_y1);
		Line2_B = SafeSubtract(Line2_x1,Line2_x2);
		Line2_C = SafeAdd(SafeMultiply(Line2_A,Line2_x1),SafeMultiply(Line2_B,Line2_y1));
		int iDeterminant=SafeSubtract(SafeMultiply(Line1_A,Line2_B),SafeMultiply(Line2_A,Line1_B));
		if (iDeterminant==0) {
			iReturn=IntersectionNo;
		}
		else {
			x=SafeDivide( SafeSubtract(SafeMultiply(Line2_B,Line1_C),SafeMultiply(Line1_B,Line2_C)) , iDeterminant,int_MaxValue );
			y=SafeDivide( SafeSubtract(SafeMultiply(Line1_A,Line2_C),SafeMultiply(Line2_A,Line1_C)), iDeterminant,int_MaxValue );
			if (bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1) {
				if (  IsInBox(x,y,Line1_x1,Line1_y1,Line1_x2,Line1_y2)
					&& IsInBox(x,y,Line1_x1,Line1_y1,Line1_x2,Line1_y2) ) iReturn=IntersectionYes;
				else iReturn=IntersectionBeyondSegment;
			}
			else iReturn=IntersectionYes;
		}
		return iReturn;
	}//end SafeIntersection

	//TODO: test IntersectionAndRelationship
	//#ifndef FloatAsInt
	//#define FloatAsInt(realnumber,precisionmultiplier) (  (int)( (realnumber)*(precisionmultiplier)+.5f )  )
	//#endif
	int IntersectionAndRelationship(out_float x, out_float y, float Line1_x1, float Line1_y1, float Line1_x2, float Line1_y2, float Line2_x1, float Line2_y1, float Line2_x2, float Line2_y2) {
	//	int ixReturn=(int)(rPrecisionMultiplier_WhenConvRealToInt*Line1_x1+.5d),iyReturn=(int)(rPrecisionMultiplier_WhenConvRealToInt*Line1_y1+.5d); //+.5 to round //default value
	//	int iReturn=IntersectionAndRelationship(ixReturn,iyReturn,floatAsInt(Line1_x1,rPrecisionMultiplier_WhenConvRealToInt), floatAsInt(Line1_y1,rPrecisionMultiplier_WhenConvRealToInt), floatAsInt(Line1_x2,rPrecisionMultiplier_WhenConvRealToInt), floatAsInt(Line1_y2,rPrecisionMultiplier_WhenConvRealToInt), floatAsInt(Line2_x1,rPrecisionMultiplier_WhenConvRealToInt), floatAsInt(Line2_y1,rPrecisionMultiplier_WhenConvRealToInt), floatAsInt(Line2_x2,rPrecisionMultiplier_WhenConvRealToInt), floatAsInt(Line2_y2,rPrecisionMultiplier_WhenConvRealToInt));
	//	x=(float)ixReturn/rPrecisionMultiplier_WhenConvRealToInt;
	//	y=(float)iyReturn/rPrecisionMultiplier_WhenConvRealToInt;
	//	return iReturn;
		float Line1_r, Line1_theta, Line2_r, Line2_theta, relative_r, relative_theta;
		int iIntersection=Intersection(x, y, Line1_x1, Line1_y1, Line1_x2, Line1_y2, Line2_x1, Line2_y1, Line2_x2, Line2_y2);
		x=0; y=0;
		if (iIntersection!=IntersectionYes) {//Line1_theta==Line2_theta
			if (Line1_x1==Line2_x2&&Line1_y1==Line2_y2) {
				return LineRelationshipParallelSameLine;
			}
			else {
				RConvert_RectToPolar(Line1_r, Line1_theta, Line1_x2-Line1_x1, Line1_y2-Line1_y1);
				RConvert_RectToPolar(relative_r, relative_theta, Line2_x2-Line1_x1, Line2_y2-Line1_y1);
				if (relative_theta==Line1_theta) {
					return LineRelationshipParallelSameLine;//another way of finding if same line
				}
			}
			return LineRelationshipParallelDifferentLine;
		}
		else {
			OrderPointsLR(Line1_x1, Line1_y1, Line1_x2, Line1_y2);
			OrderPointsLR(Line2_x1, Line2_y1, Line2_x2, Line2_y2);
			RConvert_RectToPolar(Line1_r, Line1_theta, Line1_x2-Line1_x1, Line1_y2-Line1_y1);
			RConvert_RectToPolar(Line2_r, Line2_theta, Line2_x2-Line2_x1, Line2_y2-Line2_y1);
			try {
				if (RMath::PointIsOnLine(Line2_x1,Line2_y1,Line1_x1,Line1_y1,Line1_x2,Line1_y2,true,true,Line1_theta,Line1_r)) {
					x=Line2_x1;
					y=Line2_y1;
					return LineRelationshipLineBPoint1IsOnLineA;
				}
				else if (PointIsOnLine(Line2_x2,Line2_y2,Line1_x1,Line1_y1,Line1_x2,Line1_y2,true,true,Line1_theta,Line1_r)) {
					x=Line2_x2;
					y=Line2_y2;
					return LineRelationshipLineBPoint2IsOnLineA;
				}
				else if (iIntersection==IntersectionYes) {
					if (IsInBox(x,y,Line1_x1,Line1_y1,Line1_x2,Line1_y2)&&IsInBox(x,y,Line2_x1,Line2_y1,Line2_x2,Line2_y2)) return LineRelationshipIntersectionNotTouchingEndsOfLineB;
					else return LineRelationshipIntersectionNotCrossingInRangeWouldIntersect;
				}
				else {
					return LineRelationshipIntersectionNotCrossingInRangeWouldntIntersect;
				}
			}
			catch (exception& exn) {
				PReporting::ShowExn(exn,(string)"Base IntersectionAndRelationship","checking line relationship {"
					+(string)"Line1:("+RString_ToString( Line1_x1)+","+RString_ToString( Line1_y1)+")to("+RString_ToString( Line1_x2)+","+RString_ToString( Line1_y2)+"); "
					+(string)"Line2:("+RString_ToString( Line2_x1)+","+RString_ToString( Line2_y1)+")to("+RString_ToString( Line2_x2)+","+RString_ToString( Line2_y2)+"); "
					+(string)"}");
				return LineRelationshipIntersectionNotCrossingInRangeCouldNotFinish;
			}
		}
		return LineRelationshipNone;
	}//end IntersectionAndRelationship(float,...)
	int IntersectionAndRelationship(out_double x, out_double y, double Line1_x1, double Line1_y1, double Line1_x2, double Line1_y2, double Line2_x1, double Line2_y1, double Line2_x2, double Line2_y2) {
	//	int ixReturn=(int)(rPrecisionMultiplier_WhenConvRealToInt*Line1_x1+.5d),iyReturn=(int)(rPrecisionMultiplier_WhenConvRealToInt*Line1_y1+.5d); //+.5 to round //default value
	//	int iReturn=IntersectionAndRelationship(ixReturn,iyReturn,DoubleAsInt(Line1_x1,rPrecisionMultiplier_WhenConvRealToInt), DoubleAsInt(Line1_y1,rPrecisionMultiplier_WhenConvRealToInt), DoubleAsInt(Line1_x2,rPrecisionMultiplier_WhenConvRealToInt), DoubleAsInt(Line1_y2,rPrecisionMultiplier_WhenConvRealToInt), DoubleAsInt(Line2_x1,rPrecisionMultiplier_WhenConvRealToInt), DoubleAsInt(Line2_y1,rPrecisionMultiplier_WhenConvRealToInt), DoubleAsInt(Line2_x2,rPrecisionMultiplier_WhenConvRealToInt), DoubleAsInt(Line2_y2,rPrecisionMultiplier_WhenConvRealToInt));
	//	x=(double)ixReturn/rPrecisionMultiplier_WhenConvRealToInt;
	//	y=(double)iyReturn/rPrecisionMultiplier_WhenConvRealToInt;
	//	return iReturn;
		double Line1_r, Line1_theta, Line2_r, Line2_theta, relative_r, relative_theta;
		int iIntersection=Intersection(x, y, Line1_x1, Line1_y1, Line1_x2, Line1_y2, Line2_x1, Line2_y1, Line2_x2, Line2_y2);
		x=0; y=0;
		if (iIntersection!=IntersectionYes) {//Line1_theta==Line2_theta
			if (Line1_x1==Line2_x2&&Line1_y1==Line2_y2) {
				return LineRelationshipParallelSameLine;
			}
			else {
				RConvert_RectToPolar(Line1_r, Line1_theta, Line1_x2-Line1_x1, Line1_y2-Line1_y1);
				RConvert_RectToPolar(relative_r, relative_theta, Line2_x2-Line1_x1, Line2_y2-Line1_y1);
				if (relative_theta==Line1_theta) {
					return LineRelationshipParallelSameLine;//another way of finding if same line
				}
			}
			return LineRelationshipParallelDifferentLine;
		}
		else {
			OrderPointsLR(Line1_x1, Line1_y1, Line1_x2, Line1_y2);
			OrderPointsLR(Line2_x1, Line2_y1, Line2_x2, Line2_y2);
			RConvert_RectToPolar(Line1_r, Line1_theta, Line1_x2-Line1_x1, Line1_y2-Line1_y1);
			RConvert_RectToPolar(Line2_r, Line2_theta, Line2_x2-Line2_x1, Line2_y2-Line2_y1);
			try {
				if (RMath::PointIsOnLine(Line2_x1,Line2_y1,Line1_x1,Line1_y1,Line1_x2,Line1_y2,true,true,Line1_theta,Line1_r)) {
					x=Line2_x1;
					y=Line2_y1;
					return LineRelationshipLineBPoint1IsOnLineA;
				}
				else if (PointIsOnLine(Line2_x2,Line2_y2,Line1_x1,Line1_y1,Line1_x2,Line1_y2,true,true,Line1_theta,Line1_r)) {
					x=Line2_x2;
					y=Line2_y2;
					return LineRelationshipLineBPoint2IsOnLineA;
				}
				else if (iIntersection==IntersectionYes) {
					if (IsInBox(x,y,Line1_x1,Line1_y1,Line1_x2,Line1_y2)&&IsInBox(x,y,Line2_x1,Line2_y1,Line2_x2,Line2_y2)) return LineRelationshipIntersectionNotTouchingEndsOfLineB;
					else return LineRelationshipIntersectionNotCrossingInRangeWouldIntersect;
				}
				else {
					return LineRelationshipIntersectionNotCrossingInRangeWouldntIntersect;
				}
			}
			catch (exception& exn) {
				PReporting::ShowExn(exn,(string)"Base IntersectionAndRelationship","checking line relationship {"
					+(string)"Line1:("+RString_ToString( Line1_x1)+","+RString_ToString( Line1_y1)+")to("+RString_ToString( Line1_x2)+","+RString_ToString( Line1_y2)+"); "
					+(string)"Line2:("+RString_ToString( Line2_x1)+","+RString_ToString( Line2_y1)+")to("+RString_ToString( Line2_x2)+","+RString_ToString( Line2_y2)+"); "
					+(string)"}");
				return LineRelationshipIntersectionNotCrossingInRangeCouldNotFinish;
			}
		}
		return LineRelationshipNone;
	}//end IntersectionAndRelationship(double,...)
	int IntersectionAndRelationship(out_int x, out_int y, int Line1_x1, int Line1_y1, int Line1_x2, int Line1_y2, int Line2_x1, int Line2_y1, int Line2_x2, int Line2_y2) {
		int Line1_r, Line1_theta, Line2_r, Line2_theta, relative_r, relative_theta;
		int iIntersection=Intersection(x, y, Line1_x1, Line1_y1, Line1_x2, Line1_y2, Line2_x1, Line2_y1, Line2_x2, Line2_y2);
		x=0; y=0;
		if (iIntersection!=IntersectionYes) {//Line1_theta==Line2_theta
			if (Line1_x1==Line2_x2&&Line1_y1==Line2_y2) {
				return LineRelationshipParallelSameLine;
			}
			else {
				RConvert_RectToPolar(Line1_r, Line1_theta, Line1_x2-Line1_x1, Line1_y2-Line1_y1);
				RConvert_RectToPolar(relative_r, relative_theta, Line2_x2-Line1_x1, Line2_y2-Line1_y1);
				if (relative_theta==Line1_theta) {
					return LineRelationshipParallelSameLine;//another way of finding if same line
				}
			}
			return LineRelationshipParallelDifferentLine;
		}
		else {
			OrderPointsLR(Line1_x1, Line1_y1, Line1_x2, Line1_y2);
			OrderPointsLR(Line2_x1, Line2_y1, Line2_x2, Line2_y2);
			RConvert_RectToPolar(Line1_r, Line1_theta, Line1_x2-Line1_x1, Line1_y2-Line1_y1);
			RConvert_RectToPolar(Line2_r, Line2_theta, Line2_x2-Line2_x1, Line2_y2-Line2_y1);
			try {
				if (RMath::PointIsOnLine(Line2_x1,Line2_y1,Line1_x1,Line1_y1,Line1_x2,Line1_y2,true,true,Line1_theta,Line1_r)) {
					x=Line2_x1;
					y=Line2_y1;
					return LineRelationshipLineBPoint1IsOnLineA;
				}
				else if (PointIsOnLine(Line2_x2,Line2_y2,Line1_x1,Line1_y1,Line1_x2,Line1_y2,true,true,Line1_theta,Line1_r)) {
					x=Line2_x2;
					y=Line2_y2;
					return LineRelationshipLineBPoint2IsOnLineA;
				}
				else if (iIntersection==IntersectionYes) {
					if (IsInBox(x,y,Line1_x1,Line1_y1,Line1_x2,Line1_y2)&&IsInBox(x,y,Line2_x1,Line2_y1,Line2_x2,Line2_y2)) return LineRelationshipIntersectionNotTouchingEndsOfLineB;
					else return LineRelationshipIntersectionNotCrossingInRangeWouldIntersect;
				}
				else {
					return LineRelationshipIntersectionNotCrossingInRangeWouldntIntersect;
				}
			}
			catch (exception& exn) {
				PReporting::ShowExn(exn,(string)"Base IntersectionAndRelationship","checking line relationship {"
					+(string)"Line1:("+RString_ToString( Line1_x1)+","+RString_ToString( Line1_y1)+")to("+RString_ToString( Line1_x2)+","+RString_ToString( Line1_y2)+"); "
					+(string)"Line2:("+RString_ToString( Line2_x1)+","+RString_ToString( Line2_y1)+")to("+RString_ToString( Line2_x2)+","+RString_ToString( Line2_y2)+"); "
					+(string)"}");
				return LineRelationshipIntersectionNotCrossingInRangeCouldNotFinish;
			}
		}
		return LineRelationshipNone;
	}//end IntersectionAndRelationship
	/// <summary>
	/// Safely returns arctangent of y/x, correcting
	/// for the error domain.
	/// </summary>
	/// <param name="y"></param>
	/// <param name="x"></param>
	/// <returns>arctangent of y/x, correcting
	/// for the error domain</returns>
	float SafeAtan2Radians(float y, float x) {
		if (x==0&&y==0) return 0;
		else return (float)System_Math_Atan2((double)y,(double)x);//debug assumes no need for RConvert_ToFloat since started with float range (?)
	}
	//REAL SafeAtan2Radians(REAL y, REAL x) {
	//	if (x==0&&y==0) return 0;
	//	else return (REAL)System_Math_Atan2((double)y,(double)x);
	//}
	double SafeAtan2Radians(double y, double x) {
		if (x==0&&y==0) return 0;
		else return System_Math_Atan2(y,x);
	}
	///#region geometry

	///#region math
	int AlignC(int widthOuter, int widthInner) {//formerly Centered
		return (widthOuter-widthInner)/2;
	}
	float AlignC(float widthOuter, float widthInner) {
		return (widthOuter-widthInner)/2.0f;
	}
	double AlignC(double widthOuter, double widthInner) {
		return (widthOuter-widthInner)/2.0;
	}
	decimal AlignC(decimal widthOuter, decimal widthInner) {
		return (widthOuter-widthInner)/M_2;
	}
	//TODO: test this--all wrapping methods!
	int Length(int iStart, int iInclusiveEnder) {
		return LengthEx(iStart,iInclusiveEnder+1);
	}
	int LengthEx(int iStart, int iExclusiveEnder) {
		return iExclusiveEnder-iStart;
	}
	int Wrap(int iWrap, int length) {
		return (iWrap>=length) ? (iWrap%=length) : ((iWrap<0)?(length-((-iWrap)%length)):(iWrap)) ;
	}
	float Wrap(float val, float start, float endexclusive) {
		float range=endexclusive-start;
		if (val>=endexclusive) {
			val-=RConvert_ToFloat(range*(System_Math_Floor((val-endexclusive)/range)+1.0F));
		}
		else if (val<start) {
			val+=RConvert_ToFloat(range*(System_Math_Floor((start-val)/range)+1.0F)); //i.e. -256.5 from 0 excluding 256 is:
			//-256.5+256*(Floor((0[--a.k.a.+]256.5)/256)+1)==-256.5+256*(Floor(256.5/256)+1)==-256.5+256*(1+1)==-256.5+512==255.5
		}
		return val;
	}
	double Wrap(double val, double start, double endexclusive) {
		double range=endexclusive-start;
		if (val>=endexclusive) {
			val-=range*(System_Math_Floor((val-endexclusive)/range)+1);
		}
		else if (val<start) {
			val+=range*(System_Math_Floor((start-val)/range)+1); //i.e. -256.5 from 0 excluding 256 is:
			//-256.5+256*(Floor((0[--a.k.a.+]256.5)/256)+1)==-256.5+256*(Floor(256.5/256)+1)==-256.5+256*(1+1)==-256.5+512==255.5
		}
		return val;
	}
	decimal Wrap(decimal val, decimal start, decimal endexclusive) {
		decimal range=endexclusive-start;
		if (val>=endexclusive) {
			val-=range*(Floor((val-endexclusive)/range)+M_1);
		}
		else if (val<start) {
			val+=range*(Floor((start-val)/range)+M_1); //i.e. -256.5 from 0 excluding 256 is:
			//-256.5+256*(Floor((0[--a.k.a.+]256.5)/256)+1)==-256.5+256*(Floor(256.5/256)+1)==-256.5+256*(1+1)==-256.5+512==255.5
		}
		return val;
	}
	decimal Floor(decimal val) {//debug performance VERY SLOW

		//string sVal=RString_ToString( val);
		//RMath::RemoveExpNotation(sVal);
		//int iDot=sVal.find_first_of(".",0);
		//if (iDot!=string::npos) {
		//	sVal=sVal.substr(0,iDot);
		//	return RConvert_ToDecimal(sVal);
		//}
		//else return val;
		return System_Math_Floor(val);//debug truncates value unless updated after 2011-12-30
	}
	int GetSignedCropped(uint uiNow) {
		return (int)((uiNow>2147483647)?2147483647:uiNow);
		//1111111 11111111 11111111 11111111
	}
	ushort GetUnsignedLossless(short val) {
		if (val==short_MinValue) return ushort_MaxValue;//prevents overflow! (in -1*val below)
		else if (val<0) return (ushort)((ushort)short_MaxValue+(ushort)(-1*val));//since approaches 0x7FFF+0xFFFF (that overflow prevented above)
		else return (ushort) val;
	}
	/*
	00002 Copyright (c) 2000-2003, Jelle Kok, University of Amsterdam
	00003 All rights reserved.
	00004
	00005 Redistribution and use in source and binary forms, with or without
	00006 modification, are permitted provided that the following conditions are met:
	00007
	00008 1. Redistributions of source code must retain the above copyright notice, this
	00009 list of conditions and the following disclaimer.
	00010
	00011 2. Redistributions in binary form must reproduce the above copyright notice,
	00012 this list of conditions and the following disclaimer in the documentation
	00013 and/or other materials provided with the distribution.
	00014
	00015 3. Neither the name of the University of Amsterdam nor the names of its
	00016 contributors may be used to endorse or promote products derived from this
	00017 software without specific prior written permission.
	00018
	00019 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	00020 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	00021 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	00022 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
	00023 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	00024 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	00025 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	00026 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	00027 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	00028 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	00029 */
	int GetCircleIntersectionPoints(ref_DPoint p1, ref_DPoint p2, double c1_X, double c1_Y, double c1_Radius, double c2_X, double c2_Y, double c2_Radius) { //from "int Circle::getIntersectionPoints( Circle c, VecPosition *p1, VecPosition *p2) {" <http://staff.science.uva.nl/~jellekok/robocup/2003/html/Geometry_8cpp-source.html> 2009-08-14
		double x0, y0, r0;
		double x1, y1, r1;

		x0 = c1_X;
		y0 = c1_Y;
		r0 = c1_Radius;
		x1 = c2_X;
		y1 = c2_Y;
		r1 = c2_Radius;

		double d, dx, dy, h, a, p2_x, p2_y; //x, y,

		// first calculate distance between two centers circles P0 and P1.
		dx = x1 - x0;
		dy = y1 - y0;
		d = System_Math_Sqrt(dx*dx + dy*dy);

		// normalize differences
		dx /= d; dy /= d;

		// a is distance between p0 and point that is the intersection point P2
		// that intersects P0-P1 and the line that crosses the two intersection
		// points P3 and P4.
		// Define two triangles: P0,P2,P3 and P1,P2,P3.
		// with distances a, h, r0 and b, h, r1 with d = a + b
		// We know a^2 + h^2 = r0^2 and b^2 + h^2 = r1^2 which then gives
		// a^2 + r1^2 - b^2 = r0^2 with d = a + b ==> a^2 + r1^2 - (d-a)^2 = r0^2
		// ==> r0^2 + d^2 - r1^2 / 2*d
		a = (r0*r0 + d*d - r1*r1) / (2.0 * d);

		// h is then a^2 + h^2 = r0^2 ==> h = sqrt( r0^2 - a^2 )
		double arg = r0*r0 - a*a;
		h = (arg > 0.0) ? System_Math_Sqrt(arg) : 0.0;

		// First calculate P2
		p2_x = x0 + a * dx;
		p2_y = y0 + a * dy;

		// and finally the two intersection points
		p1.X =  p2_x - h * dy;
		p1.Y =  p2_y + h * dx;
		p2.X =  p2_x + h * dy;
		p2.Y =  p2_y - h * dx;

		return (arg < 0.0) ? 0 : ((arg == 0.0 ) ? 1 :  2);
	}//end GetCircleIntersectionPoints
	void GetPointOnLineByRatio(ref_DPoint dpReturn, DPoint p1, DPoint p2, double dRatioTo1) { //VecPosition VecPosition::getVecPositionOnLineFraction( VecPosition &p,
		// determine point on line that lies at fraction dRatioTo1 of whole line
		// example: this --- 0.25 ---------  p2
		// formula: this + dRatioTo1 * ( p2 - p1 ) = p1 - dRatioTo1 * p1 + dRatioTo1 * p2 =
		//		 ( 1 - dRatioTo1 ) * p1 + dRatioTo1 * p2

		dpReturn.X=( ( p1.X ) * ( 1.0 - dRatioTo1 ) + ( p2.X * dRatioTo1 ) );
		dpReturn.Y=( ( p1.Y ) * ( 1.0 - dRatioTo1 ) + ( p2.Y * dRatioTo1 ) );
	}
	double GetAreaOfCirclesIntersection(double c1_X, double c1_Y, double c1_Radius, double c2_X, double c2_Y, double c2_Radius) { //from "double Circle::getIntersectionArea( Circle c ) {" <http://staff.science.uva.nl/~jellekok/robocup/2003/html/Geometry_8cpp-source.html> 2009-08-14
		DPoint pos1,pos2,pos3;//=new DPoint(), pos2=new DPoint(), pos3=new DPoint();//VecPosition pos1, pos2, pos3;
		double d, h, dArea=0.0;
		double ang;//AngDeg ang;
		try {
			d = RMath::Dist(c1_X,c1_Y,c2_X,c2_Y);//getCenter().getDistanceTo( c2_Center() ); // dist between two centers
			if( d > c2_Radius + c1_Radius )           // larger than sum radii
				return 0.0;                                   // circles do not intersect
			if( d <= System_Math_Abs(c2_Radius - c1_Radius ) ) {  // one totally in the other
				double dR = System_Math_Min( c2_Radius, c1_Radius );// return area smallest circ
				return RMath::DPI*dR*dR;
			}
			int iNrSol = GetCircleIntersectionPoints(pos1, pos2, c1_X,c1_Y,c1_Radius,c2_X,c2_Y,c2_Radius);
			if( iNrSol != 2 )
				return 0.0;
			// the intersection area of two circles can be divided into two segments:
			// left and right of the line between the two intersection points p1 and p2.
			// The outside area of each segment can be calculated by taking the part
			// of the circle pie excluding the triangle from the center to the
			// two intersection points.
			// The pie equals pi*r^2 * rad(2*ang) / 2*pi = 0.5*rad(2*ang)*r^2 with ang
			// the angle between the center c of the circle and one of the two
			// intersection points. Thus the angle between c and p1 and c and p3 where
			// p3 is the point that lies halfway between p1 and p2.
			// This can be calculated using ang = asin( d / r ) with d the distance
			// between p1 and p3 and r the radius of the circle.
			// The area of the triangle is 2*0.5*h*d.
			GetPointOnLineByRatio(pos3, pos1, pos2, 0.5 );//pos3 = pos1.getVecPositionOnLineFraction( pos2, 0.5 );
			d = RMath::Dist(pos1,pos3);
			h = RMath::Dist(pos3.X, pos3.Y, c1_X, c1_Y); //pos3.getDistanceTo( getCenter() );
			ang = System_Math_Asin(d/c1_Radius);
			dArea = ang*c1_Radius*c1_Radius;
			dArea = dArea - d*h;
			// and now for the other segment the same story
			h =  RMath::Dist(pos3.X, pos3.Y, c2_X, c2_Y); //pos3.getDistanceTo( c2_Center() );
			ang = System_Math_Asin(d/c2_Radius);
			dArea = dArea + ang*c2_Radius*c2_Radius;
			dArea = dArea - d*h;
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn);
		}
		return dArea;
	}//end GetAreaOfCirclesIntersection
	byte Dist(byte by1, byte by2) {
		return (byte)( (by1>by2)?by1-by2:by2-by1 );
	}
	double Dist(double x1, double y1, double x2, double y2) {
		try {
			return SafeSqrt(System_Math_Abs(x2-x1)+System_Math_Abs(y2-y1));
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"double Dist()");
		}
		return 0;
	}
	float Dist(float x1, float y1, float x2, float y2) {
		try {
			return SafeSqrt(System_Math_Abs(x2-x1)+System_Math_Abs(y2-y1));
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"double Dist()");
		}
		return 0;
	}
	int Dist1D(int x1, int x2) {
		int valReturn=0;
		try {
			valReturn=SafeSubtract(x1,x2);
			if (valReturn<0) valReturn=SafeSubtract(0,valReturn);//still need SafeSubtract since negative and positive limits are not the same
		}
		catch (exception& exn) {
			valReturn=int_MaxValue;
		}
		return valReturn;
	}
	int Dist(int x1, int y1, int x2, int y2) {
		try {
			return SafeSqrt(System_Math_Abs(x2-x1)+System_Math_Abs(y2-y2));
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"double Dist()");
		}
		return 0;
	}
	float Dist(ref_FPoint p1, ref_FPoint p2) {
		try {
			return SafeSqrt(System_Math_Abs(p2.X-p1.X)+System_Math_Abs(p2.Y-p1.Y));
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"float Dist()");
		}
		return 0;
	}
	double Dist(ref_DPoint p1, ref_DPoint p2) {
		try {
			return SafeSqrt(System_Math_Abs(p2.X-p1.X)+System_Math_Abs(p2.Y-p1.Y));
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"double Dist()");
		}
		return 0;
	}
	decimal Dist(ref_MPoint p1, ref_MPoint p2) {
		try {
			return SafeSqrt(System_Math_Abs(p2.X-p1.X)+System_Math_Abs(p2.Y-p1.Y));
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"Dist(MPoint, MPoint)");
		}
		return 0;
	}
	float SafeDivide(float val, float valDivisor, float valMax) {
		return SafeDivide(val,valDivisor,valMax,-valMax);
	}
	double SafeDivide(double val, double valDivisor, double valMax) {
		return SafeDivide(val,valDivisor,valMax,-valMax);
	}
	decimal SafeDivide(decimal val, decimal valDivisor, decimal valMax) {
		return SafeDivide(val,valDivisor,valMax,-valMax);
	}
	float SafeDivide(float val, float valDivisor, float valMax, float valMin) {
		try {
			bool bSameSign=(val<0.0f&&valDivisor<0.0f)?true:((val>=0.0f&&valDivisor>=0.0f)?true:false);
			if (valDivisor==0.0f) {
				if (val>0.0f) return valMax;
				else if (val<0.0f) return valMin;
				else return 0.0f;
			}
			//replaced by +inf and -inf below //else if (float_IsInfinity(val)) return valMax;
			else if (float_IsPositiveInfinity(val)) {
				if (float_IsPositiveInfinity(valDivisor)) return 1.0f;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (float_IsNegativeInfinity(valDivisor)) return -1.0f;
					else return valMin; //since not same sign
				}
			}
			else if (float_IsNegativeInfinity(val)) {
				if (float_IsNegativeInfinity(valDivisor)) return 1.0f;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (float_IsPositiveInfinity(valDivisor)) return -1.0f;
					else return valMin; //since not same sign (i.e. -inf/2.0)
				}
			}
			else if (float_IsPositiveInfinity(valDivisor)) {
				if (float_IsPositiveInfinity(val)) return 1.0f;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (float_IsNegativeInfinity(val)) return -1.0f;
					else return valMin;
				}
			}
			else if (float_IsNegativeInfinity(valDivisor)) {
				if (float_IsNegativeInfinity(val)) return 1.0f;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (float_IsNegativeInfinity(val)) return 1.0f;
					else return valMin;
				}
			}
				//TODO: finish this (cases of inf or -inf denominator)
			//TODO: output error if NaN?
			else if (float_IsNaN(val)) return 0.0f;
			else if (float_IsNaN(valDivisor)) return 0.0f;
			else return val/valDivisor;
		}
		catch (exception& exn)  {
			PReporting::ShowExn(exn,"SafeDivide("+RString_ToString(val)+","+RString_ToString(valDivisor)+","+RString_ToString(valMax)+")");
		}
		return 0.0f;
	} //end SafeDivide(float,float,float,float)
	double SafeDivide(double val, double valDivisor, double valMax, double valMin) {
		try {
			bool bSameSign=(val<0.0&&valDivisor<0.0)?true:((val>=0.0&&valDivisor>=0.0)?true:false);
			if (valDivisor==0.0) {
				if (val>0.0) return valMax;
				else if (val<0.0) return valMin;
				else return 0.0;
			}
			//replaced by +inf and -inf below //else if (double_IsInfinity(val)) return valMax;
			else if (double_IsPositiveInfinity(val)) {
				if (double_IsPositiveInfinity(valDivisor)) return 1.0;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (double_IsNegativeInfinity(valDivisor)) return -1.0;
					else return valMin; //since not same sign
				}
			}
			else if (double_IsNegativeInfinity(val)) {
				if (double_IsNegativeInfinity(valDivisor)) return 1.0;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (double_IsPositiveInfinity(valDivisor)) return -1.0;
					else return valMin; //since not same sign (i.e. -inf/2.0)
				}
			}
			else if (double_IsPositiveInfinity(valDivisor)) {
				if (double_IsPositiveInfinity(val)) return 1.0;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (double_IsNegativeInfinity(val)) return -1.0;
					else return valMin;
				}
			}
			else if (double_IsNegativeInfinity(valDivisor)) {
				if (double_IsNegativeInfinity(val)) return 1.0;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (double_IsNegativeInfinity(val)) return 1.0;
					else return valMin;
				}
			}
				//TODO: finish this (cases of inf or -inf denominator)
			//TODO: output error if NaN?
			else if (double_IsNaN(val)) return 0.0;
			else if (double_IsNaN(valDivisor)) return 0.0;
			else return val/valDivisor;
		}
		catch (exception& exn)  {
			PReporting::ShowExn(exn,"SafeDivide("+RString_ToString(val)+","+RString_ToString(valDivisor)+","+RString_ToString(valMax)+")");
		}
		return 0.0;
	} //end SafeDivide(double,double,double,double)
	decimal SafeDivide(decimal val, decimal valDivisor, decimal valMax, decimal valMin) {
		try {
			bool bSameSign=(val<M_0&&valDivisor<M_0)?true:((val>=M_0&&valDivisor>=M_0)?true:false);
			if (valDivisor==M_0) {
				if (val>M_0) return valMax;
				else if (val<M_0) return valMin;
				else return M_0;
			}
			//replaced by +inf and -inf below //else if (decimal_IsInfinity(val)) return valMax;
			/*
			else if (decimal_IsPositiveInfinity(val)) {
				if (decimal_IsPositiveInfinity(valDivisor)) return M_1;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (decimal_IsNegativeInfinity(valDivisor)) return M_NEG1;
					else return valMin; //since not same sign
				}
			}
			else if (decimal_IsNegativeInfinity(val)) {
				if (decimal_IsNegativeInfinity(valDivisor)) return M_1;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (decimal_IsPositiveInfinity(valDivisor)) return M_NEG1;
					else return valMin; //since not same sign (i.e. -inf/2.0)
				}
			}
			else if (decimal_IsPositiveInfinity(valDivisor)) {
				if (decimal_IsPositiveInfinity(val)) return M_1;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (decimal_IsNegativeInfinity(val)) return M_NEG1;
					else return valMin;
				}
			}
			else if (decimal_IsNegativeInfinity(valDivisor)) {
				if (decimal_IsNegativeInfinity(val)) return M_1;
				else if (bSameSign) {
					return valMax;
				}
				else {
					if (decimal_IsNegativeInfinity(val)) return M_1;
					else return valMin;
				}
			}
			*/
			//TODO: finish this (cases of inf or -inf denominator)
			//TODO: output error if NaN?
			//else if (decimal_IsNaN(val)) return M_0;
			//else if (decimal_IsNaN(valDivisor)) return M_0;
			else return val/valDivisor;
		}
		catch (exception& exn)  {
			PReporting::ShowExn(exn,"SafeDivide("+RString_ToString(val)+","+RString_ToString(valDivisor)+","+RString_ToString(valMax)+")");
		}
		return M_0;
	} //end SafeDivide(decimal,decimal,decimal,decimal)

	int SafeAbs(int val) {
		SafeAbsByRef(val);
		return val;
	}
	void SafeAbsByRef(ref_int val) {
		if (val<0) {
			if (val<int_MaxValue*-1) val=int_MaxValue;
			else val*=-1;
		}
	}
	float SafeAbs(float val) {
		SafeAbsByRef(val);
		return val;
	}
	void SafeAbsByRef(ref_float val) {
		if (val<0) {
			if (val<float_MaxValue*-1.0f) val=float_MaxValue;
			else val*=-1;
		}
	}
	int Negative(int val) {
		NegativeByRef(val);
		return val;
	}
	void NegativeByRef(ref_int val) {
		if (val>0) {
			val*=-1;
		}
	}
	int SafeDivideRound(int val, int valDivisor, int valMax) {
		int iReturn=SafeDivide(val,valDivisor,valMax,0);
		if (valDivisor!=0) {
			bool bNeg=false;
			if (val<0) bNeg=!bNeg;
			if (valDivisor<0) bNeg=!bNeg;
			//now do rounding manually:
			if (!bNeg) {
				if (val%valDivisor>SafeAbs(SafeDivide(valDivisor,2,valMax,0))) iReturn++;
			}
			else {
				if (val%valDivisor>Negative(SafeDivide(valDivisor,2,valMax,0))) iReturn--;
			}
		}
		return iReturn;
	}
	int SafeDivideRound(int val, int valDivisor, int valMax, int valMin) {
		int iReturn=SafeDivide(val,valDivisor,valMax,valMin);
		if (valDivisor!=0) {
			bool bNeg=false;
			if (val<0) bNeg=!bNeg;
			if (valDivisor<0) bNeg=!bNeg;
			if (!bNeg) {
				if (val%valDivisor>SafeAbs(SafeDivide(valDivisor,2,valMax,valMin))) iReturn++;
			}
			else {
				if (val%valDivisor>Negative(SafeDivide(valDivisor,2,valMax,valMin))) iReturn--;
			}
		}
		return iReturn;
	}
	int SafeDivide(int val, int valDivisor, int valMax) {
		return SafeDivide(val,valDivisor,valMax,-valMax);
	}
	int SafeDivide(int val, int valDivisor, int valMax, int valMin) {
		bool bNeg=false;
		try {
			bool bSameSign=(val<0.0&&valDivisor<0.0)?true:((val>=0.0&&valDivisor>=0.0)?true:false);
			if (valDivisor==0) {
				if (val>0) return valMax;
				else if (val<0) return valMin;
				else return 0;
			}
			else {
				if (val<0) {
					val*=-1;
					bNeg=!bNeg;
				}
				if (valDivisor<0) {
					valDivisor*=-1;
					bNeg=!bNeg;
				}
			}
			try {
				return bNeg?(val/valDivisor*-1):(val/valDivisor);
			}
			catch (exception& exn) {
				return bNeg? ((val>valDivisor?valMax:valMin)*-1) : (val>valDivisor?valMax:valMin);
			}
		}
		catch (exception& exn)  {//should NEVER happen
			PReporting::ShowExn(exn,"SafeDivide","dividing "+RString_ToString(val)+" by "+RString_ToString(valDivisor)+" (unexpected crash) {min:"+RString_ToString(valMin)+"; max="+RString_ToString(valMax)+"}");
		}
		return 0;
	} //end SafeDivide(int...)
	float FixFloat(float var) {
		if (float_IsNegativeInfinity(var)) {
			var=float_MinValue;
		}
		else if (float_IsPositiveInfinity(var)) {
			var=float_MaxValue;
		}
		else if (float_IsNaN(var)) {
			var=0;
		}
		return var;
	}
	double FixDouble(double var) {
		if (double_IsNegativeInfinity(var)) {
			var=double_MinValue;
		}
		else if (double_IsPositiveInfinity(var)) {
			var=double_MaxValue;
		}
		else if (double_IsNaN(var)) {
			var=0;
		}
		return var;
	}
	uint Bit(int iBit) {
		return SafePow((uint)2,iBit);
	}

	int SafeE10I(int exp) {
		return SafePow(i10, exp);
	}
	long SafeE10L(int exp) {
		return SafePow(l10, exp);
	}
	float SafeE10F(ref_int exp) {
		return SafePow(f10, exp);
	}
	double SafeE10D(ref_int exp) {
		return SafePow(d10, exp);
	}
	decimal SafeE10M(ref_int exp) {
		return SafePow(m10, exp);
	}
	//int SafePow(int basenum, int exp) {
	//	return SafePow(basenum, exp);
	//}
	int SafePow(int basenum, int exp) {
		if (basenum==0) return 0;
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
			try {
				for (int iCount=1; iCount<exp; iCount++) {
					//if (result<int_MaxValue-basenum) //useless
						result*=basenum;
					//else return int_MaxValue;
				}
			}
			catch (exception& exn) {
				return int_MaxValue;
			}
		}
		if (bNeg) {
			result=1/result;
			exp*=-1;//leaves it the way we found it
		}
		return result;
	}
	//uint SafePow(uint basenum, int exp) {
	//	return SafePow(basenum,exp);
	//}
	uint SafePow(uint basenum, int exp) {
		if (basenum==0) return 0;
		uint result;
		bool bNeg;
		if (exp<0) {
			bNeg=true;
			exp*=-1;
		}
		if (exp==0) return 1;
		else {
			bNeg=false;
			result=basenum;
			try {
				for (int iCount=1; iCount<exp; iCount++) {
					//if (result<uint_MaxValue-basenum) //useless
						result*=basenum;
					//else return uint_MaxValue;
				}
			}
			catch (exception& exn) {
				return uint_MaxValue;
			}
		}
		if (bNeg) {
			result=1/result;
			exp*=-1;//leaves it the way we found it
		}
		return result;
	}
	//long SafePow(long basenum, int exp) {
	//	return SafePow(basenum, exp);
	//}
	long SafePow(long basenum, int exp) {
		if (basenum==0) return 0;
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
			try {
				for (int iCount=1; iCount<exp; iCount++) {
					//if (result<long_MaxValue-basenum) //useless
						result*=basenum;
					//else return long_MaxValue;
				}
			}
			catch (exception& exn) {
				return long_MaxValue;
			}
		}
		if (bNeg) {
			result=1L/result;
			exp*=-1;//leaves it the way we found it
		}
		return result;
	}
	//float SafePow(float basenum, int exp) {
	//	return SafePow(basenum, exp);
	//}
	float SafePow(float basenum, int exp) {
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
			try {
				for (int iCount=1; iCount<exp; iCount++) {
					//if (result<float_MaxValue-basenum) //useless
						result*=basenum;
					//else return float_MaxValue;
				}
			}
			catch (exception& exn) {
				return float_MaxValue;
			}
		}
		if (bNeg) {
			if (result!=0.0f) result=1.0F/result; //check now since may have degraded to zero if started at <1.0f
			exp*=-1;//leaves it the way we found it
		}
		return result;
	}
	//double SafePow(double basenum, int exp) {
	//	return SafePow(basenum, exp);
	//}
	double SafePow(double basenum, int exp) {
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
			try {
				for (int iCount=1; iCount<exp; iCount++) {
					//if (result<double_MaxValue-basenum) //useless
						result*=basenum;
					//else return double_MaxValue;
				}
			}
			catch (exception& exn) {
				return double_MaxValue;
			}
		}
		if (bNeg) {
			if (result!=0.0D) result=1.0D/result; //check now since may have degraded to zero if started at <1.0
			exp*=-1; //leaves it the way we found it
		}
		return result;
	}
	//decimal SafePow(decimal basenum, int exp) {
	//	return SafePow(basenum, exp);
	//}
	decimal SafePow(decimal basenum, int exp) {
		decimal result;
		bool bNeg;
		if (exp<0) {
			bNeg=true;
			exp*=-1;
		}
		if (exp==0) return M_1;
		else {
			bNeg=false;
			result=basenum;
			try {
				for (int iCount=1; iCount<exp; iCount++) {//start at once since set result=basenum already
					//if (result<decimal_MaxValue-basenum) //useless
						result*=basenum;
					//else return decimal_MaxValue;
				}
			}
			catch (exception& exn) {
				return decimal_MaxValue;
			}
		}
		if (bNeg) {
			if (result!=M_0) result=M_1/result; //check now since may have degraded to zero if started at <1.0
			exp*=-1; //leaves it the way we found it
		}
		return result;
	}
	void Rotate(ref_float xToMove, ref_float yToMove, float xCenter, float yCenter, float rRotate) {
		xToMove-=xCenter;
		yToMove-=yCenter;
		float rTemp=RConvert_ROFXY(xToMove,yToMove), thetaTemp=RConvert_THETAOFXY_RAD(xToMove,yToMove);
		thetaTemp+=rRotate;
		xToMove=XOFRTHETA_RAD(rTemp,thetaTemp);
		yToMove=YOFRTHETA_RAD(rTemp,thetaTemp);
		xToMove+=xCenter;
		yToMove+=yCenter;
	}
	void Rotate(ref_double xToMove, ref_double yToMove, double xCenter, double yCenter, double rRotate) {
		xToMove-=xCenter;
		yToMove-=yCenter;
		double rTemp=RConvert_ROFXY(xToMove,yToMove), thetaTemp=RConvert_THETAOFXY_RAD(xToMove,yToMove);
		thetaTemp+=rRotate;
		xToMove=XOFRTHETA_RAD(rTemp,thetaTemp);
		yToMove=YOFRTHETA_RAD(rTemp,thetaTemp);
		xToMove+=xCenter;
		yToMove+=yCenter;
	}
	void Rotate(ref_float xToMove, ref_float yToMove, float rRotate) {
		float rTemp=RConvert_ROFXY(xToMove,yToMove), thetaTemp=RConvert_THETAOFXY_RAD(xToMove,yToMove);
		thetaTemp+=rRotate;
		xToMove=XOFRTHETA_RAD(rTemp,thetaTemp);
		yToMove=YOFRTHETA_RAD(rTemp,thetaTemp);
	}
	void Rotate(ref_double xToMove, ref_double yToMove, double rRotate) {
		double rTemp=RConvert_ROFXY(xToMove,yToMove), thetaTemp=RConvert_THETAOFXY_RAD(xToMove,yToMove);
		thetaTemp+=rRotate;
		xToMove=XOFRTHETA_RAD(rTemp,thetaTemp);
		yToMove=YOFRTHETA_RAD(rTemp,thetaTemp);
	}
	float Round(float val) {
		return RConvert_ToFloat(System_Math_Round((double)val));
	}
	///<summary>
	///Uses AlphaLook array in switched order to get true approach toward "toward" by factor
	///Returns AlphaLook[toward][start][factor]
	///</summary>
	byte Approach(byte start, byte toward, byte factor) {
		return Approach((float)start,(float)toward,(float)factor);//TODO: AlphaLook[toward][start][factor];//intentionally flipped since alpha causes the value to approach SOURCE color, so AlphaLook's reverse order needs to be accounted for
	}
	byte Approach(byte start, byte toward, float factor) {
		return factor<=0.0f?start:(factor>=1.0f?toward:Approach(start,toward,ByRound(factor*255.0f)));
		//try {
		//	return ByRound(((float)start)-(((float)start)-((float)toward))*(factor));
		//}
		//catch (exception& exn) {
		//	PReporting::Debug(exn,"","Base Approach(byte)");
		//	//TODO: make this more accurate
		//	return toward; //check this--may be correct since overflow means too big in the formula above
		//}
	}//end Approach(byte,byte,float)
	byte Approach(byte start, byte toward, double factor) {
		return factor<=0.0?start:(factor>=1.0?toward:Approach(start,toward,ByRound(factor*255.0)));
	}//end Approach(byte,byte,float)
	float Approach(float start, float toward, float factor) {
		try {
			return ((start)-((start)-(toward))*(factor));
		}
		catch (exception& exn) {
			PReporting::Debug(exn,"","Base Approach");
			//TODO: make this more accurate
			return toward; //check this--may be correct since overflow means too big in the formula above
		}
	}
	double Approach(double start, double toward, double factor) {
		try {
			return ((start)-((start)-(toward))*(factor));
		}
		catch (exception& exn) {
			PReporting::Debug(exn,"","Approach");
			//TODO: make this more accurate
			return toward; //check this--may be correct since overflow means too big in the formula above
		}
	}
	//float Mod(float num, float div) {
		//float result=num/div;
		//Floor(result);
		//result=num-result;
		//return result;
	//}
	float Mod(float val, float divisor) { //formerly FMOD
		return ((val>divisor) ? ( val - RMath::Floor(val/divisor)*divisor) : 0.0f );
	}
	double Mod(double val, double divisor) { //formerly DMOD
		return ((val>divisor) ? ( val - System_Math_Floor(val/divisor)*divisor) : 0 );
	}
/*
-3        -2        -1         0         1         2
+--|------+---------+----|----+--|------+----|--|-+
 a                     b       c           d  e

						  a=-2.7  b=-0.5  c=0.3  d=1.5  e=2.8
						  ======  ======  =====  =====  =====
Floor                    -3      -1      0      1      2
Ceiling                  -2       0      1      2      3
Truncate                 -2       0      0      1      2
Round (ToEven)           -3       0      0      2      3
Round (AwayFromZero)     -3      -1      0      2      3

Pax, I think you've got a mistake with: Round(AwayFromZero) -3 -2 1 2 3 System_Math_Round(-1.2, MidpointRounding.AwayFromZero) == -1 System_Math_Round(0.3, MidpointRounding.AwayFromZero)==0.0 etc.. – dtroy May 5 at 3:49

Pax Diablo answer for "What’s the difference between System_Math_Floor() and System_Math_Truncate() in .NET?" <http://stackoverflow.com/questions/14/whats-the-difference-between-math-floor-and-math-truncate-in-c/580252#580252> 2009-12-12

*/
/*
	///NOTE: CEILING rounds UP (in positive direction) even if number is negative!
	/// - FLOOR rounds DOWN (in negative direction) even if number is negative!
	/// - TRUNCATE rounds TOWARD ZERO regardless of sign
	void Ceiling(ref_float val) {
		float valTemp=(float)((long)val);//float has 23 bits for the Significand, plus an 8-bit exponent and 1-bit sign
		if (val>valTemp) val=valTemp+1.0f;
		else val=valTemp;
	}
	float CeilingRefToNonRef(ref_float val) {
		float valReturn=(float)((long)val);//float has 23 bits for the Significand, plus an 8-bit exponent and 1-bit sign
		if (val>valReturn) return valReturn+1.0f;
		else return valReturn;
	}
	float Ceiling(float val) {
		float valReturn=(float)((long)val);//float has 23 bits for the Significand, plus an 8-bit exponent and 1-bit sign
		if (val>valReturn) valReturn+=1.0f;//val=valReturn+1.0f;
		//else val=valReturn;
		return valReturn;
	}
	void Ceiling(ref_decimal val) {
		if (PReporting::iWarnings<PReporting::iMaxWarnings) {
			Console.Error.WriteLine("Warning: Performance - should have used System_Math_Floor since it is available for this overload");
			PReporting::iWarnings++;
		}
		val=System_Math_Ceiling(val);
		//decimal valReturn=(decimal)((long)val);//TODO: debug overflow of long
		//if (val>valReturn) val=valReturn+1.0f;
		//else val=valReturn;
	}
	void Ceiling(ref_double val) {
		double valTemp=(double)((long)val);//double has 52 bits for the Significand, plus an 11-bit exponent and 1-bit sign
		if (val>valTemp) val=valTemp+1.0;
		else val=valTemp;
	}
	double CeilingRefToNonRef(ref_double val) {
		double valReturn=(double)((long)val);//double has 52 bits for the Significand, plus an 11-bit exponent and 1-bit sign
		if (val>valReturn) return valReturn+1.0;
		else return valReturn;
	}

	float Floor(float val) {
		Floor(val);
		return val;
	}
	double Floor(double num) {//use System_Math_Floor, which has double and decimal
		if (PReporting::iWarnings<PReporting::iMaxWarnings) {
			Console.Error.WriteLine("Warning: Performance - should have used System_Math_Floor since it is available for this overload");
			PReporting::iWarnings++;
		}
		return System_Math_Floor(num);
		//Floor(num);RMath.Floor(num);
		//return num;
	}
	float FloorRefToNonRef(ref_float num) {
		float numNew=num;
		Floor(numNew);
		return numNew;
	}
	void Floor(ref_float num) {
		//bool bOverflow=false;//TODO: check for overflow
		if (num!=0.0F) {
			long whole=(long)num;
			float numTemp=(float)whole;
			if (num<numTemp) num=numTemp-1.0f;
			else num=numTemp;
		}
	}

	void Floor(ref_double num) {
		//Console.Error.WriteLine("Warning: Performance - should have used System_Math_Floor since it is available for this overload");
		num=System_Math_Floor(num);

	//	if (num>(double)long_MaxValue) {
	//		num=(double)long_MaxValue;
	//	}
	//	else if (num<(double)long_MinValue) {
	//		num=(double)long_MinValue;
	//	}
	//	else if (num!=0F) {
	//		long whole=(long)num;
	//		double numTemp=(double)whole;
	//		if (num<numTemp) num=numTemp-1.0;
	//		else num=numTemp;
	//	}

	}
	double FloorRefToNonRef(ref_double num) {
		if (PReporting::iWarnings<PReporting::iMaxWarnings) {
			Console.Error.WriteLine("Warning: Performance - should have used System_Math_Floor since it is available for this overload");
			PReporting::iWarnings++;
		}
		return System_Math_Floor(num);
		//double numNew=num;
		//Floor(numNew);//RMath.Floor(numNew);
		//return numNew;
	}
	*/
	byte LOWNIBBLE(byte by) {
		return (byte)(by%16); //or by&byLowNibble;
	}
	byte HIGHNIBBLE(byte by) {
		return (byte)(by/16); //or by>>4;
	}
	void SETLOWNIBBLE(ref_byte byTarget, byte byNibbleValueMustBeLessthan16) {
		byTarget=(byte)((byte)((byte)(byTarget/16)*16) + byNibbleValueMustBeLessthan16); //assumes by2<16
	}
	void SETHIGHNIBBLE(ref_byte byTarget, byte byNibbleValueMustBeLessthan16) {
		byTarget=(byte)((byte)(byTarget%16) + (byNibbleValueMustBeLessthan16*16)); //assumes by1<16
	}
	byte ByRound(float val) {
		if (val>=254.5f) return 255; //> is okay since it will be truncated below anyway
		else if (val<.5f) return 0;
		return (byte)(val+.5f);
	}
	byte ByRound(double val) {
		if (val>=255.5) return 255;
		else if (val<.5) return 0;
		return (byte)(val+.5f);
	}
	int IRound(float val) {
		if (val>=f_int_MaxValue_minus_point5) return int_MaxValue;
		else if (val<f_int_MinValue_plus_point5) return int_MinValue;
		return val<0.0f?(int)(val-.5f):(int)(val+.5f);
	}
	int IRound(double val) {
		if (val>=d_int_MaxValue_minus_point5) return int_MaxValue;
		else if (val<d_int_MinValue_plus_point5) return int_MinValue;
		return val<0.0d?(int)(val-.5d):(int)(val+.5d);
	}
	int IRound(decimal val) {
		if (val>=m_int_MaxValue_minus_point5) return int_MaxValue;
		else if (val<m_int_MinValue_plus_point5) return int_MinValue;
		return val<M_0?(int)(val-M_0_5):(int)(val+M_0_5);
	}
	int ICeiling(float val) {
		if (val>f_int_MaxValue_minus_1) return int_MaxValue;
		else if (val<f_int_MinValue_plus_1) return int_MinValue;
		//since this is ceiling algorithm, it only has to be more than .0 above the figure in order to be rounded UPWARD
		return val>0
			? ( ((float)val>((float)((int)val)))
				? ((int)val+1)
				: (int)val )
			: ( ((float)val<((float)((int)val)))
				? ((int)val-1)
				: (int)val );
	}
	int ICeiling(double val) {
		if (val>d_long_MaxValue_minus_1) return int_MaxValue;
		else if (val<d_long_MinValue_plus_1) return int_MinValue;
		return val>0
			? ( ((double)val>((double)((int)val)))
				? ((int)val+1)
				: (int)val )
			: ( ((double)val<((int)((int)val)))
				? ((int)val-1)
				: (int)val );
	}
	long LCeiling(double val) {
		if (val>d_long_MaxValue_minus_1) return long_MaxValue;
		else if (val<d_long_MinValue_plus_1) return long_MinValue;
		return val>0
			? ( ((double)val>((double)((long)val)))
				? ((long)val+1)
				: (long)val )
			: ( ((double)val<((long)((long)val)))
				? ((long)val-1)
				: (long)val );
	}

	void CropAbsoluteValueToPosMax(ref_int val) {//formerly PrepareToBePos
	//limits value since IEEE positive range is narrower (PrepareToBeNeg is not needed!)
		if (val<(-1*int_MaxValue)) val=-1*int_MaxValue;
	}
	void CropAbsoluteValueToPosMax(ref_long val) {
		if (val>(-1*long_MaxValue)) val=-1*long_MaxValue;
	}
	void CropAbsoluteValueToPosMax(ref_float val) {
		if (val>(-1.0f*float_MaxValue)) val=-1.0f*float_MaxValue;
	}
	void CropAbsoluteValueToPosMax(ref_double val) {
		if (val>(-1.0*double_MaxValue)) val=-1.0*double_MaxValue;
	}
	void CropAbsoluteValueToPosMax(ref_decimal val) {
		if (val>(M_NEG1*decimal_MaxValue)) val=M_NEG1*decimal_MaxValue;
	}

	float SafeSqrt(float val) {
		if (val>0) return RMath::Sqrt(val);
		else if (val<0) { //debug should not actually return a real number
			CropAbsoluteValueToPosMax(val); //avoids overflow when multiplying by -1 below
			return (-1.0f*RMath::Sqrt((-1.0f*val)));
		}
		else return 0;
	}
	double SafeSqrt(double val) {
		if (val>0) return System_Math_Sqrt(val);
		else if (val<0) { //debug should not actually return a real number
			CropAbsoluteValueToPosMax(val);
			return -1.0*System_Math_Sqrt(-1.0D*val);
		}
		else return 0;
	}
	float Sqrt(float val) {
		float val1=0.0f;
		float val2;
		while( (val1*val1) <= val ) val1+=0.1f;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=2.0f; val1=val2;
		return val2;
	}//end Sqrt(float)
	decimal Sqrt(decimal val) {
		decimal val1=M_0;
		decimal val2;
		while( (val1*val1) <= val ) val1+=M_0_1;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		val2=val; val2/=val1; val2+=val1; val2/=M_2; val1=val2;
		return val2;
	}//end Sqrt(decimal)
	decimal SafeSqrt(decimal val) {
		if (val>M_0) return RMath::Sqrt(val);
		else if (val<M_0) { //debug should not actually return a real number
			CropAbsoluteValueToPosMax(val);
			return M_NEG1*RMath::Sqrt(M_NEG1*val);
		}
		else return 0;
	}
	//REAL SafeSqrt(REAL val) {
	//	if (val>0) return System_Math_Sqrt(val);
	//	else if (val<0) {
	//		PrepareToBeNeg(val);
	//		return (REAL)-1*System_Math_Sqrt((double)((REAL)-1*val));
	//	}
	//	else return 0;
	//}
	int SafeSqrt(int val) {
		if (val>0) return (int)System_Math_Sqrt(val);
		else if (val<0) { //debug should not actually return a real number
			CropAbsoluteValueToPosMax(val);
			return -1*(int)System_Math_Sqrt((int)(-1*val));
		}
		else return 0;
	}
	long SafeSqrt(long val) {
		if (val>0) return (long)System_Math_Sqrt(val);
		else if (val<0) { //debug should not actually return a real number
			CropAbsoluteValueToPosMax(val);
			return -1L*(long)System_Math_Sqrt((long)(-1L*val));
		}
		else return 0;
	}
	//ulong SafeSqrt(ulong val) {
	//	return (ulong)System_Math_Sqrt(val);
	//}
	///#region math

	///#region utilities
/*
	int DaysAbsoluteValueOfDifferenceIgnoringHours(DateTime datetime1, DateTime datetime2) { //formerly DaysAbsoluteDifferenceIgnoringHours
		int iReturn=DaysDifferenceIgnoringHours(datetime1,datetime2);
		if (iReturn<0) iReturn=0-iReturn;
		return iReturn;
	}
	/// <summary>
	/// Gets difference in days (only looks at the day, NOT the time of day), taking leapyear into account.  Not optimized, goes day by day.
	/// </summary>
	/// <param name="datetime1"></param>
	/// <param name="datetime2"></param>
	/// <returns></returns>
	int DaysDifferenceIgnoringHours(DateTime datetime1, DateTime datetime2) {
		int iReturn=0;
		bool bNeg=false;
		try {
			DateTime dtDestruct=(DateTime)datetime1;//.ToUniversalTime();
			DateTime dtLast=(DateTime)datetime2;//.ToUniversalTime();
			dtDestruct=dtDestruct.AddMilliseconds(dtLast.Millisecond-dtDestruct.Millisecond);
			dtDestruct=dtDestruct.AddSeconds(dtLast.Second-dtDestruct.Second);
			dtDestruct=dtDestruct.AddMinutes(dtLast.Minute-dtDestruct.Minute);
			dtDestruct=dtDestruct.AddHours(dtLast.Hour-dtDestruct.Hour);
			long lTickDiff=dtDestruct.Ticks-dtLast.Ticks;
			//1 Tick==100ns==1/10,000,000s==1/10,000ms.
			iReturn=(int)(lTickDiff/10000000/60/60/24);//OK to truncate since should be 0 if less than 1 day difference
			//if (dtDestruct.Ticks>dtLast.Ticks) {
			//	DateTime dtTemp=dtDestruct;
			//	dtDestruct=dtLast;
			//	dtLast=dtTemp;
			//	bNeg=true;
			//}
			//while (dtDestruct.Year<dtLast.Year
			//       ||dtDestruct.Month<dtLast.Month
			//       ||dtDestruct.Day<dtLast.Day) {
			//	iReturn++;
			//	dtDestruct.AddDays(1.0d);
			//}
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"getting difference of datetime objects","DaysAbsoluteValueOfDifferenceIgnoringHours");
		}
		if (bNeg) iReturn=0-iReturn;
		return iReturn;
	}//end DaysAbsoluteValueOfDifferenceIgnoringHours
	*/
	//string ToString(IPoint& point) {
	//	return //(point!=null)?
	//	("("+RString_ToString(point.X)+","+RString_ToString(point.Y)+")");
		//:"null";
	//}
	//string ToString(FPoint& point) {
	//	return "("+RString_ToString(point.X)+","+RString_ToString(point.Y)+")"; //return (point!=null)?("("+RString_ToString(point.X)+","+RString_ToString(point.Y)+")"):"null";
	//}
	//string ToString(DPoint& point) {
	//	return "("+RString_ToString(point.X)+","+RString_ToString(point.Y)+")"; //return (point!=null)?("("+RString_ToString(point.X)+","+RString_ToString(point.Y)+")"):"null";
	//}
	/*
	int IndexOf(int arrVal[], int valX) {
		if (arrVal!=null) {
			for (int iNow=0; iNow<arrVal.length(); iNow++) {
				if (arrVal[iNow]==valX) {
					return iNow;
				}
			}
		}
		return -1;
	}
	*/
	//string RemoveExpNotation(string sNum) {
	//	RemoveExpNotation(sNum);
	//	return sNum;
	//}
	void RemoveExpNotation(ref_string sNum) {
		bool bNeg;
		size_t iExpChar;
		size_t iDot;
		int iExpOf10;
		try {
			iExpChar=sNum.find_first_of('E',0);
			if (iExpChar!=string::npos) {
				string sExp=sNum.substr(iExpChar+1,sNum.length()-(iExpChar+1));
				if (sExp.find_first_of("+",0)==0) sExp=sExp.substr(1,string::npos);
				iExpOf10=atoi(sExp.c_str());
				sNum=sNum.substr(0,iExpChar);
				if (sNum.find_first_of("-",0)==0) {
					sNum=sNum.substr(1,string::npos);
					bNeg=true;
				}
				else bNeg=false;
				iDot=sNum.find_first_of('.',0);
				if (iDot!=string::npos) {
					sNum=string_Remove(sNum,iDot, 1);
					//iInitialExp=sNum.length()-iDot;
					iExpOf10-=(sNum.length()-iDot);
				}
				if (iExpOf10>0) {
					size_t iInsertion=iExpOf10+sNum.length();
					for (int iZero=sNum.length(); iZero<iInsertion; iZero++) {
						sNum+="0";
					}
				}
				else if (iExpOf10<0) {
					size_t iInsertion=iExpOf10+sNum.length();
					if (iInsertion!=string::npos) { //could also be > but doesn't matter,
						//except that this case handles it without doing the
						//useless check that doesn't use the "for" loop in the
						//== case.
						sNum=sNum.insert(iInsertion,".");
					}
					else {
						for (int iZero=0; iZero>iInsertion; iZero--) {//ok since iInsertion is a length & always >0
							sNum="0"+sNum;
						}
						sNum="."+sNum;
					}
				}
				//else do not insert decimal point since exponent of 10 is zero
				if (bNeg) sNum="-"+sNum;
			} //end if there's notation to remove
			//else no change
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"RemoveExpNotation");
		}
	}//end RemoveExpNotation
// 		int LocationToFuzzyMaximum(int iCurrentMax, int iLoc) {
// 			if (iLoc<1) iLoc=2;//2 since multiplying by 1.5 later
// 			iLoc++;
// 			if (iCurrentMax<0) iCurrentMax=0;
// 			if (iLoc>iCurrentMax) iCurrentMax=RConvert_ToInt(RMath::SafeMultiply(RConvert_ToDouble(iLoc),(double)1.5));
// 			return iCurrentMax;
// 		}
	float FractionPartOf(float val) {
		return val-Floor(val);
	}
	double FractionPartOf(double val) {
		return val-System_Math_Floor(val);
	}
	/*
	int MinVal(int arrVal[]) {
		int val=int_MaxValue;//error condition
		if (arrVal!=null) {
			for (int iNow=0; iNow<arrVal.length(); iNow++) {
				if (arrVal[iNow]<val) val=arrVal[iNow];
			}
		}
		return val;
	}//end MinVal
	int MaxVal(int arrVal[]) {
		int val=int_MinValue;//error condition
		if (arrVal!=null) {
			for (int iNow=0; iNow<arrVal.length(); iNow++) {
				if (arrVal[iNow]>val) val=arrVal[iNow];
			}
		}
		return val;
	}//end MaxVal
	int MinPosVal(int arrVal[]) {
		int val=int_MaxValue;//error condition
		if (arrVal!=null) {
			for (int iNow=0; iNow<arrVal.length(); iNow++) {
				if ( arrVal[iNow]>=0 && arrVal[iNow]<val ) val=arrVal[iNow];
			}
		}
		return val;
	}//end MinPosVal
	int MaxPosVal(int arrVal[]) {
		int val=0;//error condition
		if (arrVal!=null) {
			for (int iNow=0; iNow<arrVal.length(); iNow++) {
				if ( arrVal[iNow]>val ) val=arrVal[iNow];//if ( arrVal[iNow]>=0 &&  arrVal[iNow]>val ) val=arrVal[iNow];
			}
		}
		return val;
	}//end MaxPosVal
	*/
	///// <summary>
	///// Crops iToChange to iMin and iExclusiveMax, and returns how many linear whole number increments were lost.
	///// <summary>
	///// <returns>Loss, if cropped iToChange, else zero.</returns>
	//int CropAndGetLossEx(ref_int iToChange, int iMin, int iExclusiveMax) {
	//	int iReturn=0;
	//	if (iToChange<iMin) {
	//		iReturn=iMin-iToChange;
	//		iToChange=iMin;
	//	}
	//	else if (iToChange>=iExclusiveMax) {
	//		iReturn=iToChange-(iExclusiveMax-1);
	//		iToChange=iExclusiveMax-1;
	//	}
	//	return iReturn;
	//}
	void CropEx(ref_int iToChange, int iMin, int iExclusiveMax) {
		if (iToChange<iMin) iToChange=iMin;
		else if (iToChange>=iExclusiveMax) iToChange=iExclusiveMax-1;
	}
	void CropZone(ref_int zone_Left, ref_int zone_Top, ref_int zone_Right, ref_int zone_Bottom, int Boundary_X, int Boundary_Y, int Boundary_Width, int Boundary_Height) {
		int iTemp;
		int screen_Right=Boundary_X+Boundary_Width;
		int screen_Bottom=Boundary_Y+Boundary_Height;
		if (zone_Left<Boundary_X) {
			iTemp=Boundary_X-zone_Left;
			zone_Left+=iTemp;
			zone_Right-=iTemp;
		}
		if (zone_Top<Boundary_Y) {
			iTemp=Boundary_Y-zone_Top;
			zone_Top+=iTemp;
			zone_Bottom-=iTemp;
		}
		if (zone_Bottom>screen_Bottom) {
			zone_Bottom-=(zone_Bottom-screen_Bottom);
		}
		if (zone_Right>screen_Right) {
			zone_Right-=(zone_Right-screen_Right);
		}
		if (zone_Right<=zone_Left) zone_Right=zone_Left+1;
		if (zone_Bottom<=zone_Top) zone_Bottom=zone_Top+1;
	}
// 		void CropRect(ref_int rect_X, ref_int rect_Y, ref_int rect_Width, ref_int rect_Height, int Boundary_X, int Boundary_Y, int Boundary_Width, int Boundary_Height) {
// 			int zone_Right=rect_X+rect_Width;
// 			int zone_Bottom=rect_Y+rect_Height;
// 			CropZone(rect_X, rect_Y, zone_Right, zone_Bottom, Boundary_X, Boundary_Y, Boundary_Width, Boundary_Height);
// 			rect_Width=(zone_Right-rect_X)-1; //-1 since from exclusive zone
// 			rect_Height=(zone_Bottom-rect_Y)-1; //-1 since from exclusive zone
// 			if (rect_Width<0) rect_Width=0;
// 			if (rect_Height<0) rect_Height=0;
// 		}
	//bool RectIntersect(out_int Return_X, out_int Return_Y, out_int Return_W, out_int Return_H, int to_X,

	///<summary>
	///Crops the RectToModify to the Boundary rect INCLUSIVELY
	///Returns false if there is nothing left of the RectToModify after cropping.
	///</summary>
	bool CropRect(ref_int RectToModify_X, ref_int RectToModify_Y, ref_int RectToModify_Width, ref_int RectToModify_Height, int Boundary_X, int Boundary_Y, int Boundary_Width, int Boundary_Height) {
		bool bGood=false;
		try {
			if (RectToModify_X<Boundary_X) {
				RectToModify_Width-=(Boundary_X-RectToModify_X);//i.e. (0 - -1 = 1 ; so subtract 1 from width) OR i.e. 1 - 0 = 1 ; so subtract 1 from width
				RectToModify_X=Boundary_X;
			}
			if (RectToModify_Y<Boundary_Y) {
				RectToModify_Height-=(Boundary_Y-RectToModify_Y);
				RectToModify_Y=Boundary_Y;
			}

			int Boundary_Right=Boundary_X+Boundary_Width;
			int RectToModify_Right=RectToModify_X+RectToModify_Width;
			if (RectToModify_Right>Boundary_Right) {
				RectToModify_Width-=RectToModify_Right-Boundary_Right;
			}
			int Boundary_Bottom=Boundary_Y+Boundary_Height;
			int RectToModify_Bottom=RectToModify_Y+RectToModify_Height;
			if (RectToModify_Bottom>Boundary_Bottom) {
				RectToModify_Height-=RectToModify_Bottom-Boundary_Bottom;
			}
			bGood=true;
			if (RectToModify_Width<1) { RectToModify_Width=0; bGood=false;}
			if (RectToModify_Height<1) { RectToModify_Height=0; bGood=false;}
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"clipping rectangle","RMath CropRect(int,...)");
		}
		return bGood;
	}//end CropRect(integers)

	///<summary>
	///Crops the RectToModify to the Boundary rect INCLUSIVELY
	///Returns false if there is nothing left of the RectToModify after cropping.
	///</summary>
	bool CropRect(ref_float RectToModify_X, ref_float RectToModify_Y, ref_float RectToModify_Width, ref_float RectToModify_Height, float Boundary_X, float Boundary_Y, float Boundary_Width, float Boundary_Height) {
		bool bGood=false;
		try {
			if (RectToModify_X<Boundary_X) {
				RectToModify_Width-=(Boundary_X-RectToModify_X);//i.e. (0 - -1 = 1 ; so subtract 1 from width) OR i.e. 1 - 0 = 1 ; so subtract 1 from width
				RectToModify_X=Boundary_X;
			}
			if (RectToModify_Y<Boundary_Y) {
				RectToModify_Height-=(Boundary_Y-RectToModify_Y);
				RectToModify_Y=Boundary_Y;
			}

			float Boundary_Right=Boundary_X+Boundary_Width;
			float RectToModify_Right=RectToModify_X+RectToModify_Width;
			if (RectToModify_Right>Boundary_Right) {
				RectToModify_Width-=RectToModify_Right-Boundary_Right;
			}
			float Boundary_Bottom=Boundary_Y+Boundary_Height;
			float RectToModify_Bottom=RectToModify_Y+RectToModify_Height;
			if (RectToModify_Bottom>Boundary_Bottom) {
				RectToModify_Height-=RectToModify_Bottom-Boundary_Bottom;
			}
			bGood=true;
			if (RectToModify_Width<=0) { RectToModify_Width=0; bGood=false;}
			if (RectToModify_Height<=0) { RectToModify_Height=0; bGood=false;}
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"clipping rectangle","RMath CropRect(float,...)");
		}
		return bGood;
	}//end CropRect(float,...)

	/// <summary>
	///Crops the ZoneToModify to the Boundary Zone INCLUSIVELY--slightly faster than CropRect because
	/// right&bottom edges don't have to be created or calculated.
	/// </summary>
	/// <param name="ZoneToModify_X"></param>
	/// <param name="ZoneToModify_Y"></param>
	/// <param name="ZoneToModify_Right"></param>
	/// <param name="ZoneToModify_Bottom"></param>
	/// <param name="Boundary_X"></param>
	/// <param name="Boundary_Y"></param>
	/// <param name="Boundary_Right"></param>
	/// <param name="Boundary_Bottom"></param>
	/// <returns>Returns false if there is nothing left of the ZoneToModify after cropping.</returns>
	bool CropZone(ref_float ZoneToModify_X, ref_float ZoneToModify_Y, ref_float ZoneToModify_Right, ref_float ZoneToModify_Bottom, float Boundary_X, float Boundary_Y, float Boundary_Right, float Boundary_Bottom) {
		bool bGood=false;
		//try {
		if (ZoneToModify_X<Boundary_X) {
			ZoneToModify_Right-=(Boundary_X-ZoneToModify_X);//i.e. (0 - -1 = 1 ; so subtract 1 from width) OR i.e. 1 - 0 = 1 ; so subtract 1 from width
			ZoneToModify_X=Boundary_X;
		}
		if (ZoneToModify_Y<Boundary_Y) {
			ZoneToModify_Bottom-=(Boundary_Y-ZoneToModify_Y);
			ZoneToModify_Y=Boundary_Y;
		}
		if (ZoneToModify_Right>Boundary_Right) {
			ZoneToModify_Right=Boundary_Right;//ZoneToModify_Right-=ZoneToModify_Right-Boundary_Right;
		}
		if (ZoneToModify_Bottom>Boundary_Bottom) {
			ZoneToModify_Bottom=Boundary_Bottom;//ZoneToModify_Height-=ZoneToModify_Bottom-Boundary_Bottom;
		}
		bGood=true;
		if (ZoneToModify_Right<ZoneToModify_X) { ZoneToModify_Right=ZoneToModify_X; bGood=false;}
		if (ZoneToModify_Y<ZoneToModify_Bottom) { ZoneToModify_Bottom=ZoneToModify_Y; bGood=false;}
		//}
		//catch (exception& exn) {
		//	PReporting::ShowExn(exn,"clipping zone","RMath CropZone(float,...)");
		//}
		return bGood;
	}//end CropZone(float,...)

	///<summary>
	///Crops the RectToModify to the Boundary rect INCLUSIVELY
	///Returns false if there is nothing left of the RectToModify after cropping.
	///</summary>
	bool CropRect(ref_double RectToModify_X, ref_double RectToModify_Y, ref_double RectToModify_Width, ref_double RectToModify_Height, double Boundary_X, double Boundary_Y, double Boundary_Width, double Boundary_Height) {
		bool bGood=false;
		try {
			if (RectToModify_X<Boundary_X) {
				RectToModify_Width-=(Boundary_X-RectToModify_X);//i.e. (0 - -1 = 1 ; so subtract 1 from width) OR i.e. 1 - 0 = 1 ; so subtract 1 from width
				RectToModify_X=Boundary_X;
			}
			if (RectToModify_Y<Boundary_Y) {
				RectToModify_Height-=(Boundary_Y-RectToModify_Y);
				RectToModify_Y=Boundary_Y;
			}

			double Boundary_Right=Boundary_X+Boundary_Width;
			double RectToModify_Right=RectToModify_X+RectToModify_Width;
			if (RectToModify_Right>Boundary_Right) {
				RectToModify_Width-=RectToModify_Right-Boundary_Right;
			}
			double Boundary_Bottom=Boundary_Y+Boundary_Height;
			double RectToModify_Bottom=RectToModify_Y+RectToModify_Height;
			if (RectToModify_Bottom>Boundary_Bottom) {
				RectToModify_Height-=RectToModify_Bottom-Boundary_Bottom;
			}
			bGood=true;
			if (RectToModify_Width<=0) { RectToModify_Width=0; bGood=false;}
			if (RectToModify_Height<=0) { RectToModify_Height=0; bGood=false;}
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"clipping rectangle","RMath CropRect");
		}
		return bGood;
	}//end CropRect(doubles)

	///<summary>
	///Crops the RectToModify to the Boundary rect
	///Returns false if there is nothing left of the RectToModify after cropping.
	///</summary>
	bool CropRect(ref_decimal RectToModify_X, ref_decimal RectToModify_Y, ref_decimal RectToModify_Width, ref_decimal RectToModify_Height, decimal Boundary_X, decimal Boundary_Y, decimal Boundary_Width, decimal Boundary_Height) {
		bool bGood=false;
		try {
			if (RectToModify_X<Boundary_X) {
				RectToModify_Width-=(Boundary_X-RectToModify_X);//i.e. (0 - -1 = 1 ; so subtract 1 from width) OR i.e. 1 - 0 = 1 ; so subtract 1 from width
				RectToModify_X=Boundary_X;
			}
			if (RectToModify_Y<Boundary_Y) {
				RectToModify_Height-=(Boundary_Y-RectToModify_Y);
				RectToModify_Y=Boundary_Y;
			}

			decimal Boundary_Right=Boundary_X+Boundary_Width;
			decimal RectToModify_Right=RectToModify_X+RectToModify_Width;
			if (RectToModify_Right>Boundary_Right) {
				RectToModify_Width-=RectToModify_Right-Boundary_Right;
			}
			decimal Boundary_Bottom=Boundary_Y+Boundary_Height;
			decimal RectToModify_Bottom=RectToModify_Y+RectToModify_Height;
			if (RectToModify_Bottom>Boundary_Bottom) {
				RectToModify_Height-=RectToModify_Bottom-Boundary_Bottom;
			}
			bGood=true;
			if (RectToModify_Width<1) { RectToModify_Width=0; bGood=false;}
			if (RectToModify_Height<1) { RectToModify_Height=0; bGood=false;}
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"clipping rectangle","RMath CropRect");
		}
		return bGood;
	}//end CropRect(decimals)


	/*

	void CropRect(ref_int rect_X, ref_int rect_Y, ref_int rect_Width, ref_int rect_Height, int Boundary_X, int Boundary_Y, int Boundary_Width, int Boundary_Height) {
		if (rect_Width>0&&rect_Height>0) {
			rect_Width-=CropAndGetLossEx(rect_X, Boundary_X, Boundary_X+Boundary_Width);
			rect_Height-=CropAndGetLossEx(rect_Y, Boundary_Y, Boundary_Y+Boundary_Height);
			if (rect_Width>0) {
				if (rect_X+rect_Width>Boundary_Width) {
					rect_Width-=((rect_X+rect_Width)-Boundary_Width);
				}
			}
			if (rect_Height>0) {
				if (rect_Y+rect_Height>Boundary_Height) {
					rect_Height-=((rect_Y+rect_Height)-Boundary_Height);
				}
			}
		}
		else {
			rect_Width=0;
			rect_Height=0;
			CropEx(rect_X, Boundary_X, Boundary_X+Boundary_Width);
			CropEx(rect_Y, Boundary_Y, Boundary_Y+Boundary_Height);
		}
	}
	*/
	///#region utilities

	///#region UI methods
	void AutoSize(out_int iWidthInner, out_int iHeightInner, int iWidthOuter, int iHeightOuter, float fPercent) {
		iWidthInner=(int)((float)iWidthOuter*fPercent+.5f);//.5f for rounding
		iHeightInner=(int)((float)iHeightOuter*fPercent+.5f);//.5f for rounding
	}
	int AutoInnerPosition(int iSizeOuter, float fPercent) {
		return (int)((float)iSizeOuter*fPercent+.5f);//.5f for rounding
	}
	///#region UI methods

}//end RMath namespace
//////#region RMath



#endif
