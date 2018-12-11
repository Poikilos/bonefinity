#ifndef GBUFFER_H
#define GBUFFER_H

#include <base.h>
#include <Targa.h>
#include <RGradient_bgra32.h> //#include <RGradient_bgra32.h>

using namespace std;

/// <summary>
/// For simple graphics buffers used as images, variable-size frames, or graphics surfaces.
/// </summary>
namespace ExpertMultimediaBase {
	extern byte by2d_ByteAsByte_times_ByteAsFloatLookup[256][256];
	//Sequential iDrawMode Draw(...) argument values:
	const int DrawModeCopyAlpha			= 0; //copy whole pixel and do no blending
	const int DrawModeBlendAlpha			= 1;
	const int DrawModeKeepGreaterAlpha	= 2;
	const int DrawModeKeepDestAlpha		= 3;

	const int INDEX_TL=0;
	const int INDEX_TR=1;
	const int INDEX_BL=2;
	const int INDEX_BR=3;
	class GBuffer {
	public:
		Targa targaLoaded;//Bitmap bmpLoaded;
		string sErrBy;
		string sFile;
		bool bBufferAsPointerNotCopy;
		//int iType;
		byte* arrbyData;
		int iWidth;
		int iHeight;
		int iBytesPP;
		int iStride;
		int iBytesTotal;
		byte arrbyBrush[4];
		byte arrbyBrush32Copied64[8];
		byte* BytePtrStart();
		void InitNull();
		GBuffer(string sFileImage);
		bool Load(string sFileNow);
		bool Save(string sFileNow);
		bool SaveJec(string sFileNow, int iSetFrames, int iColorTolerance, int iLightnessTolerance);
		bool SaveRaw(string sFileNow);
		GBuffer();
		GBuffer(int iSetWidth, int iSetHeight, int iSetBytesPP);
		GBuffer(int iSetWidth, int iSetHeight, int iSetBytesPP, bool bInitializeBuffer);
		~GBuffer();
		void Dispose();
		void Init(int iSetWidth, int iSetHeight, int iSetBytesPP);
		void Init(int iSetWidth, int iSetHeight, int iSetBytesPP, bool bInitializeBuffer);
		bool CopyTo(GBuffer* gbBlankNonNullObjectToSet);
		bool CopyTo(GBuffer &gbBlankNonNullObjectToSet);
		bool CopyToByDataRef(GBuffer &gbBlankNonNullObjectToSet);
		GBuffer* Copy();
		string Description();
		string Description(bool bVerbose);
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
		void DrawSubpixelArc(float xCenter, float yCenter,
				float fRadius, float fWidthMultiplier, float fRotate,
				float fDegStart, float fDegEnd,
				Pixel &pixelColor,
				float fPrecisionIncrement,float fPushSpiralPixPerRotation);
		bool Fill(byte byGrayVal);
		bool IsLoaded();
		bool IsLike(GBuffer &gbDest);
		bool NeedsToCrop(GBuffer &gbDest, int xDest, int yDest);
		//bool DrawToSmallerWithoutVoidAreasElseCancel(GBuffer gbDest, int xSrc, int ySrc, int iDrawMode);
		bool DrawToWithClipping(GBuffer &gbDest, int xDest, int yDest, float fOpacityMultiplier);
		bool DrawToLargerWithoutCropElseCancel(GBuffer &gbDest, int xDest, int yDest, int iDrawMode);
	};//end class GBuffer

	//////////////////////////////// PROTOTYPES ////////////////////////////////////


	bool RawOverlayNoClipToBig(GBuffer &gbDest, IPoint &ipAt, byte* arrbySrc, int iSrcWidth, int iSrcHeight, int iSrcBytesPP);
	bool OverlayNoClipToBig(GBuffer &gbDest, GBuffer &gbSrc, IPoint &ipDest, GBuffer &gradNow, int iSrcChannel);
	bool OverlayNoClipToBigCopyAlpha(GBuffer &gbDest, IPoint &ipAt, GBuffer &gbSrc, Gradient &gradNow, int iSrcChannel);
	bool OverlayNoClipToBigCopyAlpha(GBuffer &gbDest, IPoint &ipAt, GBuffer &gbSrc);
	bool MaskFromChannel(GBuffer &gbDest, GBuffer &gbSrc, int iByteInPixel);
	bool MaskFromValue(GBuffer &gbDest, GBuffer &gbSrc);
	bool InterpolatePixel(GBuffer &gbDest, GBuffer &gbSrc, int iDest, DPoint &dpSrc);
	bool EffectMoBlurSimModWidth(GBuffer &gbDest, GBuffer &gbSrc, int xOffsetTotal, byte byDecayTotal);
	bool EffectSkewModWidth(GBuffer &gbDest, GBuffer &gbSrc, int xOffsetBottom);
	bool EffectLightenOnly(byte* arrbyDest, byte* arrbySrc, int iDestByte, int iSrcByte, int iBytes);
	bool EffectLightenOnly(byte* arrbyDest, byte* arrbySrc, int iDestByte, int iSrcByte, int iBytes, float fMultiplySrc);
	bool CopyTo(GBuffer &gbDest);
}//end namespace
#endif
