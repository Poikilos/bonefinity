#ifndef RTYPES_H
#define RTYPES_H

#include <base.h>
#include <frameworkdummy.h>
#include <string>

#ifndef ref_FPoint
#define ref_FPoint FPoint&
#endif
#ifndef ref_DPoint
#define ref_DPoint DPoint&
#endif
#ifndef ref_MPoint
#define ref_MPoint MPoint&
#endif

using namespace std;
//using namespace ProtoArmor;

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
	/*inline*/ float FDist(FPoint &point1, FPoint &point2);

	class DPoint {
	public:
		double X;
		double Y;
		DPoint();
		string ToString();
	};
	double Dist(DPoint &point1, DPoint &point2);//see also pmath.h

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
