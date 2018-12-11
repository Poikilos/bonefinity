#ifndef GRADIENT_H
#define GRADIENT_H

#include <base.h>


using namespace std;

namespace ExpertMultimediaBase {
	class GBuffer;

	class Gradient {
	private:
		Uint32 u32Levels;
		Uint32 u32BytesPP;
		Uint32 u32BytesTotal;
		byte* lpbyShade;
		bool Init(Uint32 u32SetLevels);
		bool InitNull();
	public:
		Gradient();
		void ShadeAlpha(GBuffer &gbDest, Uint32 u32DestBufferLoc, byte bySrcValue, int iDrawMode);//bool Shade(byte* arrbyDest, uint u32DestLoc, byte bySrcValue);
		void ShadeAlpha(GBuffer &gbDest, Uint32 u32DestBufferLoc, Uint32 u32SrcValue, int iDrawMode);//bool Shade(byte* arrbyDest, uint u32DestLoc, Uint32 u32SrcValue);
	};
}//end namespace
#endif
