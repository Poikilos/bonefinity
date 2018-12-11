#ifndef TARGA_CPP
#define TARGA_CPP

#include <Targa.h>

using namespace std;

namespace ExpertMultimediaBase {
	//#region static targa functions
	int RLESizeUncompressed(byte* arrbySrc, int iStart, int iSrcSize, int iBytesPerChunk) {
		byte* arrbyNull=nullptr;
		return RLEUncompress(arrbyNull, 0, arrbySrc, iSrcSize, iBytesPerChunk, 0, iStart, true);
	}
	bool Compare(byte* arrbySrc1, int iSrcLoc1, byte* arrbySrc2, int iSrcLoc2, int iRun) {
		bool bMatch=false;
		int iMatch=0;
		try {
			for (int iNow; iNow<iRun; iNow++) {
				if (arrbySrc1[iSrcLoc1+iNow]==arrbySrc2[iSrcLoc2+iNow]) iMatch++;
			}
			if (iMatch==iRun) bMatch=true;
		}
		catch (exception& exn) {
			ShowExn(exn,"Compare");
		}
		catch (...) {
			ShowUnknownExn("Compare");
		}
		return bMatch;
	}

	byte* RLECompress(ref_int iReturnLength, byte* arrbySrc, int iSrcStart, int iBytesToParse, int iBytesPerChunk) {
		return RLECompress(iReturnLength, arrbySrc, iSrcStart, iBytesToParse, iBytesPerChunk, false);
	}
	byte* RLECompress(ref_int iReturnLength, byte* arrbySrc, int iSrcStart, int iBytesToParse, int iBytesPerChunk, bool bCountOnlyAndReturnNull) {
		bool bGood=true;
		iReturnLength=0;
		int iTotal=0;
		byte* arrbyReturn=nullptr;
		try {
			if (!bCountOnlyAndReturnNull) {
				byte* arrbyNull=RLECompress(iTotal,arrbySrc,iSrcStart,iBytesToParse,iBytesPerChunk,true);//OK since counts & doesn't recurse from there!
				if (iTotal>0) {
					arrbyReturn=(byte*)malloc(iTotal);//this is set using the bGood case below
					//NOTE: do NOT use arrbyNull, since was just used for counting
				}
				else {
					bGood=false;
					ShowErr("Compressed total of "+RString_ToString(iTotal)+RString_ToString(" wasn't calculated correctly"),"RLECompress");
				}
			}
			if (bGood) {
				int iSrcAbs=iSrcStart;
				int iRun=0;
				bool bRunIsCompressed=false;
				int iSrcRel=0;
				int iNow;
				bool bPartOfThisTypeOfRun;
				int iPacketByte=0;
				while (iSrcRel<iBytesToParse) {
					bRunIsCompressed=( (iSrcRel+iBytesPerChunk*2<=iBytesToParse) && Compare(arrbySrc,iSrcAbs,arrbySrc,iSrcAbs+iBytesPerChunk,iBytesPerChunk) )  ?  true  :  false;
					iRun=bRunIsCompressed?1:0;
					bPartOfThisTypeOfRun=true;
					iPacketByte=iReturnLength;
					iReturnLength++; //advance past RLE packet header byte (set it later)
					while (bPartOfThisTypeOfRun) {
						//Advance through chunk and copy it:
						for (iNow=0; iNow<iBytesPerChunk; iNow++) {
							if (!bRunIsCompressed || iRun==1) {
								if (!bCountOnlyAndReturnNull)
									arrbyReturn[iReturnLength]=arrbySrc[iSrcAbs];
								iReturnLength++;
							}
							iSrcRel++;
							iSrcAbs++;
						}
						iRun++;
						//calculate booleans for NEXT run:
						if (iRun==128) bPartOfThisTypeOfRun=false;
						else {
							if (bRunIsCompressed)
								bPartOfThisTypeOfRun=( (iSrcRel+iBytesPerChunk*2<=iBytesToParse) && Compare(arrbySrc,iSrcAbs,arrbySrc,iSrcAbs+iBytesPerChunk,iBytesPerChunk) )  ?  true  :  false;
							else
								bPartOfThisTypeOfRun=( (iSrcRel+iBytesPerChunk*2<=iBytesToParse) && !Compare(arrbySrc,iSrcAbs,arrbySrc,iSrcAbs+iBytesPerChunk,iBytesPerChunk) )  ?  true  :  false;
						}//end else not too long
					}
					if (!bCountOnlyAndReturnNull) {
						if (bRunIsCompressed) arrbyReturn[iPacketByte]=0x80 & (byte)(iRun-1);
						else arrbyReturn[iPacketByte]=(byte)(iRun-1);
					}
				}//end while iSrcRel<iBytesToParse
			}//end if bGood
		}
		catch (exception& exn) {
			ShowExn(exn,"RLECompress");
		}
		catch (...) {
			ShowUnknownExn("RLECompress");
		}
		return arrbyReturn;
	}//end RLECompress

