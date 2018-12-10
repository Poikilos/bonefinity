#ifndef GFONT_CPP
#define GFONT_CPP
#include <gfont32bgra.h>
#include <base.h>

/*
 * Created by SharpDevelop.
 * User: Jake Gustafson
 * Date: 4/21/2005
 * Time: 1:00 PMS
 *
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using namespace std;

namespace ExpertMultimediaBase {
	const int GFont_GlyphTypePlain=0;
	const int GFont_GlyphTypeItalic=1;
	const int GFont_GlyphTypeBold=2;
	const int GFont_GlyphTypeBoldItalic=3;
	bool GFont_bFirstRun; //#include <../../dxman-crossplatform/dxman.h> //GFont_bFirstRun etc


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
	bool GFont::TypeFast(GBuffer& gbDest, IPoint& ipDest, string sText) {
		return TypeFast(gbDest, ipDest, sText, 0, DrawModeCopyAlpha);
	}

	bool GFont::TypeFast(GBuffer& gbDest, IPoint& ipDest, string sText, int iGlyphType, int iDrawMode) {
		bool bGood=true;
		static bool bFirstRun=true;
		IPoint ipDestNow;
		try {
			if (bFirstRun) { //(GFont_bFirstRun) {
				Console.Error.Write("GFont::TypeFast ");
				Console.Error.Flush();
				bFirstRun=false;
			}
			ipDestNow.x=ipDest.x;
			ipDestNow.y=ipDest.y;
			if (GFont_bFirstRun) {
				Console.Error.Write("{iGlyphType:"+RString_ToString(iGlyphType)+"}");
				Console.Error.Flush();
			}
			const char* szText=sText.c_str();//debug hard-coded limitation
			if (iGlyphType>=GFont_iGlyphTypes) {
				ShowErr("Glyph type index "+RString_ToString(iGlyphType)+" is exceeds "+RString_ToString(GFont_iGlyphTypes)+" limit","TypeFast");
				iGlyphType=0;
			}
			if (GFont_bFirstRun) {
				Console.Error.Write("{iGlyphType:"+RString_ToString(iGlyphType)+"}");
				Console.Error.Flush();
			}
			bool character_bFirstRun=true;
			char cPrev='\0';
			bool bSpacingChar;
			for (int i=0; i<sText.length(); i++) {
				if (GFont_bFirstRun&&character_bFirstRun) {
					Console.Error.Write("{szText["+RString_ToString(i)+"]:"+RString_ToString(szText[i])+"} GotoFrame...");
					Console.Error.Flush();
				}
				bool bTest=arranimGlyphType[iGlyphType].GotoFrame((long)szText[i]);
				if (GFont_bFirstRun&&character_bFirstRun) {
					Console.Error.Write("{GotoFrame bTest:"+RString_ToString(bTest)+"}...");
					Console.Error.Flush();
				}

				if ( (szText[i]=='\r') || (szText[i]=='\n') ) {
					if (  ( (szText[i]=='\r') && (cPrev!='\n') )
					  ||  ( (szText[i]=='\n') && (cPrev!='\r') )  ) {
					  	ipDestNow.x=ipDest.x;
					  	ipDestNow.y+=arranimGlyphType[iGlyphType].gbFrame.iHeight;
					}
					bSpacingChar=true;
				}
				else if (szText[i]=='\t') {
					bSpacingChar=true;
					ipDestNow.x+=arranimGlyphType[iGlyphType].gbFrame.iWidth*3; //TODO: remove *3 if proportional font with wide tab character
				}
				else {
					bSpacingChar=false;
				}
				if (!bSpacingChar) {
					if (arranimGlyphType[iGlyphType].gbFrame.iBytesPP==1) {
						if (!OverlayNoClipToBigCopyAlpha(gbDest, ipDestNow, arranimGlyphType[iGlyphType].gbFrame, gradNow, -1)) {
							//OverlayNoClipToBigCopyAlpha(gbDest, ipAt,      gbSrc, 											gradNow, iSrcChannel);
							bGood=false;
							ShowErr("failed to overlay text character#"+RString_ToString((long)szText[i]),"TypeFast");
						}
					}
					else {
						if (!arranimGlyphType[iGlyphType].gbFrame.DrawToLargerWithoutCropElseCancel(gbDest,ipDestNow.x, ipDestNow.y,iDrawMode)) {//if (!ExpertMultimediaBase::OverlayNoClipToBig(gbDest, ipDestNow, arranimGlyphType[iGlyphType].gbFrame, gradNow, -1)) {
							bGood=false;
							ShowErr("failed to overlay text character#"+RString_ToString((long)szText[i]),"TypeFast");
						}
					}
					ipDestNow.x+=arranimGlyphType[iGlyphType].gbFrame.iWidth;
				}
				if (GFont_bFirstRun&&character_bFirstRun) {
					Console.Error.Write("move to next character...");
					Console.Error.Flush();
				}
				if (ipDestNow.x+arranimGlyphType[iGlyphType].gbFrame.iWidth>=gbDest.iWidth) {
					//if (GFont_bFirstRun) ShowErr("Tried to write text beyond screen");
					//break;
				}
				if (GFont_bFirstRun&&character_bFirstRun) {
					Console.Error.Write("done first character.");
					Console.Error.Flush();
				}
				character_bFirstRun=false;
				cPrev=szText[i];
			}//end for character i
		}
		catch (exception& exn) {
			ShowExn(exn,"TypeFast("+sText+RString_ToString(")"));
			bGood=false;
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("GFont TypeFast("+sText+RString_ToString(")"));
		}
		return bGood;
	}//end TypeFast
	bool GFont::TypeHTML(GBuffer& gbDest, IPoint& ipAt, IRect& irectReturn, string sText,  bool bVisible) {
		int iNextTag;
		int iLength;
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
	bool GFont::TypeHTML(GBuffer& gbDest, IPoint& ipAt, IRect& irectReturn, string sText) {
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
		Anim* panimTemp=null;
		try {
			panimTemp=new Anim();
			//panimTemp=new Anim();
			bGood=Init();
			if (bGood) {
				Console.Error.Write("Splitting font image...");
				bGood=panimTemp->SplitFromImage32(sFile, iCharWidth, iCharHeight, iRows, iColumns);
				Console.Error.Write(bGood?"Done.  CopyAsGrayTo...":"Splitting Failed!  ");
				//panimTemp->SaveSeq("0.debug-glyph", "png");
				//RawOverlayNoClipToBig(RetroEngine.gbScreenMain, ipAt, panimTemp->gbFrame.arrbyData, iCharWidth, iCharHeight, 4);
				if (bGood) {
					bGood=panimTemp->CopyAsGrayTo(arranimGlyphType[GFont_GlyphTypePlain]);
					Console.Error.Write(bGood?"CopyAsGrayTo Succeeded.  ":"CopyAsGrayTo Failed!  ");
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
			if (panimTemp!=null) {
				Console.Error.Write("deconstructing panimTemp...");
				delete panimTemp;
				Console.Error.Write("done deconstructing panimTemp.");
			}
			else Console.Error.Write("panimTemp was already null.");
			Console.Error.Flush();
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
		Anim* panimTemp=null;
		try {
			panimTemp=new Anim();
			//panimTemp=new Anim();
			bGood=Init();
			if (bGood) {
				Console.Error.Write("Splitting font image...");
				bGood=panimTemp->SplitFromImage32(sFile, iCharWidth, iCharHeight, iRows, iColumns);
				Console.Error.Write(bGood?"Done.  CopyAsGrayTo...":"Splitting Failed!  ");
				//panimTemp->SaveSeq("0.debug-glyph", "png");
				//RawOverlayNoClipToBig(RetroEngine.gbScreenMain, ipAt, panimTemp->gbFrame.arrbyData, iCharWidth, iCharHeight, 4);
				if (bGood) {
					bGood=panimTemp->CopyTo(arranimGlyphType[GFont_GlyphTypePlain]);
					Console.Error.Write(bGood?"CopyAsGrayTo Succeeded.  ":"CopyAsGrayTo Failed!  ");
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
			if (panimTemp!=null) {
				Console.Error.Write("deconstructing panimTemp...");
				delete panimTemp;
				Console.Error.Write("done deconstructing panimTemp.");
			}
			else Console.Error.Write("panimTemp was already null.");
			Console.Error.Flush();
		}
		catch (exception& exn) {
			ShowExn(exn,"GFont FromImageAsPredefinedColorFont","deconstructing panimTemp");
		}
		catch (...) {
			ShowUnknownExn("GFont FromImageAsPredefinedColorFont","deconstructing panimTemp");
		}
		return bGood;
	}//end FromImageAsPredefinedColorFont
	void GFont::ShowAsciiTable(GBuffer& gbDest, int xAt, int yAt) {
		try {
			IPoint ipAt;//=new IPoint(xAt,yAt);
			ipAt.x=xAt;
			ipAt.y=yAt;
			int lChar=0;
			for (int yChar=0; yChar<16; yChar++) {
				ipAt.x=xAt;
				for (int xChar=0; xChar<16; xChar++) {
					arranimGlyphType[GFont_GlyphTypePlain].GotoFrame(lChar);
					RawOverlayNoClipToBig(gbDest, ipAt,
													arranimGlyphType[GFont_GlyphTypePlain].gbFrame.arrbyData,
													arranimGlyphType[GFont_GlyphTypePlain].gbFrame.iWidth,
													arranimGlyphType[GFont_GlyphTypePlain].gbFrame.iHeight, 1);
					ipAt.x+=arranimGlyphType[GFont_GlyphTypePlain].gbFrame.iWidth;
					lChar++;
				}
				ipAt.y+=arranimGlyphType[GFont_GlyphTypePlain].gbFrame.iHeight;
			}
		}
		catch (exception& exn) {
			ShowExn(exn,"ShowAsciiTable");
		}
		catch (...) {
			ShowUnknownExn("ShowAsciiTable");
		}
	}
}//end namespace
#endif
