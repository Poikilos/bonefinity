#ifndef TARGA_H
#define TARGA_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <RFile.h>


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
	int RLESizeUncompressed(byte* arrbySrc, int iStart, int iSrcSize, int iBytesPerChunk);
	bool Compare(byte* arrbySrc1, int iSrcLoc1, byte* arrbySrc2, int iSrcLoc2, int iRun);
	byte* RLECompress(ref_int iReturnLength, byte* arrbySrc, int iSrcStart, int iBytesPerChunk, int iBytesToParse, bool bCountOnlyAndReturnNull);
	byte* RLECompress(ref_int iReturnLength, byte* arrbySrc, int iSrcStart, int iBytesPerChunk, int iBytesToParse);
	byte* RLEUncompress(ref_int iReturnLength, byte* arrbySrc, int iSrcStart, int iBytesPerChunk, int iBytesToParse, bool bCountOnlyAndReturnNull);
	byte* RLEUncompress(ref_int iReturnLength, byte* arrbySrc, int iSrcStart, int iBytesPerChunk, int iBytesToParse);
	int RLEUncompress(byte* arrbyDest, int iDestSize, byte* arrbySrc, int iSrcSize, int iBytesPerChunk);
	int RLEUncompress(byte* arrbyDest, int iDestSizeIrrelevantIfCountOnlyIsTrue, byte* arrbySrc, int iSrcSize, int iBytesPerChunk, int iDestStart, int iSrcStart, bool bCountOnlyAndDontTouchDest);
	//#endregion prototypes

	//#region TargaFooter
	class TargaFooter {
	private:
		byte *dump;
		uint u32SizeofDump;
	public:
		TargaFooter();
		TargaFooter(byte* lpbyDataPointerToKeep, uint u32Size);
		TargaFooter(byte* arrbyDataSrcToCopyFrom, uint u32Start, uint u32Count, uint u32ActualSourceBufferSize);
		~TargaFooter();
		bool Init();
		bool Init(byte* lpbyDataPointerToKeep, uint u32Size);
		bool Init(byte* arrbyDataSrc, uint u32SrcStart, uint u32Count, uint u32ActualSourceBufferSize);
		bool WriteTo(Byter &byterNow);
		uint ByteCount();
	};
	//#endregion TargaFooter

	class Targa {
	public:
		string sFile;
		TargaFooter footer;
		byte *arrbyData;

		Targa();
		~Targa();
		int BytesPP();
		int Stride();
		int BytesAsUncompressed();
		int BytesBuffer();
		bool Init(int iSetWidth, int iSetHeight, int iSetBytesPP, bool bReallocateBuffers);
		bool CopyTo(Targa &targaDest);
		bool DrawFast(byte* arrbyDest,  int xAtDest, int yAtDest, int iDestWidth, int iDestHeight, int iDestBytesPP, int iDestStride);
		void ToRect(ref_Rectangle rectReturn);
		void ToRect(ref_RectangleF rectReturn);
		bool From(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, bool bUsePointerNotCopyData);
		bool From(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, bool bUsePointerNotCopyData, uint u32SrcStart);
		int SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, uint u32SrcRealLen);
		int SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, uint u32SrcRealLen, bool bReInitializeAll);
		int SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, uint u32SrcRealLen, uint u32SrcStart, bool bReInitializeAll);
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
		string Description();
		string Description(bool bVerbose);
	private:
		void DeriveVars();
		void InitNull();
		bool MarkAsCompressed(bool bAsCompressed);
		//header:
			//(byte)(length of id),(byte)(int)MapType,(byte)(int)TypeTarga,(ushort)iMapOrigin,(ushort)iMapLength,(byte)iMapBitDepth,(ushort)xImageLeft,(ushort)yImageBottom,(ushort)iWidth,(ushort)iHeight,(byte)iBitDepth,(byte)bitsDescriptor,(byte[length of id])sID,(byte[iMapLength])(arrbyColorMap),(byte[iBytesAsUncompressed])(arrbyData)
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
		byte *arrbyColorMap; //array of [] bytes  //[byMapBitDepth*wMapLength] -- the palette
		//arrbyData
		//derived fields:
		int iBytesPP;
		int iStride;
		int iBytesAsUncompressed;//byte sizeof image data only
		int iBytesBuffer;
	};
}//end namespace

#endif