	//RLEUncompress(arrbyDest, iDestSize, arrbySrc, iSrcSize, iBytesPerChunk);
	int RLEUncompress(byte* arrbyDest, int iDestSize, byte* arrbySrc, int iSrcSize, int iBytesPerChunk) {
		if (iDestSize<0) iDestSize=0;
		return RLEUncompress(arrbyDest, iDestSize, arrbySrc, iSrcSize, iBytesPerChunk, 0, 0, false);
	}
	//int iBytesFound=RLEUncompress(arrbyDest, iDestSizeIrrelevantIfCountOnlyIsTrue, arrbySrc, iSrcSize, iBytesPerChunk, iSrcStart, bCountOnlyAndDontTouchDest)
	int RLEUncompress(byte* arrbyDest, int iDestSizeIrrelevantIfCountOnlyIsTrue, byte* arrbySrc, int iSrcSize, int iBytesPerChunk, int iDestStart, int iSrcStart, bool bCountOnlyAndDontTouchDest) {
		bool bGood=true;
		int iDestAbs=iDestStart;
		int iDestRel=0;
		//int iTotal=0;
		try {
			if (bGood) {
				int iSrcAbs=iSrcStart;
				int iRun=0;
				bool bRunIsCompressed=false;
				int iSrcRel=0;
				int iChunkRelNow;
				while (iSrcAbs<iSrcSize) {
					if (arrbySrc[iSrcAbs]>=128) { //if high bit is true
						bRunIsCompressed=true;
						iRun=((int)arrbySrc[iSrcAbs]-128)+1;
					}
					else {
						bRunIsCompressed=false;
						iRun=(int)arrbySrc[iSrcAbs]+1;
					}
					//first advance past packet header byte:
					iSrcRel++;
					iSrcAbs++;
					for (iChunkRelNow=0; iChunkRelNow<iRun && iSrcAbs<iSrcSize; iChunkRelNow++) {
						for (int iChunkSubByte=0; iChunkSubByte<iBytesPerChunk && (iSrcAbs+iChunkSubByte)<iSrcSize; iChunkSubByte++) {
							if (!bCountOnlyAndDontTouchDest) {
								if (iDestAbs>=iDestSizeIrrelevantIfCountOnlyIsTrue) {
									ShowError("Compressed data wanted destination bigger than "+RString_ToString(iDestSizeIrrelevantIfCountOnlyIsTrue)+" bytes.","RLEUncompress");
									return 0;
								}
								arrbyDest[iDestAbs]=arrbySrc[iSrcAbs+iChunkSubByte];
							}//end if actually uncompressing
							iDestAbs++;
							iDestRel++;
						}
						if (!bRunIsCompressed) {
							//has to be incremented separately here in case pixel needed to be used multiple times in this outer loop
							//It is important that we increment past the whole chunk now (this makes sure alignment will be right) instead of iBytesCopiedNow
							iSrcRel+=iBytesPerChunk;
							iSrcAbs+=iBytesPerChunk;
						}
					}//end for iChunkRelNow of run
					if (bRunIsCompressed) {
						//has to be incremented separately here so that pixel could be used multiple times above
						//It is important that we increment past the whole chunk now (this makes sure alignment will be right) instead of iBytesCopiedNow
						iSrcRel+=iBytesPerChunk;
						iSrcAbs+=iBytesPerChunk;
					}
				}//end while still any data (iSrcAbs<iSrcSize)
			}
		}
		catch (exception& exn) {
			ShowExn(exn,"RLEUncompress");
		}
		catch (...) {
			ShowUnknownExn("RLEUncompress");
		}
		//Run-length packet:
		//--byte with high bit=true
		//  --the rest of the bits are the run-length minus 1 (128 max, since 0 is 1 long)
		//--then one pixel of raw data, or a palette index (to be repeated by run-length)
		//Raw packet:
		//--byte with high bit=false
		//  --the rest of the bits are the number of raw pixels minus 1 (128 max, since 0 is 1 long)
		//  --then run-length number of pixels of raw data, or palette indeces
		if (!bGood) iDestRel=0;
		return iDestRel;
	}//end RLEUncompress
	//#endregion static Targa functions

	//#region TargaFooter methods
	TargaFooter::TargaFooter(byte* arrbyDataSrcToCopyFrom, uint u32Start, uint u32Count, uint u32ActualSourceBufferSize) {
		Init(arrbyDataSrcToCopyFrom, u32Start, u32Count, u32ActualSourceBufferSize);
	}
	TargaFooter::TargaFooter(byte* lpbyDataPointerToKeep, uint u32Size) {
		Init(lpbyDataPointerToKeep,u32Size);
	}
	TargaFooter::TargaFooter() {
		dump=nullptr;
		u32SizeofDump=0;
	}
	TargaFooter::~TargaFooter() {
		SafeFree(dump, "dump in ~TargaFooter");
	}
	bool TargaFooter::Init(byte* arrbyDataSrcToCopyFrom, uint u32SrcStart, uint u32Count, uint u32ActualSourceBufferSize) {
		bool bGood=true;
		SafeFree(dump, "dump in TargaFooter::Init");
		try {
			u32SizeofDump=u32Count;
			dump=(byte*)malloc(u32Count);
			uint iSrc=u32SrcStart;
			for (uint iNow=0; iNow<u32Count && iSrc<u32ActualSourceBufferSize; iNow++,iSrc++) { //debug performance
				dump[iNow]=arrbyDataSrcToCopyFrom[iSrc];
			}
		}
		catch (exception& exn) {
			ShowExn(exn, "TargaFooter::Init");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("TargaFooter::Init");
		}
		return bGood;
	}
	bool TargaFooter::Init(byte* lpbyDataPointerToKeep, uint u32Size) {
		bool bGood=true;
		try {
			if (u32Size>0 && lpbyDataPointerToKeep!=nullptr) {
				u32SizeofDump=u32Size;
				dump=lpbyDataPointerToKeep;
				//TODO: process the footer here
			}
			else {
				u32SizeofDump=0;
				dump=nullptr;
			}
		}
		catch (exception& exn) {
			bGood=false;
			string sMsg="u32Size="+RString_ToString(u32Size);
			sMsg+="; lpbyDataPointerToKeep is";
			sMsg+=((lpbyDataPointerToKeep==nullptr)?"null.":"not null.");
			ShowExn(exn,"TargaFooter::Init"+sMsg);
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("TargaFooter::Init");
		}
		return bGood;
	}
	bool TargaFooter::Init() {
		return Init(nullptr,0);
	}
	bool TargaFooter::WriteTo(Byter &byterNow) {
		bool bGood=true;
		for (uint iNow=0; iNow<u32SizeofDump; iNow++) {
			if( !byterNow.Write(dump[iNow]) ) {
				bGood=false;
				break;
			}
		}
		return bGood;
	}//end WriteTo byter
	uint TargaFooter::ByteCount() {
		return u32SizeofDump;
	}
	//#endregion TargaFooter methods

