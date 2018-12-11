#ifndef RTYPES_H
#define RTYPES_H

#include <base.h>
#include <frameworkdummy.h>
#include <string>
// for cpp:
#include <preporting.h>


namespace ExpertMultimediaBase {

	class IPoint {
	public:
		int X;
		int Y;
		IPoint();
		IPoint(int xSet, int ySet);
		IPoint(IPoint& ipSet);
		//IPoint(IZone izoneTopLeft);
		void Set(IPoint& ipSet);
		void Set(int xSet, int ySet);
		static string ToString(IPoint& pX);
		static string ToString(int x, int y);
		string ToString();
		IPoint* Copy();
		void CopyTo(IPoint& ipToModify);
	};//end IPoint
	extern IPoint ipZero;

	//class IPoint {
	//public:
	//	int X;
	//	int Y;
	//	IPoint();
	//	string ToString();
	//};
	class ILine {
		public:
		int X1;
		int Y1;
		int X2;
		int Y2;
		ILine();
		ILine(int x1, int y1, int x2, int y2);
		void Set(int x1, int y1, int x2, int y2);
	};

	class FPoint {
	public:
		float X;
		float Y;
		FPoint();
		string ToString();
	};

	class DPoint {
	public:
		double X;
		double Y;
		DPoint();
		string ToString();
	};

	class MPoint {
	public:
		decimal X;
		decimal Y;
		string ToString();
	};
	string ToString(MPoint& pX);
	string ToString(decimal x, decimal y);

	class IRect {
	public:
		int top;
		int left;
		int bottom;
		int right;
		IRect();
		string ToString();
	};

}
#endif
