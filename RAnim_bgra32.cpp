#ifndef ANIM_CPP
#define ANIM_CPP
#include <RAnim_bgra32.h> //#include "E:\Projects-cpp\Base\RAnim_bgra32.h"
#include <base.h>

using namespace std;
//using System;
//using System.Drawing;
//using System.Drawing.Imaging;
//using System.IO;
//using System.Drawing.Text;

namespace ExpertMultimediaBase {
	void SafeFree(Anim*& arranimX) {
		try {
			if (arranimX!=null) delete [] arranimX;
			arranimX=null;
		}
		catch (...) { }
	}
	//PathFileOfSeqFrame(sSetFileBaseName, sSetFileExt, lFrameTarget, iDigitsMin)
	string PathFileOfSeqFrame(string sSetFileBaseName, string sSetFileExt, long lFrameTarget, int iDigitsMin) {
		static bool bFirstRun=true;
		if (bFirstRun) Console::Error.Write("PathFileOfSeqFrame...");
		string sReturn="";
		try {
			sReturn=sSetFileBaseName;
			if (iDigitsMin>0) {
				long lDivisor=SafeE10L((int)(iDigitsMin-1));//returns long since implied base is 10L
				long lDestruct=lFrameTarget;
				while (lDivisor>0) {
					long lResult=lDestruct/lDivisor;
					sReturn+=RString_ToString(lResult);
					lDestruct-=lResult*lDivisor;
					if (lDivisor==1) lDivisor=0;
					else lDivisor/=10;
				}
			}
			else sReturn+=RString_ToString(lFrameTarget);
			sReturn+="."+sSetFileExt;
			if (bFirstRun) Console::Error.Write("done...");//Console::Error.Write("returning \""+sReturn+"\"...");
		}
		catch (exception& exn) {
			ShowExn(exn, "Anim.PathFileOfSeqFrame");
		}
		catch (...) {
			ShowUnknownExn("Anim.PathFileOfSeqFrame");
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
			ShowExn(exn, "Effect::Copy");
		}
		catch (...) {
			ShowUnknownExn("Effect::Copy");
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
	Anim::Anim() {
		InitNull();
		iMaxEffects=200;//debug hard-coded limitation
		//targaLoaded.InitNull(); //compiler already called constructor
		gbFrame.InitNull();
	}
	void Anim::InitNull() {
		//targaLoaded;
		rectNowF.X=0;
		rectNowF.Y=0;
		rectNowF.Width=0;
		rectNowF.Height=0;
		rectNow.X=0;
		rectNow.Y=0;
		rectNow.Width=0;
		rectNow.Height=0;
		sFileBaseName="";
		sFileExt="";
		//pixelformatNow=PixelFormat.Format32bppArgb;
		gbarrAnim=null;
		bLoop=false;
		lFramesCached=0;
		//sPathFile="";
		bFileSequence=false;//if true, use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
		iSeqDigitCountMin=4;//, 0 if variable (i.exn. frame1.png...frame10.png)
		lFrame=0;
		lFrames=0;
		iEffects=0;
		iMaxEffects=0;
		effectarr=null;
	}
	Anim::~Anim() {
		SafeDeleteAnim();
		SafeDeleteEffects();
	}
	string Anim::sPathFileNonSeq() {
		return sFileBaseName+"."+sFileExt;
	}
	Anim* Anim::Copy() {
		Anim* animReturn=null;
		try {
			animReturn=new Anim();
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
				animReturn->gbarrAnim=new GBuffer[lFramesCached];
				for (long l=0; l<lFrames; l++) {
					gbarrAnim[l].CopyTo(animReturn->gbarrAnim[l]);
				}
			}
			else ShowError("Uncached anim not yet implemented","Anim Copy");
			animReturn->gbarrAnim[lFrame].CopyToByDataRef(animReturn->gbFrame);
			if (iEffects>0) {
				animReturn->effectarr=new Effect[iMaxEffects];
				for (int i=0; i<iEffects; i++) {
					effectarr[i].CopyTo(animReturn->effectarr[i]);
				}
			}
			else animReturn->effectarr=null;
		}
		catch (exception& exn) {
			ShowExn(exn, "Anim Copy");
		}
		catch (...) {
			ShowUnknownExn("Anim Copy");
		}
		return animReturn;
	}//end Copy
	Anim* Anim::CopyAsGray() {
		return CopyAsGray(-1);
	}
	bool Anim::CopyTo(Anim& animReturn) {
		bool bGood=false;
		try {
			targaLoaded.CopyTo(animReturn.targaLoaded);
			animReturn.rectNowF.Set(rectNowF.X,rectNowF.Y,rectNowF.Width,rectNowF.Height);
			animReturn.rectNow.Set(rectNow.X,rectNow.Y,rectNow.Width,rectNow.Height);
			animReturn.lFramesCached=lFramesCached;
			animReturn.sFileBaseName=sFileBaseName;
			animReturn.sFileExt=sFileExt;
			//animReturn.sPathFile=sPathFile;
			animReturn.bFileSequence=bFileSequence;//if use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
			animReturn.iSeqDigitCountMin=iSeqDigitCountMin;//, 0 if variable (i.exn. frame1.png...frame10.png)
			animReturn.lFrame=lFrame;
			animReturn.lFrames=lFrames;
			animReturn.iEffects=iEffects;
			animReturn.iMaxEffects=iMaxEffects;
			animReturn.lFramesCached=lFramesCached;
			if (lFramesCached==lFrames) {
				animReturn.gbarrAnim=new GBuffer[lFramesCached];
				for (long l=0; l<lFrames; l++) {
					gbarrAnim[l].CopyTo(animReturn.gbarrAnim[l]);
				}
			}
			else ShowError("Uncached anim not yet implemented","Anim Copy");
			animReturn.gbarrAnim[lFrame].CopyToByDataRef(animReturn.gbFrame);
			if (iEffects>0) {
				animReturn.effectarr=new Effect[iMaxEffects];
				for (int i=0; i<iEffects; i++) {
					effectarr[i].CopyTo(animReturn.effectarr[i]);
				}
			}
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "Anim Copy");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Anim Copy");
		}
		return bGood;
	}//end CopyTo
	bool Anim::CopyAsGrayTo(Anim& animReturn) {
		return Anim::CopyAsGrayTo(animReturn,-1);
	}
	bool Anim::CopyAsGrayTo(Anim& animReturn, int iChannelOffset) {
		bool bGood=false;
		try {
			Console::Error.Write("Copying targa...");
			bGood=targaLoaded.CopyTo(animReturn.targaLoaded);
			Console::Error.Write(bGood?("Done ("+animReturn.targaLoaded.Description(true)+").  "):"Failed!  ");
			animReturn.rectNowF.Set(rectNowF.X,rectNowF.Y,rectNowF.Width,rectNowF.Height);
			animReturn.rectNow.Set(rectNow.X,rectNow.Y,rectNow.Width,rectNow.Height);
			animReturn.lFramesCached=lFramesCached;
			animReturn.sFileBaseName=sFileBaseName;
			animReturn.sFileExt=sFileExt;
			//animReturn.sPathFile=sPathFile;
			animReturn.bFileSequence=bFileSequence;//if use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
			animReturn.iSeqDigitCountMin=iSeqDigitCountMin;//, 0 if variable (i.exn. frame1.png...frame10.png)
			animReturn.lFrame=lFrame;
			animReturn.lFrames=lFrames;
			animReturn.iEffects=iEffects;
			animReturn.iMaxEffects=iMaxEffects;
			animReturn.lFramesCached=lFramesCached;
			//Console::Error.Write("Copying Frame");
			if (lFramesCached==lFrames) {
				Console::Error.Write("writing ");
				Console::Error.Flush();
				animReturn.gbarrAnim=new GBuffer[lFramesCached];
				Console::Error.Write(RString_ToString(lFramesCached)+" frames:");
				Console::Error.Flush();
				if ((iChannelOffset<0)||(iChannelOffset>=gbarrAnim[0].iBytesPP)) {
					if (iChannelOffset>=gbarrAnim[0].iBytesPP) {
						Console::Error.Write("Source channel is out of range {iChannelOffset:"+RString_ToString(iChannelOffset)+"; gbarrAnim[0].iBytesPP:"+RString_ToString(gbarrAnim[0].iBytesPP)+"} so defaulting to average color for value!  ");
						Console::Error.Flush();
					}
					for (long index=0; index<lFrames; index++) {
						if (!MaskFromValue(animReturn.gbarrAnim[index], gbarrAnim[index])) {
							bGood=false;
							ShowErr("<failed>");
							break;
						}
						//else Console::Error.Write(".");
					}
				}
				else {
					for (long index=0; index<lFrames; index++) {
						if (!MaskFromChannel(animReturn.gbarrAnim[index], gbarrAnim[index], iChannelOffset)) {
							bGood=false;
							ShowErr("<failed>");
						}
						//else Console::Error.Write(".");
					}
				}
			}
			else ShowError("Uncached Anim not yet implemented","Anim CopyAsGray");
			Console::Error.Write(".");
			animReturn.gbarrAnim[lFrame].CopyToByDataRef(animReturn.gbFrame);
			Console::Error.Write(".");
			if (iEffects>0) {
				animReturn.effectarr=new Effect[iMaxEffects];
				for (int index=0; index<iEffects; index++) {
					effectarr[index].CopyTo(animReturn.effectarr[index]);
				}
			}
			Console::Error.Write(".");
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "Anim CopyAsGray");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Anim CopyAsGray");
		}
		return bGood;
	}
	/// <summary>
	/// Makes a new Anim object from any channel (or the value) of this one.
	/// </summary>
	/// <param name="iChannelOffset">Pick a channel to copy.  Set to -1 to take average of RGB.</param>
	/// <returns></returns>
	Anim* Anim::CopyAsGray(int iChannelOffset) {
		Anim* animReturn=null;
		try {
			animReturn=new Anim();
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
				animReturn->gbarrAnim=new GBuffer[lFramesCached];
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
			else ShowError("Uncached Anim not yet implemented","Anim CopyAsGray");
			animReturn->gbarrAnim[lFrame].CopyToByDataRef(animReturn->gbFrame);
			if (iEffects>0) {
				animReturn->effectarr=new Effect[iMaxEffects];
				for (int index=0; index<iEffects; index++) {
					effectarr[index].CopyTo(animReturn->effectarr[index]);
				}
			}
		}
		catch (exception& exn) {
			ShowExn(exn, "Anim CopyAsGray");
		}
		catch (...) {
            ShowUnknownExn("Anim CopyAsGray");
		}
		return animReturn;
	}//end CopyAsGray
	bool Anim::SaveSeq(string sSetFileBaseName, string sSetFileExt) {
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
	string Anim::Dump(bool bDumpVars) {
		static bool bFirstRun=true;
		if (bFirstRun) Console::Error.Write("Anim Dump("+RString_ToString(bDumpVars)+")...");
		if (bDumpVars) {
			string sReturn="";
			try {
				sReturn+="{lFrames:"+RString_ToString(lFrames)
					+"; lFramesCached:"+RString_ToString(lFramesCached)
					+"; lFrame:"+RString_ToString(lFrame)
					+"; sFileExt:"+sFileExt
					//+"; sPathFile:"+sPathFile
					+"; sFileBaseName:"+sFileBaseName
					+"; bFileSequence:"+RString_ToString(bFileSequence)
					+"; iEffects:"+RString_ToString(iEffects)
					+"; iMaxEffects:"+RString_ToString(iMaxEffects)
					+"; iSeqDigitCountMin:"+RString_ToString(iSeqDigitCountMin);
				if (gbFrame.arrbyData!=null) {
					if (gbFrame.iBytesTotal>0) {
						try {
						sReturn+="; gbFrame.iBytesTotal:"+RString_ToString(gbFrame.iBytesTotal)
							+"; gbFrame.iWidth:"+RString_ToString(gbFrame.iWidth)
							+"; gbFrame.iHeight:"+RString_ToString(gbFrame.iHeight)
							+"; gbFrame.iWidth:"+RString_ToString(gbFrame.iWidth)
							+"; gbFrame.iBytesPP:"+RString_ToString(gbFrame.iBytesPP)
							+"; gbFrame.iStride:"+RString_ToString(gbFrame.iStride);
						}
						catch (exception& exn) {
							ShowExn(exn,"Anim Dump("+RString_ToString(bDumpVars)+") while accessing gbFrame");
						}
						catch (...) {
				            ShowUnknownExn("Anim Dump("+RString_ToString(bDumpVars)+") while accessing gbFrame");
						}
					}
				}
				else sReturn="gbFrame.arrbyData:null";
				sReturn+=";}";
			}
			catch (exception& exn) {
				ShowExn(exn, "Anim Dump("+RString_ToString(bDumpVars)+")");
			}
			catch (...) {
	            ShowUnknownExn("Anim Dump("+RString_ToString(bDumpVars)+")");
			}
			bFirstRun=false;
			return sReturn;
		}
		else {
            bFirstRun=false;
			return Dump();
		}
	}//end Dump(bDumpVars);
	string Anim::Dump() {
		string sReturn="Anim{";
		sReturn+="lFrames:";
		sReturn+=RString_ToString(lFrames)+";";
		sReturn+="}";
		return sReturn;
	}
	bool Anim::SaveCurrentSeqFrame() {
		bool bGood=CopyFrameToInternalBitmap();
		if (bGood) bGood=SaveInternalBitmap(ExpertMultimediaBase::PathFileOfSeqFrame(sFileBaseName, sFileExt, lFrame, iSeqDigitCountMin));
		return bGood;
	}
	bool Anim::SaveSeqFrame(long lFrameSave) {
		return SaveSeqFrame(sFileBaseName, sFileExt, lFrameSave);
	}
	bool Anim::SaveSeqFrame(string sSetFileBaseName, string sSetFileExt, long lFrameSave) {
		bool bGood=true;
		if (!GotoFrame(lFrameSave)) {
			bGood=false;
			ShowError("Failed to goto frame "+RString_ToString(lFrameSave)+" of Anim","SaveSeqFrame");
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
			else ShowError("Failed to copy data to frame image","SaveSeqFrame("+RString_ToString(lFrameSave)+")");
		}
		return bGood;
	}
	bool Anim::ResetBitmapUsingFrameNow() {
		bool bGood=true;
		try {
			if (gbFrame.arrbyData==null) {
				bGood=false;
				ShowError("gbFrame.arrbyData is null","ResetBitmapUsingFrameNow");
			}
			if (bGood) {
				if ( (targaLoaded.BytesBuffer()==0)
				   || (gbFrame.iStride!=targaLoaded.Stride())
				   || (gbFrame.iWidth!=rectNow.Width)
				   || (gbFrame.iHeight!=rectNow.Height)
				   || (gbFrame.iBytesPP!=targaLoaded.Stride()/rectNow.Width)
				   || (gbFrame.iBytesTotal!=(targaLoaded.Stride()*rectNow.Height)) ) {
					targaLoaded.From(gbFrame.iWidth, gbFrame.iHeight, gbFrame.iBytesPP, gbFrame.arrbyData, false); //true would copy pointer instead of data
				}
				//targaLoaded.Save(RetroEngine.sDataFolderSlash+"1.test-blank.png");
				//sLogLine="Saved blank test PNG for Bitmap debug";
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "ResetBitmapUsingFrameNow");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("ResetBitmapUsingFrameNow");
		}
		return bGood;
	}//end ResetBitmapUsingFrameNow
	/*
	PixelFormat PixelFormatNow() {
		return pixelformatNow;
	}
	*/
	bool Anim::CopyFrameToInternalBitmap() {
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
				*lpbyNow=gbFrame.arrbyData[iBy];
				lpbyNow++;
			}
			if (bReCompress) targaLoaded.SetCompressionRLE(true);
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "Anim CopyFrameToInternalBitmap");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Anim CopyFrameToInternalBitmap");
		}
		return bGood;
	}//end CopyFrameToInternalBitmap()
	bool Anim::LoadInternalBitmap(string sFile) {
		bool bGood=true;
		try {
			bGood=CopyFrameFromInternalBitmap();
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "LoadInternalBitmap(\""+sFile+"\") (problem with file type or location)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("LoadInternalBitmap(\""+sFile+"\") (problem with file type or location)");
		}
		return bGood;
	}
	int Anim::CountSeqFrames(string sSetFileBaseName, string sSetFileExt) {
		return CountSeqFrames(sSetFileBaseName, sSetFileExt, 4);
	}
	int Anim::CountSeqFrames(string sSetFileBaseName, string sSetFileExt, int iSeqDigitCountNow) {
		static bool bFirstRun=true;
		if (bFirstRun) Console::Error.Write("CountSeqFrames...");
		long iNow=0;
		string sMsg="";
		bool bGood=true;
		try {
			sFileBaseName=sSetFileBaseName;
			sFileExt=sSetFileExt;
			while (  File::Exists( ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName,sSetFileExt,iNow,iSeqDigitCountNow) )  ) {
				if (bFirstRun) sMsg+=("["+RString_ToString(iNow)+"]");
				iNow++;
			}
			if (bFirstRun) Console::Error.Write(sMsg+"done counting...");
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "CountSeqFrames");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("CountSeqFrames");
		}
		bFirstRun=false;
		return iNow;
	}//end CountSeqFrames
	bool Anim::LoadSeq(string sSetFileNameWithAsterisk) {
		int iStar=(int)sSetFileNameWithAsterisk.find_first_of("*");
		string sSetFileBaseName=SafeSubstring(sSetFileNameWithAsterisk,0,iStar);
		string sExt=SafeSubstring(sSetFileNameWithAsterisk,iStar+1,sSetFileNameWithAsterisk.length()-(iStar+1));
		if (StartsWith(sExt,".")) sExt=SafeSubstring(sExt,1);
		bool bGood=true;
		if (SafeLength(sSetFileBaseName)<1) {
			bGood=false;
			ShowError("Couldn't get file extension given the filename "+sSetFileNameWithAsterisk,"Anim LoadSeq");
		}
		else {
			bGood=LoadSeq(sSetFileBaseName, sExt);
		}
		return bGood;
	}
	bool Anim::LoadSeq(string sSetFileBaseName, string sSetFileExt) {
		int iTemp=CountSeqFrames(sSetFileBaseName,sSetFileExt,iSeqDigitCountMin);
		return LoadSeq(sSetFileBaseName, sSetFileExt, iTemp, iSeqDigitCountMin, 0);
	}
	bool Anim::LoadSeq(string sSetFileBaseName, string sSetFileExt, int &iFrameCountToUseAndModify, int iSeqDigitCountNow) {
		return LoadSeq(sSetFileBaseName, sSetFileExt, iFrameCountToUseAndModify,iSeqDigitCountNow,0);
	}
	bool Anim::LoadSeq(string sSetFileBaseName, string sSetFileExt, int &iFrameCountToUseAndModify, int iSeqDigitCountNow, int iStartFrame) {
		sFileBaseName=sSetFileBaseName;
		sFileExt=sSetFileExt;
		bool bGood=true;
       	static bool bFirstRun=true;
       	if (bFirstRun) Console::Error.Write("starting LoadSeq");
       	if (bFirstRun) Console::Error.Write("("+sSetFileBaseName+","+sSetFileExt+","+RString_ToString(iFrameCountToUseAndModify)+","+RString_ToString(iSeqDigitCountNow)+","+RString_ToString(iStartFrame)+")...");
       	if (iFrameCountToUseAndModify<=0) {
			ShowError("Tried to load zero-length frame array!","LoadSeq("+sSetFileBaseName+","+sSetFileExt+","+RString_ToString(iFrameCountToUseAndModify)+",...)");
			bGood=false;
		}
		//long iEnder=(long)iFrameCountToUseAndModify+(long)iStartFrame;
		long iFrameRel=0;
		string sFrameFileNow="";
		if (bGood) {
	       	if (bFirstRun) Console::Error.Write("buffer array...");
			GBuffer* gbarrTemp=new GBuffer[iFrameCountToUseAndModify];
			lFrames=(long)iFrameCountToUseAndModify;
			lFramesCached=(long)iFrameCountToUseAndModify;
			iFrameCountToUseAndModify=0;
	       	if (bFirstRun) Console::Error.Write("("+RString_ToString(lFramesCached)+")frames...");
			for (long iNow=0; iNow<lFramesCached; iNow++) {
		       	if (bFirstRun) Console::Error.Write(" ["+RString_ToString(iNow)+"]as("+RString_ToString(iNow+iStartFrame)+")");
		       	sFrameFileNow=ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName, sSetFileExt, (long)iNow, iSeqDigitCountNow);
		       	//if (gbarrTemp[iFrameRel]==null) gbarrTemp[iFrameRel]=new GBuffer(sFrameFileNow)
				//else
				if (gbarrTemp[iNow].Load(sFrameFileNow)) iFrameCountToUseAndModify++;
				else {
					bGood=false;
					ShowError("Couldn't load \""+sFrameFileNow+"\"","Anim LoadSeq");
					break;
				}
			}
			gbarrAnim=gbarrTemp;
			if (gbarrAnim==null) {
				ShowError("FrameArray:null! ","Anim LoadSeq");
				bGood=false;
			}
			else {
				if (bFirstRun) Console::Error.Write("FrameArray:ok...");
			}
			if (bGood) {
		       	if (bFirstRun) Console::Error.Write("GotoFrame(0)--");
				if (!GotoFrame(0)) {
					bGood=false;
	                if (bFirstRun) Console::Error.Write("failed...");
				}
				else Console::Error.Write("yes...");
			}
		}//end if bGood //else already displayed errors
		if (bFirstRun) Console::Error.WriteLine(bGood?"Success (LoadSeq).":"Done with errors (LoadSeq).");
		bFirstRun=false;
		return bGood;
	}//Anim::LoadSeq
	bool Anim::CopyFrameFromInternalBitmap() {
		bool bGood=true;
		try {
			if (targaLoaded.arrbyData==null) {
				bGood=false;
				ShowError("targa is not loaded!","Anim CopyFrameFromInternalBitmap");
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
					gbFrame.arrbyData[iBy]=*lpbyNow;
					lpbyNow++;
				}
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "Anim CopyFrameFromInternalBitmap");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Anim CopyFrameFromInternalBitmap");
		}
		return bGood;
	}//end CopyFrameFromInternalBitmap
	/// <summary>
	/// Saves the image in the format from which it was loaded.
	/// </summary>
	/// <param name="sFileName">File name, make sure extension is same as loaded file.</param>
	/// <returns>false if exception</returns>
	bool Anim::SaveInternalBitmap(string sFileName) {
		bool bGood=true;
		try {
			bGood=targaLoaded.Save(sFileName);
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "SaveInternalBitmap(\""+sFileName+"\")");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("SaveInternalBitmap(\""+sFileName+"\")");
		}
		return bGood;
	}
	//bool FromFrames(byte[][] by2dFrames, long lFrames, int iBytesPPNow, int iWidthNow, int iHeightNow) {
	//}
	bool Anim::GotoFrame(int iFrameX) {
		return GotoFrame((long)iFrameX);
	}
	bool Anim::GotoFrame(long lFrameX) {
		//refers to a file if a file is used.
		bool bGood=true;
		static bool bFirstRun=true;
		try {
			if (lFramesCached>0) {
				if (lFramesCached==lFrames) {
					if (gbarrAnim==null) {
						ShowError("null frame array!","GotoFrame {sFileBaseName:"+sFileBaseName+"}");
						bGood=false;
					}
					if (bGood) {
						if (lFrameX<lFramesCached && lFrameX>=0) {
							gbarrAnim[lFrameX].CopyToByDataRef(gbFrame);
							lFrame=lFrameX;
							if (gbFrame.arrbyData==null) {
								bGood=false;
								ShowError("Null frame!","GotoFrame("+RString_ToString(lFrameX)+") {sFileBaseName:"+sFileBaseName+"}");
							}
							else if (gbFrame.iWidth==0) {
								bGood=false;
								ShowError("Bad frame data ("+RString_ToString(gbFrame.iWidth)+"x"+RString_ToString(gbFrame.iHeight)+"x"+RString_ToString(gbFrame.iBytesPP*8)+")!","GotoFrame("+RString_ToString(lFrameX)+")");
							}
							else if (bFirstRun) {
								Console::Error.Write("GotoFrame dimensions:"+RString_ToString(gbFrame.iWidth)+"x"+RString_ToString(gbFrame.iHeight)+"x"+RString_ToString(gbFrame.iBytesPP*8)+"... ");
							}
						}
						else {
							bGood=false;
							ShowError("Frame "+RString_ToString(lFrameX)+" is out of range!");
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
			else ShowErr("Can't GotoFrame, because lFramesCached is "+RString_ToString(lFramesCached)+" in the anim!");
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Anim GotoFrame("+RString_ToString(lFrameX)+")");
		}
		catch (...) {
            bGood=false;
            ShowUnknownExn("Anim GotoFrame");
		}
		bFirstRun=false;
		return bGood;
	}//end GotoFrame
	bool Anim::GotoNextFrame() {
		bool bReturn=false;
		if (lFrame+1==lFrames) {
			if (bLoop) bReturn=GotoFrame(0);
		}
		else bReturn=GotoFrame(lFrame+1);
		return bReturn;
	}
	bool Anim::GotoNextFrame(bool bSetAsLoop) {
		bLoop=bSetAsLoop;
		return GotoNextFrame();
	}
	bool Anim::DrawFrameOverlay(GBuffer &gbDest, IPoint &ipDest, long lFrameX) {
		bool bGood=GotoFrame(lFrameX);
		if (DrawFrameOverlay(gbDest, ipDest)==false) bGood=false;
		return bGood;
	}
	bool Anim::DrawFrameOverlay(GBuffer &gbDest, IPoint &ipDest) {
		GBuffer gbSafeFrame;
		if (gbFrame.arrbyData!=null) gbFrame.CopyTo(gbSafeFrame);
		return OverlayNoClipToBigCopyAlpha(gbDest, ipDest, gbSafeFrame);
	}
	int Anim::MinDigitsRequired(int iNumber) {
		string sNumber=RString_ToString(iNumber);
		return sNumber.length();
	}
	bool Anim::LastFrame() {
		return (gbFrame.arrbyData!=null && lFrame==(lFrames-1));
	}
	string Anim::PathFileOfSeqFrame(long lFrameTarget) {
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
			ShowExn(exn, "Anim PathFileOfSeqFrame("+RString_ToString(lFrameTarget)+")");
		}
		catch (...) {
			ShowUnknownExn("Anim PathFileOfSeqFrame("+RString_ToString(lFrameTarget)+")");
		}
		return sReturn;
	}
	//bool Anim::FromGifAnim(string sFile) {
	//}
	//bool Anim::ToGifAnim(string sFile) {
		//image.Save(,System.Drawing.Imaging.ImageFormat.Gif);
	//}
	bool Anim::SplitFromImage32(string sFile, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		GBuffer gbTemp;
		bool bGood=false;
		try {
			gbTemp.Load(sFile);
			if (gbTemp.iBytesTotal==0) {
				ShowError("Failed to write font file to GBuffer","SplitFromImage32");
				bGood=false;
			}
			else {
				//targaLoaded=gbTemp.targaLoaded;//SplitFromImage32 remakes targaLoaded
				bGood=SplitFromImage32(gbTemp, iCellWidth, iCellHeight, iRows, iColumns, ipAsCellSpacing, irectAsMargins);
				if (!bGood) ShowError("Failed to split image","GBuffer SplitFromImage32(sFile,...,spacing)");
				//sLogLine="Saving test bitmap for debug...";
				//gbTemp.Save("1.test bitmap for debug.tif", ImageFormat.Tiff);
				//gbTemp.SaveRaw("1.test bitmap for debug.raw");
				//sLogLine="Done saving test Bitmap for debug";
			}
		}
		catch (exception& exn) {
			ShowExn(exn, "GBuffer SplitFromImage32(sFile,...,spacing)");
		}
		catch (...) {
			ShowUnknownExn("GBuffer SplitFromImage32(sFile,...,spacing)");
		}
		return bGood;
	}//end SplitFromImage32 alternate
	bool Anim::SplitFromImage32(string sFile, int iCellWidth, int iCellHeight, int iRows, int iColumns) {
		bool bGood=false;
		try {
			IPoint ipAsCellSpacing;
			IRect irectAsMargins;
			bGood=SplitFromImage32(sFile, iCellWidth, iCellHeight, iRows, iColumns, ipAsCellSpacing, irectAsMargins);
			if (!bGood) ShowError("Failed to split image","GBuffer SplitFromImage32(sFile,...)");
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "GBuffer SplitFromImage32(sFile,...)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("GBuffer SplitFromImage32(sFile,...)");
		}
		return bGood;
	}
	bool Anim::SplitFromImage32(GBuffer &gbSrc, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		bool bGood=true;
		string sFuncNow="SplitFromImage32(gb,...,spacing)";
		lFrames=(long)iRows*(long)iColumns;
		lFramesCached=lFrames; //so cached anim will be accessed
		try {
			SafeDeleteAnim();
			if (lFrames<=0) {
				ShowError("Frame count is "+RString_ToString(lFrames)+"!",sFuncNow);
				bGood=false;
			}
			if (bGood) {
				gbarrAnim=new GBuffer[lFrames];
				targaLoaded.Init(iCellWidth, iCellHeight, gbSrc.iBytesPP, true);
				for (int lFrameX=0; lFrameX<lFrames; lFrameX++) {
					Console::Error.Write("_");
					gbarrAnim[lFrameX].Init(iCellWidth, iCellHeight, gbSrc.iBytesPP); //assumes 32-bit
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
				int iCellPitchX=gbSrc.iBytesPP*(iCellWidth+ipAsCellSpacing.X);
				int iCellPitchY=gbSrc.iStride*(iCellHeight+ipAsCellSpacing.Y);
				long lFrameLoad=0;
				for (int yCell=0; yCell<iRows; yCell++) {
					//Console::Error.WriteLine("[cell row]");
					for (int xCell=0; xCell<iColumns; xCell++) {
						//Console::Error.Write("."); //RString_ToString(lFrameLoad)+gbFrame.Description();
						iDestByte=0;
						iSrcByteOfCellNow=iSrcByteOfCellTopLeft + yCell*iCellPitchY + xCell*iCellPitchX;
						iSrcByte=iSrcByteOfCellNow;
						GotoFrame(lFrameLoad);
						for (int iLine=0; iLine<iHeight; iLine++) {
							//TODO: finish this--make it safer:
							if (gbFrame.arrbyData==null) {bGood=false; ShowErr("this->gbFrame: "+gbFrame.Description(true)+"!  ");}
							else if (gbFrame.arrbyData==null) {bGood=false; ShowErr("gb source: "+gbSrc.Description(true)+"!  ");}
							else if (iDestStride<=0) {bGood=false; ShowErr("DestStride was "+RString_ToString(iDestStride)+"!  ");}
							else if (iDestByte<0||iDestByte>=gbFrame.iBytesTotal) {bGood=false; ShowErr("dest loc "+RString_ToString(iDestByte)+" out of range!  ");}
							else if (iSrcByte<0||iSrcByte>=gbSrc.iBytesTotal) {bGood=false; ShowErr("source loc "+RString_ToString(iSrcByte)+" out of range!  ");}
							else memcpy(&gbFrame.arrbyData[iDestByte],&gbSrc.arrbyData[iSrcByte],iDestStride);
							//if (Byter.CopyFast(gbFrame.arrbyData, gbSrc.arrbyData, iDestByte, iSrcByte, iDestStride)==false)
							//	bGood=false;
							iDestByte+=iDestStride;
							iSrcByte+=iSrcStride;
						}
						//Console::Error.Write(gbFrame.Description()+" ");
						lFrameLoad++;
					}
				}
				if (!bGood) ShowError("There was data copy error while interpreting the gbuffer to a font, make sure the iCellWidth etc. variables are set correctly.",sFuncNow);
				//else bGood=GrayMapFromPixMapChannel(3);
				lFramesCached=lFrames;
			}
		}
		catch (exception& exn) {
			ShowExn(exn,sFuncNow);
		}
		catch (...) {
			ShowUnknownExn(sFuncNow);
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
	bool Anim::TransposeFramesAsMatrix(int iResultRows, int iResultCols) {
		//TODO: exception handling
		bool bGood=false;
		GBuffer* gbarrNew;
		string sDebug="starting TransposeFramesAsMatrix()\n";
		//StringToFile("X:\\anim.TranslateFrameOrder debug.txt", sDebug);
		try {
			gbarrNew=new GBuffer[lFrames];
			int iFrames=(int)lFrames;
			for (int iFrame=0; iFrame<iFrames; iFrame++) {
				int iNew=(int)(iFrame/iResultCols)+(int)(iFrame%iResultCols)*iResultRows; //switched (must be)
				sDebug+="old:"+RString_ToString(iFrame)+"; new:"+RString_ToString(iNew)+"\n";
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
			ShowExn(exn, "TransposeFramesAsMatrix");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("TransposeFramesAsMatrix");
		}
		sDebug+="Finished.";
		//TODO: in future don't set bGood according to StringToFile
		//bGood=StringToFile("X:\\anim.TranslateFrameOrder debug.txt", sDebug);
		return bGood;
	}//end TransposeFramesAsMatrix

	//GBuffer ToOneImage(int iCellW, int iCellH, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
	//bool SplitFromImage32(GBuffer &gbSrc, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
	GBuffer* Anim::ToOneImage(int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		bool bGood=true;
		string sFuncNow="ToOneImage(...)";
		lFrames=iRows*iColumns; //int iFrames=iRows*iColumns;
		lFramesCached=lFrames; //so cached anim will be accessed
		GBuffer* gbNew=null;
		try {
			//gbarrAnim=new GBuffer[lFrames];
			GotoFrame(0);
			gbNew=new GBuffer(iCellWidth*iColumns, iCellHeight*iRows, gbFrame.iBytesPP);
			//bmpLoaded=new Bitmap(iCellWidth, iCellHeight, PixelFormatNow());
			//for (int lFrameX=0; lFrameX<lFrames; lFrameX++) {
			//	gbarrAnim[lFrameX]=new GBuffer(iCellWidth, iCellHeight, 4); //assumes 32-bit
			//}
			lFrame=0; //TODO: use new var instead of class var
			int iDestByteOfCellTopLeft=irectAsMargins.top*gbNew->iStride + irectAsMargins.left*gbNew->iBytesPP;
			int iDestByteOfCellNow;
			int iDestByte;
			int iSrcByte;
			int iSrcStride=gbFrame.iStride;
			int iHeight=gbFrame.iHeight;
			int iDestStride=gbNew->iStride;
			int iCellPitchX=gbNew->iBytesPP*(iCellWidth+ipAsCellSpacing.X);
			int iCellPitchY=gbNew->iStride*(iCellHeight+ipAsCellSpacing.Y);
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
						memcpy(&gbNew->arrbyData[iDestByte],&gbFrame.arrbyData[iSrcByte], iSrcStride);
						iSrcByte+=iSrcStride;
						iDestByte+=iDestStride;
					}
					lFrameLoad++;
				}
			}
			if (!bGood) ShowError("There was data copy error while interpreting the GBuffer to a font, make sure the \"iCellWidth\" etc. variables are set correctly.",sFuncNow);
			//else bGood=GrayMapFromPixMapChannel(3);
			bGood=true;
			lFramesCached=lFrames;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, sFuncNow);
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn(sFuncNow);
		}
		return gbNew;
	}//end ToOneImage
	bool Anim::SafeDeleteAnim() {
		bool bGood=false;
		Console::Error.Write("<"+((gbarrAnim!=null)?RString_ToString("delete"):RString_ToString("null"))+" gbarrAnim");
		try {
			if (gbarrAnim!=null) {
				delete [] gbarrAnim;
				gbarrAnim=null;
			}
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "SafeDeleteAnim");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("SafeDeleteAnim");
		}
		Console::Error.Write(" finished>");
		return bGood;
	}//end SafeDeleteAnim
	bool Anim::SafeDeleteEffects() {
		bool bGood=false;
		Console::Error.Write("<"+((gbarrAnim!=null)?RString_ToString("delete"):RString_ToString("null"))+" effectarr");
		Console::Error.Flush();
		try {
			if (effectarr!=null) {
				Console::Error.Flush();
				delete [] effectarr;
				Console::Error.Flush();
				effectarr=null;
			}
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "SafeDeleteEffects");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("SafeDeleteEffects");
		}
		Console::Error.Write(" finished>");
		return bGood;
	}//end SafeDeleteEffects
	int Anim::Width() {
		int iReturn=0;
		if (gbFrame.arrbyData!=NULL) iReturn=gbFrame.iWidth;
		return iReturn;
	}
	int Anim::Height() {
		int iReturn=0;
		if (gbFrame.arrbyData!=NULL) iReturn=gbFrame.iHeight;
		return iReturn;
	}
	bool Anim::DrawToLargerWithoutCropElseCancel(GBuffer &gbDest, int xDest, int yDest, int iDrawMode) {
		bool bGood=false;
		static bool bFirstRun=true;
		if (bFirstRun) Console::Error.Write("DrawToLargerWithoutCropElseCancel...");
		try {
			if (gbFrame.arrbyData==null) {
				bGood=false;
				ShowError("Tried to draw null frame["+RString_ToString(lFrame)+"]","Anim DrawToLargerWithoutCropElseCancel");
			}
			else {
				if (bFirstRun) Console::Error.Write("calling gbFrame...");
				bGood=gbFrame.DrawToLargerWithoutCropElseCancel(gbDest, xDest, yDest, iDrawMode);
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "DrawToLargerWithoutCropElseCancel");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("DrawToLargerWithoutCropElseCancel");
		}
		bFirstRun=false;
		return bGood;
	}//end DrawToLargerWithoutCropElseCancel
	int Anim::IFrames() {
		return (int)lFrames;
	}
	//#endregion class Anim methods
}//end namespace
#endif
