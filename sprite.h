#ifndef SPRITE_H
#define SPRITE_H

#include <anim32bgra.h>
//#include "C:\My Documents\Projects-cpp\Base\anim32bgra.h"

using namespace std;

namespace ExpertMultimediaBase {

class Sprite {
private:
	//runtime vars (to save):
	int** i2dSeq;
	int iSeq; //index in 1st dimension of i2dSeq
	int iFrame; //index in 2nd dimension of i2dSeq
	int* iarrFrames; //size of sub-arrays of i2dSeq
	int iSeqs; //number of sub-arrays of i2dSeq
	//string* sarrNames; //names of the sequences, i.e. "walk"
	//save-only (non-runtime) vars (commented purposely)
	//int** i2dFrameRates; //(NOT 1 to 1 match with i2dSeq) FPS of each frame,
			//followed by how many frames for which to use that speed
			//(if doesn't have a second value, keep for whole anim!)
	bool* barrAutoAnimate;
	bool bDieIfAdvanceAfterLastFrame;
	//runtime-only vars:
	bool bActive;
	int** i2dDelay; //(1 to 1 match with i2dSeq) millisecond delays for each frame, calculated from "RLE" i2dFrameRates

	void InitNull();
	void SafeFree();
public:
	//runtime-only vars:
	int iAnim; //references an anim in animarr
	//runtime vars (to save):
	string sAnim; //references an anim in animarr
	string sName;
	bool bLoop;
	
	Sprite();
	~Sprite();
	bool Load(string sFileNow);
	bool Save(string sFileNow);
	bool SetNextFrame();
	bool SetFrame(long iSetFrame);
	bool Advance(float fMilliseconds); //returns false if fail OR if this sequence doesn't auto-animate
	bool SetSeq(int iSetSeq);
	bool SetSeq(string sSetSeq);
	int Seq();
	int Frame();
};

}//end namespace
#endif
