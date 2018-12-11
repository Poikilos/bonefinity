#ifndef GFONT_CPP
#define GFONT_CPP
/*
 * Created by SharpDevelop.
 * User: Jake Gustafson
 * Date: 4/21/2005
 * Time: 1:00 PMS
 *
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */

#include <RFont_bgra32.h>
using namespace std;

namespace ExpertMultimediaBase {
	const int GFont_GlyphTypePlain=0;
	const int GFont_GlyphTypeItalic=1;
	const int GFont_GlyphTypeBold=2;
	const int GFont_GlyphTypeBoldItalic=3;
	SizeF GFont_sizeTemp;
	// bool GFont_bFirstRun=true;  // #include <../../dxman-crossplatform/dxman.h> //GFont_bFirstRun etc


	bool GFont::SaveSeq(string sFileBaseName, string sFileExt, int iGlyphType) {
		bool bGood=false;
		try {
			bGood=arranimGlyphType[iGlyphType].SaveSeq(sFileBaseName, sFileExt);
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"GFont SaveSeq","saving font to image sequence");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("GFont SaveSeq");
		}
		if (!bGood) {
			ShowError("Error saving font image sequence.","GFont SaveSeq");
			//try {
			//	ShowErr("Failed to save "+sFileExt+" files named starting with "+sFileBaseName+" "+arranimGlyphType[iGlyphType].ToString(true),"SaveSeq");
			//}
			//catch (exception& exn2) {
			//	ShowErr("Exception accessing font glyphs, and failed to save "+sFileExt+" files named starting with "+sFileBaseName+"--"+e2.ToString(),"SaveSeq");
			//}
		}
		return bGood;
	}
	bool GFont::TypeFast(GBuffer* gbDest, IPoint& ipDest, bool MoveThePoint, string sText) {
		return TypeFast(gbDest, ipDest, MoveThePoint, sText, GFont_GlyphTypePlain, DrawModeCopyAlpha, false, GFont_sizeTemp);
	}
	bool GFont::TypeFast(GBuffer* gbDest, IPoint& ipDest, bool MoveThePoint, string sText, int iGlyphType, int iDrawMode) {
		return TypeFast(gbDest, ipDest, MoveThePoint, sText, iGlyphType, iDrawMode, false, GFont_sizeTemp);
	}
	///<summary>
	///GFont::TypeFast
	///</summary>
	bool GFont::TypeFast(GBuffer* gbDest, IPoint& ipDest, bool MoveThePoint, string sText, int iGlyphType, int iDrawMode, bool bTestOnlyNoDraw, SizeF& sizeReturn) {
		bool bGood=true;
		static bool bFirstRun=true;
		static IPoint ipDestNow;
		marginLeftX=ipDest.X;
		sizeReturn.Width=0;//TODO: not compatible with nested text areas???
		sizeReturn.Height=0;//TODO: not compatible with nested text areas???
		sizeReturn.Width+=ipDest.X;//re-adjusted after the try block
		sizeReturn.Height+=ipDest.Y;//re-adjusted after the try block
		try {
			if (bFirstRun) { //(GFont_bFirstRun) {
				Console::Error.Write("GFont::TypeFast ");
				Console::Error.Flush();

			}
			ipDestNow.X=ipDest.X;
			ipDestNow.Y=ipDest.Y;
			if (bFirstRun) {
				Console::Error.Write("{iGlyphType:"+RString_ToString(iGlyphType)+"}");
				Console::Error.Flush();
			}
			const char* szText=sText.c_str();//debug hard-coded limitation to utf-8
			if (iGlyphType>=GFont_iGlyphTypes) {
				ShowErr("Glyph type index "+RString_ToString(iGlyphType)+" is exceeds "+RString_ToString(GFont_iGlyphTypes)+" limit","TypeFast");
				iGlyphType=0;
			}
			if (bFirstRun) {
				Console::Error.Write("{iGlyphType:"+RString_ToString(iGlyphType)+"}");
				Console::Error.Flush();
			}
			bool character_bFirstRun=true;
			char cPrev='\0';
			bool bSpacingChar;
			if (bFirstRun) {
				Console::Error.Write("Drawing string:\"");
				Console::Error.Flush();
			}
			for (size_t i=0; i<sText.length(); i++) {
				if (bFirstRun&&character_bFirstRun) {
					Console::Error.Write(sText.substr(i,1));
					Console::Error.Flush();
				}
				if (bFirstRun&&character_bFirstRun) {
					Console::Error.Write("{szText["+RString_ToString((int)i)+"]:"+RString_ToString((int)szText[i])+"} GotoFrame...");
					Console::Error.Flush();
				}
				bool IsFrameGood=arranimGlyphType[iGlyphType].GotoFrame((long)szText[i]);
				if (bFirstRun&&character_bFirstRun) {
					Console::Error.Write("{GotoFrame IsFrameGood:"+RString_ToString(IsFrameGood)+"}...");
					Console::Error.Flush();
				}

				if ( (szText[i]=='\r') || (szText[i]=='\n') ) {
					if (  ( (szText[i]=='\r') && ((i+1>=sText.length())||(szText[i+1]!='\n')) )
					  ||  ( (szText[i]=='\n') )  ) { //&& (cPrev!='\r')
						ipDestNow.X=marginLeftX;//ipDest.X;
						ipDestNow.Y+=arranimGlyphType[iGlyphType].Height();
						//if (ipDestNow.Y+arranimGlyphType[iGlyphType].Height()>sizeReturn.Height) sizeReturn.Height=ipDestNow.Y+arranimGlyphType[iGlyphType].Height();//+width to go to other side of it
					}
					// else does not move the cursor at all since is part of a 2-byte endline sequence
					bSpacingChar=true;
				}
				else if (szText[i]=='\t') {
					bSpacingChar=true;
					ipDestNow.X+=arranimGlyphType[iGlyphType].Width()*4; //TODO: remove *4 if proportional font that has wide tab character
					if (ipDestNow.X+arranimGlyphType[iGlyphType].Width()>sizeReturn.Width) sizeReturn.Width=ipDestNow.X+arranimGlyphType[iGlyphType].Width();//+width to go to other side of it
				}
				else if (szText[i]==' ') {
					bSpacingChar=true;
					ipDestNow.X+=arranimGlyphType[iGlyphType].Width();
					if (ipDestNow.X+arranimGlyphType[iGlyphType].Width()>sizeReturn.Width) sizeReturn.Width=ipDestNow.X+arranimGlyphType[iGlyphType].Width();//+width to go to other side of it
				}
				else {
					bSpacingChar=false;
				}

				if (!bSpacingChar) {
					if (!bTestOnlyNoDraw) {
						if ( (ipDestNow.X+arranimGlyphType[iGlyphType].Width()<=gbDest->iWidth)
									&& (ipDestNow.Y+arranimGlyphType[iGlyphType].Height()<=gbDest->iHeight)
									&& (ipDestNow.X>=0)
									&& (ipDestNow.Y>=0)
										) {
							if (arranimGlyphType[iGlyphType].get_BytesPP()==1) {
								if (bFirstRun&&character_bFirstRun) cerr << "TypeFast ["<<arranimGlyphType[iGlyphType].get_BytesPP()<<"-BytesPP to "<< gbDest->iBytesPP<<"-BytesPP] is calling OverlayNoClipToBigCopyAlpha..." << flush;
								if (!OverlayNoClipToBigCopyAlpha(gbDest, ipDestNow, arranimGlyphType[iGlyphType].frame_ptr, gradNow, -1)) {
									//OverlayNoClipToBigCopyAlpha(gbDest, ipAt,	  gbSrc, 											gradNow, iSrcChannel);
									bGood=false;
									//ShowErr("failed to OverlayNoClipToBigCopyAlpha text character#"+RString_ToString((long)szText[i]),"TypeFast");
								}
							}
							else {
								if (bFirstRun&&character_bFirstRun) cerr << "TypeFast ["<<arranimGlyphType[iGlyphType].get_BytesPP()<<"-BytesPP to "<< gbDest->iBytesPP<<"-BytesPP] is calling DrawToLargerWithoutCropElseCancel..." << flush;
								if (IsFrameGood) {
									if (!arranimGlyphType[iGlyphType].frame_ptr->DrawToLargerWithoutCropElseCancel(gbDest,ipDestNow.X, ipDestNow.Y,iDrawMode)) {
									//if (!ExpertMultimediaBase::OverlayNoClipToBig(gbDest, ipDestNow, arranimGlyphType[iGlyphType]->frame_ptr, gradNow, -1)) {
										bGood=false;
										//ShowErr("failed to DrawToLargerWithoutCropElseCancel text character#"+RString_ToString((long)szText[i]),"TypeFast");
									}
								}
							}
						}
					}
					ipDestNow.X+=arranimGlyphType[iGlyphType].Width();
					if (ipDestNow.X+arranimGlyphType[iGlyphType].Width()>sizeReturn.Width) sizeReturn.Width=ipDestNow.X+arranimGlyphType[iGlyphType].Width();//+width to go to other side of it
				}
				if (bFirstRun&&character_bFirstRun) {
					Console::Error.Write("move to next character...");
					Console::Error.Flush();
				}
				if (ipDestNow.X+arranimGlyphType[iGlyphType].Width()>=gbDest->iWidth) {
					//if (bFirstRun) ShowErr("Tried to write text beyond screen");
					//break;
				}
				if (bFirstRun&&character_bFirstRun) {
					Console::Error.Write("done first character.");
					Console::Error.Flush();
				}
				character_bFirstRun=false;
				cPrev=szText[i];
				if (ipDestNow.Y+arranimGlyphType[iGlyphType].Height()>sizeReturn.Height) sizeReturn.Height=ipDestNow.Y+arranimGlyphType[iGlyphType].Height();//+width to go to other side of it
			}//end for character i
			if (ExpertMultimediaBase::bMegaDebug) {
				Console::Error.WriteLine("\"...OK");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"TypeFast("+sText+RString_ToString(")"));
			if (ExpertMultimediaBase::bMegaDebug) {
				Console::Error.WriteLine("\"...");
			}
		}
		catch (...) {
			bGood=false;
			if (ExpertMultimediaBase::bMegaDebug) {
				Console::Error.WriteLine("\"...");
			}
			ShowUnknownExn("GFont TypeFast("+sText+RString_ToString(")"));
		}
		if (MoveThePoint) {
			ipDest.X=marginLeftX;
			//ipDest.X=ipDestNow.X;
			ipDest.Y=ipDestNow.Y;
		}
		sizeReturn.Width-=ipDestNow.X;
		sizeReturn.Height-=ipDestNow.Y;
		bFirstRun=false;
		return bGood;
	}//end TypeFast
	bool GFont::TypeFast_NewLine(GFont* ContinueFrom_ElseNull, IPoint& ipDest) {
		if (ContinueFrom_ElseNull!=nullptr) {
			marginLeftX=ContinueFrom_ElseNull->marginLeftX;
		}
		ipDest.X=marginLeftX;
		ipDest.Y+=arranimGlyphType[GFont_GlyphTypePlain].Height();
	}//end TypeFast_NewLine
	///<summary>
	///Measures text resulting width & height (may be zero or unpredictable if fails (where return is false)
	///</summary>
	bool GFont::MeasureTextByRef_UsingTypeFast(SizeF& sizeReturn, GBuffer* gbDest, IPoint& ipDest, string sText, int iGlyphType) {
		bool bGood=true;
		static bool MoveThePoint=false;
		try {
			TypeFast(gbDest, ipDest, MoveThePoint, sText, iGlyphType, DrawModeBlendAlpha, true, sizeReturn);
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"GFont::MeasureTextByRef");
		}
		return bGood;
	}//end MeasureTextByRef
	bool GFont::TypeHTML(GBuffer* gbDest, IPoint& ipAt, IRect& irectReturn, string sText,  bool bVisible) {
		//int iNextTag;
		//int iLength;
		bool bGood=true;
		try {
			//TODO: finish TypeHTML

		}
		catch (exception& exn) {
			ShowExn(exn,"TypeHTML("+sText+RString_ToString(")"));
			bGood=false;
		}
		catch (...) {
			ShowUnknownExn("TypeHTML("+sText+RString_ToString(")"));
		}
		return bGood;
	}
	bool GFont::TypeHTML(GBuffer* gbDest, IPoint& ipAt, IRect& irectReturn, string sText) {
		return TypeHTML(gbDest, ipAt, irectReturn, sText, true);
	}
	GFont::GFont() {
		InitNull();
	}
	void GFont::InitNull() {
	}
	bool GFont::Init() {
		bool bGood=true;
		try {
			//arranimGlyphType=new[GFont.iGlyphTypes] Anim;
			if (!ResetGlyphTypeArray()) bGood=false;
			Console::Error.WriteLine("Init GFont...done");
		}
		catch (exception& exn) {
			ShowExn(exn,"Init");
			bGood=false;
		}
		return bGood;
	}
	bool GFont::ResetGlyphTypeArray() {
		bool bGood=false;
		try {
			//arranimGlyphType=new Anim[iGlyphTypes];
			bGood=true;
		}
		catch (exception& exn) {
			ShowExn(exn,"ResetGlyphTypeArray","initializing GFont GlyphType array");
			bGood=false;
		}
		catch (...) {
			ShowUnknownExn("ResetGlyphTypeArray","initializing GFont GlyphType array");
		}
		return bGood;
	}
	bool GFont::FromImageValue(string sFile, int iCharWidth, int iCharHeight, int iRows, int iColumns) {
		bool bGood=false;
		Anim* panimTemp=nullptr;
		try {
			panimTemp=new Anim();
			//panimTemp=new Anim();
			bGood=Init();
			if (bGood) {
				Console::Error.Write("Splitting font image...");
				bGood=panimTemp->SplitFromImage32(sFile, iCharWidth, iCharHeight, iRows, iColumns);
				Console::Error.Write(bGood?"Done.  CopyAsGrayTo...":"Splitting Failed!  ");
				// panimTemp->SaveSeq("0.debug-glyph", "png");
				// RawOverlayNoClipToBig(RetroEngine.main_screen_ptr, ipAt, panimTemp->frame_ptr->arrbyData, iCharWidth, iCharHeight, 4);
				if (bGood) {
					bGood=panimTemp->CopyAsGrayTo(arranimGlyphType[GFont_GlyphTypePlain]);
					Console::Error.Write(bGood?"CopyAsGrayTo Succeeded.  ":"CopyAsGrayTo Failed!  ");
					int frame_count=arranimGlyphType[GFont_GlyphTypePlain].IFrames();
					for (int frame_i=0; frame_i<frame_count; frame_i++) {
						GBuffer* this_frame_ptr=arranimGlyphType[GFont_GlyphTypePlain].GetFramePtr(frame_i);
						if (this_frame_ptr!=nullptr) {
							this_frame_ptr->sFile=sFile+"<"+RString_ToString(frame_i)+">";
						}
					}
					Console::Error.WriteLine("Finished naming frame images.");
					//arranimGlyphType[GFont_GlyphTypePlain].CopyTo(arranimGlyphType[GFont_GlyphTypeBold]);
					//arranimGlyphType[GFont_GlyphTypePlain].CopyTo(arranimGlyphType[GFont_GlyphTypeItalic]);
					//arranimGlyphType[GFont_GlyphTypePlain].CopyTo(arranimGlyphType[GFont_GlyphTypeBoldItalic]);
					//TODO: finish modifying the Glyph Types now
				}
				//ShowAsciiTable();
				//sLogLine=".arranimGlyphType[GFont_GlyphTypePlain].ToString(true) "+arranimGlyphType[GFont_GlyphTypePlain].ToString(true);
			}
			else ShowErr("Failed to initialize","FromImageValue");
		}
		catch (exception& exn) {
			ShowExn(exn,"FromImageValue");
		}
		catch (...) {
			ShowUnknownExn("FromImageValue");
		}
		try {
			if (panimTemp!=nullptr) {
				Console::Error.Write("deconstructing panimTemp...");
				delete panimTemp;
				Console::Error.Write("done deconstructing panimTemp.");
			}
			else Console::Error.Write("panimTemp was already null.");
			Console::Error.Flush();
		}
		catch (exception& exn) {
			ShowExn(exn,"GFont FromImageValue","deconstructing panimTemp");
		}
		catch (...) {
			ShowUnknownExn("GFont FromImageValue","deconstructing panimTemp");
		}
		return bGood;
	}//end FromImageValue
	bool GFont::FromImageAsPredefinedColorFont(string sFile, int iCharWidth, int iCharHeight, int iRows, int iColumns) {
		bool bGood=false;
		Anim* panimTemp=nullptr;
		try {
			panimTemp=new Anim();
			//panimTemp=new Anim();
			bGood=Init();
			if (bGood) {
				Console::Error.Write("Splitting font image...");
				bGood=panimTemp->SplitFromImage32(sFile, iCharWidth, iCharHeight, iRows, iColumns);
				Console::Error.Write(bGood?"Done.  CopyAsGrayTo...":"Splitting Failed!  ");
				// panimTemp->SaveSeq("0.debug-glyph", "png");
				// RawOverlayNoClipToBig(RetroEngine.main_screen_ptr, ipAt, panimTemp->frame_ptr->arrbyData, iCharWidth, iCharHeight, 4);
				if (bGood) {
					bGood=panimTemp->CopyTo(arranimGlyphType[GFont_GlyphTypePlain]);
					Console::Error.Write(bGood?"CopyAsGrayTo Succeeded.  ":"CopyAsGrayTo Failed!  ");
					//arranimGlyphType[GFont_GlyphTypePlain].CopyTo(arranimGlyphType[GFont_GlyphTypeBold]);
					//arranimGlyphType[GFont_GlyphTypePlain].CopyTo(arranimGlyphType[GFont_GlyphTypeItalic]);
					//arranimGlyphType[GFont_GlyphTypePlain].CopyTo(arranimGlyphType[GFont_GlyphTypeBoldItalic]);
					//TODO: finish modifying the Glyph Types now
				}
				//ShowAsciiTable();
				//sLogLine=".arranimGlyphType[GFont_GlyphTypePlain].ToString(true) "+arranimGlyphType[GFont_GlyphTypePlain].ToString(true);
			}
			else ShowErr("Failed to initialize","FromImageAsPredefinedColorFont");
		}
		catch (exception& exn) {
			ShowExn(exn,"FromImageAsPredefinedColorFont");
		}
		catch (...) {
			ShowUnknownExn("FromImageAsPredefinedColorFont");
		}
		try {
			if (panimTemp!=nullptr) {
				Console::Error.Write("deconstructing panimTemp...");
				delete panimTemp;
				Console::Error.Write("done deconstructing panimTemp.");
			}
			else Console::Error.Write("panimTemp was already nullptr.");
			Console::Error.Flush();
		}
		catch (exception& exn) {
			ShowExn(exn,"GFont FromImageAsPredefinedColorFont","deconstructing panimTemp");
		}
		catch (...) {
			ShowUnknownExn("GFont FromImageAsPredefinedColorFont","deconstructing panimTemp");
		}
		return bGood;
	}//end FromImageAsPredefinedColorFont
	void GFont::ShowAsciiTable(GBuffer* gbDest, int xAt, int yAt) {
		try {
			IPoint ipAt;//=new IPoint(xAt,yAt);
			ipAt.X=xAt;
			ipAt.Y=yAt;
			int lChar=0;

			for (int yChar=0; yChar<16; yChar++) {
				ipAt.X=xAt;
				for (int xChar=0; xChar<16; xChar++) {
					if (arranimGlyphType[GFont_GlyphTypePlain].GotoFrame(lChar)) {
						RawOverlayNoClipToBig(gbDest, ipAt,
							arranimGlyphType[GFont_GlyphTypePlain].frame_ptr->arrbyData,
							arranimGlyphType[GFont_GlyphTypePlain].Width(),
							arranimGlyphType[GFont_GlyphTypePlain].Height(), 1);
					}
					else Console::Error.WriteLine("Failed to go to glyph "+RString_ToString(lChar));
					ipAt.X+=arranimGlyphType[GFont_GlyphTypePlain].Width();
					lChar++;
				}
				ipAt.Y+=arranimGlyphType[GFont_GlyphTypePlain].Height();
			}
		}
		catch (exception& exn) {
			ShowExn(exn,"ShowAsciiTable");
		}
		catch (...) {
			ShowUnknownExn("ShowAsciiTable");
		}
	}
	void GFont::SetColor(byte r, byte g, byte b) {
		gradNow.SetColors(r,g,b,255,r,g,b,0);
	}
	///<summary>
	///Draws a GBuffer to another with some really fast effects
	///[bOutline was formerly u32Stat]
	///</summary>
	bool IsFirstExplosion=true;
	bool GBuffer_FX_Scaled(GBuffer* gbDest, GBuffer* gbSrc, int x2D, int y2D,
			float fOpacity, float fExplodedness, bool bOutline, float fScale, string DebugNote, GFont* debug_gfont) {
		bool bGood=true;
		if (fExplodedness>=1.0f) {
			if (IsFirstExplosion) {
				Console::Error.Write("GBuffer_FX_Scaled first 1.0 explosion...");
				Console::Error.Out.Flush();
			}
		}
		IPoint ipAt;
		ipAt.X=x2D;
		ipAt.Y=y2D;
		bool RememberTextCursorPosition=false;
		if (gbDest!=nullptr) {
			if (debug_gfont!=nullptr) {
				static SizeF debug_size;
				static bool IsFontHeightKnown = debug_gfont->MeasureTextByRef_UsingTypeFast(debug_size, gbDest, ipAt, "A", GFont_GlyphTypePlain);
				ipAt.Y -= int(debug_size.Height+.5f);
				if (ipAt.X<0) ipAt.X=0;
				else if (ipAt.X+100>gbDest->iWidth) ipAt.X=gbDest->iWidth-100;
				if (ipAt.Y<0) ipAt.Y=0;
				else if (ipAt.Y+20>gbDest->iHeight) ipAt.Y=gbDest->iHeight-20;

				debug_gfont->TypeFast(gbDest,ipAt,RememberTextCursorPosition, DebugNote+"...");
			}
			//ipAt.Y+=(int)debugtextcursor_LastTextBlockSize.Height;

			static bool bFirstDrawErr=true;
			static bool bFirstDrawExn=true;
			byte* arrbyDest=nullptr;
			bool bShowVars=false; //dump variables to standard output
			float fInverseScale;
			UInt32* lpu32Dest;
			UInt32* lpu32Src;
			int iStride;
			int iDestW, iDestH;
			UInt32* lpu32DestNow;

			int iSrcPixels;
			int iDestPixels;
			byte* lpbyDest;
			byte* lpbySrc;
			float bSrc,gSrc,rSrc,aSrc, bDest,gDest,rDest, bAtScaledExplodedDest,gAtScaledExplodedDest,rAtScaledExplodedDest, bCooked,gCooked,rCooked;
			//int xExploder, yExploder;
			int xSrcNow, ySrcNow;
			bool bExplode;
			int iDestScaled;
			//int iDestScaledExploded;
			float fExplodeExp;
			//float fExplodeLog;
			//float fRemains;
			//float fRemainsExp;
			//float fRemainsLog;
			//exp example: .75=0.5625
			//log example: .75 =
			int iSrcW, iSrcH;
			int iScaledW;
			int iScaledH;
			int iScaledRight;
			int iScaledBottom;
			int iSrcStride;
			//int yMaxExploder, xMaxExploder;//, xOffScaledOnly,yOffScaledOnly,
			//int iHalfWidth, iHalfHeight;//explosion vars
			float fX, fY;
			float fDestW, fDestH;
			float xfFlat, yfFlat;
			float fSrcW, fSrcH;
			float fSrcStride;
			int xDestRel,yDestRel;
			int yDestNow;
			int xDestNow=-1;//this is set later
			int iSrcNow;
			float fAlphaness;
			float rExploded, gExploded, bExploded;
			int iDestStart;
			int iDestBytes;
			int destTotalPixelCount=gbDest->iWidth*gbDest->iHeight;

			try {
				if (gbDest->arrbyData==nullptr) {
					if (bFirstDrawErr) {
						Console::Error.WriteLine("Null data in dest image in GBuffer_FX_Scaled ("+DebugNote+")");
						bShowVars=true;
						bFirstDrawErr=false;
					}
					bGood=false;
				}
				else if (gbSrc==nullptr) {
					if (bFirstDrawErr) {
						Console::Error.WriteLine("Null source image in GBuffer_FX_Scaled ("+DebugNote+")");
						bShowVars=true;
						bFirstDrawErr=false;
					}
					bGood=false;
				}
				else if (gbSrc->arrbyData==nullptr) {
					if (bFirstDrawErr) {
						Console::Error.WriteLine("Null data in source image in GBuffer_FX_Scaled ("+DebugNote+")");
						bShowVars=true;
						bFirstDrawErr=false;
					}
					bGood=false;
				}
				else { //else OK
					fX=0; fY=0;
					iDestW=gbDest->iWidth;
					iDestH=gbDest->iHeight;
					fDestW=(float)iDestW;
					fDestH=(float)iDestH;
					xfFlat=(float)x2D;
					yfFlat=(float)y2D;
					fSrcW=(float)gbSrc->iWidth;
					fSrcH=(float)gbSrc->iHeight;
					iSrcW=(int)gbSrc->iWidth;
					iSrcH=(int)gbSrc->iHeight;
					RMath::ResetRand();//makes explosion uniform (?)
					if (fScale<.05f) fScale=.05;
					else if (fScale>10.0f) fScale=10000.0f;//debug only (sometimes a huge shot appears in camera, but that may be due to actual trajectory
					fInverseScale=1.0f/fScale;
					if (fOpacity>1.0f) fOpacity=1.0f;
					lpu32Dest=(UInt32*)gbDest->arrbyData;
					lpu32Src=(UInt32*)gbSrc->arrbyData;
					iStride=gbDest->iStride;
					iDestBytes=iStride*(int)gbDest->iHeight;
					iDestStart=x2D + (y2D*iDestW);
				}// end else good

				if (bGood && ((int)((xfFlat+fSrcW*fScale)+.5f)>0) && ((int)((yfFlat+fSrcH*fScale)+.5f)>0) && x2D<iDestW && y2D<iDestH) {
				//if (bGood) {
					lpu32DestNow=&lpu32Dest[iDestStart];
					iSrcPixels=gbSrc->iWidth*gbSrc->iHeight;
					iDestPixels=gbDest->iWidth*gbDest->iHeight;
					lpbyDest=(byte*)lpu32DestNow;
					lpbySrc=nullptr;//=image.buffer;
					bExplode=(fExplodedness>0.0f)?true:false;
					iDestScaled=0; //iDestScaledExploded=0;

					if (bExplode) fExplodedness-=.25f; //start explosion just as a red dude, not blown up yet at frame 1
					if (fExplodedness>1.0f) fExplodedness=1.0f;
					else if (fExplodedness<0.0f) fExplodedness=0.0f;

					fExplodeExp=fExplodedness*fExplodedness;
					//fExplodeLog=(fExplodedness>0.0f)?sqrt(fExplodedness):0.0f;
					//fRemains=(1.0f-fExplodedness);
					//fRemainsExp=fRemains*fRemains;
					//fRemainsLog=1.0f-fExplodeLog;
					//exp example: .75=0.5625
					//log example: .75 =
					iScaledW=(int)((float)gbSrc->iWidth*fScale);
					iScaledH=(int)((float)gbSrc->iHeight*fScale);
					iScaledRight=x2D+iScaledW;
					iScaledBottom=y2D+iScaledH;
					iSrcStride=gbSrc->iWidth*4;
					//iHalfWidth=iScaledW/2; iHalfHeight=iScaledH/2;//explosion vars
					fY=0.0f, fX=0.0f;
					fSrcStride=(float)iSrcStride;
					xDestRel=0; yDestRel=0;

					for (yDestNow=y2D; yDestNow<iScaledBottom; yDestNow++, fY+=1.0f, yDestRel++) {
						fX=0.0f;
						xDestRel=0;
						if (yDestNow>=iDestH) break;
						else if (yDestNow>=0) {
							for (int xDestNow=x2D; xDestNow<iScaledRight; xDestNow++, fX+=1.0f, xDestRel++) {
								//iDestScaledExploded=0;
								ySrcNow=(int)(fY*fInverseScale);
								xSrcNow=(int)(fX*fInverseScale);
								if (xSrcNow<0) break;
								if (xSrcNow>=gbSrc->iWidth) break;
								if (ySrcNow<0) break;
								if (ySrcNow>=gbSrc->iHeight) break;
								iSrcNow=ySrcNow*iSrcW + xSrcNow;
								if (xDestNow>=iDestW) break;
								else if (xDestNow>=0) {
									lpbySrc=(byte*)&lpu32Src[iSrcNow];
									bSrc=*lpbySrc;
									gSrc=lpbySrc[1];
									rSrc=lpbySrc[2];
									aSrc=lpbySrc[3];
									aSrc*=fOpacity; //changes alpha according to param
									if (fOpacity<1.0f) { //debug this use of opacity is unique to DXMan; makes object red when transparent
										gSrc/=2.0f;
										bSrc/=4.0f;
									}
									if (bExplode) {
										if (fExplodedness<=.5f) {
											rSrc=255.0f;//APPROACH(255.0f,255.0f,1.0f-fExplodedness*2.0f);
											gSrc=APPROACH(255.0f,128.0f,fExplodedness/.5f);
											bSrc=APPROACH(255.0f,0.0f,fExplodedness/.5f);
										}
										else {
											rSrc=APPROACH(255.0f,0.0f,(fExplodedness-.5f)/.5f);
											gSrc=APPROACH(128.0f,0.0f,(fExplodedness-.5f)/.5f);
											bSrc=APPROACH(0.0f,0.0f,(fExplodedness-.5f)/.5f);
										}
									}

									iDestScaled=xDestNow + ((yDestNow)*iDestW);
									//if (bExplode) {
										//uses -= for implosion
										//if (xDestRel<iHalfWidth) xExploder-=(int)(RMath::FRand(-xMaxExploder,0)*fExplodeLog);
										//else xExploder-=(int)(RMath::FRand(0,xMaxExploder)*fExplodeLog);
										//if (yDestRel<iHalfHeight) yExploder=(int)(RMath::FRand(-yMaxExploder,0)*fExplodeLog);
										//else yExploder=(int)(RMath::FRand(0,yMaxExploder)*fExplodeLog);
										//aSrc*=fRemainsLog;
										//iDestScaledExploded=xDestNow+xExploder + ((yDestNow+yExploder)*iDestW);
									//}
									//else {
										//iDestScaledExploded=iDestScaled;
									//}

									if ( (xDestNow>=iDestW) || (yDestNow>=iDestH) ) break;
									else if ( (xDestNow>=0) && (yDestNow>=0) ) {
										if ((bOutline)&&lpbySrc[3]<255.0f) {//if ((bOutline)&&aSrc<192.0f) {//debug: blue outline is only for DXMan
											bSrc=192.0f;
											gSrc=64.0f;
										}
										if (aSrc<15) {//transparent (draw nothing)
										}
										else if (aSrc>241) {//solid
											if (bExplode) {
												//also paint second pixel (image-shaped) for explosion
												rExploded=APPROACH(255.0f,rDest,fExplodeExp);//rExploded=APPROACH(rDest,APPROACH(255.0f,rSrc,fExplodeExp),fAlphaness);
												gExploded=APPROACH(255.0f,gDest,fExplodeExp);//gExploded=APPROACH(gDest,APPROACH(255.0f,gSrc,fExplodeExp),fAlphaness);
												bExploded=APPROACH(255.0f,bDest,fExplodeExp);//bExploded=APPROACH(bDest,APPROACH(200.0f,bSrc,fExplodeExp),fAlphaness);
												arrbyDest=(byte*)&lpu32Dest[iDestScaled];
												*arrbyDest=(byte)bExploded;
												arrbyDest[1]=(byte)gExploded;
												arrbyDest[2]=(byte)rExploded;
											}// end if bExplode
											else {//lpu32Dest[iDestScaledExploded]=_RGB32BIT(by255,rSrc,gSrc,bSrc);
												//NOTE: iDestScaledExploded was set to iDestScaled if not bExploded
												if (iDestScaled>=destTotalPixelCount) break;
												else if (iDestScaled>0) {
													arrbyDest=(byte*)&lpu32Dest[iDestScaled];
													*arrbyDest=(byte)bSrc;
													arrbyDest[1]=(byte)gSrc;
													arrbyDest[2]=(byte)rSrc;
													//arrbyDest[3]=(byte)aSrc;
												}
											}//else non-exploded solid pixel
										}//end if solid
										else {//blend alpha
											///NOTE: boundary checking was already done in outer case (both dest coords and exploded dest coords)
											fAlphaness=(float)aSrc/255.0f;
											lpbyDest=(byte*)&lpu32Dest[iDestScaled];//first get original location's color
											bDest=*lpbyDest;
											gDest=lpbyDest[1];
											rDest=lpbyDest[2];
											lpbyDest=(byte*)&lpu32Dest[iDestScaled];//now get exploded location's color
											bAtScaledExplodedDest=*lpbyDest;
											gAtScaledExplodedDest=lpbyDest[1];
											rAtScaledExplodedDest=lpbyDest[2];
											bCooked=APPROACH(bAtScaledExplodedDest,bSrc,fAlphaness);//by3dAlphaLookup[blue][*lpbyDest][bySrcA];
											gCooked=APPROACH(gAtScaledExplodedDest,gSrc,fAlphaness);//by3dAlphaLookup[green][*lpbyDest][bySrcA];
											rCooked=APPROACH(rAtScaledExplodedDest,rSrc,fAlphaness);//by3dAlphaLookup[red][*lpbyDest][bySrcA];
											if (bExplode) {
												//also paint second pixel (image-shaped) for explosion
												rExploded=APPROACH(rDest,APPROACH(255.0f,rSrc,fExplodeExp),fAlphaness);
												gExploded=APPROACH(gDest,APPROACH(255.0f,gSrc,fExplodeExp),fAlphaness);
												bExploded=APPROACH(bDest,APPROACH(200.0f,bSrc,fExplodeExp),fAlphaness);

												arrbyDest=(byte*)&lpu32Dest[iDestScaled];
												*arrbyDest=(byte)bExploded;
												arrbyDest[1]=(byte)gExploded;
												arrbyDest[2]=(byte)rExploded;
											}// end if bExplode
											else {//lpu32Dest[iDestScaled]=u32Pixel;
												arrbyDest=(byte*)&lpu32Dest[iDestScaled];
												*arrbyDest=(byte)bCooked;
												arrbyDest[1]=(byte)gCooked;
												arrbyDest[2]=(byte)rCooked;
											}//end else don't explode
										}//end if blend alpha
									}//end else if (xDestNow+xExploder>=0)
								}// end else if xDestNow>=0//if good
								//else {
								//	break;
								//}
							}// end for xDestNow
						}//end else if yDestNow>0
						if (iDestScaled>=destTotalPixelCount) break;
					}// end for yDestNow
				}// end if SOURCE in bounds (bGood)
			}// end try
			catch (exception& exn) {
				bGood=false;
				bShowVars=true;
				if (bFirstDrawExn) {
					ShowExn(exn,"GBuffer_FX_Scaled ("+DebugNote+")");
					bFirstDrawExn=false;
				}
			}
			catch (...) {
				bGood=false;
				bShowVars=true;
				if (bFirstDrawExn) {
					ShowUnknownExn("GBuffer_FX_Scaled ("+DebugNote+")");
					bFirstDrawExn=false;
				}
			}
			if (bShowVars) {
				ShowError("  { iDestScaled:"+RString_ToString(iDestScaled)
								+"; source.w:"+RString_ToString(gbSrc->iWidth)
								+"; source.h:"+RString_ToString(gbSrc->iHeight)
								+"; x2D:"+RString_ToString(x2D)
								+"; y2D:"+RString_ToString(y2D)
								+"; fX:"+RString_ToString(fX)
								+"; fY:"+RString_ToString(fY)
								+"; xDestNow:"+RString_ToString(xDestNow)
								+"; yDestNow:"+RString_ToString(yDestNow)
								+"; fInverseScale:"+RString_ToString(fInverseScale)
								+"; fScale:"+RString_ToString(fScale)
								+"; fExplodedness:"+RString_ToString(fExplodedness)
								+"; DebugNote:"+RString_ToString(DebugNote)
								+"; }");
			}
			if (debug_gfont!=nullptr) {
				debug_gfont->TypeFast(gbDest,ipAt,RememberTextCursorPosition, DebugNote+"...done");
			}
		}
		else {
			static bool bFirstNullDestDrawErr=true;
			if (bFirstNullDestDrawErr) {
				Console::Error.WriteLine("GBuffer_FX_Scaled Error: gbDest is nullptr "+DebugNote);
				bFirstNullDestDrawErr=false;
			}
		}
		if (fExplodedness>=1.0f) {
			if (IsFirstExplosion) Console::Error.WriteLine("done (GBuffer_FX_Scaled first 1.0 explosion).");
			IsFirstExplosion=false;
		}
		return(bGood);
	} //end GBuffer_FX_Scaled

}//end namespace
#endif
