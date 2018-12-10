#ifndef ANIM32BGRA_CPP
#define ANIM32BGRA_CPP
#include <anim32bgra.h>
//#include "C:\My Documents\Projects-cpp\Base\anim32bgra.h"

using namespace std;
//using System;
//using System.Drawing;
//using System.Drawing.Imaging;
//using System.IO;
//using System.Drawing.Text;

namespace ExpertMultimediaBase {

	void SafeFree(Anim32BGRA*& animarrX) {
		try {
			if (animarrX!=null) delete [] animarrX;
			animarrX=null;
		}
		catch (...) { }
	}
	//PathFileOfSeqFrame(sSetFileBaseName, sSetFileExt, lFrameTarget, iDigitsMin)
	string PathFileOfSeqFrame(string sSetFileBaseName, string sSetFileExt, long lFrameTarget, int iDigitsMin) {
		static bool bFirstRun=true;
		if (bFirstRun) Console.Write("PathFileOfSeqFrame...");
		string sReturn="";
		try {
			sReturn=sSetFileBaseName;
			if (iDigitsMin>0) {
				long lDivisor=SafeE10L((int)(iDigitsMin-1));//returns long since implied base is 10L
				long lDestruct=lFrameTarget;
				while (lDivisor>0) {
					long lResult=lDestruct/lDivisor;
					sReturn+=ToString(lResult);
					lDestruct-=lResult*lDivisor;
					if (lDivisor==1) lDivisor=0;
					else lDivisor/=10;
				}
			}
			else sReturn+=ToString(lFrameTarget);
			sReturn+="."+sSetFileExt;
			if (bFirstRun) Console.Write("done...");//Console.Write("returning \""+sReturn+"\"...");
		}
		catch (exception& exn) {
			ShowException(exn, "Anim32BGRA.PathFileOfSeqFrame");
		}
		catch (...) {
            ShowUnknownException("Anim32BGRA.PathFileOfSeqFrame");
		}
		bFirstRun=false;
		return sReturn;
	}


	Clip::Clip() {
		effectarr=null;
	}
	
	Effect* Effect::Copy() {
		Effect* fxReturn=null;
		try {
			fxReturn=new Effect();
			//fxReturn.varsFX=varsFX.Copy(); //TODO: re-implement this
			fxReturn->bitsAttrib=bitsAttrib;
			fxReturn->iEffect=iEffect;
			fxReturn->iDest=iDest;
			fxReturn->iDestResourceType=iDestResourceType;
			fxReturn->iOverlay=iOverlay;
			fxReturn->iOverlayResourceType=iOverlayResourceType;
			fxReturn->iMask=iMask;
			fxReturn->iMaskResourceType=iMaskResourceType;
			fxReturn->lStartFrame=lStartFrame;
			fxReturn->lFrames=lFrames;
			fxReturn->sScript=sScript;
		}
		catch (exception& exn) {
			ShowException(exn, "Effect::Copy");
		}
		catch (...) {
            ShowUnknownException("Effect::Copy");
		}
		return fxReturn;
	}
	bool Effect::CopyTo(Effect& fxReturn) {
		fxReturn.bitsAttrib=bitsAttrib;
		fxReturn.iEffect=iEffect;
		fxReturn.iDest=iDest;
		fxReturn.iDestResourceType=iDestResourceType;
		fxReturn.iOverlay=iOverlay;
		fxReturn.iOverlayResourceType=iOverlayResourceType;
		fxReturn.iMask=iMask;
		fxReturn.iMaskResourceType=iMaskResourceType;
		fxReturn.lStartFrame=lStartFrame;
		fxReturn.lFrames=lFrames;
		fxReturn.sScript=sScript;
		return true;
	}
	Effect::Effect() {
		Init();
	}
	void Effect::Init() {
		bitsAttrib=0;
		iEffect=EffectNone;
		//TODO: Remember to prevent circular references in the following ints!!
		iDest=0;
		iDestResourceType=ResourceTypeNone;
		iOverlay=0;
		iOverlayResourceType=ResourceTypeNone;
		iMask=0;
		iMaskResourceType=ResourceTypeNone;
		lStartFrame=0;
		lFrames=0;
		sScript="";
	}



