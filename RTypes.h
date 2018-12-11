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
		string ToString();
	};
	extern IPoint ipZero;

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

}
#endif
