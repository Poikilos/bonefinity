#ifndef RTYPES_CPP
#define RTYPES_CPP

#include <RTypes.h>


using namespace std;

namespace ExpertMultimediaBase {
	IPoint ipZero;
	IPoint::IPoint() {
		X=0;
		Y=0;
	}
	IPoint::IPoint(int xSet, int ySet) {
		Set(xSet,ySet);
	}
	IPoint::IPoint(IPoint& ipSet) {
		Set(ipSet);
	}
	//IPoint::IPoint(IZone izoneTopLeft) {
	//	Set(izoneTopLeft.Left,izoneTopLeft.Top);
	//}
	void IPoint::Set(IPoint& ipSet) {
		try {
			X=ipSet.X;
			Y=ipSet.Y;
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"constructing/setting point using other point","IPoint Set(IPoint)");
		}
	}
	void IPoint::Set(int xSet, int ySet) {
		X=xSet;
		Y=ySet;
	}
	string IPoint::ToString(IPoint& pX) {//static
		return //pX!=null?
		pX.ToString();//:"null";
	}
	string IPoint::ToString(int x, int y) {//static
		return "("+RString_ToString(x)+","+RString_ToString(y)+")";
	}
	string IPoint::ToString() {
		return ExpertMultimediaBase::ToString(X,Y);//calls previous method (above)
	}
	IPoint* IPoint::Copy() {
		IPoint* ipReturn=nullptr;
		try {
			ipReturn=new IPoint();
			ipReturn->X=X;
			ipReturn->Y=Y;
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"","IPoint copy");
		}
		return ipReturn;
	}
	void IPoint::CopyTo(IPoint& ipToModify) {
		try {
			ipToModify.X=X;
			ipToModify.Y=Y;
		}
		catch (exception& exn) {
			PReporting::ShowExn(exn,"","IPoint copy");
		}
		return;
	}
	ILine::ILine() {
		Set(0,0,0,0);
	}
	ILine::ILine(int x1, int y1, int x2, int y2) {
		Set(x1, y1, x2, y2);
	}
	void ILine::Set(int x1, int y1, int x2, int y2) {
		X1=x1; Y1=y1; X2=x2; Y2=y2;
	}

	FPoint::FPoint() {
		X=0;
		Y=0;
	}
	string FPoint::ToString() {
		return "("+RString_ToString(X)+","+RString_ToString(Y)+")";
	}

	DPoint::DPoint() {
		X=0;
		Y=0;
	}
	string DPoint::ToString() {
		return "("+RString_ToString(X)+","+RString_ToString(Y)+")";
	}
	//double Dist(DPoint &point1, DPoint &point2) {//see RMath
	//	register double xSquaring=(point1.X-point2.X);
	//	register double ySquaring=(point1.Y-point2.Y);
	//	register double valSumOfSquares=xSquaring*xSquaring+ySquaring*ySquaring;
	//	return ((valSumOfSquares>0)?sqrt(valSumOfSquares):0);
	//}

	string MPoint::ToString() {
			return ExpertMultimediaBase::ToString(X,Y);
	}
	string ToString(MPoint& pX) {
		return pX.ToString();//return pX!=null?pX.ToString():"null";
	}
	string ToString(decimal x, decimal y) {
		return "("+Convert::ToString(x)+","+Convert::ToString(y)+")";
	}

	IRect::IRect() {
		top=0;
		left=0;
		bottom=0;
		right=0;
	}
	string IRect::ToString() {
		return "("+RString_ToString(left)+","+RString_ToString(top)+")to("+RString_ToString(right)+","+RString_ToString(bottom)+")";
	}
}

#endif