	//#region Targa methods
	Targa::Targa() {
		InitNull();
	}
	Targa::~Targa() {
		SafeFree(arrbyData, "arrbyData in ~Targa");
	}
	int Targa::BytesPP() {
		return iBytesPP;
	}
	int Targa::Stride() {
		return iStride;
	}
	int Targa::BytesAsUncompressed() {
		return iBytesAsUncompressed;
	}
	int Targa::BytesBuffer() {
		return iBytesBuffer;
	}
	bool Targa::Init(int iSetWidth, int iSetHeight, int iSetBytesPP, bool bReallocateBuffers) {
		bool bGood=true;
		if (bReallocateBuffers) {
			SafeFree(arrbyColorMap, "arrbyColorMap in Targa::Init");
			iMapLength=0;
			sID="";
			SafeFree(arrbyData, "arrbyData in Targa::Init");
		}
		iWidth=iSetWidth;
		iHeight=iSetHeight;
		iBitDepth=iSetBytesPP*8;
		DeriveVars();//does get iBytesPP
		if (iSetBytesPP==4) {
			TypeTarga=TypeUncompressedTrueColor;
			bitsDescriptor=lownibbleAlpha8888;
		}
		else if (iSetBytesPP==3) {
			TypeTarga=TypeUncompressedTrueColor;
			bitsDescriptor=lownibble565Or888NoAlpha;
		}
		else if (iSetBytesPP==1) {
			TypeTarga=TypeUncompressedGrayscale;
			bitsDescriptor=0;//TODO: is this right?
		}
		//TODO: implement palette etc modes
		iBytesBuffer=iBytesAsUncompressed;
		if (iBytesBuffer<=0) {
			ShowError("iBytesBuffer="+RString_ToString(iBytesBuffer)+"!","Targa::Init");
			bGood=false;
		}
		if (bReallocateBuffers && bGood) {
			arrbyData=(byte*)malloc(iBytesBuffer);
		}
		return bGood;
	}//end Targa::Init
	bool Targa::CopyTo(Targa &targaDest) {
		bool bGood=false;
		try {
			targaDest.Init(iWidth,iHeight,iBytesPP,false);
			if (iBytesBuffer>0) targaDest.arrbyData=(byte*)malloc(iBytesBuffer);
			targaDest.MapType=MapType; //1 byte
			targaDest.TypeTarga=TypeTarga; //1 byte
			targaDest.iMapOrigin=iMapOrigin; //2 bytes
			targaDest.iMapLength=iMapLength; //2 bytes
			targaDest.iMapBitDepth=iMapBitDepth; //1 byte
			targaDest.xImageLeft=xImageLeft; //2 bytes
			targaDest.yImageBottom=yImageBottom; //2 bytes //TODO: don't flip if not zero
			targaDest.iWidth=iWidth; //2 bytes
			targaDest.iHeight=iHeight; //2 bytes
			targaDest.iBitDepth=iBitDepth; //1 byte //TODO: 16 is 5.5.5.1 (!!!)IF(!!!) low nibble of descriptor is 1 (otherwise 5.6.5 and descriptor low nibble is zero)
			targaDest.bitsDescriptor=bitsDescriptor; //1 byte  //(default zero)
			targaDest.sID=sID; //array of [iTagLength] bytes  //[bySizeofID] -- custom non-terminated string
			targaDest.iBytesPP=iBytesPP;
			targaDest.iStride=iStride;
			targaDest.iBytesAsUncompressed=iBytesAsUncompressed;//byte sizeof image data only
			targaDest.iBytesBuffer=iBytesBuffer;
			int iNow;
			if (iBytesBuffer>0) {
				for (iNow=0; iNow<iBytesBuffer; iNow++) {
					targaDest.arrbyData[iNow]=arrbyData[iNow];
				}
			}
			else ShowErr("Warning: Program tried to duplicate null targa.","Targa CopyTo");
			if (iMapLength>0) {
				targaDest.arrbyColorMap=(byte*)malloc(iMapLength); //array of [] bytes  //[byMapBitDepth*wMapLength] -- the palette
				for (iNow=0; iNow<iMapLength; iNow++) {
					targaDest.arrbyColorMap[iNow]=arrbyColorMap[iNow];
				}
			}
			bGood=true;
		}
		catch (exception& exn) {
			ShowExn(exn,"Targa CopyTo");
			bGood=false;
		}
		catch (...) {
			ShowUnknownExn("Targa CopyTo");
			bGood=false;
		}
		return bGood;
	}//end Targa::CopyTo(Targa);
	bool Targa::DrawFast(byte* arrbyDest, int xAtDest, int yAtDest, int iDestWidth, int iDestHeight, int iDestBytesPP, int iDestStride) {
		bool bGood=true;
		try {
			/*
			byte* lpbySrc;
			byte* lpbyDest;
			byte* lpbySrcLine=arrbyData;
			byte* lpbyDestLine=//&arrbyDest[rectDestAdjusted.Y*iDestStride+rectDestAdjusted;
			uint* lpu32Src=(uint*)arrbyData;
			uint* lpu32Dest=(uint*)arrbyDest;
			ushort lpwSrc=(ushort*)arrbyData;
			ushort lpwDest=(ushort*)arrbyDest;
			*/
			//int xDest;
			int yDest=yAtDest;
			int iDestByteLineStart=xAtDest*iDestBytesPP;
			int iDestByteNow;
			int iSrcByteLineStart=0;
			//int iSrcByteNow;
			int iSrcStart=0;//NYI (not really used correctly) in compressed mode; only different if cropping self
			//int iLineEnder=iStride;//NYI in compressed mode; only different if cropping self
			int iStrideLimited=iStride;//NYI in compressed mode; only different if cropping self, only used if doing a direct byte buffer copy for the whole line
			if (iDestBytesPP==iBytesPP) {
				if (IsCompressed()) {
					bool bGood=true;
					int iDestAbs=iDestByteLineStart;
					int iDestRel=0;
					//int iTotal=0;
					try {
						if (bGood) {
							int iSrcAbs=iSrcStart;
							int iRun=0;
							bool bRunIsCompressed=false;
							int iSrcRel=0;
							int iChunkRelNow;
							int xSrcFake=0;
							int iDestLastXToFirstXOnNextLine=iDestStride-(iWidth*iDestBytesPP);
							int iSrcSize=iBytesAsUncompressed;
							int iDestSize=iDestStride*iDestHeight;
							while (iSrcAbs<iSrcSize) {
								if (arrbyData[iSrcAbs]>=128) { //if high bit is true
									bRunIsCompressed=true;
									iRun=((int)arrbyData[iSrcAbs]-128)+1;
								}
								else {
									bRunIsCompressed=false;
									iRun=(int)arrbyData[iSrcAbs]+1;
								}
								//first advance past packet header byte:
								iSrcRel++;
								iSrcAbs++;
								for (iChunkRelNow=0; iChunkRelNow<iRun && iSrcAbs<iSrcSize; iChunkRelNow++) {
									for (int iChunkSubByte=0; iChunkSubByte<iBytesPP && (iSrcAbs+iChunkSubByte)<iSrcSize; iChunkSubByte++) {
										if (iDestAbs>=iDestSize) {
											ShowError("Compressed data wanted destination bigger than "+RString_ToString(iDestSize)+" bytes.","DrawFast (same bit depth, uncompressing)");
											return 0;
										}
										arrbyDest[iDestAbs]=arrbyData[iSrcAbs+iChunkSubByte];
										iDestAbs++;
										iDestRel++;
									}
									if (!bRunIsCompressed) {
										//has to be incremented separately here in case pixel needed to be used multiple times in this outer loop
										//It is important that we increment past the whole chunk now (this makes sure alignment will be right) instead of iBytesCopiedNow
										iSrcRel+=iBytesPP;
										iSrcAbs+=iBytesPP;
									}
									xSrcFake++;
									if (xSrcFake==iWidth) {
										xSrcFake=0;
										iDestRel+=iDestLastXToFirstXOnNextLine;
										iDestAbs+=iDestLastXToFirstXOnNextLine;
									}
								}//end for iChunkRelNow of run
								if (bRunIsCompressed) {
									//has to be incremented separately here so that pixel could be used multiple times above
									//It is important that we increment past the whole chunk now (this makes sure alignment will be right) instead of iBytesCopiedNow
									iSrcRel+=iBytesPP;
									iSrcAbs+=iBytesPP;
								}
							}//end while still any data (iSrcAbs<iSrcSize)
						}
					}
					catch (exception& exn) {
						ShowExn(exn,"DrawFast (same bit depth, uncompressing)");
					}
					catch (...) {
						ShowUnknownExn("DrawFast (same bit depth, uncompressing)");
					}
				}//end if compressed
				else {//else uncompressed already
					for (int ySrc=0; ySrc<iHeight; ySrc++,yDest++,iSrcByteLineStart+=iStride,iDestByteLineStart+=iDestStride) {
						iDestByteNow=iDestByteLineStart;//xDest=xAtDest;
						memcpy(&arrbyDest[iDestByteNow],&arrbyData[iSrcByteLineStart],iStrideLimited);
						//for (iSrcByteNow=iSrcByteLineStart; iSrcByteNow<iLineEnder; iSrcByteNow++, iDestByteNow++) { //for (int xSrc=0; xSrc<iWidth; xSrc++,xDest++) {
						//	arrbyDest[iDestByteNow]=arrbyData[iSrcByteNow];
						//}
					}
				}
			}
			else {//else dissimilar bitdepths
				if (IsCompressed()) {
					ShowError("Not implemented: Can't copy from compressed targa of dissimilar bit depth","Targa::DrawFast");
				}
				else {
					for (int ySrc=0; ySrc<iHeight; ySrc++,yDest++,iSrcByteLineStart+=iStride,iDestByteLineStart+=iDestStride) {
						iDestByteNow=iDestByteLineStart;//xDest=xAtDest;
						memcpy(&arrbyDest[iDestByteNow],&arrbyData[iSrcByteLineStart],iStrideLimited);
						//for (iSrcByteNow=iSrcByteLineStart; iSrcByteNow<iLineEnder; iSrcByteNow++, iDestByteNow++) { //for (int xSrc=0; xSrc<iWidth; xSrc++,xDest++) {
						//	arrbyDest[iDestByteNow]=arrbyData[iSrcByteNow];
						//}
					}
				}
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Targa::DrawFast");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Targa::DrawFast");
		}
		return bGood;
	}
	void Targa::ToRect(ref_Rectangle rectReturn) {
		rectReturn.Width=iWidth;
		rectReturn.Height=iHeight;
	}
	void Targa::ToRect(ref_RectangleF rectReturn) {
		rectReturn.Width=(float)iWidth;
		rectReturn.Height=(float)iHeight;
	}
	bool Targa::From(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, bool bUsePointerNotCopyData) {
		return From(iWidthTo,iHeightTo,iBytesPP,arrbySrc,bUsePointerNotCopyData,0);
	}
	bool Targa::From(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, bool bUsePointerNotCopyData, uint u32SrcStart) {
		bool bGood=true;
		bGood=Init(iWidthTo,iHeightTo,iBytesPP,!bUsePointerNotCopyData);
		if (bGood) {
			if (bUsePointerNotCopyData) {
				arrbyData=&arrbySrc[u32SrcStart];
			}
			else {
				try {
					//memcpy(arrbyData,&arrbySrc[u32SrcStart],iWidthTo*iHeightTo*iBytesPP);
					//FLIP:
					int iDestLine=iHeight-1;
					byte* srcPlacePointer=arrbySrc;
					byte* destPlacePointer=&arrbyData[iStride*(iDestLine)];
					for (int iSrcLine=0; iSrcLine<iHeightTo; iSrcLine++) {
						memcpy(destPlacePointer,srcPlacePointer,iStride);
						iDestLine--;
						srcPlacePointer+=iStride;
						destPlacePointer-=iStride;
					}
				}
				catch (exception& exn) {
					bGood=false;
					ShowExn(exn,"Targa::From");
				}
				catch (...) {
					bGood=false;
					ShowUnknownExn("Targa::From");
				}
			}
		}
		else {
			ShowError("couldn't initialize.","Targa::From");
		}
		return bGood;
	}//end From a source buffer
	int Targa::SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, uint u32SrcRealLen) {
		return SafeCopyFrom(iWidthTo, iHeightTo, iBytesPP, arrbySrc, u32SrcRealLen, true);
	}
	int Targa::SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, uint u32SrcRealLen, bool bReInitializeAll) {
		return SafeCopyFrom(iWidthTo,iHeightTo,iBytesPP,arrbySrc,u32SrcRealLen,0,bReInitializeAll);
	}
	//SafeCopyFrom(iWidthTo, iHeightTo, iBytesPP, arrbySrc, u32SrcRealLen, u32SrcStart, bReInitializeAll)
	int Targa::SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* arrbySrc, uint u32SrcRealLen, uint u32SrcStart, bool bReInitializeAll) {
		int iFound=0;
		bool bGood=true;
		static bool bFirstRun=true;
		string sFuncNow="Targa::SafeCopyFrom";
		if (bFirstRun) Console::Error.Write(sFuncNow);
		string sArgs="(w="+RString_ToString(iWidthTo)+",h="+RString_ToString(iHeightTo)+",BytesPP="+RString_ToString(iBytesPP)+","+(arrbySrc==nullptr?"src=null":"src=ok")+",len="+RString_ToString(u32SrcRealLen)+",start="+RString_ToString(u32SrcStart)+","+RString_ToString(bReInitializeAll)+")";
		sFuncNow+=sArgs;
		if (bFirstRun) Console::Error.Write(sArgs);
		static bool bFirstFatalSafeCopyFrom=true;
		if (arrbyData==nullptr) {
			ShowError("Null image! Forcing reinitialize",sFuncNow);
			if (bFirstFatalSafeCopyFrom) {
				Console::Error.WriteLine();
				Console::Error.WriteLine();
				Console::Error.WriteLine("IMAGE BUFFER IS nullptr!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				Console::Error.WriteLine();
				bFirstFatalSafeCopyFrom=false;
			}
			bReInitializeAll=true;
		}
		if (bReInitializeAll) {
			if (!Init(iWidthTo,iHeightTo,iBytesPP,true)) {
				bGood=false;
				ShowError("couldn't reinit.",sFuncNow);
			}
		}
		if (bGood) {
			if (arrbySrc==nullptr) {
				bGood=false;
				ShowError("null source!",sFuncNow);
			}
		}
		if (bGood) {
			try {
				int iSrcAbs=(int)u32SrcStart;
				int iLen=iWidthTo*iHeightTo*iBytesPP;
				int iSrcRealLen=(int)u32SrcRealLen;
				int iNow=0;
				for (iNow=0; iNow<iLen && iSrcAbs<iSrcRealLen; iNow++, iSrcAbs++) {
					arrbyData[iNow]=arrbySrc[iSrcAbs];
					iFound++;
				}
				if (iNow<iLen) {//ok since iNow is now a length (since increments past end then exits loop)
					bGood=false;
					ShowError("Only"+RString_ToString(iNow)+" of "+RString_ToString(iLen)+" expected bytes of image data were found in file.",sFuncNow);
				}
			}
			catch (exception& exn) {
				bGood=false;
				ShowExn(exn,sFuncNow);
			}
			catch (...) {
				bGood=false;
				ShowUnknownExn(sFuncNow);
			}
			if (bFirstRun) Console::Error.Write(bGood?"Targa::SafeCopyFrom Success...":"Targa::SafeCopyFrom finished with errors...");
		}
		bFirstRun=false;
		return iFound;
	}//end SafeCopyFrom source buffer
	byte* Targa::GetBufferPointer() {
		return arrbyData;
	}
	bool Targa::IsLoaded() {
		return (arrbyData!=nullptr);
	}
	bool Targa::Save(string sFileNow) {
		sFile=sFileNow;
		return Save();
	}
	int Targa::SizeofFileEstimate() {
		int returnSize=0;
		returnSize+=1;//id string length byte
		returnSize+=1;//map type byte
		returnSize+=1;//targa type byte
		returnSize+=2;//map origin word
		returnSize+=2;//map length word
		returnSize+=1;//bit depth byte
		returnSize+=2;//image left side location word
		returnSize+=2;//image bottom location word
		returnSize+=2;//width word
		returnSize+=2;//height word
		returnSize+=1;//bit depth byte
		returnSize+=1;//descriptor bits byte
		int idLengthOrMax=sID.length();
		if (idLengthOrMax>255) idLengthOrMax=255;
		returnSize+=idLengthOrMax;
		returnSize+=iMapLength;
		returnSize+=iBytesBuffer;
		returnSize+=footer.ByteCount();
		return returnSize;
	}
	bool Targa::Save() {
		string sMsg="before initialization";
		bool bGood=true;
		try {
			Byter byterNow;
			byterNow.OpenWrite(sFile, SizeofFileEstimate());
			ushort wNow=0;
			byte byNow=0;
			int iPlacePrev=0;
			//uint u32Test=0;
			//(byte)(length of id)
			iPlacePrev=byterNow.Place();
			if (sID.length()>255) sID=sID.substr(0,255);
			uint u32IDLenNow=(sID.empty())?0:(uint)sID.length();
			byNow=(byte)u32IDLenNow;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) {
					Console::Error.Write("could not write id string length byte (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(byte)(int)MapType
			iPlacePrev=byterNow.Place();
			byNow=(byte)MapType;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) {
					Console::Error.Write("could not write map type byte (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(byte)(int)TypeTarga
			iPlacePrev=byterNow.Place();
			byNow=(byte)TypeTarga;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) {
					Console::Error.Write("could not write targa type byte (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(ushort)iMapOrigin
			iPlacePrev=byterNow.Place();
			wNow=(ushort)iMapOrigin;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2){
					Console::Error.Write("could not write map origin word (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(ushort)iMapLength
			iPlacePrev=byterNow.Place();
			wNow=(ushort)iMapLength;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) {
					Console::Error.Write("could not write map length word (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(byte)iMapBitDepth
			iPlacePrev=byterNow.Place();
			byNow=(byte)iMapBitDepth;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) {
					Console::Error.Write("could not write map bit depth byte (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(ushort)xImageLeft
			iPlacePrev=byterNow.Place();
			wNow=(ushort)xImageLeft;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) {
					Console::Error.Write("could not write image left side location word (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(ushort)yImageBottom
			iPlacePrev=byterNow.Place();
			wNow=(ushort)yImageBottom;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) {
					Console::Error.Write("could not write image bottom location word (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(ushort)iWidth
			iPlacePrev=byterNow.Place();
			wNow=(ushort)iWidth;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) {
					Console::Error.Write("could not write width word (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(ushort)iHeight
			iPlacePrev=byterNow.Place();
			wNow=(ushort)iHeight;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) {
					Console::Error.Write("could not write height word (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(byte)iBitDepth
			iPlacePrev=byterNow.Place();
			byNow=(byte)iBitDepth;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) {
					Console::Error.Write("could not write bit depth byte (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(byte)bitsDescriptor
			iPlacePrev=byterNow.Place();
			byNow=(byte)bitsDescriptor;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) {
					Console::Error.Write("could not write descriptor bits byte (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
			}
			//(byte[length of id])sID
			if (u32IDLenNow>0) {
				iPlacePrev=byterNow.Place();
				byterNow.WriteAscii(sID,u32IDLenNow);
				if (byterNow.Place()-iPlacePrev != u32IDLenNow) {
					Console::Error.Write("could not write id ascii string (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes)...");
					Console::Error.Flush();
					bGood=false;
				}
			}
			//(byte[iMapLength])(arrbyColorMap)
			if (iMapLength>0) {
				iPlacePrev=byterNow.Place();
				for (int iNow=0; iNow<iMapLength; iNow++) {
					byterNow.Write(arrbyColorMap[iNow]);
				}
				if (byterNow.Place()-iPlacePrev != iMapLength) {
					bGood=false;
					sMsg="Not all "+RString_ToString(iMapLength)+" color map bytes were found (wrote "+RString_ToString(byterNow.Place()-iPlacePrev)+" bytes).";
				}
			}
			else SafeFree(arrbyColorMap, "arrbyColorMap in Targa::Save");
			//(byte[iWidth*iHeight*iBytesPP])(arrbyData)
			if (iBytesBuffer>0) {
				if (arrbyData!=nullptr) {
					iPlacePrev=byterNow.Place();
					for (int iNow=0; iNow<iBytesBuffer; iNow++) {
						byterNow.Write(arrbyData[iNow]);
					}
					if (byterNow.Place()-iPlacePrev != iBytesAsUncompressed) {
						bGood=false;
						sMsg="Not all "+RString_ToString(iBytesBuffer)+" image data bytes were salvageable ("+RString_ToString(byterNow.Place()-iPlacePrev)+" saved).";
					}
				}
				else {
					bGood=false;
					sMsg="Targa::Save error: can't find any data to save.";
				}
			}
			else {
				bGood=false;
				sMsg="Targa::Save error: bad header wanted "+RString_ToString(iBytesBuffer)+" bytes of data";
			}
			iPlacePrev=byterNow.Place();
			footer.WriteTo(byterNow);
			if (byterNow.Place()-iPlacePrev != footer.ByteCount()) {
				sMsg="Not all "+RString_ToString(footer.ByteCount())+" footer data bytes were salvageable ("+RString_ToString(byterNow.Place()-iPlacePrev)+" saved).";
			}
			if (!byterNow.Save()) {
				bGood=false;
				sMsg="Targa::Save couldn't write file.";
			}
			if (!bGood) {
				ShowError(sMsg,"Targa::Save");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Targa::Save");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Targa::Save");
		}
		return bGood;
	}//end Save
	bool Targa::Load(string sFileNow) {
		static bool bFirstRun=true;
		if (bFirstRun) Console::Error.Write("Targa::Load...");
		sFile=sFileNow;
		string sMsg="before initialization";
		bool bGood=true;
		try {
			Byter byterNow;
			if (bFirstRun) Console::Error.Write("calling byter...");
			byterNow.OpenRead(sFile);
			ushort wNow=0;
			byte byNow=0;
			int iPlacePrev=0;
			uint u32Test=0;
			//(byte)(length of id)
			iPlacePrev=byterNow.Place();
			if (bFirstRun) Console::Error.Write("reading at["+RString_ToString(byterNow.Place())+"]{");
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			if (bFirstRun) Console::Error.Write(  "idLen:"  +  (  bGood ? (RString_ToString((int)byNow)+"; ") : "missing;" )  );
			uint u32IDLenNow=(uint)byNow;
			//(byte)(int)MapType
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			if (bFirstRun) Console::Error.Write(  "MapType:"  +  (  bGood ? (RString_ToString((int)byNow)+"; ") : "missing;" )  );
			MapType=(int)byNow;
			//(byte)(int)TypeTarga
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			if (bFirstRun) Console::Error.Write(  "TypeTarga:"  +  (  bGood ? (RString_ToString((int)byNow)+"; ") : "missing;" )  );
			TypeTarga=(int)byNow;
			//(ushort)iMapOrigin
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			if (bFirstRun) Console::Error.Write(  "iMapOrigin:"  +  (  bGood ? (RString_ToString((int)wNow)+"; ") : "missing;" )  );
			iMapOrigin=(int)wNow;
			//(ushort)iMapLength
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			if (bFirstRun) Console::Error.Write(  "iMapLength:"  +  (  bGood ? (RString_ToString((int)wNow)+"; ") : "missing;" )  );
			iMapLength=(int)wNow;
			//(byte)iMapBitDepth
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			if (bFirstRun) Console::Error.Write(  "iMapBitDepth:"  +  (  bGood ? (RString_ToString((int)byNow)+"; ") : "missing;" )  );
			iMapBitDepth=(int)byNow;
			//(ushort)xImageLeft
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			if (bFirstRun) Console::Error.Write(  "ImageLeft:"  +  (  bGood ? (RString_ToString((int)wNow)+"; ") : "missing;" )  );
			xImageLeft=(int)wNow;
			//(ushort)yImageBottom
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			if (bFirstRun) Console::Error.Write(  "ImageBottom:"  +  (  bGood ? (RString_ToString((int)wNow)+"; ") : "missing;" )  );
			yImageBottom=(int)wNow;
			//(ushort)iWidth
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			if (bFirstRun) Console::Error.Write(  "iWidth:"  +  (  bGood ? (RString_ToString((int)wNow)+"; ") : "missing;" )  );
			iWidth=(int)wNow;
			//(ushort)iHeight
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			if (bFirstRun) Console::Error.Write(  "iHeight:"  +  (  bGood ? (RString_ToString((int)wNow)+"; ") : "missing;" )  );
			iHeight=(int)wNow;
			//(byte)iBitDepth
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			if (bFirstRun) Console::Error.Write(  "iBitDepth:"  +  (  bGood ? (RString_ToString((int)byNow)+"; ") : "missing;" )  );
			iBitDepth=(int)byNow;
			iBytesPP=iBitDepth/8;
			//(byte)bitsDescriptor
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			if (bFirstRun) Console::Error.Write(  "AttributeBitsSum:"  +  (  bGood ? (RString_ToString((int)byNow)+"; ") : "missing;" )  );
			bitsDescriptor=byNow;
			//(byte[length of id])sID
			if (u32IDLenNow>0) {
				iPlacePrev=byterNow.Place();
				if (!byterNow.ReadAscii(sID,u32IDLenNow,u32Test)) bGood=false;
				if (bFirstRun) Console::Error.Write(  "id:"  +  (  bGood ? (sID+"; ") : "missing;" )  );
				if (u32Test!=u32IDLenNow) bGood=false;
			}
			else sID="";
			Init(iWidth,iHeight,iBytesPP,true); //DOES DeriveVars and create buffer
			//(byte[iMapLength])(arrbyColorMap)
			if (bFirstRun) Console::Error.Write("map:");
			if (iMapLength>0) {
				arrbyColorMap=(byte*)malloc(iMapLength);
				iPlacePrev=byterNow.Place();
				for (int iNow=0; iNow<iMapLength; iNow++) {
					byterNow.Read(arrbyColorMap[iNow]);
				}
				if (byterNow.Place()-iPlacePrev != iMapLength) {
					sMsg="Not all "+RString_ToString(iMapLength)+" color map bytes were found.";
					if (bFirstRun) Console::Error.Write("only "+RString_ToString(byterNow.Place()-iPlacePrev)+"!;");
					bGood=false;
				}
				else {
					if (bFirstRun) Console::Error.Write("good;");
				}
			}
			else {
				SafeFree(arrbyColorMap, "arrbyColorMap in Targa::Load");
				if (bFirstRun) Console::Error.Write("none;");
			}
			//(byte[iWidth*iHeight*iBytesPP])(arrbyData)
			iBytesBuffer=byterNow.BytesLeftUsed();
			//TODO: iBytesBuffer=//finish this! Count compressed area, limited by output size, and load rest into footer!
			if (iBytesAsUncompressed<iBytesBuffer) iBytesBuffer=iBytesAsUncompressed;
			if (bFirstRun) Console::Error.Write("buffer:"+RString_ToString(iBytesBuffer)+"; ");
			int iFound=0;

			if (iBytesBuffer<=0) {
				bGood=false;
				ShowError("Only "+RString_ToString(iBytesBuffer)+" were expected given this file's header","Targa::Load");
			}
			if (bGood) {
				//the following is ONLY OK SINCE called Init above
				if (bFirstRun) Console::Error.Write("( ");
				if (IsCompressed()) {
					if (bFirstRun) Console::Error.Write("compression found...");
					int iStart=byterNow.Place();
					//iBytesBuffer=RLESizeUncompressed(byterNow.arrbyData,iStart,iBytesLeft,iBytesPP);
					SafeFree(arrbyData, "arrbyData in Targa::Load");
					arrbyData=(byte*)malloc(iBytesBuffer);
					for (int iNow=0; iNow<iBytesBuffer; iNow++) {
						arrbyData[iNow]=byterNow.arrbyData[iStart+iNow];
						iFound++;
					}
					byterNow.Advance(iFound);
				}
				else {
					if (bFirstRun) Console::Error.Write("no compression...");
					int iTest=SafeCopyFrom(iWidth,iHeight,iBytesPP,byterNow.arrbyData,byterNow.Length(),byterNow.Place(),false);
					if (iTest!=iBytesBuffer) {
						bGood=false;
						ShowError("Expected "+RString_ToString(iBytesBuffer)+" but found "+RString_ToString(iTest)+"...");
					}
					byterNow.Advance(iTest);
				}
				if (bFirstRun) Console::Error.Write(" )...");
			}
			if (bGood&&IsSavedAsFlipped()) {
				if (IsCompressed()) {
					if (!SetCompressionRLE(false)) {
						bGood=false;
						ShowError("Failed to uncompress for flip!","Targa::Load");
					}
					if (!Flip()) {
						bGood=false;
						ShowError("Failed to uncompress for flip!","Targa::Load");
					}
					if (!SetCompressionRLE(true)) {
						bGood=false;
						ShowError("Failed to recompress after flip!","Targa::Load");
					}
				}
				else {
					if (!Flip()) {
						bGood=false;
						ShowError("Failed to flip targa!","Targa::Load");
					}
					else if (bFirstRun) {
						Console::Error.Write("Flip():success...");
					}
				}
			}
			if (bFirstRun) Console::Error.Write("footer:");
			if (byterNow.BytesLeftUsed()>0) {
				footer.Init(byterNow.arrbyData, (uint)byterNow.Place(), (uint)byterNow.BytesLeftUsed(), (uint)byterNow.Length());
				if (bFirstRun) Console::Error.Write(RString_ToString(footer.ByteCount())+"; ");
			}
			if (bFirstRun) Console::Error.Write("} ");
			if (bFirstRun) Console::Error.Write(bGood?"Targa::Load Success--dump":"Targa::Load Done with errors--dump");
			if (bFirstRun) Console::Error.Write(Dump());
			if (!bGood) {
				ShowError(sMsg,"Targa::Load");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Targa::Load");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Targa::Load");
		}
		bFirstRun=false;
		return bGood;
	}//end Targa::Load
	bool Targa::IsOK() {
		bool bGood=(iWidth>0 && iHeight>0 && iBytesPP>0 && arrbyData!=nullptr);
		if (!IsCompressed()) {
			if (iBytesBuffer!=(iWidth*iHeight*iBytesPP)) bGood=false;
			if (iBytesBuffer!=iBytesAsUncompressed) bGood=false;
		}
		return bGood;
	}
	bool Targa::Flip() {
		bool bGood=true;
		if (!IsOK()) {
			bGood=false;
			ShowError("Targa is corrupt or did not load properly (IsOK() returned false)!","Targa::Flip");
		}
		if (bGood&&IsCompressed()) {
			bGood=false;
			ShowError("Targa is still compressed and cannot be flipped before uncompressed!","Targa::Flip");
		}
		if (bGood) {
			try {
				byte* arrbyTemp=(byte*)malloc(iBytesBuffer);
				int xSrc=0, ySrc=iHeight-1, xDest, yDest;
				for (yDest=0; yDest<iHeight; yDest++, ySrc--) {
					xSrc=0;
					for (xDest=0; xDest<iWidth; xDest++, xSrc++) {
						for (int iChan=0; iChan<iBytesPP; iChan++) {
							arrbyTemp[yDest * iStride + xDest * iBytesPP + iChan]=
							arrbyData[ySrc  * iStride + xSrc  * iBytesPP + iChan];
						}
					}
				}
				SafeFree(arrbyData, "arrbyData in Targa::Flip");
				arrbyData=arrbyTemp;
			}
			catch (exception& exn) {
				bGood=false;
				ShowExn(exn,"Targa::Flip");
			}
			catch (...) {
				bGood=false;
				ShowUnknownExn("Targa::Flip");
			}
		}
		return bGood;
	}
	bool Targa::HasAttrib(byte bit) {
		return ( (bitsDescriptor&bit) != 0 );
	}
	bool Targa::IsSavedAsFlipped() {
		return !( (yImageBottom!=0)||(HasAttrib(bitNoFlip_NonTruevision)) );
	}
	bool Targa::IsCompressed() {
		return (TypeTarga==TypeCompressedColorMapped
				||TypeTarga==TypeCompressedTrueColor
				||TypeTarga==TypeCompressedGrayscale
				||TypeTarga==TypeCompressedColorMappedHuffmanAndDeltaAndRLE
				||TypeTarga==TypeCompressedColorMappedHuffmanAndDeltaAndRLE4PassQuadTree);
	}
	bool Targa::SetCompressionRLE(bool bOn) {
		bool bGood=true;
		try {
			if (iWidth*iHeight*iBytesPP<=0) {
				ShowErr("Tried to turn compression "+RString_ToString(bOn?"on":"off")+" for "+RString_ToString(iWidth)+RString_ToString("x")+RString_ToString(iHeight)+RString_ToString("x")+RString_ToString(iBytesPP)+RString_ToString(" image!"));
			}
			else if (iBytesBuffer<=0) {
				ShowErr("Tried to turn compression "+RString_ToString(bOn?"on":"off")+" for "+RString_ToString(iBytesBuffer)+"-buffer "+RString_ToString(BytesAsUncompressed())+"-length image!");
			}
			else {
				if (bOn) {
					if (!IsCompressed()) { //Compress
						byte* arrbyCompressed=RLECompress(iBytesBuffer, arrbyData, 0, iBytesBuffer, iBytesPP);
						SafeFree(arrbyData, "arrbyData in Targa::SetCompressionRLE");
						arrbyData=arrbyCompressed;
					}
				}
				else {
					if (IsCompressed()) { //Uncompress
						int iNewSize=iWidth*iHeight*iBytesPP;
						byte* arrbyUncompressed=(byte*)malloc(iNewSize); //byte* arrbyUncompress=RLEUncompress(iBytesBuffer, arrbyData, 0, iBytesBuffer, iBytesPP);
						int iTest=RLEUncompress(arrbyUncompressed, iNewSize, arrbyData, iBytesBuffer, iBytesPP);
						if (iTest!=iNewSize) {
							ShowError("Uncompressed "+RString_ToString(iTest)+" bytes but expected "+RString_ToString(iNewSize)+" ("+RString_ToString(iWidth)+"x"+RString_ToString(iHeight)+" at "+RString_ToString(iBytesPP*8)+"bpp ["+RString_ToString(iBytesPP)+" bytes per pixel])","Targa::SetCompressionRLE("+RString_ToString(bOn)+")");
						}
						SafeFree(arrbyData, "arrbyData in Targa::SetCompressionRLE");
						arrbyData=arrbyUncompressed;
						iBytesBuffer=iNewSize;
					}
				}
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Targa::SetCompressionRLE("+RString_ToString(bOn)+")");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Targa::SetCompressionRLE("+RString_ToString(bOn)+")");
		}
		return bGood;
	}//end SetCompressionRLE(bool bOn)
	string Targa::Dump() {
		return Dump(false);
	}
	string Targa::Dump(bool bDumpFull) {
		string sReturn="{";
		sReturn+="dimensions:"+RString_ToString(iWidth)
			+"x"+RString_ToString(iHeight)
			+"x"+RString_ToString(iBytesPP)+";";
		sReturn+=" buffer:"+RString_ToString(((arrbyData==nullptr)?"null":"ok"))+"; ";
		sReturn+=" ImageSize:"+RString_ToString(iBytesAsUncompressed)+"; ";
		sReturn+=" BufferSize:"+RString_ToString(iBytesBuffer)+"; ";
		if (bDumpFull) {
			//do some stuff here maybe later
		}
		sReturn+="}";
		return sReturn;
	}
	string Targa::Description() {
		return Description(false);
	}
	string Targa::Description(bool bVerbose) {
		if (bVerbose) return RString_ToString((arrbyData==nullptr)?"null":"")+RString_ToString(iWidth)+"x"+RString_ToString(iHeight)+"x"+RString_ToString(iBytesPP*8)+" "+RString_ToString(iBytesAsUncompressed)+"-length "+RString_ToString(iBytesBuffer)+"-buffer";
		else return RString_ToString((arrbyData==nullptr)?"null":"")+RString_ToString(iWidth)+"x"+RString_ToString(iHeight)+"x"+RString_ToString(iBytesPP*8);
	}
	//private methods:
	void Targa::DeriveVars() {
		iBytesPP=iBitDepth/8;
		iStride=iWidth*iBytesPP;
		iBytesAsUncompressed=iStride*iHeight;
	}
	void Targa::InitNull() {
		//bySizeofID=0 (size of sID)
		MapType=0;
		TypeTarga=0;
		iMapOrigin=0;
		iMapLength=0;
		iMapBitDepth=0;
		xImageLeft=0;
		yImageBottom=0;
		iWidth=0;
		iHeight=0;
		iBitDepth=0;
		bitsDescriptor=0;
		sID="";
		arrbyColorMap=nullptr;
		arrbyData=nullptr;
		DeriveVars();
		iBytesBuffer=iBytesAsUncompressed;
	}
	bool Targa::MarkAsCompressed(bool bAsCompressed) {
		bool bGood=false;
		if (bAsCompressed) {
			if (TypeTarga==TypeUncompressedColorMapped) {TypeTarga=TypeCompressedColorMapped;bGood=true;}
			else if (TypeTarga==TypeUncompressedTrueColor) {TypeTarga=TypeCompressedTrueColor;bGood=true;}
			else if (TypeTarga==TypeUncompressedGrayscale) {TypeTarga=TypeCompressedGrayscale;bGood=true;}
		}
		else {
			if (TypeTarga==TypeCompressedColorMapped) {TypeTarga=TypeUncompressedColorMapped;bGood=true;}
			else if (TypeTarga==TypeCompressedTrueColor) {TypeTarga=TypeUncompressedTrueColor;bGood=true;}
			else if (TypeTarga==TypeCompressedGrayscale) {TypeTarga=TypeUncompressedGrayscale;bGood=true;}
			else if (TypeTarga==TypeCompressedColorMappedHuffmanAndDeltaAndRLE) {TypeTarga=TypeUncompressedColorMapped;bGood=true;}
			else if (TypeTarga==TypeCompressedColorMappedHuffmanAndDeltaAndRLE4PassQuadTree) {TypeTarga=TypeUncompressedColorMapped;bGood=true;}
		}
		return bGood;
	}
}//end namespace

#endif
