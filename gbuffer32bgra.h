#ifndef GBUFFER32BGRA_H
#define GBUFFER32BGRA_H

#include <base.h>
#include <targa.h>
//#include <gradient.h>

using namespace std;

/// <summary>
/// For simple graphics buffers used as images, variable-size frames, or graphics surfaces.
/// </summary>
namespace ExpertMultimediaBase {
	//Sequential iDrawMode Draw(...) argument values:
	const int DrawModeCopyAlpha			= 0;
	const int DrawModeBlendAlpha		= 1;
	const int DrawModeKeepGreaterAlpha	= 2;
	const int DrawModeKeepDestAlpha		= 3;

	const int INDEX_TL=0;
	const int INDEX_TR=1;
	const int INDEX_BL=2;
	const int INDEX_BR=3;
	class GBuffer32BGRA {
	public:
		Targa targaLoaded;//Bitmap bmpLoaded;
		string sErrBy;
		string sFile;
		bool bBufferAsPointerNotCopy;
		//int iType;
		byte* byarrData;
		int iWidth;
		int iHeight;
		int iBytesPP;
		int iStride;
		int iBytesTotal;
		byte byarrBrush[4];
		byte byarrBrush32Copied64[8];

		byte* BytePtrStart();
		void InitNull();
		GBuffer32BGRA(string sFileImage);
		bool Load(string sFileNow);
		bool Save(string sFileNow);
		bool SaveRaw(string sFileNow);
		GBuffer32BGRA();
	    GBuffer32BGRA(int iWidthNow, int iHeightNow, int iBytesPPNow);
		GBuffer32BGRA(int iWidthNow, int iHeightNow, int iBytesPPNow, bool bInitializeBuffer);
		~GBuffer32BGRA();
		void Init(int iWidthNow, int iHeightNow, int iBytesPPNow);
		void Init(int iWidthNow, int iHeightNow, int iBytesPPNow, bool bInitializeBuffer);
        bool CopyTo(GBuffer32BGRA* gbBlankNonNullObjectToSet);
		bool CopyTo(GBuffer32BGRA &gbBlankNonNullObjectToSet);
		GBuffer32BGRA* Copy();
		bool SetBrushColor(byte r, byte g, byte b, byte a);
		bool SetBrushColor(byte r, byte g, byte b);
		bool SetBrushColor(string sHexCode);
	    bool DrawRect(int xDest, int yDest, int iRectWidth, int iRectHeight);
		bool DrawRect(IRect &irectExclusive);
		bool DrawRect(Rectangle &rectNow);
		bool DrawRectFilled(Rectangle &rectNow);
		bool DrawRectBorderSym(Rectangle &rectDest, Rectangle &rectHole);
		bool DrawRectBorder(int xDest, int yDest, int iRectWidth, int iRectHeight, int iThick);
		bool DrawRectFilled(int xDest, int yDest, int iRectWidth, int iRectHeight);
		bool DrawVertLine(int xDest, int yDest, int iPixelCopies);
		bool DrawHorzLine(int xDest, int yDest, int iPixelCopies);

		//additions in cpp only (not in c#):
		void DrawAlphaPix(int xPix, int yPix, byte r, byte g, byte b, byte a);
		void DrawSubpixelDot(float xDot, float yDot, Pixel &pixelColor);
		void DrawSubpixelDot(float xDot, float yDot, byte* lpbySrcPixel);
		void DrawSubpixelLine(float xStart, float yStart, float xEnd, float yEnd,
				Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement);
		void DrawSubpixelLine(FPOINT &pointStart, FPOINT &pointEnd,
				Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement);
		void DrawSubPixArc(float xCenter, float yCenter,
				float fRadius, float fWidthMultiplier, float fRotate,
				float fDegStart, float fDegEnd,
				Pixel &pixelColor,
				float fPrecisionIncrement,float fPushSpiralPixPerRotation);
		bool Fill(byte byGrayVal);
		bool IsLike(GBuffer32BGRA &gbDest);
		bool NeedsToCrop(GBuffer32BGRA &gbDest, int xDest, int yDest);
		//bool DrawToSmallerWithoutVoidAreasElseCancel(GBuffer32BGRA gbDest, int xSrc, int ySrc, int iDrawMode);
		bool DrawToLargerWithoutCropElseCancel(GBuffer32BGRA &gbDest, int xDest, int yDest, int iDrawMode);
	};//end class GBuffer32BGRA

	//////////////////////////////// PROTOTYPES ////////////////////////////////////

	double dDiagonalUnit = 1.4142135623730950488016887242097;//the sqrt. of 2, dist of diagonal pixel

	bool RawOverlayNoClipToBig(GBuffer32BGRA &gbDest, IPoint &ipAt, byte* byarrSrc, int iSrcWidth, int iSrcHeight, int iSrcBytesPP);
	bool OverlayNoClipToBig(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, IPoint &ipDest, Gradient &gradNow, int iSrcChannel);
	bool OverlayNoClipToBigCopyAlpha(GBuffer32BGRA &gbDest, IPoint &ipAt, GBuffer32BGRA &gbSrc, Gradient &gradNow, int iSrcChannel);
	bool OverlayNoClipToBigCopyAlpha(GBuffer32BGRA &gbDest, IPoint &ipAt, GBuffer32BGRA &gbSrc);
	bool MaskFromChannel(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int iByteInPixel);
	bool MaskFromValue(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc);
	bool InterpolatePixel(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int iDest, DPoint &dpSrc);
	bool EffectMoBlurSimModWidth(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int xOffsetTotal, byte byDecayTotal);
	bool EffectSkewModWidth(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int xOffsetBottom);
	bool EffectLightenOnly(byte* byarrDest, byte* byarrSrc, int iDestByte, int iSrcByte, int iBytes);
	bool EffectLightenOnly(byte* byarrDest, byte* byarrSrc, int iDestByte, int iSrcByte, int iBytes, float fMultiplySrc);
	bool CopyTo(GBuffer32BGRA &gbDest);
}//end namespace
#endif
