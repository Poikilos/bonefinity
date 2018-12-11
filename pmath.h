#ifndef PMATH_H
#define PMATH_H

#include <iostream>
#include <vector>
#include <string>
#include <climits>  // <limits.h>
#include "preporting.h"
#include "frameworkdummy.h"
#include "pointinfo.h"
#include "pixelinfo.h"
#include <base.h>  // DPoint etc

using namespace std;

namespace ExpertMultimediaBase {

	class PMath {
	public:
		///<summary>
		///Elements are used by [x][y] coordinates.  Resulting value is distance from [0][0].
		///NOTE: calling program is responsible for resizing this and not overflowing bounds if pfDistCache is used directly!
		///Make sure points are translated to positive values when used as indeces!
		///</summary>
		static float** pfDistCache;
		static int pfDistCache_w;
		static int pfDistCache_h;
		///<summary>
		///formerly static std::vector<POINTINFO> ppiCache;
		///</summary>
		static POINTINFO* ppiCache;
		static unsigned int ppiCache_Max;
		static unsigned int ppiCache_Used;
		///<summary>
		///
		///</summary>
		static PIXELINFO* ppxiCache;
		static unsigned int ppxiCache_Max;
		static unsigned int ppxiCache_Used;
		static bool ppxiCache_bHorizontalMovement;
		static bool ppxiCache_bVerticalMovement;

		static void staticconstructorPMath();
		static void staticdeconstructorPMath();
		static void ResizeDistanceCache(int w, int h);
		static void ResizePointInfoCache(unsigned int iRadius, bool bHorizontalMovement, bool bVerticalMovement);
		static void ResizePixelInfoCache(unsigned int iDesiredSize, bool bPreservePreviousData);
		static float f180_DIV_PI;
		static double d180_DIV_PI;
		static float FPi;
		static double DPi;
		static const byte by0;
		static const byte by255;
		static int Abs(int val);
		//static void Approach(int& x, int& y, int xDest, int yDest, float rMultiplier);
		//static void Approach(int& valToMove, int valDest, float rMultiplier);
		static void Approach(float& x, float& y, float xDest, float yDest, float rMultiplier);
		static void Approach(float& valToMove, float valDest, float rMultiplier);
		static void Approach(double& x, double& y, double xDest, double yDest, double rMultiplier);
		static void Approach(double& valToMove, double valDest, double rMultiplier);
		static bool HavePoint(unsigned int* haystack_X, unsigned int* haystack_Y, unsigned int haystack_Count, unsigned int xNeedle, unsigned int yNeedle);
		static bool HavePoint(double* haystack_X, double* haystack_Y, unsigned int haystack_Count, double xNeedle, double yNeedle);
		static bool HavePointRounded(double* haystack_X, double* haystack_Y, unsigned int haystack_Count, unsigned int xNeedle, unsigned int yNeedle);
		static float ROfXY(float X, float Y);
		static double ROfXY(double X, double Y);
		static float ThetaDegOfXY(float X, float Y);
		static double ThetaDegOfXY(double X, double Y);
		static float XOfRThetaDeg(float r, float theta);
		static float YOfRThetaDeg(float r, float theta);
		static double XOfRThetaDeg(double r, double theta);
		static double YOfRThetaDeg(double r, double theta);
		static void Travel2d(float& xToMove, float& yToMove, float rDirectionDeg, float rDistance);
		static void Travel2d(double& xToMove, double& yToMove, double rDirectionDeg, double rDistance);
		static float Dist(float x1, float y1, float x2, float y2);
		static double Dist(double x1, double y1, double x2, double y2);
		static double Nextness(double prev, double next, double val);
		static void Rotate(double& xToMove, double& yToMove, double rotate_deg);
		static void Rotate(double& xToMove, double& yToMove, double xCenter, double yCenter, double rotate_deg);
		static double RadiansToDegrees(double val);
		//static double ArcLengthOfCircleSliceStraightSideLength(double val);
		static double DegreesPerPixelAt(double radius_pixels);
		static byte ByRound(float val);
		static byte ByRound(double val);
		static int IRoundCropped(float val, int start, int endbefore);
		static int IRoundCropped(double val, int start, int endbefore);
		static float Abs(float val);
		static double Abs(double val);
	};

	class PMathStatic {//for static constructors (an instance is created in pmath.cpp)
	public:
		PMathStatic();
		~PMathStatic();
	};

	extern PMathStatic pmathstatic;//PMathStatic* pmathstatic=NULL;
}//end namespace

#endif
