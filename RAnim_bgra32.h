#ifndef ANIM_H
#define ANIM_H
#include <RImage_bgra32.h>
//#include "E:\Projects-cpp\Base\RImage_bgra32.h"

using namespace std;
//using System.Drawing.Text;

namespace ExpertMultimediaBase {
	string PathFileOfSeqFrame(string sSetFileBaseName, string sSetFileExt, long lFrameTarget, int iDigitsMin);
	const int ResourceTypeNone = 0;
	const int ResourceTypeAnim = 1;
	const int ResourceTypeClip = 2;
	const int EffectNone=0;
	const int EffectSkew=1;

	class Effect {
	public:
		//Vars varsFX; //re-implement this
		UInt32 bitsAttrib;
		int iEffect;
		//TODO: Remember to prevent circular references in the following ints!!
		int iDest;
		int iDestResourceType;
		int iOverlay;
		int iOverlayResourceType;
		int iMask;
		int iMaskResourceType;
		long lStartFrame;
		long lFrames;
		string sScript;
		Effect* Copy();
		bool CopyTo(Effect& fxReturn);
		Effect();
		int IFrames();
	private:
		void Init();
	};

	class Anim {
	private:
		Targa targaLoaded;
		RectangleF rectNowF;
		Rectangle rectNow;
		string sFileBaseName;
		string sFileExt;
		string sPathFileNonSeq();
		//int iMaxEffects;
		//PixelFormat pixelformatNow=PixelFormat.Format32bppArgb;
		GBuffer* gbarrAnim;
	public:
		GBuffer gbFrame;
		bool bLoop;
		long lFramesCached;
		//private string sPathFile="";
		bool bFileSequence;//if true, use sFileBaseName+digits+"."+sExt, and sPathFile is first frame
		int iSeqDigitCountMin;//, 0 if variable (i.exn. frame1.png...frame10.png)
		long lFrame;
		long lFrames;
		int iEffects;
		int iMaxEffects;
		Effect* effectarr;
		Anim();
		void InitNull();
		~Anim();
		Anim* Copy();
		Anim* CopyAsGray();
		Anim* CopyAsGray(int iChannelOffset);
		bool CopyTo(Anim& animOut);
		bool CopyAsGrayTo(Anim& animOut);
		bool CopyAsGrayTo(Anim& animOut, int iChannelOffset);
		bool SaveSeq(string sSetFileBaseName, string sSetFileExt);
		string Dump(bool bDumpVars);
		string Dump();
		bool SaveCurrentSeqFrame();
		bool SaveSeqFrame(long lFrameSave);
		bool SaveSeqFrame(string sSetFileBaseName, string sSetFileExt, long lFrameSave);
		//ImageFormat ImageFormatFromExt();
		//ImageFormat ImageFormatFromExt(string sSetFileExt);
		bool ResetBitmapUsingFrameNow();
		//PixelFormat PixelFormatNow();
		bool CopyFrameToInternalBitmap();
		bool LoadInternalBitmap(string sFile);
		int CountSeqFrames(string sSetFileBaseName, string sSetFileExt);
		int CountSeqFrames(string sSetFileBaseName, string sSetFileExt, int iSeqDigitCountNow);
		bool LoadSeq(string sSetFileNameWithAsterisk);
		bool LoadSeq(string sSetFileBaseName, string sSetFileExt);
		bool LoadSeq(string sSetFileBaseName, string sSetFileExt, int &iFrameCountToUseAndModify, int iSeqDigitCountNow);
		bool LoadSeq(string sSetFileBaseName, string sSetFileExt, int &iFrameCountToUseAndModify, int iSeqDigitCountNow, int iStartFrame);
		bool CopyFrameFromInternalBitmap();
		bool SaveInternalBitmap(string sFileName);
		bool GotoFrame(long lFrameX);
		bool GotoFrame(int iFrameX);
		bool GotoNextFrame();
		bool GotoNextFrame(bool bLoop);
		bool DrawFrameOverlay(GBuffer &gbDest, IPoint &ipDest, long lFrame);
		bool DrawFrameOverlay(GBuffer &gbDest, IPoint &ipDest);
		int MinDigitsRequired(int iNumber);
		bool LastFrame();
		string PathFileOfSeqFrame(long lFrameTarget);
		bool SplitFromImage32(string sFile, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins);
		bool SplitFromImage32(string sFileImage, int iCellWidth, int iCellHeight, int iRows, int iColumns);
		bool SplitFromImage32(GBuffer &gbSrc, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins);
		bool TransposeFramesAsMatrix(int iResultRows, int iResultCols);
		//GBuffer ToOneImage(int iCellW, int iCellH, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		//bool SplitFromImage32(GBuffer &gbSrc, int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins) {
		GBuffer* ToOneImage(int iCellWidth, int iCellHeight, int iRows, int iColumns, IPoint ipAsCellSpacing, IRect irectAsMargins);
		bool SafeDeleteAnim();
		bool SafeDeleteEffects();
		int Width();
		int Height();
		bool DrawToLargerWithoutCropElseCancel(GBuffer &gbDest, int xDest, int yDest, int iDrawMode);
		int IFrames();
	};//end class Anim;

	class Clip {
	public:
		int iParent; //index of parent Anim in arranim
		long lFrameStart;
		long lFrames;
		long lFrame;
		Effect* effectarr; //remember to also process arranim[iParent].effectarr[]
		int iEffects;
		Clip();
	private:
		int iMaxEffects;
	};

	void SafeFree(Anim*& arranimX);

}
#endif
