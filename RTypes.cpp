#ifndef RTYPES_CPP
#define RTYPES_CPP

#include "RTypes.h"
#include "base.h"
#include "preporting.h"

using namespace std;

namespace ExpertMultimediaBase {
	IPoint ipZero;
	IPoint::IPoint() {
		X=0;
		Y=0;
	}
	string IPoint::ToString() {
		return "("+RString_ToString(X)+","+RString_ToString(Y)+")";
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

}

#endif
