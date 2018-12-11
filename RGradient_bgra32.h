#ifndef GRADIENT_H
#define GRADIENT_H

#include <base.h>
// for cpp:
#include <cstring>  // memcpy etc


namespace ExpertMultimediaBase {

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
		Uint32 get_u32BytesPP();
		Uint32 get_u32BytesTotal();
		byte* get_lpbyShade();
		void SetColors(byte r1, byte g1, byte b1, byte a1, byte r2, byte g2, byte b2, byte a2);
	};
}//end namespace
#endif
