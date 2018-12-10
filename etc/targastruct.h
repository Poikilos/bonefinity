#ifndef TARGA_H
#ifndef GBUFFER_H
//not compatible with above

#ifndef TARGASTRUCT_H
#define TARGASTRUCT_H
const int INDEX_TL=0;
const int INDEX_TR=1;
const int INDEX_BL=2;
const int INDEX_BR=3;

// container structure for targa .TGA file
typedef struct TARGAINFOFOOTER_STRUCT {
	BYTE *dump;
	Uint32 dwSizeofDump;
} TARGAINFOFOOTER, *LPTARGAINFOFOOTER;

typedef struct TARGA_STRUCT {
	// Begin header fields in order of writing //
	//BYTE bySizeofID;
	BYTE byMapType;
	BYTE byTgaType;
	USHORT wMapOrigin;
	USHORT wMapLength;
	BYTE byMapBitDepth;
	USHORT xImageOrigin;
	USHORT yImageOrigin;
	USHORT width;
	USHORT height;
	BYTE byBitDepth;
	BYTE bitsDescriptor;//(default zero)
	string sTag;//[bySizeofID] -- custom non-terminated string
	BYTE *byarrColorMap;//[byMapBitDepth*wMapLength] -- the palette
	// End header fields //
	BYTE *byarrData;
	TARGAINFOFOOTER footer;
} TARGA, *LPTARGA;

//PROTOTYPE:
LPTARGA TargaNew(int nWidth1, int nHeight1, int bpp, BYTE byType1);
TARGA** TargaNewSeq(int nWidth1, int nHeight1, int bpp, BYTE byType1, int iFrames);
LPTARGA TargaCopy(LPTARGA lptarga1);
void TargaFill(LPTARGA lptarga, BYTE *byPix, unsigned int uiPixBytes);
void TargaFlip(BYTE *image, int bytes_per_line, int height);
LPTARGA TargaLoad(char* sFile);
TARGA** TargaLoadSeq(char* sFileFirstFrame, int &iReturnFrames);
//void TargaSave(LPTARGA targa, char* sFile); //commented for debug only
void TargaUnload(LPTARGA &lptarga);
void TargaUnloadSeq(TARGA** &lplptarga, int iFrames);

void DrawAlphaPix(LPTARGA lptargaDest, int xPix, int yPix, byte r, byte g, byte b, byte a);
void DrawSubpixelDot(LPTARGA lptargaDest, float xDot, float yDot, Pixel &pixelColor);
void DrawSubpixelDot(LPTARGA lptargaDest, float xDot, float yDot, byte* lpbySrcPixel);
void DrawSubpixelLine(LPTARGA lptargaDest, float xStart, float yStart, float xEnd, float yEnd,
		Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement);
void DrawSubpixelLine(LPTARGA lptargaDest, FPOINT &pointStart, FPOINT &pointEnd,
		Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement);
void DrawSubPixArc(LPTARGA lptargaDest, float xCenter, float yCenter,
		float fRadius, float fWidthMultiplier, float fRotate,
		float fDegStart, float fDegEnd,
		Pixel &pixelColor,
		float fPrecisionIncrement,float fPushSpiralPixPerRotation);


#endif
#endif
#endif
