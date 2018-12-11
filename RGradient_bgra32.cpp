#ifndef GRADIENT_CPP
#define GRADIENT_CPP

#include <RGradient_bgra32.h>

using namespace std;

namespace ExpertMultimediaBase {
	Gradient::Gradient() {
		InitNull();
		Init(256);
	}
	bool Gradient::InitNull() {
		u32Levels=0;
		u32BytesPP=0;
		u32BytesTotal=0;
		lpbyShade=nullptr;
		return true;
	}
	bool Gradient::Init(Uint32 u32SetLevels) {
		bool bGood=false;
		try {
			SafeFree(lpbyShade,"lpbyShade in Gradient::Init");
			u32BytesPP=4;
			u32BytesTotal=u32SetLevels*u32BytesPP;
			lpbyShade=(byte*)malloc(sizeof(byte)*u32BytesTotal);
			int iLoc=0;
			for (Uint32 iNow=0; iNow<u32SetLevels; iNow++) {
				lpbyShade[iLoc]=SafeByte( ((double)iNow/(double)(u32SetLevels-1)) * 255.0 );
				iLoc++;
				lpbyShade[iLoc]=SafeByte( ((double)iNow/(double)(u32SetLevels-1)) * 255.0 );
				iLoc++;
				lpbyShade[iLoc]=SafeByte( ((double)iNow/(double)(u32SetLevels-1)) * 255.0 );
				iLoc++;
				lpbyShade[iLoc]=SafeByte( ((double)iNow/(double)(u32SetLevels-1)) * 255.0 );
				iLoc++;
			}
			u32Levels=u32SetLevels;
			bGood=true;
		}
		catch (exception& exn) {
			ShowExn(exn,"Gradient Init");
		}
		catch (...) {
			ShowUnknownExn("Gradient Init");
		}
		return bGood;
	}//end Init
	Uint32 Gradient::get_u32BytesPP() {
		return this->u32BytesPP;
	}
	Uint32 Gradient::get_u32BytesTotal() {
		return this->u32BytesTotal;
	}
	byte* Gradient::get_lpbyShade() {
		return this->lpbyShade;
	}
	void Gradient::SetColors(byte r1, byte g1, byte b1, byte a1, byte r2, byte g2, byte b2, byte a2) {
		int iLoc=0;
		float fDestness=-1.0f;
		float fPixelOffset=0.0f;
		float fLastLevel=(float)(u32Levels-1);
		Uint32 dwFill=0;
		if (u32BytesPP>0) {
			if (r1==r2&&g1==g2&&b1==b2) {
				byte* lpAtFill=(byte*)&dwFill;
				lpAtFill[0]=b1;
				lpAtFill[1]=g1;
				lpAtFill[2]=r1;
				lpAtFill[3]=a1;
				//memfill(lpbyShade,u32BytesTotal,lpAtFill,u32BytesPP);
				if (a1!=a2 && u32BytesPP>=4) {
					byte* lpDestA=&lpbyShade[3];
					byte* lpDestPixel=lpbyShade;
					for (Uint32 iPixelOffset=0; iPixelOffset<u32BytesTotal; iPixelOffset+=u32BytesPP) {

						fDestness=fPixelOffset/fLastLevel;
						memcpy(lpDestPixel,lpAtFill,3);
						*lpDestA=SafeByte( APPROACH(a2,a1,fDestness) );
						lpDestA+=u32BytesPP;
						lpDestPixel+=u32BytesPP;
						fPixelOffset+=1.0f;
					}
				}
				else {
					byte* lpDestPixel=lpbyShade;
					for (Uint32 iPixelOffset=0; iPixelOffset<u32BytesTotal; iPixelOffset+=u32BytesPP) {
						memcpy(lpDestPixel,lpAtFill,u32BytesPP);
						lpDestPixel+=u32BytesPP;
					}
				}
			}
			else {
				for (Uint32 iPixelOffset=0; iPixelOffset<u32BytesTotal; iPixelOffset+=u32BytesPP) {
					fDestness=fPixelOffset/fLastLevel;
					lpbyShade[iLoc]=SafeByte( APPROACH(b2,b1,fDestness) );
					if (u32BytesPP>1) {
													lpbyShade[iLoc+1]=SafeByte( APPROACH(g2,g1,fDestness) );
						if (u32BytesPP>2) {
													lpbyShade[iLoc+2]=SafeByte( APPROACH(r2,r1,fDestness) );
							if (u32BytesPP>3) lpbyShade[iLoc+3]=SafeByte( APPROACH(a2,a1,fDestness) );
						}
					}
					iLoc+=(int)u32BytesPP;
					fPixelOffset+=1.0f;
				}
			}
		}
		else {
			Console::Error.WriteLine("ERROR: u32BytesPP is zero in SetColors");
		}
	}
}//end namespace
#endif
