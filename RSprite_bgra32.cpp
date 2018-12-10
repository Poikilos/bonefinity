#ifndef SPRITE_CPP
#define SPRITE_CPP

#include <sprite.h>
//#include "E:\Projects-cpp\Base\sprite.h"

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
		bActive;
		arr2iDelay=null; //(1 to 1 match with arr2iSeq) millisecond delays for each frame, calculated from "RLE" arr2iFrameRates
		//runtime-only vars:
		iAnim; //references an anim in arranim
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
			bActive;
			arr2iDelay=null;
			iAnim;
			sAnim="";
			sName="";
			bLoop=false;
		}
		catch (exception& exn) { ShowExn(exn,"Sprite::Load");
		}
		catch (...) { ShowUnknownExn("Sprite::Load");
		}
	}//end Sprite::Load
	bool Sprite::Save(string sFileNow) {
		Variables vsTemp;
		vsTemp.Save(sFileNow);
	}
	bool Sprite::SetNextFrame() {
	}
	bool Sprite::SetFrame(long iSetFrame) {
	}
	bool Sprite::Advance(float fMilliseconds) {
		 //TODO: return false if fail OR if this sequence doesn't auto-animate
	}
	bool Sprite::SetSeq(int iSetSeq) {
	}
	bool Sprite::SetSeq(string sSetSeq) {
	}
	int Sprite::Seq() {
		return iSeq;
	}
	int Sprite::Frame() {
		return iFrame;
	}

}//end namespace
#endif
