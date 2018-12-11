#ifndef SPRITE_CPP
#define SPRITE_CPP

#include <RSprite_bgra32.h>
//#include "E:\Projects-cpp\Base\RSprite_bgra32.h"

using namespace std;

namespace ExpertMultimediaBase {
	Sprite::Sprite() {
		InitNull();
	}
	Sprite::~Sprite() {
	}
	void Sprite::InitNull() {
		//runtime vars (to save):
		arr2iSeq=null;
		iSeq=0; //index in 1st dimension of arr2iSeq
		iFrame=0; //index in 2nd dimension of arr2iSeq
		arriFrames=null; //size of sub-arrays of arr2iSeq
		iSeqs=1; //number of sub-arrays of arr2iSeq
		//sarrNames=null; //names of the sequences, i.e. "walk"
		//save-only (non-runtime) vars (commented purposely)
		//int** arr2iFrameRates; //(NOT 1 to 1 match with arr2iSeq) FPS of each frame,
				//followed by how many frames for which to use that speed
				//(if doesn't have a second value, keep for whole anim!)
		barrAutoAnimate=false;
		bDieIfAdvanceAfterLastFrame=false;
		//runtime-only vars:
		bActive=true;
		arr2iDelay=null; //(1 to 1 match with arr2iSeq) millisecond delays for each frame, calculated from "RLE" arr2iFrameRates
		//runtime-only vars:
		iAnim=-1; //references an anim in arranim
		//runtime vars (to save):
		sAnim=""; //references an anim in arranim
		sName="";
		bLoop=false;
	}
	void Sprite::SafeFree() {
		ExpertMultimediaBase::SafeFree(arr2iSeq,iSeqs);
		ExpertMultimediaBase::SafeFree(arriFrames);
		ExpertMultimediaBase::SafeFree(arr2iDelay,iSeqs);
	}
	//public methods:
	bool Sprite::Load(string sFileNow) {
		bool bGood=false;
		//TODO: finish this (rewrite adding array count functions etc (see *.sprite)
		Variables vsTemp;
		vsTemp.Load(sFileNow);
		SafeFree();
		InitNull();
		bool bMulti=false;
		if (vsTemp.Exists("sequences")) {
			iSeqs=vsTemp.GetForcedInt("sequences");
			bMulti=true;
		}
		if (iSeqs<1) iSeqs=1;
		try {
			arr2iSeq=(int**)malloc(sizeof(int*)*iSeqs);
			arr2iDelay=(int**)malloc(sizeof(int*)*iSeqs);
			arriFrames=(int*)malloc(sizeof(int)*iSeqs);
			int iTempFPS;
			if (vsTemp.Exists("fps")) iTempFPS=vsTemp.GetForcedInt("fps");
			else iTempFPS=30;
			if (iTempFPS<0) iTempFPS=30;
			iSeq=0;
			iFrame=0;
			arriFrames=null;
			iSeqs=1;
			barrAutoAnimate=false;
			bDieIfAdvanceAfterLastFrame=false;
			bActive=true;
			arr2iDelay=null;
			iAnim=-1;//references an Anim in animarr
			sAnim="";
			sName="";
			bLoop=false;
			bGood=true;
		}
		catch (exception& exn) { ShowExn(exn,"Sprite::Load");
		}
		catch (...) { ShowUnknownExn("Sprite::Load");
		}
		return bGood;
	}//end Sprite::Load
	bool Sprite::Save(string sFileNow) {
		Variables vsTemp;
		return vsTemp.Save(sFileNow);
	}
	bool Sprite::SetNextFrame() {
		bool bGood=false;
		//TODO: finish this--SetNextFrame()
		Console::Error.WriteLine("NOT YET IMPLEMENTED: Sprite::SetNextFrame()");
		return bGood;
	}
	bool Sprite::SetFrame(long iSetFrame) {
		bool bGood=false;
		//TODO: finish this--SetFrame(long iSetFrame)
		Console::Error.WriteLine("NOT YET IMPLEMENTED: Sprite::SetFrame(long iSetFrame)");
		return bGood;
	}
	bool Sprite::Advance(float fMilliseconds) {
		bool bGood=false;
		//TODO: finish this--Advance(float fMilliseconds)
		 //TODO: return false if fail OR if this sequence doesn't auto-animate
		Console::Error.WriteLine("NOT YET IMPLEMENTED: Sprite::Advance(float fMilliseconds)");
		return bGood;
	}
	bool Sprite::SetSeq(int iSetSeq) {
		bool bGood=false;
		//TODO: finish this--SetSeq(int iSetSeq)
		Console::Error.WriteLine("NOT YET IMPLEMENTED: Sprite::SetSeq(int iSetSeq)");
		return bGood;
	}
	bool Sprite::SetSeq(string sSetSeq) {
		bool bGood=false;
		//TODO: finish this--SetSeq(string sSetSeq)
		Console::Error.WriteLine("NOT YET IMPLEMENTED: Sprite::SetSeq(string sSetSeq)");
		return bGood;
	}
	int Sprite::Seq() {
		return iSeq;
	}
	int Sprite::Frame() {
		return iFrame;
	}
}//end namespace
#endif
