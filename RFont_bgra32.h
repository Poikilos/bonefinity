#ifndef GFONT_H
#define GFONT_H
#include <RGradient_bgra32.h>
#include <RAnim_bgra32.h>

using namespace std;

/*
 * Created by SharpDevelop.
 * User: Jake Gustafson
 * Date: 4/21/2005
 * Time: 1:00 PMS
 *
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
#define GFont_iGlyphTypes	1 //debug should be 4 in fully implemented version or however many Glyph* constants there are
//using System.IO;
namespace ExpertMultimediaBase {
	extern const int GFont_GlyphTypePlain;
	extern const int GFont_GlyphTypeItalic;
	extern const int GFont_GlyphTypeBold;
	extern const int GFont_GlyphTypeBoldItalic;
	extern bool GFont_bFirstRun; //#include <../../dxman-crossplatform/dxman.h> //GAMESTATE_RUN_bFirstRun etc

	class GFont {
	public:
		Anim arranimGlyphType[GFont_iGlyphTypes];
		//extern const StatusQ statusq;
		//extern int iOutWidth;
		//extern int iOutHeight;
		//extern int iBytesPP;
		//extern byte[] arrbyScreen;
		//extern GBuffer gbOut;

		GFont();
		bool SaveSeq(string sFileBaseName, string sFileExt, int iGlyphType);
		bool TypeFast(GBuffer& gbDest, IPoint& ipDest, string sText);
		bool TypeFast(GBuffer& gbDest, IPoint& ipDest, string sText, int iGlyphType, int iDrawMode);
		bool TypeFast(SizeF& sizeReturn, GBuffer& gbDest, IPoint& ipDest, string sText, int iGlyphType, int iDrawMode, bool bTestOnlyNoDraw);
		bool TypeHTML(GBuffer& gbDest, IPoint& ipAt, IRect& irectReturn, string sText,  bool bVisible);
		bool TypeHTML(GBuffer& gbDest, IPoint& ipAt, IRect& irectReturn, string sText);
		bool ResetGlyphTypeArray();
		bool FromImageAsPredefinedColorFont(string sFile, int iCharWidth, int iCharHeight, int iRows, int iColumns);
		bool FromImageValue(string sFile, int iCharWidth, int iCharHeight, int iRows, int iColumns);
		bool MeasureTextByRef_UsingTypeFast(SizeF& sizeReturn, GBuffer& gbDest, IPoint& ipDest, string sText, int iGlyphType);
	private:
		Gradient gradNow;
		bool Init();
		void InitNull();
		void ShowAsciiTable(GBuffer& gbDest, int xAt, int yAt);
	};//end class GFont
}//end namespace
#endif
