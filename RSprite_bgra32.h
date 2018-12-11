#ifndef SPRITE_H
#define SPRITE_H

#include <RAnim_bgra32.h>
//#include "E:\Projects-cpp\Base\RAnim_bgra32.h"

using namespace std;

namespace ExpertMultimediaBase {

	class Sprite {
	private:
		//runtime vars (to save):
		int** arr2iSeq;
		int iSeq; //index in 1st dimension of arr2iSeq
		int iFrame; //index in 2nd dimension of arr2iSeq
		int* arriFrames; //size of sub-arrays of arr2iSeq
		int iSeqs; //number of sub-arrays of arr2iSeq
		//string* sarrNames; //names of the sequences, i.e. "walk"
		//save-only (non-runtime) vars (commented purposely)
		//int** arr2iFrameRates; //(NOT 1 to 1 match with arr2iSeq) FPS of each frame,
				//followed by how many frames for which to use that speed
				//(if doesn't have a second value, keep for whole anim!)
		bool* barrAutoAnimate;
		bool bDieIfAdvanceAfterLastFrame;
		//runtime-only vars:
		bool bActive;
		int** arr2iDelay; //(1 to 1 match with arr2iSeq) millisecond delays for each frame, calculated from "RLE" arr2iFrameRates

		void InitNull();
		void SafeFree();
	public:
		//runtime-only vars:
		int iAnim; //references an anim in arranim
		//runtime vars (to save):
		string sAnim; //references an anim in arranim
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
