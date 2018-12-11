#ifndef ANIM_CPP
#define ANIM_CPP
#include <RAnim_bgra32.h>

using namespace std;
//NEVER DO: using namespace RMath;

//using System;
//using System.Drawing;
//using System.Drawing.Imaging;
//using System.IO;
//using System.Drawing.Text;

namespace ExpertMultimediaBase {
	int RAnim_debug_index=0;
	void SafeFree(Anim*& arranimX, string DebugNote) {
		try {
			if (arranimX!=nullptr) delete [] arranimX;
			arranimX=nullptr;
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
				long lDivisor=RMath::SafeE10L((int)(iDigitsMin-1));//returns long since implied base is 10L
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
		effectarr=nullptr;
	}
	Effect* Effect::Copy() {
		Effect* fxReturn=nullptr;
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
		frame_ptr=nullptr;
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
		if (frame_ptrs.capacity()>0) {
			for (size_t i=0; i<frame_ptrs.capacity(); i++) {
				this->frame_ptrs[i]=nullptr;
			}
		}
		frame_ptr=nullptr;

		//if (bDebug) {
		Console::Error.WriteLine("Initialized Anim #"+RString_ToString(RAnim_debug_index));
		//}
		RAnim_debug_index++;

		bLoop=false;
		//sPathFile="";
		bFileSequence=false;//if true, use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
		iSeqDigitCountMin=4;//, 0 if variable (i.exn. frame1.png...frame10.png)
		lFrame=0;
		lFrames=0;
		iEffects=0;
		iMaxEffects=0;
		effectarr=nullptr;
	}
	Anim::~Anim() {
		Console::Error.Write("~Anim...SafeDeleteAnim...");
		Console::Error.Out.Flush();
		SafeDeleteAnim();
		Console::Error.Write("SafeDeleteEffects...");
		Console::Error.Out.Flush();
		SafeDeleteEffects();
		Console::Error.WriteLine("done (~Anim).");
	}
	string Anim::sPathFileNonSeq() {
		return sFileBaseName+"."+sFileExt;
	}
	Anim* Anim::Copy() {
		Anim* animReturn=nullptr;
		try {
			animReturn=new Anim();
			targaLoaded.CopyTo(animReturn->targaLoaded);
			animReturn->rectNowF.Set(rectNowF.X,rectNowF.Y,rectNowF.Width,rectNowF.Height);
			animReturn->rectNow.Set(rectNow.X,rectNow.Y,rectNow.Width,rectNow.Height);
			animReturn->sFileBaseName=sFileBaseName;
			animReturn->sFileExt=sFileExt;
			//animReturn->sPathFile=sPathFile;
			animReturn->bFileSequence=bFileSequence;//if use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
			animReturn->iSeqDigitCountMin=iSeqDigitCountMin;//, 0 if variable (i.exn. frame1.png...frame10.png)
			animReturn->lFrame=lFrame;
			animReturn->lFrames=lFrames;
			animReturn->iEffects=iEffects;
			animReturn->iMaxEffects=iMaxEffects;
			animReturn->reserve(lFrames);
			for (long l=0; l<lFrames; l++) {
				animReturn->frame_ptrs[l]=this->frame_ptrs[l]->Copy();
				animReturn->frame_ptrs[l]->sFile="Copy of "+this->frame_ptrs[l]->sFile;
			}
			animReturn->frame_ptr=animReturn->frame_ptrs[lFrame];
			if (iEffects>0) {
				animReturn->effectarr=new Effect[iMaxEffects];
				for (int i=0; i<iEffects; i++) {
					effectarr[i].CopyTo(animReturn->effectarr[i]);
				}
			}
			else animReturn->effectarr=nullptr;
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
			animReturn.sFileBaseName=sFileBaseName;
			animReturn.sFileExt=sFileExt;
			//animReturn.sPathFile=sPathFile;
			animReturn.bFileSequence=bFileSequence;//if use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
			animReturn.iSeqDigitCountMin=iSeqDigitCountMin;//, 0 if variable (i.exn. frame1.png...frame10.png)
			animReturn.lFrames=lFrames;
			animReturn.iEffects=iEffects;
			animReturn.iMaxEffects=iMaxEffects;
			animReturn.reserve(lFrames);
			for (long l=0; l<lFrames; l++) {
				animReturn.frame_ptrs[l]=this->frame_ptrs[l]->Copy();
				animReturn.frame_ptrs[l]->sFile="Copy of "+this->frame_ptrs[l]->sFile;
			}
			animReturn.GotoFrame(lFrame); //animReturn.lFrame=lFrame; animReturn.frame_ptr=animReturn.frame_ptrs[lFrame];
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
		Console::Error.Write("CopyAsGrayTo...");
		Console::Error.Out.Flush();
		try {
			Console::Error.Write("copying targa");
			Console::Error.Out.Flush();
			bGood=targaLoaded.CopyTo(animReturn.targaLoaded);
			Console::Error.Write(bGood?("OK ("+animReturn.targaLoaded.Description(true)+")..."):"FAIL...");
			Console::Error.Out.Flush();
			animReturn.rectNowF.Set(rectNowF.X,rectNowF.Y,rectNowF.Width,rectNowF.Height);
			animReturn.rectNow.Set(rectNow.X,rectNow.Y,rectNow.Width,rectNow.Height);
			animReturn.sFileBaseName=sFileBaseName;
			animReturn.sFileExt=sFileExt;
			//animReturn.sPathFile=sPathFile;
			animReturn.bFileSequence=bFileSequence;//if use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
			animReturn.iSeqDigitCountMin=iSeqDigitCountMin;//, 0 if variable (i.exn. frame1.png...frame10.png)
			animReturn.lFrames=lFrames;
			animReturn.iEffects=iEffects;
			animReturn.iMaxEffects=iMaxEffects;
			Console::Error.Write("writing ");
			Console::Error.Flush();
			animReturn.reserve(lFrames);
			Console::Error.Write(RString_ToString(lFrames)+" frames:");
			Console::Error.Flush();
			if ((iChannelOffset<0)||(iChannelOffset>=this->frame_ptrs[0]->iBytesPP)) {
				if (iChannelOffset>=this->frame_ptrs[0]->iBytesPP) {
					Console::Error.Write("Source channel is out of range {iChannelOffset:"+RString_ToString(iChannelOffset)+"; thisframe_ptrs[0]->iBytesPP:"+RString_ToString(this->frame_ptrs[0]->iBytesPP)+"} so defaulting to average color for value!  ");
					Console::Error.Flush();
				}
				for (long index=0; index<lFrames; index++) {
					animReturn.frame_ptrs[index]=new GBuffer();
					if (!MaskFromValue(animReturn.frame_ptrs[index], this->frame_ptrs[index])) {
						bGood=false;
						ShowErr("<failed>");
						break;
					}
					//else Console::Error.Write(".");
				}
			}
			else {
				for (long index=0; index<lFrames; index++) {
					animReturn.frame_ptrs[index]=new GBuffer();
					if (!MaskFromChannel(animReturn.frame_ptrs[index], frame_ptrs[index], iChannelOffset)) {
						bGood=false;
						ShowErr("<failed>");
					}
					//else Console::Error.Write(".");
				}
			}
			Console::Error.Write(".");
			animReturn.GotoFrame(this->lFrame); //animReturn.lFrame=lFrame; animReturn.frame_ptr=animReturn.frame_ptrs[lFrame];
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
		Anim* animReturn=nullptr;
		Console::Error.WriteLine("CopyAsGray...");
		try {
			animReturn=new Anim();
			targaLoaded.CopyTo(animReturn->targaLoaded);
			animReturn->rectNowF.Set(rectNowF.X,rectNowF.Y,rectNowF.Width,rectNowF.Height);
			animReturn->rectNow.Set(rectNow.X,rectNow.Y,rectNow.Width,rectNow.Height);
			animReturn->sFileBaseName=sFileBaseName;
			animReturn->sFileExt=sFileExt;
			//animReturn->sPathFile=sPathFile;
			animReturn->bFileSequence=bFileSequence;//if use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
			animReturn->iSeqDigitCountMin=iSeqDigitCountMin;//, 0 if variable (i.exn. frame1.png...frame10.png)
			animReturn->lFrame=lFrame;
			animReturn->lFrames=lFrames;
			animReturn->iEffects=iEffects;
			animReturn->iMaxEffects=iMaxEffects;
			animReturn->reserve(lFrames);
			Console::Error.WriteLine("getting masks from "+RString_ToString(lFrames)+" frame(s)...");
			if (iChannelOffset<0) {
				for (long index=0; index<lFrames; index++) {
					animReturn->frame_ptrs[index]=new GBuffer();
					MaskFromValue(animReturn->frame_ptrs[index], this->frame_ptrs[index]);
				}
			}
			else {
				for (long index=0; index<lFrames; index++) {
					animReturn->frame_ptrs[index]=new GBuffer();
					MaskFromChannel(animReturn->frame_ptrs[index], this->frame_ptrs[index], iChannelOffset);
				}
			}
			animReturn->frame_ptr=animReturn->frame_ptrs[lFrame];
			Console::Error.WriteLine("copying effects...");
			if (iEffects>0) {
				animReturn->effectarr=new Effect[iMaxEffects];
				for (int index=0; index<iEffects; index++) {
					effectarr[index].CopyTo(animReturn->effectarr[index]);
				}
			}
			Console::Error.WriteLine("OK (CopyAsGray)");
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
					+"; lFrame:"+RString_ToString(lFrame)
					+"; sFileExt:"+sFileExt
					//+"; sPathFile:"+sPathFile
					+"; sFileBaseName:"+sFileBaseName
					+"; bFileSequence:"+RString_ToString(bFileSequence)
					+"; iEffects:"+RString_ToString(iEffects)
					+"; iMaxEffects:"+RString_ToString(iMaxEffects)
					+"; iSeqDigitCountMin:"+RString_ToString(iSeqDigitCountMin);
				if (frame_ptr!=nullptr) {
					if (this->frame_ptr->iBytesTotal>0) {
						try {
							sReturn+="; this->frame_ptr->arrbyData:"+RString_ToString((this->frame_ptr->arrbyData!=nullptr)?"non-null":"null");
							sReturn+="; this->frame_ptr->iBytesTotal:"+RString_ToString(this->frame_ptr->iBytesTotal)
								+"; this->frame_ptr->iWidth:"+RString_ToString(this->frame_ptr->iWidth)
								+"; this->frame_ptr->iHeight:"+RString_ToString(this->frame_ptr->iHeight)
								+"; this->frame_ptr->iWidth:"+RString_ToString(this->frame_ptr->iWidth)
								+"; this->frame_ptr->iBytesPP:"+RString_ToString(this->frame_ptr->iBytesPP)
								+"; this->frame_ptr->iStride:"+RString_ToString(this->frame_ptr->iStride);
						}
						catch (exception& exn) {
							ShowExn(exn,"Anim Dump("+RString_ToString(bDumpVars)+") while accessing this->frame_ptr");
						}
						catch (...) {
							ShowUnknownExn("Anim Dump("+RString_ToString(bDumpVars)+") while accessing this->frame_ptr");
						}
					}
				}
				else sReturn="frame_ptr:null";
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
			if (frame_ptr==nullptr) {
				bGood=false;
				ShowError("frame_ptr is null","ResetBitmapUsingFrameNow");
			}
			else if (frame_ptr->arrbyData==nullptr) {
				bGood=false;
				ShowError("frame_ptr->arrbyData is null","ResetBitmapUsingFrameNow");
			}
			if (bGood) {
				if ( (targaLoaded.BytesBuffer()==0)
				   || (frame_ptr->iStride!=targaLoaded.Stride())
				   || (frame_ptr->iWidth!=rectNow.Width)
				   || (frame_ptr->iHeight!=rectNow.Height)
				   || (frame_ptr->iBytesPP!=targaLoaded.Stride()/rectNow.Width)
				   || (frame_ptr->iBytesTotal!=(targaLoaded.Stride()*rectNow.Height)) ) {
					targaLoaded.From(frame_ptr->iWidth, frame_ptr->iHeight, frame_ptr->iBytesPP, frame_ptr->arrbyData, false); //true would copy pointer instead of data
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
			if (frame_ptr!=nullptr) {
				if (frame_ptr->arrbyData!=nullptr) {
					targaLoaded.ToRect(rectNowF);
					rectNow.Set((int)rectNowF.X, (int)rectNowF.Y, (int)rectNowF.Width, (int)rectNowF.Height);
					bool bReCompress=targaLoaded.IsCompressed();
					targaLoaded.SetCompressionRLE(false);
					byte* lpbyNow = targaLoaded.GetBufferPointer();
					for (int iBy=0; iBy<this->frame_ptr->iBytesTotal; iBy++) {
						*lpbyNow=this->frame_ptr->arrbyData[iBy];
						lpbyNow++;
					}
					if (bReCompress) targaLoaded.SetCompressionRLE(true);
				}
				else Console::Error.WriteLine("CopyFrameToInternalBitmap Error: frame_ptr->arrbyData is nullptr");
			}
			else Console::Error.WriteLine("CopyFrameToInternalBitmap Error: frame_ptr is nullptr");
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
		bool bFoundAny=false;
		long iFirstFrame=0;
		long iHighestFirstFrameForWhichToSearch=2;
		string sMsg="";
		bool bGood=true;
		try {
			sFileBaseName=sSetFileBaseName;
			sFileExt=sSetFileExt;
			while (true) {
				if (  File::Exists( ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName,sSetFileExt,iFirstFrame,iSeqDigitCountNow) )  ) {
						bFoundAny=true;
						break;
				}
				if (iFirstFrame>=iHighestFirstFrameForWhichToSearch) break;
				iFirstFrame++;
			}
			if (bFoundAny) {
				if (bFirstRun) Console::Error.Write("first frame is "+RString_ToString(iFirstFrame)+"...");
				iNow=iFirstFrame;
				while (  File::Exists( ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName,sSetFileExt,iNow,iSeqDigitCountNow) )  ) {
					if (bFirstRun) sMsg+=("["+RString_ToString(iNow)+"]");
					iNow++;
				}
			}
			else {
				if (bFirstRun) Console::Error.Write("no frames...");
				iFirstFrame=-1;
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
		int iReturn=bFoundAny?(iNow-iFirstFrame):(0);
		if (bFirstRun) {
			Console::Error.Write(RString_ToString(iReturn)+"...");
			Console::Error.Flush();
		}
		bFirstRun=false;
		return iReturn;
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
		//long iFrameRel=0;
		string sFrameFileNow="";
		if (bGood) {
			if (bFirstRun) Console::Error.Write("buffer array...");
			lFrames=(long)iFrameCountToUseAndModify;
			iFrameCountToUseAndModify=0;
			if (bFirstRun) Console::Error.Write("("+RString_ToString(lFrames)+") frames...");
			long iAbsFrame=0;
			long iFirstFrame=iStartFrame;
			long iHighestFirstFrameForWhichToSearch=2+iStartFrame;
			bool bFoundAny=false;
			while (true) {
				if (  File::Exists( ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName,sSetFileExt,iFirstFrame,iSeqDigitCountNow) )  ) {
						bFoundAny=true;
						break;
				}
				if (iFirstFrame>=iHighestFirstFrameForWhichToSearch) break;
				iFirstFrame++;
			}
			if (bFoundAny) {
				iAbsFrame=iFirstFrame;
			}

			this->reserve(lFrames);
			for (long iNow=0; iNow<lFrames; iNow++) {
				if (bFirstRun) Console::Error.Write(" ["+RString_ToString(iNow)+"]as("+RString_ToString(iNow+iStartFrame)+")");
				sFrameFileNow=ExpertMultimediaBase::PathFileOfSeqFrame(sSetFileBaseName, sSetFileExt, (long)iAbsFrame, iSeqDigitCountNow);
				frame_ptrs[iNow]=new GBuffer();
				if (frame_ptrs[iNow]->Load(sFrameFileNow)) iFrameCountToUseAndModify++;
				else {
					bGood=false;
					ShowError("Couldn't load \""+sFrameFileNow+"\"","Anim LoadSeq");
					break;
				}
				iAbsFrame++;
			}
			if (lFrames<1) {
				ShowError("FrameArray:empty! ","Anim LoadSeq");
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
			if (targaLoaded.arrbyData==nullptr) {
				bGood=false;
				ShowError("targa is not loaded!","Anim CopyFrameFromInternalBitmap");
			}
			if (this->frame_ptr==nullptr) {
				bGood=false;
				ShowError("frame is not loaded!","Anim CopyFrameFromInternalBitmap");
			}
			if (bGood) {

				targaLoaded.ToRect(rectNowF);
				rectNow.Set((int)rectNowF.X, (int)rectNowF.Y,
									(int)rectNowF.Width, (int)rectNowF.Height);
				if (  (this->frame_ptr->arrbyData==nullptr)
				   || (this->frame_ptr->iStride!=targaLoaded.Stride())
				   || (this->frame_ptr->iWidth!=rectNow.Width)
				   || (this->frame_ptr->iHeight!=rectNow.Height)
				   || (this->frame_ptr->iBytesPP!=targaLoaded.Stride()/rectNow.Width)
				   || (this->frame_ptr->iBytesTotal!=(targaLoaded.Stride()*rectNow.Height)) ) {
					this->frame_ptr->Init(rectNow.Width,rectNow.Height,this->frame_ptr->iBytesPP,true);
				}
				byte* lpbyNow=targaLoaded.GetBufferPointer();
				for (int iBy=0; iBy<this->frame_ptr->iBytesTotal; iBy++) {
					this->frame_ptr->arrbyData[iBy]=*lpbyNow;
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
			if (lFrames>0) {//frame_ptrs.capacity()>0) {
				if (lFrameX<lFrames && lFrameX>=0) {
					frame_ptr=frame_ptrs[lFrameX];
					lFrame=lFrameX;
					if (frame_ptr==nullptr) {
						bGood=false;
						ShowError("Null frame!","GotoFrame("+RString_ToString(lFrameX)+") {sFileBaseName:"+sFileBaseName+"}");
					}
					else if (frame_ptr->arrbyData==nullptr) {
						bGood=false;
						ShowError("Null data in frame!","GotoFrame("+RString_ToString(lFrameX)+") {sFileBaseName:"+sFileBaseName+"}");
					}
					else if (this->Width()==0) {
						bGood=false;
						ShowError("Bad frame data ("+RString_ToString(this->Width())+"x"+RString_ToString(this->Height())+"x"+RString_ToString(this->get_BytesPP()*8)+")!","GotoFrame("+RString_ToString(lFrameX)+") {sFileBaseName:"+sFileBaseName+"}");
					}
					else if (bFirstRun) {
						Console::Error.Write("GotoFrame {sFileBaseName:"+sFileBaseName+"} dimensions:"+RString_ToString(this->Width())+"x"+RString_ToString(this->Height())+"x"+RString_ToString(this->get_BytesPP()*8)+"... ");
					}
				}
				else {
					bGood=false;
					ShowError("Frame "+RString_ToString(lFrameX)+" is out of range! {sFileBaseName:"+sFileBaseName+"}");
				}
			}
			else {
				ShowError("empty frame array!","GotoFrame {sFileBaseName:"+sFileBaseName+"}");
				bGood=false;
			}
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
	bool Anim::DrawFrameOverlay(GBuffer* dest_image_ptr, IPoint &ipDest, long lFrameX) {
		bool bGood=GotoFrame(lFrameX);
		if (bGood) {
			if (!DrawFrameOverlay(dest_image_ptr, ipDest)) bGood=false;
		}
		return bGood;
	}
	bool Anim::DrawFrameOverlay(GBuffer* dest_image_ptr, IPoint &ipDest) {
		return OverlayNoClipToBigCopyAlpha(dest_image_ptr, ipDest, this->frame_ptr);
	}
	int Anim::MinDigitsRequired(int iNumber) {
		string sNumber=RString_ToString(iNumber);
		return sNumber.length();
	}
	bool Anim::isLastFrame() {
		return (this->frame_ptr!=nullptr && this->frame_ptr->arrbyData!=nullptr && lFrame==(lFrames-1));
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
			gbTemp.sFile="(SplitFromImage32 Temp) "+sFile;
			if (gbTemp.iBytesTotal==0) {
				ShowError("Failed to write font file to GBuffer","SplitFromImage32");
				bGood=false;
			}
			else {
				//targaLoaded=gbTemp.targaLoaded;//SplitFromImage32 remakes targaLoaded
				bGood=SplitFromImage32(&gbTemp, iCellWidth, iCellHeight, iRows, iColumns, ipAsCellSpacing, irectAsMargins);
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
	///<summary>
	///Splits cells from a single image containing multiple frames of the same animation
	///(does copy name, appending frame number--which results in unusable name that is only for debugging)
	///</summary>
	bool Anim::SplitFromImage32(GBuffer* gbSrc, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		bool bGood=true;
		string sFuncNow="SplitFromImage32(gb,...,spacing)";
		Console::Error.Write("SplitFromImage32...");
		Console::Error.Out.Flush();
		lFrames=(long)iRows*(long)iColumns;
		try {
			SafeDeleteAnim();
			if (lFrames<=0) {
				ShowError("Frame count is "+RString_ToString(lFrames)+"!",sFuncNow);
				bGood=false;
			}
			if (gbSrc==nullptr) {
				ShowError("gbSrc is nullptr!",sFuncNow);
				bGood=false;
			}
			if (bGood) {
				this->reserve(lFrames);
				targaLoaded.Init(iCellWidth, iCellHeight, gbSrc->iBytesPP, true);
				for (int lFrameX=0; lFrameX<lFrames; lFrameX++) {
					if (bMegaDebug){
						Console::Error.Write("creating frame "+RString_ToString(lFrameX)+"...");
						Console::Error.Flush();
					}
					frame_ptrs[lFrameX]=new GBuffer(iCellWidth, iCellHeight, gbSrc->iBytesPP);//frame_ptrs[lFrameX]->Init(iCellWidth, iCellHeight, gbSrc->iBytesPP);
				}
				Console::Error.Write("preparing dimensions...");
				Console::Error.Flush();
				//lFrame=0; //not used
				int iSrcByteOfCellTopLeft=irectAsMargins.top*gbSrc->iStride + irectAsMargins.left*gbSrc->iBytesPP;
				int iSrcByteOfCellNow;
				//int iSrcAdder=gbSrc->iStride-gbSrc->iBytesPP*iCellWidth;
				//int iSrcNextCellAdder=
				//int iSrcStride=iColumns*iWidth*4; //assumes 32-bit source
				int iSrcByte;
				int iDestByte;
				//int iCellNow=0;
				//int iCellStride=iWidth*iBytesPP;
				//int yStrideAdder=iSrcStride*(iHeight-1);
				//int iSrcAdder=iSrcStride-iWidth*iBytesPP;
				int iDestStride=frame_ptrs[0]->iStride;
				int iHeight=frame_ptrs[0]->iHeight;
				int iSrcStride=gbSrc->iStride;
				int iCellPitchX=gbSrc->iBytesPP*(iCellWidth+ipAsCellSpacing.X);
				int iCellPitchY=gbSrc->iStride*(iCellHeight+ipAsCellSpacing.Y);
				long lFrameLoad=0;
				for (int yCell=0; yCell<iRows; yCell++) {
					//Console::Error.WriteLine("[cell row]");
					for (int xCell=0; xCell<iColumns; xCell++) {
						//Console::Error.Write("."); //RString_ToString(lFrameLoad)+frame_ptr->Description();
						iDestByte=0;
						iSrcByteOfCellNow=iSrcByteOfCellTopLeft + yCell*iCellPitchY + xCell*iCellPitchX;
						iSrcByte=iSrcByteOfCellNow;
						if (GotoFrame(lFrameLoad)) {
							for (int iLine=0; iLine<iHeight; iLine++) {
								//TODO: finish this--make it safer:
								if (this->frame_ptr==nullptr) {bGood=false; ShowErr("this->frame_ptr is nullptr; gb source: "+gbSrc->Description(true)+"!  ");}
								else if (this->frame_ptr->arrbyData==nullptr) {bGood=false; ShowErr("this->frame_ptr->arrbyData is nullptr; gb source: "+gbSrc->Description(true)+"!  ");}
								else if (iDestStride<=0) {bGood=false; ShowErr("DestStride was "+RString_ToString(iDestStride)+"!  ");}
								else if (iDestByte<0||iDestByte>=this->get_byte_count()) {bGood=false; ShowErr("dest loc "+RString_ToString(iDestByte)+" out of range!  ");}
								else if (iSrcByte<0||iSrcByte>=gbSrc->iBytesTotal) {bGood=false; ShowErr("source loc "+RString_ToString(iSrcByte)+" out of range!  ");}
								else memcpy(&this->frame_ptr->arrbyData[iDestByte],&gbSrc->arrbyData[iSrcByte],iDestStride);
								GBuffer* this_frame_ptr=GetFramePtr(lFrameLoad);
								if (this_frame_ptr!=nullptr) {
									//this->frame_ptr->sFile=gbSrc->sFile+"<"+RString_ToString(lFrameLoad)+">";
									this_frame_ptr->sFile=gbSrc->sFile+"<"+RString_ToString(lFrameLoad)+">";
								}
								else Console::Error.WriteLine("SplitFromImage32 Error: Failed to load frame "+RString_ToString(lFrameLoad)+" from "+gbSrc->sFile);
								//if (Byter.CopyFast(this->frame_ptr->arrbyData, gbSrc->arrbyData, iDestByte, iSrcByte, iDestStride)==false)
								//	bGood=false;
								iDestByte+=iDestStride;
								iSrcByte+=iSrcStride;
							}
						}
						else Console::Error.WriteLine("SplitFromImage32 failed to GotoFrame "+RString_ToString(lFrameLoad));
						//Console::Error.Write(this->frame_ptr->Description()+" ");
						lFrameLoad++;
					}
				}
				if (!bGood) ShowError("There was data copy error while interpreting the gbuffer to a font, make sure the iCellWidth etc. variables are set correctly.",sFuncNow);
				// else bGood=GrayMapFromPixMapChannel(3);
				else Console::Error.WriteLine("done (SplitFromImage32).");
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
		std::vector<GBuffer*> new_frame_ptrs;
		new_frame_ptrs.reserve(lFrames);
		for (size_t i=0; i<lFrames; i++) {
			new_frame_ptrs[i]=nullptr;
		}
		string sDebug="starting TransposeFramesAsMatrix()\n";
		//StringToFile("X:\\anim.TranslateFrameOrder debug.txt", sDebug);
		try {
			int iFrames=(int)lFrames;
			for (int this_frame_index=0; this_frame_index<iFrames; this_frame_index++) {
				int iNew=(int)(this_frame_index/iResultCols)+(int)(this_frame_index%iResultCols)*iResultRows; //switched (must be)
				sDebug+="old:"+RString_ToString(this_frame_index)+"; new:"+RString_ToString(iNew)+"\n";
				new_frame_ptrs[this_frame_index]=frame_ptrs[iNew];
			}
			for (int this_frame_index=0; this_frame_index<iFrames; this_frame_index++) {
				frame_ptrs[this_frame_index]=new_frame_ptrs[this_frame_index];
			}
			frame_ptr=frame_ptrs[lFrame];
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
	//bool SplitFromImage32(GBuffer* gbSrc, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
	GBuffer* Anim::ToOneImage(int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		bool bGood=true;
		string sFuncNow="ToOneImage(...)";
		lFrames=iRows*iColumns; //int iFrames=iRows*iColumns;
		GBuffer* gbNew=nullptr;
		try {
			GotoFrame(0);
			gbNew=new GBuffer(iCellWidth*iColumns, iCellHeight*iRows, this->get_BytesPP());
			gbNew->sFile="<unknown ToOneImage result>";

			lFrame=0; //TODO: use new var instead of class var
			int iDestByteOfCellTopLeft=irectAsMargins.top*gbNew->iStride + irectAsMargins.left*gbNew->iBytesPP;
			int iDestByteOfCellNow;
			int iDestByte;
			int iSrcByte;
			int iSrcStride=this->get_stride();
			int iHeight=this->Height();
			int iDestStride=gbNew->iStride;
			int iCellPitchX=gbNew->iBytesPP*(iCellWidth+ipAsCellSpacing.X);
			int iCellPitchY=gbNew->iStride*(iCellHeight+ipAsCellSpacing.Y);
			long lFrameLoad=0;
			for (int yCell=0; yCell<iRows; yCell++) {
				for (int xCell=0; xCell<iColumns; xCell++) {
					iSrcByte=0;
					iDestByteOfCellNow=iDestByteOfCellTopLeft + yCell*iCellPitchY + xCell*iCellPitchX;
					iDestByte=iDestByteOfCellNow;
					if (GotoFrame(lFrameLoad)) {
						gbNew->sFile="Mosaic of "+frame_ptrs[lFrameLoad]->sFile;
						//TODO: finish this: check whether next line is needed
						//frame_ptr->Save("debugToOneImage"+SequenceDigits(lFrameLoad)+".png",ImageFormat.Png);
						for (int iLine=0; iLine<iHeight; iLine++) {
							//TODO: finish this: make it safer:
							memcpy(&gbNew->arrbyData[iDestByte], &frame_ptr->arrbyData[iSrcByte], iSrcStride);
							iSrcByte+=iSrcStride;
							iDestByte+=iDestStride;
						}
					}
					else Console::Error.WriteLine("ToOneImage Failed to GotoFrame "+RString_ToString(lFrameLoad));
					lFrameLoad++;
				}
			}
			if (!bGood) ShowError("There was data copy error while interpreting the GBuffer to a font, make sure the \"iCellWidth\" etc. variables are set correctly.",sFuncNow);
			//else bGood=GrayMapFromPixMapChannel(3);
			bGood=true;
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
		Console::Error.Write("<"+((frame_ptrs.capacity()>0)?RString_ToString("delete"):RString_ToString("0"))+" Anim frame(s)"); //says Anim frame(s) finished if ok
		Console::Error.Out.Flush();
		try {
			for (int i=0; i<(int)frame_ptrs.capacity(); i++) {
				if (frame_ptrs[i]!=nullptr) {
					delete frame_ptrs[i];
					frame_ptrs[i]=nullptr;
					Console::Error.Write(".");
					Console::Error.Out.Flush();
				}
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
		Console::Error.Write("<"+((effectarr!=nullptr)?RString_ToString("delete"):RString_ToString("null"))+" effectarr");
		Console::Error.Flush();
		try {
			if (effectarr!=nullptr) {
				Console::Error.Flush();
				delete [] effectarr;
				Console::Error.Flush();
				effectarr=nullptr;
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
		if (frame_ptr!=nullptr) iReturn=frame_ptr->iWidth;
		return iReturn;
	}
	int Anim::Height() {
		int iReturn=0;
		if (frame_ptr!=nullptr) iReturn=frame_ptr->iHeight;
		return iReturn;
	}
	int Anim::get_BytesPP() {
		int iReturn=0;
		if (frame_ptr!=nullptr) iReturn=frame_ptr->iBytesPP;
		return iReturn;
	}
	int Anim::get_stride() {
		int iReturn=0;
		if (frame_ptr!=nullptr) iReturn=frame_ptr->iStride;
		return iReturn;
	}
	int Anim::get_byte_count() {
		int iReturn=0;
		if (frame_ptr!=nullptr) iReturn=frame_ptr->iBytesTotal;
		return iReturn;
	}
	int Anim::get_frame_number() {
		return (int)lFrame;
	}
	bool Anim::DrawToLargerWithoutCropElseCancel(GBuffer* gbDest, int xDest, int yDest, int iDrawMode) {
		bool bGood=false;
		static bool bFirstRun=true;
		if (bFirstRun) Console::Error.Write("DrawToLargerWithoutCropElseCancel...");
		try {
			if (frame_ptr==nullptr) {
				bGood=false;
				ShowError("Tried to draw null frame["+RString_ToString(lFrame)+"]","Anim DrawToLargerWithoutCropElseCancel");
			}
			else if (frame_ptr->arrbyData==nullptr) {
				bGood=false;
				ShowError("Tried to draw frame with null buffer ["+RString_ToString(lFrame)+"]","Anim DrawToLargerWithoutCropElseCancel");
			}
			else {
				if (bFirstRun) Console::Error.Write("calling frame_ptr DrawToLargerWithoutCropElseCancel...");
				bGood=frame_ptr->DrawToLargerWithoutCropElseCancel(gbDest, xDest, yDest, iDrawMode);
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
	void Anim::reserve(size_t this_frame_count) {
		size_t old_size=frame_ptrs.capacity();
		for (size_t i=0; i<old_size; i++) {
			if (this->frame_ptrs[i]!=nullptr) {
				Console::Error.Write("Anim::reserve is freeing a used frame...");
				Console::Error.Out.Flush();
				delete this->frame_ptrs[i];
				Console::Error.Write("OK ");
			}
			this->frame_ptrs[i]=nullptr;
		}
		this->frame_ptrs.reserve(this_frame_count);
		for (size_t i=old_size; i<this_frame_count; i++) {
			this->frame_ptrs[i]=nullptr;
		}
	}
	GBuffer* Anim::GetFramePtr(int this_frame_number) {
		GBuffer* result=nullptr;
		if (this_frame_number>=0 && this_frame_number<IFrames()) {
			result = frame_ptrs[this_frame_number];
		}
		return result;
	}
	//#endregion class Anim methods
}//end namespace
#endif
