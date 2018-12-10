#ifndef TARGA_H
#define TARGA_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <byter.h>

using namespace std;

namespace ExpertMultimediaBase {
	//Sequential targa.TypeTarga values:
	const int TypeNoImageData		 										= 0;
	const int TypeUncompressedColorMapped									= 1;
	const int TypeUncompressedTrueColor										= 2;
	const int TypeUncompressedGrayscale										= 3;
	const int TypeCompressedColorMapped										= 9;
	const int TypeCompressedTrueColor										= 10;
	const int TypeCompressedGrayscale										= 11;
	const int TypeCompressedColorMappedHuffmanAndDeltaAndRLE				= 32;
	const int TypeCompressedColorMappedHuffmanAndDeltaAndRLE4PassQuadTree	= 33;
	//Sequential targa.MapType values
	const int MapType256					= 1;
	//targa.bitsDescriptor bits:
	const byte lownibble565Or888NoAlpha		= 0;	//bit3
	const byte lownibbleAlpha5551			= 1;	//bit3 //TODO: read GGGBBBBB ARRRRRGG since targa is always low-high (little endian)
	const byte lownibbleAlpha8888			= 8;	//bit3
	const byte bitReserved4					= 16;	//bit4
	const byte bitNoFlip_NonTruevision		= 32;	//bit5 //Truevision is a registered trademark of Truevision
	const byte bitInterleave4Way			= 64;	//bit6
	const byte bitInterleave2Way			= 128;	//bit7
	//#region prototypes
	int RLESizeUncompressed(byte* byarrSrc, int iStart, int iSrcSize, int iBytesPerChunk);
	bool Compare(byte* byarrSrc1, int iSrcLoc1, byte* byarrSrc2, int iSrcLoc2, int iRun);
	byte* RLECompress(ref_int iReturnLength, byte* byarrSrc, int iSrcStart, int iBytesPerChunk, int iBytesToParse, bool bCountOnlyAndReturnNull);
	byte* RLECompress(ref_int iReturnLength, byte* byarrSrc, int iSrcStart, int iBytesPerChunk, int iBytesToParse);
	byte* RLEUncompress(ref_int iReturnLength, byte* byarrSrc, int iSrcStart, int iBytesPerChunk, int iBytesToParse, bool bCountOnlyAndReturnNull);
	byte* RLEUncompress(ref_int iReturnLength, byte* byarrSrc, int iSrcStart, int iBytesPerChunk, int iBytesToParse);
	int RLEUncompress(byte* byarrDest, int iDestSize, byte* byarrSrc, int iSrcSize, int iBytesPerChunk);
	int RLEUncompress(byte* byarrDest, int iDestSizeIrrelevantIfCountOnlyIsTrue, byte* byarrSrc, int iSrcSize, int iBytesPerChunk, int iDestStart, int iSrcStart, bool bCountOnlyAndDontTouchDest);
	//#endregion prototypes
	
	//#region TargaFooter
	class TargaFooter {
	private:
		byte *dump;
		uint dwSizeofDump;
	public:
	    TargaFooter();
	    TargaFooter(byte* lpbyDataPointerToKeep, uint dwSize);
	    TargaFooter(byte* byarrDataSrcToCopyFrom, uint dwStart, uint dwCount, uint dwActualSourceBufferSize);
		~TargaFooter();
		bool Init();
		bool Init(byte* lpbyDataPointerToKeep, uint dwSize);
		bool Init(byte* byarrDataSrc, uint dwSrcStart, uint dwCount, uint dwActualSourceBufferSize);
		bool WriteTo(Byter &byterNow);
		uint ByteCount();
	};
	//#endregion TargaFooter

	class Targa {
	public:
		string sFile;
		TargaFooter footer;
		byte *byarrData;

		Targa();
		~Targa();
		int BytesPP();
		int Stride();
		int BytesAsUncompressed();
		int BytesBuffer();
		bool Init(int iSetWidth, int iSetHeight, int iSetBytesPP, bool bReallocateBuffers);
		bool CopyTo(Targa &targaDest);
		bool DrawFast(byte* byarrDest,  int xAtDest, int yAtDest, int iDestWidth, int iDestHeight, int iDestBytesPP, int iDestStride);
		void ToRect(ref_Rectangle rectReturn);
		void ToRect(ref_RectangleF rectReturn);
		bool From(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, bool bUsePointerNotCopyData);
		bool From(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, bool bUsePointerNotCopyData, uint dwSrcStart);
		int SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, uint dwSrcRealLen);
		int SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, uint dwSrcRealLen, bool bReInitializeAll);
		int SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, uint dwSrcRealLen, uint dwSrcStart, bool bReInitializeAll);
		byte* GetBufferPointer();
		bool IsLoaded();
		bool Save();
		bool Save(string sFileNow);
		bool Load(string sFileNow);
		bool IsOK();
		bool Flip();
		bool HasAttrib(byte bit);
		bool IsSavedAsFlipped();
		bool IsCompressed();
		bool SetCompressionRLE(bool bOn);
		string Dump();
		string Dump(bool bDumpFull);
	private:
		void DeriveVars();
		void InitNull();
		bool MarkAsCompressed(bool bAsCompressed);
		//header:
			//(byte)(length of id),(byte)(int)MapType,(byte)(int)TypeTarga,(ushort)iMapOrigin,(ushort)iMapLength,(byte)iMapBitDepth,(ushort)xImageLeft,(ushort)yImageBottom,(ushort)iWidth,(ushort)iHeight,(byte)iBitDepth,(byte)bitsDescriptor,(byte[length of id])sID,(byte[iMapLength])(byarrColorMap),(byte[iBytesAsUncompressed])(byarrData)
		//int iIDLength; //1 byte implied (length of sID)
		int MapType; //1 byte
		int TypeTarga; //1 byte
		int iMapOrigin; //2 bytes
		int iMapLength; //2 bytes
		int iMapBitDepth; //1 byte
		int xImageLeft; //2 bytes
		int yImageBottom; //2 bytes //TODO: don't flip if not zero
		int iWidth; //2 bytes
		int iHeight; //2 bytes
		int iBitDepth; //1 byte //TODO: 16 is 5.5.5.1 (!!!)IF(!!!) low nibble of descriptor is 1 (otherwise 5.6.5 and descriptor low nibble is zero)
		byte bitsDescriptor; //1 byte  //(default zero)
		string sID; //array of [iTagLength] bytes  //[bySizeofID] -- custom non-terminated string
		byte *byarrColorMap; //array of [] bytes  //[byMapBitDepth*wMapLength] -- the palette
		//byarrData
		//derived fields:
		int iBytesPP; 
		int iStride;
		int iBytesAsUncompressed;//byte sizeof image data only
		int iBytesBuffer;
	};
}//end namespace

#endif
