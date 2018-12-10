#ifndef TARGA_CPP
#define TARGA_CPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <targa.h>
//#include "C:\My Documents\Projects-cpp\Base\targa.h"

using namespace std;

namespace ExpertMultimediaBase {
	//#region static targa functions
	int RLESizeUncompressed(byte* byarrSrc, int iStart, int iSrcSize, int iBytesPerChunk) {
		byte* byarrNull=null;
		return RLEUncompress(byarrNull, 0, byarrSrc, iSrcSize, iBytesPerChunk, 0, iStart, true);
	}
	bool Compare(byte* byarrSrc1, int iSrcLoc1, byte* byarrSrc2, int iSrcLoc2, int iRun) {
		bool bMatch=false;
		int iMatch=0;
		try {
			for (int iNow; iNow<iRun; iNow++) {
				if (byarrSrc1[iSrcLoc1+iNow]==byarrSrc2[iSrcLoc2+iNow]) iMatch++;
			}
			if (iMatch==iRun) bMatch=true;
		}
		catch (char* carrExn) {
			ShowAndDeleteException(carrExn,"Compare");
		}
		catch (...) {
			ShowUnknownException("Compare");
		}
	}
	
	byte* RLECompress(ref_int iReturnLength, byte* byarrSrc, int iSrcStart, int iBytesToParse, int iBytesPerChunk) {
		return RLECompress(iReturnLength, byarrSrc, iSrcStart, iBytesToParse, iBytesPerChunk, false);
	}
	byte* RLECompress(ref_int iReturnLength, byte* byarrSrc, int iSrcStart, int iBytesToParse, int iBytesPerChunk, bool bCountOnlyAndReturnNull) {
		bool bGood=true;
		iReturnLength=0;
		int iTotal=0;
		byte* byarrReturn=null;
		try {
			if (!bCountOnlyAndReturnNull) {
				byte* test=RLECompress(iTotal,byarrSrc,iSrcStart,iBytesToParse,iBytesPerChunk,true);
				if (iTotal>0) byarrReturn=(byte*)malloc(iTotal);
				else bGood=false;
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
					bRunIsCompressed=( (iSrcRel+iBytesPerChunk*2<=iBytesToParse) && Compare(byarrSrc,iSrcAbs,byarrSrc,iSrcAbs+iBytesPerChunk,iBytesPerChunk) )  ?  true  :  false;
					iRun=bRunIsCompressed?1:0;
					bPartOfThisTypeOfRun=true;
					iPacketByte=iReturnLength;
					iReturnLength++; //advance past RLE packet header byte (set it later)
					while (bPartOfThisTypeOfRun) {
						//Advance through chunk and copy it:
						for (int iNow=0; iNow<iBytesPerChunk; iNow++) {
							if (!bRunIsCompressed || iRun==1) {
								if (!bCountOnlyAndReturnNull)
									byarrReturn[iReturnLength]=byarrSrc[iSrcAbs];
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
								bPartOfThisTypeOfRun=( (iSrcRel+iBytesPerChunk*2<=iBytesToParse) && Compare(byarrSrc,iSrcAbs,byarrSrc,iSrcAbs+iBytesPerChunk,iBytesPerChunk) )  ?  true  :  false;
							else
								bPartOfThisTypeOfRun=( (iSrcRel+iBytesPerChunk*2<=iBytesToParse) && !Compare(byarrSrc,iSrcAbs,byarrSrc,iSrcAbs+iBytesPerChunk,iBytesPerChunk) )  ?  true  :  false;
						}//end else not too long
					}
					if (!bCountOnlyAndReturnNull) {
						if (bRunIsCompressed) byarrReturn[iPacketByte]=0x80 & (byte)(iRun-1);
						else byarrReturn[iPacketByte]=(byte)(iRun-1);
					}
				}//end while iSrcRel<iBytesToParse
			}
		}
		catch (char* carrExn) {
			ShowAndDeleteException(carrExn,"RLECompress");
		}
		catch (...) {
			ShowUnknownException("RLECompress");
		}
		return byarrReturn;
	}//end RLECompress

