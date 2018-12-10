#ifndef GRADIENT_CPP
#define GRADIENT_CPP

/*
#include <gradient.h>

using namespace std;

namespace ExpertMultimediaBase {
	bool Gradient::Shade(byte* byarrDest, Uint32 dwDestLoc, byte bySrcValue) {
		return Shade(byarrDest, dwDestLoc, (Uint32)bySrcValue);
	}
	bool Gradient::Shade(byte* byarrDest, Uint32 dwDestLoc, Uint32 dwSrcValue) {
		try {
			if (lpbyShade!=null) {
				byarrDest[dwDestLoc]=lpbyShade[dwSrcValue];
			}
		}
		catch (char* carrExn) {
			ShowAndDeleteException(carrExn, "Gradient::Shade");
		}
		catch (char* carrExn) {
			ShowUnknownException("Gradient::Shade");
		}
	}
	Gradient::Gradient() {
		InitNull();
		Init(256);
	}
	bool Gradient::InitNull() {
		dwLevels=0;
		lpbyShade=NULL;
	}
	bool Gradient::Init(Uint32 dwSetLevels) {
		try {
			SafeFree(lpbyShade);
			lpbyShade=(byte*)malloc(sizeof(byte)*dwSetLevels);
			dwLevels=dwSetLevels;
		}
		catch (char* carrExn) {
			ShowAndDeleteException(carrExn, "Gradient::Init");
		}
		catch (char* carrExn) {
			ShowUnknownException("Gradient::Init");
		}
	}
}//end namespace
*/
#endif