    //#region class Anim methods
	Anim32BGRA::Anim32BGRA() {
		bLoop=false;
		sFileBaseName="";
		sFileExt="";
		iSeqDigitCountMin=4; //i.e. frame0000.tga
		gbarrAnim=null;
		effectarr=null;
		iMaxEffects=200;//debug hard-coded limitation
		//targaLoaded.InitNull(); //compiler already called constructor
		gbFrame.InitNull();
	}
	Anim32BGRA::~Anim32BGRA() {
		SafeDeleteAnim();
		SafeDeleteEffects();
	}
	string Anim32BGRA::sPathFileNonSeq() {
        return sFileBaseName+"."+sFileExt;
	}
	Anim32BGRA* Anim32BGRA::Copy() {
		Anim32BGRA* animReturn=null;
		try {
			animReturn=new Anim32BGRA();
			targaLoaded.CopyTo(animReturn->targaLoaded);
			animReturn->rectNowF.Set(rectNowF.X,rectNowF.Y,rectNowF.Width,rectNowF.Height);
			animReturn->rectNow.Set(rectNow.X,rectNow.Y,rectNow.Width,rectNow.Height);
			animReturn->lFramesCached=lFramesCached;
			animReturn->sFileBaseName=sFileBaseName;
			animReturn->sFileExt=sFileExt;
			//animReturn->sPathFile=sPathFile;
			animReturn->bFileSequence=bFileSequence;//if use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
			animReturn->iSeqDigitCountMin=iSeqDigitCountMin;//, 0 if variable (i.exn. frame1.png...frame10.png)
			animReturn->lFrame=lFrame;
			animReturn->lFrames=lFrames;
			animReturn->iEffects=iEffects;
			animReturn->iMaxEffects=iMaxEffects;
			animReturn->lFramesCached=lFramesCached;
			if (lFramesCached==lFrames) {
				animReturn->gbarrAnim=new GBuffer32BGRA[lFramesCached];
				for (long l=0; l<lFrames; l++) {
					gbarrAnim[l].CopyTo(animReturn->gbarrAnim[l]);
				}
			}
			else ShowError("Uncached anim not yet implemented","Anim32BGRA::Copy");
			animReturn->gbarrAnim[lFrame].CopyToByDataRef(animReturn->gbFrame);
			if (iEffects>0) {
				animReturn->effectarr=new Effect[iMaxEffects];
				for (int i=0; i<iEffects; i++) {
					effectarr[i].CopyTo(animReturn->effectarr[i]);
				}
			}
		}
		catch (exception& exn) {
			ShowException(exn, "Anim32BGRA::Copy");
		}
		catch (...) {
            ShowUnknownException("Anim32BGRA::Copy");
		}
		return animReturn;
	}//end Copy
	Anim32BGRA* Anim32BGRA::CopyAsGray() {
		return CopyAsGray(-1);
	}
	/// <summary>
	/// Makes a new Anim32BGRA object from any channel (or the value) of this one.
	/// </summary>
	/// <param name="iChannelOffset">Pick a channel to copy.  Set to -1 to take average of RGB.</param>
	/// <returns></returns>
	Anim32BGRA* Anim32BGRA::CopyAsGray(int iChannelOffset) {
		Anim32BGRA* animReturn=null;
		try {
			animReturn=new Anim32BGRA();
			targaLoaded.CopyTo(animReturn->targaLoaded);
			animReturn->rectNowF.Set(rectNowF.X,rectNowF.Y,rectNowF.Width,rectNowF.Height);
			animReturn->rectNow.Set(rectNow.X,rectNow.Y,rectNow.Width,rectNow.Height);
			animReturn->lFramesCached=lFramesCached;
			animReturn->sFileBaseName=sFileBaseName;
			animReturn->sFileExt=sFileExt;
			//animReturn->sPathFile=sPathFile;
			animReturn->bFileSequence=bFileSequence;//if use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
			animReturn->iSeqDigitCountMin=iSeqDigitCountMin;//, 0 if variable (i.exn. frame1.png...frame10.png)
			animReturn->lFrame=lFrame;
			animReturn->lFrames=lFrames;
			animReturn->iEffects=iEffects;
			animReturn->iMaxEffects=iMaxEffects;
			animReturn->lFramesCached=lFramesCached;
			if (lFramesCached==lFrames) {
				animReturn->gbarrAnim=new GBuffer32BGRA[lFramesCached];
				if (iChannelOffset<0) {
					for (long index=0; index<lFrames; index++) {
						MaskFromValue(animReturn->gbarrAnim[index], gbarrAnim[index]);
					}
				}
				else {
					for (long index=0; index<lFrames; index++) {
						MaskFromChannel(animReturn->gbarrAnim[index], gbarrAnim[index], iChannelOffset);
					}
				}
			}
			else ShowError("Uncached Anim32BGRA not yet implemented","Anim32BGRA::CopyAsGray");
			animReturn->gbarrAnim[lFrame].CopyToByDataRef(animReturn->gbFrame);
			if (iEffects>0) {
				animReturn->effectarr=new Effect[iMaxEffects];
				for (int index=0; index<iEffects; index++) {
					effectarr[index].CopyTo(animReturn->effectarr[index]);
				}
			}
		}
		catch (exception& exn) {
			ShowException(exn, "Anim32BGRA::CopyAsGray");
		}
		catch (...) {
            ShowUnknownException("Anim32BGRA::CopyAsGray");
		}
		return animReturn;
	}//end CopyAsGray
	bool Anim32BGRA::SaveSeq(string sSetFileBaseName, string sSetFileExt) {
		bool bGood=true;
		sFileExt=sSetFileExt;
		sFileBaseName=sSetFileBaseName; //StringToFile(sSetFileBaseName+".txt", Dump(true));
		for (long lFrameSave=0; lFrameSave<lFrames; lFrameSave++) {
			if (!SaveSeqFrame(lFrameSave)) {
				bGood=false;
				ShowError("Couldn't save frame "+ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName, sSetFileExt, lFrame, iSeqDigitCountMin), "SaveSeq");
			}
		}
		return bGood;
	}//end SaveSeq
	string Anim32BGRA::Dump(bool bDumpVars) {
		static bool bFirstRun=true;
		if (bFirstRun) Console.Write("Anim32BGRA::Dump("+ToString(bDumpVars)+")...");
		if (bDumpVars) {
			string sReturn="";
			try {
				sReturn+="{lFrames:"+ToString(lFrames)
					+"; lFramesCached:"+ToString(lFramesCached)
					+"; lFrame:"+ToString(lFrame)
					+"; sFileExt:"+sFileExt
					//+"; sPathFile:"+sPathFile
					+"; sFileBaseName:"+sFileBaseName
					+"; bFileSequence:"+ToString(bFileSequence)
					+"; iEffects:"+ToString(iEffects)
					+"; iMaxEffects:"+ToString(iMaxEffects)
					+"; iSeqDigitCountMin:"+ToString(iSeqDigitCountMin);
				if (gbFrame.byarrData!=null) {
					if (gbFrame.iBytesTotal>0) {
						try {
						sReturn+="; gbFrame.iBytesTotal:"+ToString(gbFrame.iBytesTotal)
							+"; gbFrame.iWidth:"+ToString(gbFrame.iWidth)
							+"; gbFrame.iHeight:"+ToString(gbFrame.iHeight)
							+"; gbFrame.iWidth:"+ToString(gbFrame.iWidth)
							+"; gbFrame.iBytesPP:"+ToString(gbFrame.iBytesPP)
							+"; gbFrame.iStride:"+ToString(gbFrame.iStride);
						}
						catch (exception& exn) {
							ShowException(exn,"Anim32BGRA::Dump("+ToString(bDumpVars)+") while accessing gbFrame");
						}
						catch (...) {
				            ShowUnknownException("Anim32BGRA::Dump("+ToString(bDumpVars)+") while accessing gbFrame");
						}
					}
				}
				else sReturn="gbFrame.byarrData:null";
				sReturn+=";}";
			}
			catch (exception& exn) {
				ShowException(exn, "Anim32BGRA::Dump("+ToString(bDumpVars)+")");
			}
			catch (...) {
	            ShowUnknownException("Anim32BGRA::Dump("+ToString(bDumpVars)+")");
			}
			bFirstRun=false;
			return sReturn;
		}
		else {
            bFirstRun=false;
			return Dump();
		}
	}//end Dump(bDumpVars);
	string Anim32BGRA::Dump() {
		string sReturn="Anim32BGRA{";
		sReturn+="lFrames:";
		sReturn+=ToString(lFrames)+";";
		sReturn+="}";
	}
	bool Anim32BGRA::SaveCurrentSeqFrame() {
		bool bGood=CopyFrameToInternalBitmap();
		if (bGood) bGood=SaveInternalBitmap(ExpertMultimediaBase::PathFileOfSeqFrame(sFileBaseName, sFileExt, lFrame, iSeqDigitCountMin));
		return bGood;
	}
	bool Anim32BGRA::SaveSeqFrame(long lFrameSave) {
		return SaveSeqFrame(sFileBaseName, sFileExt, lFrameSave);
	}
	bool Anim32BGRA::SaveSeqFrame(string sSetFileBaseName, string sSetFileExt, long lFrameSave) {
		bool bGood=true;
		if (!GotoFrame(lFrameSave)) {
			bGood=false;
			ShowError("Failed to goto frame "+ToString(lFrameSave)+" of Anim32BGRA","SaveSeqFrame");
			//TODO: use lStartFrame and calculate frame offset, and make it more obvious by adding a param
		}
		else {
			bGood=CopyFrameToInternalBitmap();
			if (bGood) {
				sFileExt=sSetFileExt;
				bGood=SaveInternalBitmap(ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName, sSetFileExt, lFrameSave, iSeqDigitCountMin));
				if (bGood==false) {
					ShowError("Failed to save "+ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName, sSetFileExt, lFrameSave, iSeqDigitCountMin),"SaveCurrentSeqFrame");
				}
			}
			else ShowError("Failed to copy data to frame image","SaveSeqFrame("+ToString(lFrameSave)+")");
		}
		return bGood;
	}
	bool Anim32BGRA::ResetBitmapUsingFrameNow() {
		bool bGood=true;
		try {
			if (gbFrame.byarrData==null) {
				bGood=false;
				ShowError("gbFrame.byarrData is null","ResetBitmapUsingFrameNow");
			}
			if (bGood) {
				if ( (targaLoaded.BytesBuffer()==0)
				   || (gbFrame.iStride!=targaLoaded.Stride())
				   || (gbFrame.iWidth!=rectNow.Width)
				   || (gbFrame.iHeight!=rectNow.Height)
				   || (gbFrame.iBytesPP!=targaLoaded.Stride()/rectNow.Width)
				   || (gbFrame.iBytesTotal!=(targaLoaded.Stride()*rectNow.Height)) ) {
					targaLoaded.From(gbFrame.iWidth, gbFrame.iHeight, gbFrame.iBytesPP, gbFrame.byarrData, false); //true would copy pointer instead of data
				}
				//targaLoaded.Save(RetroEngine.sDataFolderSlash+"1.test-blank.png");
				//sLogLine="Saved blank test PNG for Bitmap debug";
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "ResetBitmapUsingFrameNow");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("ResetBitmapUsingFrameNow");
		}
		return bGood;
	}//end ResetBitmapUsingFrameNow
	/*
	PixelFormat PixelFormatNow() {
		return pixelformatNow;
	}
	*/
	bool Anim32BGRA::CopyFrameToInternalBitmap() {
		bool bGood=true;
		try {
			bGood=ResetBitmapUsingFrameNow();
			if (!bGood) ShowError("Failed to reset internal frame image","CopyFrameToInternalBitmap()");
			targaLoaded.ToRect(rectNowF);
			rectNow.Set((int)rectNowF.X, (int)rectNowF.Y,
								(int)rectNowF.Width, (int)rectNowF.Height);
			bool bReCompress=targaLoaded.IsCompressed();
			targaLoaded.SetCompressionRLE(false);
			byte* lpbyNow = targaLoaded.GetBufferPointer();
			for (int iBy=0; iBy<gbFrame.iBytesTotal; iBy++) {
				*lpbyNow=gbFrame.byarrData[iBy];
				lpbyNow++;
			}
			if (bReCompress) targaLoaded.SetCompressionRLE(true);
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "Anim32BGRA::CopyFrameToInternalBitmap");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Anim32BGRA::CopyFrameToInternalBitmap");
		}
		return bGood;
	}//end CopyFrameToInternalBitmap()
	bool Anim32BGRA::LoadInternalBitmap(string sFile) {
		bool bGood=true;
		try {
			bGood=CopyFrameFromInternalBitmap();
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "LoadInternalBitmap(\""+sFile+"\") (problem with file type or location)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("LoadInternalBitmap(\""+sFile+"\") (problem with file type or location)");
		}
		return bGood;
	}
	int Anim32BGRA::CountSeqFrames(string sSetFileBaseName, string sSetFileExt) {
		return CountSeqFrames(sSetFileBaseName, sSetFileExt, 4);
	}
	int Anim32BGRA::CountSeqFrames(string sSetFileBaseName, string sSetFileExt, int iSeqDigitCountNow) {
		static bool bFirstRun=true;
		if (bFirstRun) Console.Write("CountSeqFrames...");
		long iNow=0;
		string sMsg="";
		bool bGood=true;
		try {
            sFileBaseName=sSetFileBaseName;
            sFileExt=sSetFileExt;
			while (  File.Exists( ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName,sSetFileExt,iNow,iSeqDigitCountNow) )  ) {
				if (bFirstRun) sMsg+=("["+ToString(iNow)+"]");
				iNow++;
			}
			if (bFirstRun) Console.Write(sMsg+"done counting...");
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "CountSeqFrames");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("CountSeqFrames");
		}
		bFirstRun=false;
		return iNow;
	}//end CountSeqFrames
	bool Anim32BGRA::LoadSeq(string sSetFileNameWithAsterisk) {
		int iStar=(int)sSetFileNameWithAsterisk.find_first_of("*");
		string sSetFileBaseName=SafeSubstring(sSetFileNameWithAsterisk,0,iStar);
		string sExt=SafeSubstring(sSetFileNameWithAsterisk,iStar+1,sSetFileNameWithAsterisk.length()-(iStar+1));
		if (StartsWith(sExt,".")) sExt=SafeSubstring(sExt,1);
		bool bGood=true;
		if (SafeLength(sSetFileBaseName)<1) {
			bGood=false;
			ShowError("Couldn't get file extension given the filename "+sSetFileNameWithAsterisk,"Anim32BGRA::LoadSeq");
		}
		else {
			bGood=LoadSeq(sSetFileBaseName, sExt);
		}
		return bGood;
	}
	bool Anim32BGRA::LoadSeq(string sSetFileBaseName, string sSetFileExt) {
		int iTemp=CountSeqFrames(sSetFileBaseName,sSetFileExt,iSeqDigitCountMin);
		return LoadSeq(sSetFileBaseName, sSetFileExt, iTemp, iSeqDigitCountMin, 0);
	}
	bool Anim32BGRA::LoadSeq(string sSetFileBaseName, string sSetFileExt, int &iFrameCountToUseAndModify, int iSeqDigitCountNow) {
		return LoadSeq(sSetFileBaseName, sSetFileExt, iFrameCountToUseAndModify,iSeqDigitCountNow,0);
	}
	bool Anim32BGRA::LoadSeq(string sSetFileBaseName, string sSetFileExt, int &iFrameCountToUseAndModify, int iSeqDigitCountNow, int iStartFrame) {
		sFileBaseName=sSetFileBaseName;
		sFileExt=sSetFileExt;
		bool bGood=true;
       	static bool bFirstRun=true;
       	if (bFirstRun) Console.Write("starting LoadSeq");
       	if (bFirstRun) Console.Write("("+sSetFileBaseName+","+sSetFileExt+","+ToString(iFrameCountToUseAndModify)+","+ToString(iSeqDigitCountNow)+","+ToString(iStartFrame)+")...");
       	if (iFrameCountToUseAndModify<=0) {
			ShowError("Tried to load zero-length frame array!","LoadSeq("+sSetFileBaseName+","+sSetFileExt+","+ToString(iFrameCountToUseAndModify)+",...)");
			bGood=false;
		}
		//long iEnder=(long)iFrameCountToUseAndModify+(long)iStartFrame;
		long iFrameRel=0;
		string sFrameFileNow="";
		if (bGood) {
	       	if (bFirstRun) Console.Write("buffer array...");
			GBuffer32BGRA* gbarrTemp=new GBuffer32BGRA[iFrameCountToUseAndModify];
			lFrames=(long)iFrameCountToUseAndModify;
			lFramesCached=(long)iFrameCountToUseAndModify;
			iFrameCountToUseAndModify=0;
	       	if (bFirstRun) Console.Write("("+ToString(lFramesCached)+")frames...");
			for (long iNow=0; iNow<lFramesCached; iNow++) {
		       	if (bFirstRun) Console.Write(" ["+ToString(iNow)+"]as("+ToString(iNow+iStartFrame)+")");
		       	sFrameFileNow=ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName, sSetFileExt, (long)iNow, iSeqDigitCountNow);
		       	//if (gbarrTemp[iFrameRel]==null) gbarrTemp[iFrameRel]=new GBuffer32BGRA(sFrameFileNow)
				//else
				if (gbarrTemp[iNow].Load(sFrameFileNow)) iFrameCountToUseAndModify++;
				else {
					bGood=false;
					ShowError("Couldn't load \""+sFrameFileNow+"\"","Anim32BGRA::LoadSeq");
					break;
				}
			}
			gbarrAnim=gbarrTemp;
			if (gbarrAnim==null) {
				ShowError("FrameArray:null! ","Anim32BGRA::LoadSeq");
				bGood=false;
			}
			else {
				if (bFirstRun) Console.Write("FrameArray:ok...");
			}
			if (bGood) {
		       	if (bFirstRun) Console.Write("GotoFrame(0)--");
				if (!GotoFrame(0)) {
					bGood=false;
	                if (bFirstRun) Console.Write("failed...");
				}
				else Console.Write("yes...");
			}
		}//end if bGood //else already displayed errors
       	if (bFirstRun) Console.WriteLine(bGood?"Success.":"Done with errors.");
       	bFirstRun=false;
		return bGood;
	}//Anim32BGRA::LoadSeq
	bool Anim32BGRA::CopyFrameFromInternalBitmap() {
		bool bGood=true;
		try {
			if (targaLoaded.byarrData==null) {
				bGood=false;
				ShowError("targa is not loaded!","Anim32BGRA::CopyFrameFromInternalBitmap");
			}
			if (bGood) {
				targaLoaded.ToRect(rectNowF);
				rectNow.Set((int)rectNowF.X, (int)rectNowF.Y,
									(int)rectNowF.Width, (int)rectNowF.Height);
				if (  (gbFrame.iStride!=targaLoaded.Stride())
				   || (gbFrame.iWidth!=rectNow.Width)
				   || (gbFrame.iHeight!=rectNow.Height)
				   || (gbFrame.iBytesPP!=targaLoaded.Stride()/rectNow.Width)
				   || (gbFrame.iBytesTotal!=(targaLoaded.Stride()*rectNow.Height)) ) {
					gbFrame.Init(rectNow.Width,rectNow.Height,gbFrame.iBytesPP,true);
				}
				byte* lpbyNow=targaLoaded.GetBufferPointer();
				for (int iBy=0; iBy<gbFrame.iBytesTotal; iBy++) {
					gbFrame.byarrData[iBy]=*lpbyNow;
					lpbyNow++;
				}
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "Anim32BGRA::CopyFrameFromInternalBitmap");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Anim32BGRA::CopyFrameFromInternalBitmap");
		}
		return bGood;
	}//end CopyFrameFromInternalBitmap
	/// <summary>
	/// Saves the image in the format from which it was loaded.
	/// </summary>
	/// <param name="sFileName">File name, make sure extension is same as loaded file.</param>
	/// <returns>false if exception</returns>
	bool Anim32BGRA::SaveInternalBitmap(string sFileName) {
		bool bGood=true;
		try {
			bGood=targaLoaded.Save(sFileName);
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "SaveInternalBitmap(\""+sFileName+"\")");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("SaveInternalBitmap(\""+sFileName+"\")");
		}
		return bGood;
	}
	//bool FromFrames(byte[][] by2dFrames, long lFrames, int iBytesPPNow, int iWidthNow, int iHeightNow) {
	//}
	bool Anim32BGRA::GotoFrame(int iFrameX) {
		GotoFrame((long)iFrameX);
	}
	bool Anim32BGRA::GotoFrame(long lFrameX) {
		//refers to a file if a file is used.
		bool bGood=true;
		static bool bFirstRun=true;
		try {
			if (lFramesCached==lFrames) {
				if (gbarrAnim==null) {
					ShowError("null frame array!","GotoFrame");
					bGood=false;
				}
				if (bGood) {
					if (lFrameX<lFramesCached && lFrameX>=0) {
						gbarrAnim[lFrameX].CopyToByDataRef(gbFrame);
						lFrame=lFrameX;
						if (gbFrame.byarrData==null) {
							bGood=false;
							ShowError("Null frame!","GotoFrame("+ToString(lFrameX)+")");
						}
						else if (gbFrame.iWidth==0) {
							bGood=false;
							ShowError("Bad frame data ("+ToString(gbFrame.iWidth)+"x"+ToString(gbFrame.iHeight)+"x"+ToString(gbFrame.iBytesPP*8)+")!","GotoFrame("+ToString(lFrameX)+")");
						}
						else if (bFirstRun) {
							Console.Write("GotoFrame dimensions:"+ToString(gbFrame.iWidth)+"x"+ToString(gbFrame.iHeight)+"x"+ToString(gbFrame.iBytesPP*8)+"...");
						}
					}
					else {
						bGood=false;
						ShowError("Frame "+ToString(lFrameX)+" is out of range!");
					}
				}
			}
			else {//if ((sPathFile!=null) && (sPathFile.length()>0)) {
				bGood=false;
				ShowError("GotoFrame of non-cached sequence is not available in this version");//debug NYI
				//image.SelectActiveFrame(image.FrameDimensionsList[lFrameX], (int)lFrameX);
				//debug NYI load from file
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"Anim32BGRA::GotoFrame("+ToString(lFrameX)+")");
		}
		catch (...) {
            bGood=false;
            ShowUnknownException("Anim32BGRA::GotoFrame");
		}
		bFirstRun=false;
		return bGood;
	}//end GotoFrame
	bool Anim32BGRA::GotoNextFrame() {
		if (lFrame+1==lFrames) {
			if (bLoop) GotoFrame(0);
		}
		else GotoFrame(lFrame+1);
	}
	bool Anim32BGRA::GotoNextFrame(bool bSetAsLoop) {
		bLoop=bSetAsLoop;
		return GotoNextFrame();
	}
	bool Anim32BGRA::DrawFrameOverlay(GBuffer32BGRA &gbDest, IPoint &ipDest, long lFrameX) {
		bool bGood=GotoFrame(lFrameX);
		if (DrawFrameOverlay(gbDest, ipDest)==false) bGood=false;
		return bGood;
	}
	bool Anim32BGRA::DrawFrameOverlay(GBuffer32BGRA &gbDest, IPoint &ipDest) {
		GBuffer32BGRA gbSafeFrame;
		if (gbFrame.byarrData!=null) gbFrame.CopyTo(gbSafeFrame);
		return OverlayNoClipToBigCopyAlpha(gbDest, ipDest, gbSafeFrame);
	}
	int Anim32BGRA::MinDigitsRequired(int iNumber) {
		string sNumber=ToString(iNumber);
		return sNumber.length();
	}
	bool Anim32BGRA::LastFrame() {
		return (gbFrame.byarrData!=null && lFrame==(lFrames-1));
	}
	string Anim32BGRA::PathFileOfSeqFrame(long lFrameTarget) {
		string sReturn="";
		try {
			//debug performance:
			//string sSetFileExt=sPathFile.Substring(sPathFile.LastIndexOf(".")+1);
			//int iLengthBase=(sPathFile.length()-iSeqDigitCountMin)-(1+sSetFileExt.length());
			//string sFileBaseName1=sPathFile.Substring(iLengthBase); //INCLUDES Path
			//sReturn=PathFileOfSeqFrame(sFileBaseName1, sSetFileExt, lFrameTarget, iSeqDigitCountMin);
			sReturn=ExpertMultimediaBase::PathFileOfSeqFrame(sFileBaseName, sFileExt, lFrameTarget, iSeqDigitCountMin);
		}
		catch (exception& exn) {
			ShowException(exn, "Anim32BGRA::PathFileOfSeqFrame("+ToString(lFrameTarget)+")");
		}
		catch (...) {
			ShowUnknownException("Anim32BGRA::PathFileOfSeqFrame("+ToString(lFrameTarget)+")");
		}
		return sReturn;
	}
	//bool Anim32BGRA::FromGifAnim(string sFile) {
	//}
	//bool Anim32BGRA::ToGifAnim(string sFile) {
		//image.Save(,System.Drawing.Imaging.ImageFormat.Gif);
	//}
	bool Anim32BGRA::SplitFromImage32(string sFile, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		GBuffer32BGRA gbTemp;
		bool bGood=false;
		try {
			gbTemp.Load(sFile);
			if (gbTemp.iBytesTotal==0) {
				ShowError("Failed to write font file to GBuffer32BGRA","SplitFromImage32");
				bGood=false;
			}
			else {
				//targaLoaded=gbTemp.targaLoaded;//SplitFromImage32 remakes targaLoaded
				bGood=SplitFromImage32(gbTemp, iCellWidth, iCellHeight, iRows, iColumns, ipAsCellSpacing, irectAsMargins);
				//sLogLine="Saving test bitmap for debug...";
				//gbTemp.Save("1.test bitmap for debug.tif", ImageFormat.Tiff);
				//gbTemp.SaveRaw("1.test bitmap for debug.raw");
				//sLogLine="Done saving test Bitmap for debug";
			}
		}
		catch (exception& exn) {
			ShowException(exn, "SplitFromImage32 alternate");
		}
		catch (...) {
			ShowUnknownException("SplitFromImage32 alternate");
		}
		return bGood;
	}//end SplitFromImage32 alternate
	bool Anim32BGRA::SplitFromImage32(string sFileImage, int iCellWidth, int iCellHeight, int iRows, int iColumns) {
		bool bGood=false;
 		try {
           	IPoint ipAsCellSpacing;
			IRect irectAsMargins;
			bGood=SplitFromImage32(sFileImage, iCellWidth, iCellHeight, iRows, iColumns, ipAsCellSpacing, irectAsMargins);
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "SplitFromImage32 alternate#2");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("SplitFromImage32 alternate#2");
		}
		return bGood;
	}
	bool Anim32BGRA::SplitFromImage32(GBuffer32BGRA &gbSrc, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		bool bGood=true;
		string sFuncNow="SplitFromImage32(...)";
		lFrames=(long)iRows*(long)iColumns;
		lFramesCached=lFrames; //so cached anim will be accessed
		try {
			SafeDeleteAnim();
			if (lFrames<=0) {
				ShowError("Frame count is "+ToString(lFrames)+"!",sFuncNow);
				bGood=false;
			}
			if (bGood) {
				gbarrAnim=new GBuffer32BGRA[lFrames];
				targaLoaded.Init(iCellWidth, iCellHeight, gbSrc.iBytesPP, true);
				for (int lFrameX=0; lFrameX<lFrames; lFrameX++) {
					gbarrAnim[lFrameX].Init(iCellWidth, iCellHeight, 4); //assumes 32-bit
				}
				//lFrame=0; //not used
				int iSrcByteOfCellTopLeft=irectAsMargins.top*gbSrc.iStride + irectAsMargins.left*gbSrc.iBytesPP;
				int iSrcByteOfCellNow;
				//int iSrcAdder=gbSrc.iStride-gbSrc.iBytesPP*iCellWidth;
				//int iSrcNextCellAdder=
				//int iSrcStride=iColumns*iWidth*4; //assumes 32-bit source
				int iSrcByte;
				int iDestByte;
				//int iCellNow=0;
				//int iCellStride=iWidth*iBytesPP;
				//int yStrideAdder=iSrcStride*(iHeight-1);
				//int iSrcAdder=iSrcStride-iWidth*iBytesPP;
				int iDestStride=gbarrAnim[0].iStride;
				int iHeight=gbarrAnim[0].iHeight;
				int iSrcStride=gbSrc.iStride;
				int iCellPitchX=gbSrc.iBytesPP*(iCellWidth+ipAsCellSpacing.x);
				int iCellPitchY=gbSrc.iStride*(iCellHeight+ipAsCellSpacing.y);
				long lFrameLoad=0;
				for (int yCell=0; yCell<iRows; yCell++) {
					for (int xCell=0; xCell<iColumns; xCell++) {
						iDestByte=0;
						iSrcByteOfCellNow=iSrcByteOfCellTopLeft + yCell*iCellPitchY + xCell*iCellPitchX;
						iSrcByte=iSrcByteOfCellNow;
						GotoFrame(lFrameLoad);
						for (int iLine=0; iLine<iHeight; iLine++) {
							//TODO: finish this--make it safer:
							memcpy(&gbFrame.byarrData[iDestByte],&gbSrc.byarrData[iSrcByte],iDestStride);
							//if (Byter.CopyFast(gbFrame.byarrData, gbSrc.byarrData, iDestByte, iSrcByte, iDestStride)==false)
							//	bGood=false;
							iDestByte+=iDestStride;
							iSrcByte+=iSrcStride;
						}
						lFrameLoad++;
					}
				}
				if (!bGood) ShowError("There was data copy error while interpreting the GBuffer32BGRA to a font, make sure the \"iCellWidth\" etc. variables are set correctly.",sFuncNow);
				//else bGood=GrayMapFromPixMapChannel(3);
				lFramesCached=lFrames;
			}
		}
		catch (exception& exn) {
			ShowException(exn,sFuncNow);
		}
		catch (...) {
            ShowUnknownException(sFuncNow);
		}
		return bGood;
	}//end SplitFromImage32
	/// <summary>
	/// Changes the frame order from top to bottom to left to right using
	/// the idea that the frames are currently wrapped to a square whose 
	/// number of rows are specified by the number of output columns you
	/// specify.
	/// </summary>
	/// <param name="iResultCols"></param>
	/// <returns></returns>
	bool Anim32BGRA::TransposeFramesAsMatrix(int iResultRows, int iResultCols) {
		//TODO: exception handling
		bool bGood=false;
		GBuffer32BGRA* gbarrNew;
		string sDebug="starting TransposeFramesAsMatrix()\n";
		//StringToFile("X:\\anim.TranslateFrameOrder debug.txt", sDebug);
		try {
			gbarrNew=new GBuffer32BGRA[lFrames];
			int iFrames=(int)lFrames;
			for (int iFrame=0; iFrame<iFrames; iFrame++) {
				int iNew=(int)(iFrame/iResultCols)+(int)(iFrame%iResultCols)*iResultRows; //switched (must be)
				sDebug+="old:"+ToString(iFrame)+"; new:"+ToString(iNew)+"\n";
				gbarrAnim[iNew].CopyTo(gbarrNew[iFrame]);
			}
			SafeDeleteAnim();
			gbarrAnim=gbarrNew; 
			gbarrAnim[lFrame].CopyToByDataRef(gbFrame);
			bGood=true;
			//StringToFile("X:\\anim.TranslateFrameOrder debug.txt", sDebug);
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "TransposeFramesAsMatrix");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("TransposeFramesAsMatrix");
		}
		sDebug+="Finished.";
		//TODO: in future don't set bGood according to StringToFile
		//bGood=StringToFile("X:\\anim.TranslateFrameOrder debug.txt", sDebug);
		return bGood;
	}//end TransposeFramesAsMatrix
	
	//GBuffer32BGRA ToOneImage(int iCellW, int iCellH, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
	//bool SplitFromImage32(GBuffer32BGRA &gbSrc, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
	GBuffer32BGRA* Anim32BGRA::ToOneImage(int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		bool bGood=true;
		string sFuncNow="ToOneImage(...)";
		int iFrames=iRows*iColumns;
		lFramesCached=lFrames; //so cached anim will be accessed
		GBuffer32BGRA* gbNew=null;
		try {
			//gbarrAnim=new GBuffer32BGRA[lFrames];
			GotoFrame(0);
			gbNew=new GBuffer32BGRA(iCellWidth*iColumns, iCellHeight*iRows, gbFrame.iBytesPP);
			//bmpLoaded=new Bitmap(iCellWidth, iCellHeight, PixelFormatNow());
			//for (int lFrameX=0; lFrameX<lFrames; lFrameX++) {
			//	gbarrAnim[lFrameX]=new GBuffer32BGRA(iCellWidth, iCellHeight, 4); //assumes 32-bit
			//}
			lFrame=0; //TODO: use new var instead of class var
			int iDestByteOfCellTopLeft=irectAsMargins.top*gbNew->iStride + irectAsMargins.left*gbNew->iBytesPP;
			int iDestByteOfCellNow;
			int iDestByte;
			int iSrcByte;
			int iSrcStride=gbFrame.iStride;
			int iHeight=gbFrame.iHeight;
			int iDestStride=gbNew->iStride;
			int iCellPitchX=gbNew->iBytesPP*(iCellWidth+ipAsCellSpacing.x);
			int iCellPitchY=gbNew->iStride*(iCellHeight+ipAsCellSpacing.y);
			long lFrameLoad=0;
			for (int yCell=0; yCell<iRows; yCell++) {
				for (int xCell=0; xCell<iColumns; xCell++) {
					iSrcByte=0;
					iDestByteOfCellNow=iDestByteOfCellTopLeft + yCell*iCellPitchY + xCell*iCellPitchX;
					iDestByte=iDestByteOfCellNow;
					GotoFrame(lFrameLoad);
					//TODO: finish this: check whether next line is needed
					//gbFrame.Save("debugToOneImage"+SequenceDigits(lFrameLoad)+".png",ImageFormat.Png);
					for (int iLine=0; iLine<iHeight; iLine++) {
						//TODO: finish this: make it safer:
						memcpy(&gbNew->byarrData[iDestByte],&gbFrame.byarrData[iSrcByte], iSrcStride);
						iSrcByte+=iSrcStride;
						iDestByte+=iDestStride;
					}
					lFrameLoad++;
				}
			}
			if (!bGood) ShowError("There was data copy error while interpreting the GBuffer32BGRA to a font, make sure the \"iCellWidth\" etc. variables are set correctly.",sFuncNow);
			//else bGood=GrayMapFromPixMapChannel(3);
			bGood=true;
			lFramesCached=lFrames;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, sFuncNow);
		}
		catch (...) {
			bGood=false;
			ShowUnknownException(sFuncNow);
		}
		return gbNew;
	}//end ToOneImage
	bool Anim32BGRA::SafeDeleteAnim() {
		bool bGood=false;
		try {
			if (gbarrAnim!=null) {
				delete [] gbarrAnim;
				gbarrAnim=null;
			}
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "SafeDeleteAnim");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("SafeDeleteAnim");
		}
		return bGood;
	}//end SafeDeleteAnim
	bool Anim32BGRA::SafeDeleteEffects() {
		bool bGood=false;
		try {
			if (effectarr!=null) {
				delete [] effectarr;
				effectarr=null;
			}
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "SafeDeleteEffects");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("SafeDeleteEffects");
		}
		return bGood;
	}//end SafeDeleteEffects
	int Anim32BGRA::Width() {
		int iReturn=0;
		if (gbFrame.byarrData!=NULL) iReturn=gbFrame.iWidth;
		return iReturn;
	}
	int Anim32BGRA::Height() {
		int iReturn=0;
		if (gbFrame.byarrData!=NULL) iReturn=gbFrame.iHeight;
		return iReturn;
	}
	bool Anim32BGRA::DrawToLargerWithoutCropElseCancel(GBuffer32BGRA &gbDest, int xDest, int yDest, int iDrawMode) {
		bool bGood=false;
		static bool bFirstRun=true;
		if (bFirstRun) Console.Write("DrawToLargerWithoutCropElseCancel...");
		try {
			if (gbFrame.byarrData==null) {
				bGood=false;
				ShowError("Tried to draw null frame["+ToString(lFrame)+"]","Anim32BGRA::DrawToLargerWithoutCropElseCancel");
			}
			else {
				if (bFirstRun) Console.Write("calling gbFrame...");
				bGood=gbFrame.DrawToLargerWithoutCropElseCancel(gbDest, xDest, yDest, iDrawMode);
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "DrawToLargerWithoutCropElseCancel");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("DrawToLargerWithoutCropElseCancel");
		}
		bFirstRun=false;
		return bGood;
	}//end DrawToLargerWithoutCropElseCancel
	int Anim32BGRA::IFrames() {
		return (int)lFrames;
	}
	//#endregion class Anim32BGRA methods
	

}//end namespace
#endif