	//RLEUncompress(byarrDest, iDestSize, byarrSrc, iSrcSize, iBytesPerChunk);
	int RLEUncompress(byte* byarrDest, int iDestSize, byte* byarrSrc, int iSrcSize, int iBytesPerChunk) {
		if (iDestSize<0) iDestSize=0;
		return RLEUncompress(byarrDest, iDestSize, byarrSrc, iSrcSize, iBytesPerChunk, 0, 0, false);
	}
	//int iBytesFound=RLEUncompress(byarrDest, iDestSizeIrrelevantIfCountOnlyIsTrue, byarrSrc, iSrcSize, iBytesPerChunk, iSrcStart, bCountOnlyAndDontTouchDest)
	int RLEUncompress(byte* byarrDest, int iDestSizeIrrelevantIfCountOnlyIsTrue, byte* byarrSrc, int iSrcSize, int iBytesPerChunk, int iDestStart, int iSrcStart, bool bCountOnlyAndDontTouchDest) {
		bool bGood=true;
		int iDestAbs=iDestStart;
		int iDestRel=0;
		int iTotal=0;
		try {
			if (bGood) {
				int iSrcAbs=iSrcStart;
				int iRun=0;
				bool bRunIsCompressed=false;
				int iSrcRel=0;
				int iChunkRelNow;
				while (iSrcAbs<iSrcSize) {
					if (byarrSrc[iSrcAbs]>=128) { //if high bit is true
						bRunIsCompressed=true;
						iRun=((int)byarrSrc[iSrcAbs]-128)+1;
					}
					else {
						bRunIsCompressed=false;
						iRun=(int)byarrSrc[iSrcAbs]+1;
					}
					//first advance past packet header byte:
					iSrcRel++;
					iSrcAbs++;
					for (iChunkRelNow=0; iChunkRelNow<iRun && iSrcAbs<iSrcSize; iChunkRelNow++) {
						for (int iChunkSubByte=0; iChunkSubByte<iBytesPerChunk && (iSrcAbs+iChunkSubByte)<iSrcSize; iChunkSubByte++) {
                            if (!bCountOnlyAndDontTouchDest) {
								if (iDestAbs>=iDestSizeIrrelevantIfCountOnlyIsTrue) {
									ShowError("Compressed data wanted destination bigger than "+ToString(iDestSizeIrrelevantIfCountOnlyIsTrue)+" bytes.","RLEUncompress");
									return 0;
								}
								byarrDest[iDestAbs]=byarrSrc[iSrcAbs+iChunkSubByte];
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
		catch (char* carrExn) {
			ShowAndDeleteException(carrExn,"RLEUncompress");
		}
		catch (...) {
			ShowUnknownException("RLEUncompress");
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
	TargaFooter::TargaFooter(byte* byarrDataSrcToCopyFrom, uint dwStart, uint dwCount, uint dwActualSourceBufferSize) {
		Init(byarrDataSrcToCopyFrom, dwStart, dwCount, dwActualSourceBufferSize);
	}
	TargaFooter::TargaFooter(byte* lpbyDataPointerToKeep, uint dwSize) {
		Init(lpbyDataPointerToKeep,dwSize);
	}
	TargaFooter::TargaFooter() {
		dump=null;
		dwSizeofDump=0;
	}
	TargaFooter::~TargaFooter() {
		SafeFree(dump);
	}
	bool TargaFooter::Init(byte* byarrDataSrcToCopyFrom, uint dwSrcStart, uint dwCount, uint dwActualSourceBufferSize) {
		bool bGood=true;
		SafeFree(dump);
		try {
			dwSizeofDump=dwCount;
			dump=(byte*)malloc(dwCount);
			uint iSrc=dwSrcStart;
			for (uint iNow=0; iNow<dwCount && iSrc<dwActualSourceBufferSize; iNow++,iSrc++) {
				dump[iNow]=byarrDataSrcToCopyFrom[iSrc];
			}
		}
		catch (exception& exn) {
			ShowException(exn, "TargaFooter::Init");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("TargaFooter::Init");
		}
		return bGood;
	}
	bool TargaFooter::Init(byte* lpbyDataPointerToKeep, uint dwSize) {
		bool bGood=true;
		try {
			if (dwSize>0 && lpbyDataPointerToKeep!=null) {
				dwSizeofDump=dwSize;
				dump=lpbyDataPointerToKeep;
				//TODO: process the footer here
			}
			else {
				dwSizeofDump=0;
				dump=null;
			}
		}
		catch (char* sExn) {
			bGood=false;
			string sMsg="dwSize="+ToString(dwSize);
			sMsg+="; lpbyDataPointerToKeep is";
			sMsg+=((lpbyDataPointerToKeep==null)?"null.":"not null.");
			ShowAndDeleteException(sExn,"TargaFooter::Init",sMsg);
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("TargaFooter::Init");
		}
		return bGood;
	}
	bool TargaFooter::Init() {
		return Init(null,0);
	}
	bool TargaFooter::WriteTo(Byter &byterNow) {
		bool bGood=true;
		for (uint iNow=0; iNow<dwSizeofDump; iNow++) {
			if( !byterNow.Write(dump[iNow]) ) {
				bGood=false;
				break;
			}
		}
		return bGood;
	}//end WriteTo byter
	uint TargaFooter::ByteCount() {
		return dwSizeofDump;
	}
	//#endregion TargaFooter methods
	
	//#region Targa methods
	Targa::Targa() {
		InitNull();
	}
	Targa::~Targa() {
		SafeFree(byarrData);
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
			SafeFree(byarrColorMap);
			iMapLength=0;
			sID="";
			SafeFree(byarrData);
		}
		iWidth=iSetWidth;
		iHeight=iSetHeight;
		iBitDepth=iSetBytesPP*8;
		DeriveVars();//does get iBytesPP
		//if (iSetBytesPP==4 || iSetBytesPP==3) TypeTarga=TypeUncompressedTrueColor;
		//else if (iSetBytesPP==1) TypeTarga=TypeUncompressedGrayscale;
		iBytesBuffer=iBytesAsUncompressed;
		if (iBytesBuffer<=0) {
			ShowError("iBytesBuffer="+ToString(iBytesBuffer)+"!","Targa::Init");
			bGood=false;
		}
		if (bReallocateBuffers && bGood) {
			byarrData=(byte*)malloc(iBytesBuffer);
		}
		return bGood;
	}//end Targa::Init
	bool Targa::CopyTo(Targa &targaDest) {
		try {
			targaDest.Init(iWidth,iHeight,iBytesPP,false);
			targaDest.byarrData=(byte*)malloc(iBytesBuffer);
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
			for (iNow=0; iNow<iBytesBuffer; iNow++) {
				targaDest.byarrData[iNow]=byarrData[iNow];
			}
			targaDest.byarrColorMap=(byte*)malloc(iMapLength); //array of [] bytes  //[byMapBitDepth*wMapLength] -- the palette
		    for (iNow=0; iNow<iMapLength; iNow++) {
				targaDest.byarrColorMap[iNow]=byarrColorMap[iNow];
			}
		}
		catch (char* sExn) {
			ShowAndDeleteException(sExn,"Targa::CopyTo");
		}
		catch (...) {
	        ShowUnknownException("Targa::CopyTo");
		}
	}//end Targa::CopyTo(Targa);
	bool Targa::DrawFast(byte* byarrDest, int xAtDest, int yAtDest, int iDestWidth, int iDestHeight, int iDestBytesPP, int iDestStride) {
		bool bGood=true;
		try {
			/*
			byte* lpbySrc;
			byte* lpbyDest;
   			byte* lpbySrcLine=byarrData;
   			byte* lpbyDestLine=//&byarrDest[rectDestAdjusted.Y*iDestStride+rectDestAdjusted;
   			uint* lpdwSrc=(uint*)byarrData;
   			uint* lpdwDest=(uint*)byarrDest;
			ushort lpwSrc=(ushort*)byarrData;
			ushort lpwDest=(ushort*)byarrDest;
			*/
			int xDest;
			int yDest=yAtDest;
			int iDestByteLineStart=xAtDest*iDestBytesPP;
			int iDestByteNow;
			int iSrcByteLineStart=0;
			int iSrcByteNow;
			int iSrcStart=0;//NYI (not really used correctly) in compressed mode; only different if cropping self
			int iLineEnder=iStride;//NYI in compressed mode; only different if cropping self
			int iStrideLimited=iStride;//NYI in compressed mode; only different if cropping self, only used if doing a direct byte buffer copy for the whole line
			if (iDestBytesPP==iBytesPP) {
	   			if (IsCompressed()) {
					bool bGood=true;
					int iDestAbs=iDestByteLineStart;
					int iDestRel=0;
					int iTotal=0;
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
								if (byarrData[iSrcAbs]>=128) { //if high bit is true
									bRunIsCompressed=true;
									iRun=((int)byarrData[iSrcAbs]-128)+1;
								}
								else {
									bRunIsCompressed=false;
									iRun=(int)byarrData[iSrcAbs]+1;
								}
								//first advance past packet header byte:
								iSrcRel++;
								iSrcAbs++;
								for (iChunkRelNow=0; iChunkRelNow<iRun && iSrcAbs<iSrcSize; iChunkRelNow++) {
									for (int iChunkSubByte=0; iChunkSubByte<iBytesPP && (iSrcAbs+iChunkSubByte)<iSrcSize; iChunkSubByte++) {
										if (iDestAbs>=iDestSize) {
											ShowError("Compressed data wanted destination bigger than "+ToString(iDestSize)+" bytes.","DrawFast (same bit depth, uncompressing)");
											return 0;
										}
										byarrDest[iDestAbs]=byarrData[iSrcAbs+iChunkSubByte];
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
					catch (char* carrExn) {
						ShowAndDeleteException(carrExn,"DrawFast (same bit depth, uncompressing)");
					}
					catch (...) {
						ShowUnknownException("DrawFast (same bit depth, uncompressing)");
					}
				}//end if compressed
				else {//else uncompressed already
					for (int ySrc=0; ySrc<iHeight; ySrc++,yDest++,iSrcByteLineStart+=iStride,iDestByteLineStart+=iDestStride) {
                        iDestByteNow=iDestByteLineStart;//xDest=xAtDest;
						memcpy(&byarrDest[iDestByteNow],&byarrData[iSrcByteLineStart],iStrideLimited);
						//for (iSrcByteNow=iSrcByteLineStart; iSrcByteNow<iLineEnder; iSrcByteNow++, iDestByteNow++) { //for (int xSrc=0; xSrc<iWidth; xSrc++,xDest++) {
						//	byarrDest[iDestByteNow]=byarrData[iSrcByteNow];
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
						memcpy(&byarrDest[iDestByteNow],&byarrData[iSrcByteLineStart],iStrideLimited);
						//for (iSrcByteNow=iSrcByteLineStart; iSrcByteNow<iLineEnder; iSrcByteNow++, iDestByteNow++) { //for (int xSrc=0; xSrc<iWidth; xSrc++,xDest++) {
						//	byarrDest[iDestByteNow]=byarrData[iSrcByteNow];
						//}
					}
				}
			}
		}
		catch (char* sExn) {
			ShowAndDeleteException(sExn,"Targa::DrawFast");
		}
		catch (...) {
	        ShowUnknownException("Targa::DrawFast");
		}
	}
	void Targa::ToRect(ref_Rectangle rectReturn) {
		rectReturn.Width=iWidth;
		rectReturn.Height=iHeight;
	}
	void Targa::ToRect(ref_RectangleF rectReturn) {
		rectReturn.Width=(float)iWidth;
		rectReturn.Height=(float)iHeight;
	}
	bool Targa::From(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, bool bUsePointerNotCopyData) {
		return From(iWidthTo,iHeightTo,iBytesPP,byarrSrc,bUsePointerNotCopyData,0);
	}
	bool Targa::From(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, bool bUsePointerNotCopyData, uint dwSrcStart) {
		bool bGood=true;
		bGood=Init(iWidthTo,iHeightTo,iBytesPP,!bUsePointerNotCopyData);
		if (bGood) {
			if (bUsePointerNotCopyData) {
				byarrData=&byarrSrc[dwSrcStart];
			}
			else {
				try {
					memcpy(byarrData,&byarrSrc[dwSrcStart],iWidthTo*iHeightTo*iBytesPP);
				}
				catch (char* carrExn) {
					bGood=false;
					ShowAndDeleteException(carrExn, "Targa::From");
				}
				catch (...) {
					bGood=false;
					ShowUnknownException("Targa::From");
				}
			}
		}
		else {
			ShowError("couldn't initialize.","Targa::From");
		}
		return bGood;
	}//end From a source buffer
	int Targa::SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, uint dwSrcRealLen) {
        SafeCopyFrom(iWidthTo, iHeightTo, iBytesPP, byarrSrc, dwSrcRealLen, true);
	}
	int Targa::SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, uint dwSrcRealLen, bool bReInitializeAll) {
		return SafeCopyFrom(iWidthTo,iHeightTo,iBytesPP,byarrSrc,dwSrcRealLen,0,bReInitializeAll);
	}
	//SafeCopyFrom(iWidthTo, iHeightTo, iBytesPP, byarrSrc, dwSrcRealLen, dwSrcStart, bReInitializeAll)
	int Targa::SafeCopyFrom(int iWidthTo, int iHeightTo, int iBytesPP, byte* byarrSrc, uint dwSrcRealLen, uint dwSrcStart, bool bReInitializeAll) {
		int iFound=0;
		bool bGood=true;
		static bool bFirstRun=true;
		string sFuncNow="Targa::SafeCopyFrom";
		if (bFirstRun) Console.Write(sFuncNow);
		string sArgs="(w="+ToString(iWidthTo)+",h="+ToString(iHeightTo)+",BytesPP="+ToString(iBytesPP)+","+(byarrSrc==null?"src=null":"src=ok")+",len="+ToString(dwSrcRealLen)+",start="+ToString(dwSrcStart)+","+ToString(bReInitializeAll)+")";
		sFuncNow+=sArgs;
		if (bFirstRun) Console.Write(sArgs);
		static bool bFirstFatalSafeCopyFrom=true;
		if (byarrData==null) {
			ShowError("Null image! Forcing reinitialize",sFuncNow);
			if (bFirstFatalSafeCopyFrom) {
				cout<<endl<<endl<<"IMAGE BUFFER IS NULL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl<<endl;
				cerr<<endl<<endl<<"IMAGE BUFFER IS NULL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl<<endl;
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
			if (byarrSrc==null) {
				bGood=false;
				ShowError("null source!",sFuncNow);
			}
		}
		if (bGood) {
			try {
				int iSrcAbs=(int)dwSrcStart;
				int iLen=iWidthTo*iHeightTo*iBytesPP;
				int iSrcRealLen=(int)dwSrcRealLen;
				int iNow=0;
				for (iNow=0; iNow<iLen && iSrcAbs<iSrcRealLen; iNow++, iSrcAbs++) {
					byarrData[iNow]=byarrSrc[iSrcAbs];
					iFound++;
				}
				if (iNow<iLen) {//ok since iNow is now a length (since increments past end then exits loop)
					bGood=false;
					ShowError("Only"+ToString(iNow)+" of "+ToString(iLen)+" expected bytes of image data were found in file.",sFuncNow);
				}
			}
			catch (char* carrExn) {
				bGood=false;
				ShowAndDeleteException(carrExn, sFuncNow);
			}
			catch (...) {
				bGood=false;
				ShowUnknownException(sFuncNow);
			}
			if (bFirstRun) Console.Write(bGood?"Targa::SafeCopyFrom Success...":"Targa::SafeCopyFrom finished with errors...");
		}
		bFirstRun=false;
		return iFound;
	}//end SafeCopyFrom source buffer
	byte* Targa::GetBufferPointer() {
		return byarrData;
	}
	bool Targa::IsLoaded() {
		return (byarrData!=null);
	}
	bool Targa::Save(string sFileNow) {
		sFile=sFileNow;
		return Save();
	}
	bool Targa::Save() {
		string sMsg="before initialization";
		bool bGood=true;
		try {
			Byter byterNow;
			byterNow.OpenWrite(sFile);
			ushort wNow=0;
			byte byNow=0;
			int iPlacePrev=0;
			uint dwTest=0;
			//(byte)(length of id)
			iPlacePrev=byterNow.Place();
			if (/*sID!=null && */sID.length()>255) sID=sID.substr(0,255);
			uint dwIDLenNow=(/*sID==null || */sID=="")?0:(uint)sID.length();
			byNow=(byte)dwIDLenNow;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			//(byte)(int)MapType
			iPlacePrev=byterNow.Place();
			byNow=(byte)MapType;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			//(byte)(int)TypeTarga
			iPlacePrev=byterNow.Place();
			byNow=(byte)TypeTarga;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			//(ushort)iMapOrigin
			iPlacePrev=byterNow.Place();
			wNow=(ushort)iMapOrigin;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			//(ushort)iMapLength
			iPlacePrev=byterNow.Place();
			wNow=(ushort)iMapLength;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			//(byte)iMapBitDepth
			iPlacePrev=byterNow.Place();
			byNow=(byte)iMapBitDepth;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			//(ushort)xImageLeft
			iPlacePrev=byterNow.Place();
			wNow=(ushort)xImageLeft;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			//(ushort)yImageBottom
			iPlacePrev=byterNow.Place();
			wNow=(uint)yImageBottom;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			//(ushort)iWidth
			iPlacePrev=byterNow.Place();
			wNow=(ushort)iWidth;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			//(ushort)iHeight
			iPlacePrev=byterNow.Place();
			wNow=(ushort)iHeight;
			byterNow.Write(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
			//(byte)iBitDepth
			iPlacePrev=byterNow.Place();
			byNow=(byte)iBitDepth;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			//(byte)bitsDescriptor
			iPlacePrev=byterNow.Place();
			byNow=(byte)bitsDescriptor;
			byterNow.Write(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
			//(byte[length of id])sID
			if (dwIDLenNow>0) {
				iPlacePrev=byterNow.Place();
				byterNow.WriteAscii(sID,dwIDLenNow);
				if (byterNow.Place()-iPlacePrev != dwIDLenNow) bGood=false;
			}
			//(byte[iMapLength])(byarrColorMap)
			if (iMapLength>0) {
				iPlacePrev=byterNow.Place();
				for (int iNow=0; iNow<iMapLength; iNow++) {
					byterNow.Write(byarrColorMap[iNow]);
				}
				if (byterNow.Place()-iPlacePrev != iMapLength) {
					bGood=false;
					sMsg="Not all "+ToString(iMapLength)+" color map bytes were found.";
				}
			}
			else SafeFree(byarrColorMap);
			//(byte[iWidth*iHeight*iBytesPP])(byarrData)
			if (iBytesBuffer>0) {
				if (byarrData!=null) {
					for (int iNow=0; iNow<iBytesBuffer; iNow++) {
						byterNow.Write(byarrData[iNow]);
					}
					if (byterNow.Place()-iPlacePrev != iBytesAsUncompressed) {
						bGood=false;
						sMsg="Not all "+ToString(iBytesBuffer)+" image data bytes were saveable.";
					}
				}
				else {
					bGood=false;
					sMsg="Targa::Save error: can't find any data to save.";
				}
			}
			else {
				bGood=false;
				sMsg="Targa::Save error: bad header wanted "+ToString(iBytesBuffer)+" bytes of data";
			}
			footer.WriteTo(byterNow);
			if (!byterNow.Save()) {
				bGood=false;
				sMsg="Targa::Save couldn't write file.";
			}
			if (!bGood) {
				ShowError(sMsg,"Targa::Save");
			}
		}
		catch (char* carrExn) {
			bGood=false;
			ShowAndDeleteException(carrExn, "Targa::Save");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Targa::Save");
		}
		return bGood;
	}//end Save
	bool Targa::Load(string sFileNow) {
       	static bool bFirstRun=true;
       	if (bFirstRun) Console.Write("Targa::Load...");
		sFile=sFileNow;
		string sMsg="before initialization";
		bool bGood=true;
		try {
			Byter byterNow;
       		if (bFirstRun) Console.Write("calling byter...");
			byterNow.OpenRead(sFile);
			ushort wNow=0;
			byte byNow=0;
			int iPlacePrev=0;
			uint dwTest=0;
			//(byte)(length of id)
			iPlacePrev=byterNow.Place();
       		if (bFirstRun) Console.Write("reading at["+ToString(byterNow.Place())+"]{");
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
       		if (bFirstRun) Console.Write(  "idLen:"  +  (  bGood ? (ToString((int)byNow)+"; ") : "missing;" )  );
			uint dwIDLenNow=(uint)byNow;
			//(byte)(int)MapType
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
       		if (bFirstRun) Console.Write(  "MapType:"  +  (  bGood ? (ToString((int)byNow)+"; ") : "missing;" )  );
			MapType=(int)byNow;
			//(byte)(int)TypeTarga
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
       		if (bFirstRun) Console.Write(  "TypeTarga:"  +  (  bGood ? (ToString((int)byNow)+"; ") : "missing;" )  );
			TypeTarga=(int)byNow;
			//(ushort)iMapOrigin
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
       		if (bFirstRun) Console.Write(  "iMapOrigin:"  +  (  bGood ? (ToString((int)wNow)+"; ") : "missing;" )  );
			iMapOrigin=(int)wNow;
			//(ushort)iMapLength
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
       		if (bFirstRun) Console.Write(  "iMapLength:"  +  (  bGood ? (ToString((int)wNow)+"; ") : "missing;" )  );
			iMapLength=(int)wNow;
			//(byte)iMapBitDepth
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
       		if (bFirstRun) Console.Write(  "iMapBitDepth:"  +  (  bGood ? (ToString((int)byNow)+"; ") : "missing;" )  );
			iMapBitDepth=(int)byNow;
			//(ushort)xImageLeft
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
       		if (bFirstRun) Console.Write(  "ImageLeft:"  +  (  bGood ? (ToString((int)wNow)+"; ") : "missing;" )  );
			xImageLeft=(int)wNow;
			//(ushort)yImageBottom
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
       		if (bFirstRun) Console.Write(  "ImageBottom:"  +  (  bGood ? (ToString((int)wNow)+"; ") : "missing;" )  );
			yImageBottom=(int)wNow;
			//(ushort)iWidth
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
       		if (bFirstRun) Console.Write(  "iWidth:"  +  (  bGood ? (ToString((int)wNow)+"; ") : "missing;" )  );
			iWidth=(int)wNow;
			//(ushort)iHeight
			iPlacePrev=byterNow.Place();
			byterNow.Read(wNow);
			if (byterNow.Place()-iPlacePrev != 2) bGood=false;
       		if (bFirstRun) Console.Write(  "iHeight:"  +  (  bGood ? (ToString((int)wNow)+"; ") : "missing;" )  );
			iHeight=(int)wNow;
			//(byte)iBitDepth
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
       		if (bFirstRun) Console.Write(  "iBitDepth:"  +  (  bGood ? (ToString((int)byNow)+"; ") : "missing;" )  );
			iBitDepth=(int)byNow;
			iBytesPP=iBitDepth/8;
			//(byte)bitsDescriptor
			iPlacePrev=byterNow.Place();
			byterNow.Read(byNow);
			if (byterNow.Place()-iPlacePrev != 1) bGood=false;
       		if (bFirstRun) Console.Write(  "AttributeBitsSum:"  +  (  bGood ? (ToString((int)byNow)+"; ") : "missing;" )  );
			bitsDescriptor=byNow;
			//(byte[length of id])sID
			if (dwIDLenNow>0) {
				iPlacePrev=byterNow.Place();
				if (!byterNow.ReadAscii(sID,dwIDLenNow,dwTest)) bGood=false;
       			if (bFirstRun) Console.Write(  "id:"  +  (  bGood ? (sID+"; ") : "missing;" )  );
				if (dwTest!=dwIDLenNow) bGood=false;
			}
			else sID="";
			Init(iWidth,iHeight,iBytesPP,true); //DOES DeriveVars and create buffer
			//(byte[iMapLength])(byarrColorMap)
       		if (bFirstRun) Console.Write("map:");
			if (iMapLength>0) {
				byarrColorMap=(byte*)malloc(iMapLength);
				iPlacePrev=byterNow.Place();
				for (int iNow=0; iNow<iMapLength; iNow++) {
					byterNow.Read(byarrColorMap[iNow]);
				}
				if (byterNow.Place()-iPlacePrev != iMapLength) {
					sMsg="Not all "+ToString(iMapLength)+" color map bytes were found.";
					if (bFirstRun) Console.Write("only "+ToString(byterNow.Place()-iPlacePrev)+"!;");
					bGood=false;
				}
				else {
					if (bFirstRun) Console.Write("good;");
				}
			}
			else {
				SafeFree(byarrColorMap);
				if (bFirstRun) Console.Write("none;");
			}
			//(byte[iWidth*iHeight*iBytesPP])(byarrData)
			iBytesBuffer=byterNow.BytesLeftUsed();
			//TODO: iBytesBuffer=//finish this! Count compressed area, limited by output size, and load rest into footer!
			if (iBytesAsUncompressed<iBytesBuffer) iBytesBuffer=iBytesAsUncompressed;
       		if (bFirstRun) Console.Write("buffer:"+ToString(iBytesBuffer)+"; ");
       		int iFound=0;
       		
			if (iBytesBuffer<=0) {
				bGood=false;
				ShowError("Only "+ToString(iBytesBuffer)+" were expected given this file's header","Targa::Load");
			}
			if (bGood) {
				//the following is ONLY OK SINCE called Init above
				if (bFirstRun) Console.Write("( ");
				if (IsCompressed()) {
					if (bFirstRun) Console.Write("compression found...");
					int iStart=byterNow.Place();
					//iBytesBuffer=RLESizeUncompressed(byterNow.byarrData,iStart,iBytesLeft,iBytesPP);
					SafeFree(byarrData);
					byarrData=(byte*)malloc(iBytesBuffer);
					for (int iNow=0; iNow<iBytesBuffer; iNow++) {
						byarrData[iNow]=byterNow.byarrData[iStart+iNow];
						iFound++;
					}
					byterNow.Advance(iFound);
				}
				else {
					if (bFirstRun) Console.Write("no compression...");
					int iTest=SafeCopyFrom(iWidth,iHeight,iBytesPP,byterNow.byarrData,byterNow.Length(),byterNow.Place(),false);
					if (iTest!=iBytesBuffer) {
						bGood=false;
						ShowError("Expected "+ToString(iBytesBuffer)+" but found "+ToString(iTest)+"...");
					}
					byterNow.Advance(iTest);
				}
				if (bFirstRun) Console.Write(" )...");
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
						Console.Write("Flip():success...");
					}
				}
			}
       		if (bFirstRun) Console.Write("footer:");
			if (byterNow.BytesLeftUsed()>0) {
       			footer.Init(byterNow.byarrData, (uint)byterNow.Place(), (uint)byterNow.BytesLeftUsed(), (uint)byterNow.Length());
       			if (bFirstRun) Console.Write(ToString(footer.ByteCount())+"; ");
			}
       		if (bFirstRun) Console.Write("} ");
       		if (bFirstRun) Console.Write(bGood?"Targa::Load Success--dump":"Targa::Load Done with errors--dump");
       		if (bFirstRun) Console.Write(Dump());
			if (!bGood) {
				ShowError(sMsg,"Targa::Load");
			}
		}
		catch (char* carrExn) {
			bGood=false;
			ShowAndDeleteException(carrExn, "Targa::Load");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Targa::Load");
		}
		bFirstRun=false;
		return bGood;
	}//end Targa::Load
	bool Targa::IsOK() {
		bool bGood=(iWidth>0 && iHeight>0 && iBytesPP>0 && byarrData!=null);
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
				byte* byarrTemp=(byte*)malloc(iBytesBuffer);
				int xSrc=0, ySrc=iHeight-1, xDest, yDest;
				for (yDest=0; yDest<iHeight; yDest++, ySrc--) {
					xSrc=0;
					for (xDest=0; xDest<iWidth; xDest++, xSrc++) {
						for (int iChan=0; iChan<iBytesPP; iChan++) {
							byarrTemp[yDest * iStride + xDest * iBytesPP + iChan]=
							byarrData[ySrc  * iStride + xSrc  * iBytesPP + iChan];
						}
					}
				}
				SafeFree(byarrData);
				byarrData=byarrTemp;
			}
			catch (exception& exn) {
				bGood=false;
				ShowException(exn,"Targa::Flip");
			}
			catch (...) {
				bGood=false;
				ShowUnknownException("Targa::Flip");
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
			if (bOn) {
				if (!IsCompressed()) { //Compress
					byte* byarrCompressed=RLECompress(iBytesBuffer, byarrData, 0, iBytesBuffer, iBytesPP);
					SafeFree(byarrData);
					byarrData=byarrCompressed;
				}
			}
			else {
				if (IsCompressed()) { //Uncompress
					int iNewSize=iWidth*iHeight*iBytesPP;
					byte* byarrUncompressed=(byte*)malloc(iNewSize); //byte* byarrUncompress=RLEUncompress(iBytesBuffer, byarrData, 0, iBytesBuffer, iBytesPP);
					int iTest=RLEUncompress(byarrUncompressed, iNewSize, byarrData, iBytesBuffer, iBytesPP);
					if (iTest!=iNewSize) {
						ShowError("Uncompressed "+ToString(iTest)+" bytes but expected "+ToString(iNewSize)+" ("+ToString(iWidth)+"x"+ToString(iHeight)+" at "+ToString(iBytesPP*8)+"bpp ["+ToString(iBytesPP)+" bytes per pixel])","Targa::SetCompressionRLE("+ToString(bOn)+")");
					}
					SafeFree(byarrData);
					byarrData=byarrUncompressed;
					iBytesBuffer=iNewSize;
				}
			}
		}
		catch (char* carrExn) {
			bGood=false;
			ShowAndDeleteException(carrExn, "Targa::SetCompressionRLE("+ToString(bOn)+")");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Targa::SetCompressionRLE("+ToString(bOn)+")");
		}
	}
	string Targa::Dump() {
		return Dump(false);
	}
	string Targa::Dump(bool bDumpFull) {
		string sReturn="{";
		sReturn+="dimensions:"+ToString(iWidth)
			+"x"+ToString(iHeight)
			+"x"+ToString(iBytesPP)+";";
		sReturn+=" buffer:"+ToString(((byarrData==null)?"null":"ok"))+"; ";
		sReturn+=" ImageSize:"+ToString(iBytesAsUncompressed)+"; ";
		sReturn+=" BufferSize:"+ToString(iBytesBuffer)+"; ";
		if (bDumpFull) {
			//do some stuff here maybe later
		}
		sReturn+="}";
		return sReturn;
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
		byarrColorMap=null;
		byarrData=null;
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
