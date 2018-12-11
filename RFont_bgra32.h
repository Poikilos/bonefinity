#ifndef GFONT_H
#define GFONT_H
#include <RGradient_bgra32.h>
#include <RAnim_bgra32.h>
// for cpp:
#include <base.h>
#include <RMath.h>

/*
 * Created by SharpDevelop.
 * User: Jake Gustafson
 * Date: 4/21/2005
 * Time: 1:00 PMS
 *
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */

#define GFont_iGlyphTypes	1  // debug: should be 4 (or however many Glyph* constants there are) in fully implemented version

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
		bool TypeFast(GBuffer* gbDest, IPoint& ipDest, bool MoveThePoint, string sText);
		bool TypeFast(GBuffer* gbDest, IPoint& ipDest, bool MoveThePoint, string sText, int iGlyphType, int iDrawMode);
		bool TypeFast(GBuffer* gbDest, IPoint& ipDest, bool MoveThePoint, string sText, int iGlyphType, int iDrawMode, bool bTestOnlyNoDraw, SizeF& sizeReturn);
		bool TypeFast_NewLine(GFont* ContinueFrom_ElseNull, IPoint& ipDest);
		bool TypeHTML(GBuffer* gbDest, IPoint& ipAt, IRect& irectReturn, string sText,  bool bVisible);
		bool TypeHTML(GBuffer* gbDest, IPoint& ipAt, IRect& irectReturn, string sText);
		bool ResetGlyphTypeArray();
		bool FromImageAsPredefinedColorFont(string sFile, int iCharWidth, int iCharHeight, int iRows, int iColumns);
		bool FromImageValue(string sFile, int iCharWidth, int iCharHeight, int iRows, int iColumns);
		bool MeasureTextByRef_UsingTypeFast(SizeF& sizeReturn, GBuffer* gbDest, IPoint& ipDest, string sText, int iGlyphType);
		void SetColor(byte r, byte g, byte b);
		int marginLeftX=0;
		int lineGap=1;
	private:
		Gradient gradNow;
		bool Init();
		void InitNull();
		void ShowAsciiTable(GBuffer* gbDest, int xAt, int yAt);
	};//end class GFont
	bool GBuffer_FX_Scaled(GBuffer* gbDest, GBuffer* gbSrc, int x2D, int y2D, float fOpacity, float fExplodedness, bool bOutline, float fScale, string DebugNote, GFont* debug_gfont);
}//end namespace
#endif
