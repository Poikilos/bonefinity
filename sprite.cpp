#ifndef SPRITE_CPP
#define SPRITE_CPP

#include <sprite.h>

using namespace std;

namespace ExpertMultimediaBase {
	Sprite::Sprite() {
		InitNull();
	}
	Sprite::~Sprite() {
	}
	void Sprite::InitNull() {
		//runtime vars (to save):
		i2dSeq=null;
		iSeq=0; //index in 1st dimension of i2dSeq
		iFrame=0; //index in 2nd dimension of i2dSeq
		iarrFrames=null; //size of sub-arrays of i2dSeq
		iSeqs=1; //number of sub-arrays of i2dSeq
		//sarrNames=null; //names of the sequences, i.e. "walk"
		//save-only (non-runtime) vars (commented purposely)
		//int** i2dFrameRates; //(NOT 1 to 1 match with i2dSeq) FPS of each frame,
				//followed by how many frames for which to use that speed
				//(if doesn't have a second value, keep for whole anim!)
		barrAutoAnimate=false;
		bDieIfAdvanceAfterLastFrame=false;
		//runtime-only vars:
		bActive;
		i2dDelay=null; //(1 to 1 match with i2dSeq) millisecond delays for each frame, calculated from "RLE" i2dFrameRates
		//runtime-only vars:
		iAnim; //references an anim in animarr
		//runtime vars (to save):
		sAnim=""; //references an anim in animarr
		sName="";
		bLoop=false;
	}
	void Sprite::SafeFree() {
		ExpertMultimediaBase::SafeFree(i2dSeq,iSeqs);
		ExpertMultimediaBase::SafeFree(iarrFrames);
		ExpertMultimediaBase::SafeFree(i2dDelay,iSeqs);
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
			i2dSeq=(int**)malloc(sizeof(int*)*iSeqs);
			i2dDelay=(int**)malloc(sizeof(int*)*iSeqs);
			iarrFrames=(int*)malloc(sizeof(int)*iSeqs);
			int iTempFPS;
			if (vsTemp.Exists("fps")) iTempFPS=vsTemp.GetForcedInt("fps");
			else iTempFPS=30;
			if (iTempFPS<0) iTempFPS=30;
			iSeq=0;
			iFrame=0;
			iarrFrames=null;
			iSeqs=1;
			barrAutoAnimate=false;
			bDieIfAdvanceAfterLastFrame=false;
			bActive;
			i2dDelay=null;
			iAnim;
			sAnim="";
			sName="";
			bLoop=false;
		}
		catch (exception& exn) { ShowException(exn,"Sprite::Load");
		}
		catch (...) { ShowUnknownException("Sprite::Load");
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
