#ifndef GBUFFER32BGRA_CPP
#define GBUFFER32BGRA_CPP

#include <gbuffer32bgra.h>
//#include "C:\My Documents\Projects-cpp\Base\gbuffer32bgra.h"

using namespace std;

namespace ExpertMultimediaBase {

	/////////////////////////// class GBuffer32BGRA methods //////////////////////////////
	//public methods:
	byte* GBuffer32BGRA::BytePtrStart() {
		return byarrData;
	}
	void GBuffer32BGRA::InitNull() {
		sErrBy="GBuffer32BGRA";
		sFile="1.unnamed-gbuffer.raw";
		byarrData=NULL;
	 	iWidth=0;
		iHeight=0;
		iBytesPP=0;
		iStride=0;
		iBytesTotal=0;
		bBufferAsPointerNotCopy=false;
	}
	GBuffer32BGRA::GBuffer32BGRA() {
		InitNull();
	}
	GBuffer32BGRA::GBuffer32BGRA(string sFileImage) {
		if (!Load(sFileImage)) iBytesTotal=0;
	}
	bool GBuffer32BGRA::Load(string sFileNow) {
		bool bGood=true;
       	static bool bFirstRun=true;
       	if (bFirstRun) Console.Write("GBuffer32BGRA::Load...");
		//BitmapData bmpdata;
		//GraphicsUnit gunit;
		RectangleF rectNowF;
		Rectangle rectNow;
		sFile=sFileNow;
		try {
       		if (bFirstRun) Console.Write("calling targa...");
			bGood=targaLoaded.Load(sFile);//bmpLoaded=new Bitmap(sFile);
       		if (bFirstRun) Console.Write("get pointer...");
			byarrData=targaLoaded.GetBufferPointer();
			bBufferAsPointerNotCopy=true;
			//gunit = GraphicsUnit.Pixel;
			targaLoaded.ToRect(rectNowF);//rectNowF = bmpLoaded.GetBounds(gunit);
			rectNow.Set((int)rectNowF.X, (int)rectNowF.Y,
								(int)rectNowF.Width, (int)rectNowF.Height);
			//bmpdata = bmpLoaded.LockBits(rectNow, ImageLockMode.ReadOnly, PixelFormatNow());
       		if (bFirstRun) Console.Write("set vars...");
			iStride=targaLoaded.Stride(); //iStride=bmpdata.Stride;
			iWidth=rectNow.Width;
			iHeight=rectNow.Height;
			iBytesPP=targaLoaded.BytesPP();//assumes 32-bit //iBytesPP=iStride/iWidth;
			iBytesTotal=targaLoaded.BytesAsUncompressed();//BytesBuffer()
			if (bFirstRun) Console.Write("dimensions:"+ToString(iWidth)
										+"x"+ToString(iHeight)
										+"x"+ToString(iBytesPP*8)+"...");
			//SafeFree(byarrData);
			//byarrData=(byte*)malloc(iBytesTotal);
			//byte* lpbyNow = (byte*) bmpdata.Scan0.ToPointer();
			//for (int iBy=0; iBy<iBytesTotal; iBy++) {
			//	byarrData[iBy]=*lpbyNow;
			//	lpbyNow++;
			//}
			//bmpLoaded.UnlockBits(bmpdata);
		}
		catch (exception& exn) {
			bGood=false;
			if (&sFile==null) sFile="null";
			ShowException(exn, "gbuffer32bgra.Load(\""+sFile+"\")");
		}
		catch (...) {
			bGood=false;
			if (&sFile==null) sFile="null";
			ShowUnknownException("gbuffer32bgra.Load(\""+sFile+"\")");
		}
		bFirstRun=false;
		return bGood;
	}//end Load
	bool GBuffer32BGRA::Save(string sFileNow) {
		bool bGood=true;
		sFile=sFileNow;
		//BitmapData bmpdata;
		//GraphicsUnit gunit;
		//RectangleF rectNowF;
		//Rectangle rectNow;
		try {
			//TODO: finish this
			if (!targaLoaded.IsLoaded()) //if (!targaLoaded.IsLike(iWidth,iHeight,iBytesPP))
				targaLoaded.From(iWidth,iHeight,iBytesPP,byarrData,true);
			//targaLoaded.ChangeType(TypeAs);
			targaLoaded.Save(sFileNow);
			//if (!bBufferAsPointerNotCopy)
			//	targaLoaded.Close();
		}
		catch (exception& exn) {
			bGood=false;
			if (&sFile==null) sFile="null";
			ShowException(exn, "gbuffer32bgra.Save(\""+sFileNow+"\")");
		}
		catch (...) {
			bGood=false;
			if (&sFile==null) sFile="null";
			ShowUnknownException("gbuffer32bgra.Save(\""+sFileNow+"\")");
		}
		return bGood;
	}//end Save
	bool GBuffer32BGRA::SaveRaw(string sFileNow) {
		bool bGood=true;
		try {
			Byter byterTemp;
			byterTemp.SetLength(iBytesTotal);
			Uint32 dwTest=0;
			if (false==byterTemp.Write(byarrData, (Uint32)iBytesTotal, dwTest)) {
				bGood=false;
				ShowError("Failed to write raw data to buffer","SaveRaw("+sFileNow+")");
			}
			if (false==byterTemp.Save(sFileNow)) {
				bGood=false;
				ShowError("Failed to save raw data to file","SaveRaw("+sFileNow+")");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "gbuffer32bgra.SaveRaw("+sFileNow+")");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("gbuffer32bgra.SaveRaw("+sFileNow+")");
		}
		return bGood;
	}//end SaveRaw
	GBuffer32BGRA::GBuffer32BGRA(int iSetWidth, int iHeightNow, int iSetBytesPP) {
		InitNull();
		Init(iSetWidth, iHeightNow, iSetBytesPP, true);
	}
	GBuffer32BGRA::GBuffer32BGRA(int iSetWidth, int iHeightNow, int iSetBytesPP, bool bInitializeBuffer) {
		InitNull();
		Init(iSetWidth, iHeightNow, iSetBytesPP, bInitializeBuffer);
	}
	GBuffer32BGRA::~GBuffer32BGRA() {
        if (!bBufferAsPointerNotCopy) {
			SafeFree(byarrData);
		}
	}
	void GBuffer32BGRA::Init(int iSetWidth, int iHeightNow, int iSetBytesPP) {
		Init(iSetWidth,iHeightNow,iSetBytesPP,true);
	}
	void GBuffer32BGRA::Init(int iSetWidth, int iSetHeight, int iSetBytesPP, bool bInitializeBuffer) {
		static bool bFirstRun=true;
		iWidth=iSetWidth;
		iHeight=iSetHeight;
		iBytesPP=iSetBytesPP;
		iStride=iWidth*iBytesPP;
		iBytesTotal=iStride*iHeight;
		if (bFirstRun) Console.Write("GBuffer32BGRA::Init");
		if (bFirstRun) Console.Write("("+ToString(iWidth)+","+ToString(iHeight)+","+ToString(iBytesPP)+","+ToString(bInitializeBuffer)+")...");
		if (bInitializeBuffer) {
			try {
				if (bFirstRun) Console.Write("free...");
				SafeFree(byarrData);
				if (bFirstRun) Console.Write("allocate...");
				byarrData=(byte*)malloc(iBytesTotal);
				if (bFirstRun) Console.Write("done gbuffer32bgra.Init...");
			}
			catch (...) {
				ShowError("Couldn't allocate memory",".Init("+ToString(iSetWidth)+","+ToString(iSetHeight)+","+ToString(iSetBytesPP)+","+ToString(bInitializeBuffer)+")");
			}
		}
		bFirstRun=false;
	}//end init
	GBuffer32BGRA* GBuffer32BGRA::Copy() {
		bool bGood=true;
		GBuffer32BGRA* gbNew=NULL;
		gbNew=new GBuffer32BGRA(iWidth,iHeight,iBytesPP,false);
		bGood=CopyTo(gbNew);
		return gbNew;
		//TODO: make sure delete not free is called if Copy is deleted!!!!!!!
	}
	bool GBuffer32BGRA::CopyTo(GBuffer32BGRA* gbBlankNonNullObjectToSet) {
		bool bGood=true;
		try {
			gbBlankNonNullObjectToSet->Init(iWidth,iHeight,iBytesPP);
			memcpy(gbBlankNonNullObjectToSet->byarrData,byarrData,iBytesTotal);
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn, "gbuffer32bgra.Copy()");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("gbuffer32bgra.Copy()");
		}
		return bGood;
	}//end CopyTo(*)
	bool GBuffer32BGRA::CopyTo(GBuffer32BGRA &gbBlankNonNullObjectToSet) {
		return CopyTo(&gbBlankNonNullObjectToSet);
	}//end CopyTo(&)
	bool GBuffer32BGRA::CopyToByDataRef(GBuffer32BGRA &gbBlankNonNullObjectToSet) {
		bool bGood=true;
		gbBlankNonNullObjectToSet.Init(iWidth,iHeight,iBytesPP,false);
		gbBlankNonNullObjectToSet.byarrData=byarrData;
		return bGood;
	}
	bool GBuffer32BGRA::SetBrushColor(byte r, byte g, byte b, byte a) {
		string sFuncNow="SetBrushColor(r,g,b,a)";
		bool bGood=true;
		try {
			byarrBrush[0]=b;
			byarrBrush[1]=g;
			byarrBrush[2]=r;
			byarrBrush[3]=a;
	        byarrBrush32Copied64[0]=byarrBrush[0];
	        byarrBrush32Copied64[1]=byarrBrush[1];
	        byarrBrush32Copied64[2]=byarrBrush[2];
	        byarrBrush32Copied64[3]=byarrBrush[3];
	        byarrBrush32Copied64[4]=byarrBrush[0];
	        byarrBrush32Copied64[5]=byarrBrush[1];
	        byarrBrush32Copied64[6]=byarrBrush[2];
	        byarrBrush32Copied64[7]=byarrBrush[3];
		}
		catch (exception& exn) {
			sFuncNow="set brush color rgba";
			ShowException(exn,sFuncNow);
			bGood=false;
		}
		catch (...) {
			sFuncNow="set brush color rgba";
			ShowUnknownException(sFuncNow);
			bGood=false;
		}
		return bGood;
	}//end set brush color rgba
	bool GBuffer32BGRA::SetBrushColor(byte r, byte g, byte b) {
		return SetBrushColor(r,g,b,255);
	}//end set brush color rgb
	bool GBuffer32BGRA::SetBrushColor(string sHexCode) {
		bool bGood=true;
		try {
			
			if (StartsWith(sHexCode,"#")) sHexCode=sHexCode.substr(1);
			if (sHexCode.length()<6) {
				ShowError("This hex color code in the file is not complete","SetBrushColor("+sHexCode+")");
				bGood=false;
			}
			else {
				sHexCode=ToUpper(sHexCode);
				if (false==SetBrushColor(ByteFromHexChars(sHexCode.substr(0,2)),
				               ByteFromHexChars(sHexCode.substr(2,2)),
				               ByteFromHexChars(sHexCode.substr(4,2)), 255)) {
					bGood=false;
				}
			}
		}
		catch (exception& exn) {
			ShowException(exn,"SetBrushColor("+sHexCode+")","can't interpret specified hex color code");
			bGood=false;
		}
		catch (...) {
			ShowUnknownException("SetBrushColor("+sHexCode+")","can't interpret specified hex color code");
			bGood=false;
		}
		return bGood;
	}//end SetBrushColor from hex code

	//#region Draw methods

	bool GBuffer32BGRA::DrawRect(int xDest, int yDest, int iRectWidth, int iRectHeight) {
		bool bGood=false;
		DrawHorzLine(xDest,yDest,iRectWidth);
		iHeight--;
		DrawHorzLine(xDest,yDest+iRectHeight,iRectWidth);
		yDest++;
		iRectHeight--;//skip the verticle lines' other end too
		if (iRectHeight>0) {
			DrawVertLine(xDest,yDest,iRectHeight);
			iRectWidth--;
			bGood=DrawVertLine(xDest+iRectWidth,yDest,iRectHeight);
		}
		return bGood;
	}
	bool GBuffer32BGRA::DrawRect(IRect &irectExclusive) {
		return DrawRect(irectExclusive.left, irectExclusive.top,
					 irectExclusive.right-irectExclusive.left,
					 irectExclusive.bottom-irectExclusive.top);
	}
	bool GBuffer32BGRA::DrawRect(Rectangle &rectNow) {
		return DrawRect(rectNow.X, rectNow.X, rectNow.Width, rectNow.Height);
	}
	bool GBuffer32BGRA::DrawRectFilled(Rectangle &rectNow) {
		return DrawRectFilled(rectNow.X, rectNow.Y, rectNow.Width, rectNow.Height);
	}
	/// <summary>
	/// DrawRectBorder horizontally and vertically symmetrical
	/// </summary>
	/// <param name="rectDest"></param>
	/// <param name="rectHole"></param>
	/// <returns></returns>
	bool GBuffer32BGRA::DrawRectBorderSym(Rectangle &rectDest, Rectangle &rectHole) {
		bool bGood=true;
		int xNow;
		int yNow;
		int iSetWidth;
		int iSetHeight;
		try {
			xNow=rectDest.X;
			yNow=rectDest.Y;
			iSetWidth=rectDest.Width;
			iSetHeight=rectHole.Y-rectDest.Y;
			bool bTest=DrawRectFilled(xNow, yNow, iSetWidth, iSetHeight);//top full width
			if (bTest==false) bGood=false;
			yNow+=rectHole.Height+iSetHeight;
			//would need to change iSetHeight here if asymmetrical
			bTest=DrawRectFilled(xNow, yNow, iSetWidth, iSetHeight);//bottom full width
			if (bTest==false) bGood=false;
			yNow-=rectHole.Height;
			iSetWidth=rectHole.X-rectDest.X;
			iSetHeight=rectHole.Height;
			bTest=DrawRectFilled(xNow, yNow, iSetWidth, iSetHeight);//left remaining height
			if (bTest==false) bGood=false;
			xNow+=rectHole.Width+iSetWidth;
			//would need to change iSetWidth here if asymmetrical
			bTest=DrawRectFilled(xNow, yNow, iSetWidth, iSetHeight);//right remaining height
			if (bTest==false) bGood=false;
		}
		catch (exception& exn) {
			ShowException(exn,"DrawRectBorderSym");
			bGood=false;
		}
		catch (...) {
			ShowUnknownException("DrawRectBorderSym");
			bGood=false;
		}
		return bGood;
	} //DrawRectBorderSym
	bool GBuffer32BGRA::DrawRectBorder(int xDest, int yDest, int iRectWidth, int iRectHeight, int iThick) {
		Rectangle rectOuter;
		Rectangle rectInner;
		rectOuter.X=xDest;
		rectOuter.Y=yDest;
		rectOuter.Width=iRectWidth;
		rectOuter.Height=iRectHeight;
		rectInner.X=xDest+iThick;
		rectInner.Y=yDest+iThick;
		rectInner.Width=iRectWidth-(iThick*2);
		rectInner.Height=iRectHeight-(iThick*2);
		if ((rectInner.Width<1) || (rectInner.Height<1)) {
			return DrawRectFilled(rectOuter);
		}
		else return DrawRectBorderSym(rectOuter, rectInner);
	}//DrawRectBorder
	bool GBuffer32BGRA::DrawRectFilled(int xDest, int yDest, int iRectWidth, int iRectHeight) {
		if ((iRectWidth<1)||(iRectHeight<1)) return false;
		bool bGood=true;
		try {
			int iDest=yDest*iStride+xDest*iBytesPP;

			byte* lpDest=&byarrData[iDest];
			byte* lpSrc=byarrBrush32Copied64;
			byte* lpDestNow;
			byte* lpDestStartNow=lpDest;

			for (int yNow=0; yNow<iRectHeight; yNow++) {
				lpDestNow=lpDestStartNow;
				for (int i=iRectWidth/2; i!=0; i--) { //since coping 8 bytes (2 copies of brush pixel) at a time for speed
					memcpy(lpDestNow,lpSrc,8);
					//*((Uint64*)lpDestNow) = *((Uint64*)lpSrc);
					lpDestNow+=8;
				}
				if ((iRectWidth%2)!=0) {
					memcpy(lpDestNow,lpSrc,4);
					//*((Uint32*)lpDestNow) = *((Uint32*)lpSrc);
				}

				lpDestStartNow+=iStride;
			}

		}
		catch (exception& exn) {
			ShowException(exn,"DrawRectFilled");
			bGood=false;
		}
		catch (...) {
			ShowUnknownException("DrawRectFilled");
			bGood=false;
		}
		return bGood;
	} //DrawRectFilled
	bool GBuffer32BGRA::DrawVertLine(int xDest, int yDest, int iPixelCopies) {
		if (iPixelCopies<1) return false;
		bool bGood=true;
		try {
			int iDest=yDest*iStride+xDest*iBytesPP;
			byte* lpDest=&byarrData[iDest];
			byte* lpSrc=byarrBrush;
			byte* lpDestNow=lpDest;
			//TODO: use memcpy
			for (int i=iPixelCopies; i!=0; i--) {
				*((Uint32*)lpDestNow) = *((Uint32*)lpSrc);
				lpDestNow+=iStride;
			}
		}
		catch (exception& exn) {
			ShowException(exn,"DrawVertLine");
			bGood=false;
		}
		catch (...) {
			ShowUnknownException("DrawVertLine");
			bGood=false;
		}
		return bGood;
	}//DrawVertLine
	bool GBuffer32BGRA::DrawHorzLine(int xDest, int yDest, int iPixelCopies) {
		if (iPixelCopies<1) return false;
		bool bGood=true;
		try {
			int iDest=yDest*iStride+xDest*iBytesPP;
			byte* lpDest=&byarrData[iDest];
			byte* lpSrc=byarrBrush32Copied64;
			byte* lpDestNow=lpDest;
			for (int i=iPixelCopies/2; i!=0; i--) { //copy 2 pixels at a time for speed
				memcpy(lpDestNow,lpSrc,8);//*((UInt64*)lpDestNow) = *((UInt64*)lpSrc);
				lpDestNow+=8;
			}
			if ((iPixelCopies%2)!=0) {
				*((UInt32*)lpDestNow) = *((UInt32*)lpSrc);
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"DrawHorzLine");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("DrawHorzLine");
		}
		return bGood;
	}//end DrawHorzLine
	//#endregion Draw methods

		//additions in cpp only (not in c#):
	void GBuffer32BGRA::DrawAlphaPix(int xPix, int yPix, byte r, byte g, byte b, byte a) {
		try {
			int iChannel=yPix*iStride+xPix*iBytesPP;

			//The ++ operators are right:
			if ((iChannel+2>=0) && (iChannel+2<iStride*iHeight))
			if (((iChannel+3)/4)<(iWidth*iBytesPP*iHeight)) {
				byarrData[iChannel]=by3dAlphaLookup[b][byarrData[iChannel]][a];
				byarrData[++iChannel]=by3dAlphaLookup[g][byarrData[iChannel]][a];
				byarrData[++iChannel]=by3dAlphaLookup[r][byarrData[iChannel]][a];
			}
		}
		catch (...) {
			ShowUnknownException("DrawAlphaPix using RGB bytes");
		}
	}//
	void GBuffer32BGRA::DrawSubpixelDot(float xDot, float yDot, Pixel &pixelColor) {
		try {
			//TODO: finish this (finish subpixel accuracy)
			// Begin header fields in order of writing //
			//Targa struct reference:
			//bySizeofID byMapType byTgaType wMapOrigin wMapLength byMapBitDepth
			//xImageOrigin yImageOrigin width height byBitDepth bitsDescriptor sTag
			//*byarrColorMap *byarrData footer;
			bool bGood=true;
			//LPIPOINT *lpipointarrNow=malloc(4*sizeof(LPIIPOINT));

			int xMin=IFLOOR(xDot);
			int xMax=ICEILING(xDot);
			int yMin=IFLOOR(yDot);
			int yMax=ICEILING(yDot);
			float xfMin=(float)xMin;
			float xfMax=(float)xMax;
			float yfMin=(float)yMin;
			float yfMax=(float)yMax;
			//int iBytesPP=byBitDepth/8;
			//int iStride=iWidth*iBytesPP;
			//int iStart=yMin*iStride+xMin*iBytesPP;
			float xEccentric,yEccentric,xNormal,yNormal;
			xNormal=1.0f-fabs(xDot-xfMin);
			xEccentric=1.0f-fabs(xDot-xfMax);
			yNormal=1.0f-fabs(yDot-yfMin);
			yEccentric=1.0f-fabs(yDot-yfMax);
			DrawAlphaPix(xMin,yMin,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xNormal*yNormal));
			DrawAlphaPix(xMax,yMin,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xEccentric*yNormal));
			DrawAlphaPix(xMin,yMax,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xNormal*yEccentric));
			DrawAlphaPix(xMax,yMax,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xEccentric*yEccentric));
		}
		catch (exception& exn) {
			ShowException(exn,"DrawSubpixelDot by rgb pixel");
		}
		catch (...) {
			ShowUnknownException("DrawSubpixelDot by rgb pixel");
		}
	}//end DrawSubpixelDot
	void GBuffer32BGRA::DrawSubpixelDot(float xDot, float yDot, byte* lpbySrcPixel) {
		try {
			bool bMake=false;
			if (lpbySrcPixel==NULL) {
				lpbySrcPixel=(byte*)malloc(4);
				lpbySrcPixel[0]=0;
				lpbySrcPixel[1]=0;
				lpbySrcPixel[2]=0;
				lpbySrcPixel[3]=0;
				bMake=true;
			}
			static Pixel pixelNow;
			pixelNow.Set(lpbySrcPixel[2],lpbySrcPixel[1],lpbySrcPixel[0],lpbySrcPixel[3]);
			DrawSubpixelDot(xDot,yDot,pixelNow);
			if (bMake) {
				free(lpbySrcPixel);
				lpbySrcPixel=NULL;
			}
		}
		catch (exception& exn) {
			ShowException(exn,"DrawSubpixelDot from pixel pointer");
		}
		catch (...) {
			ShowUnknownException("DrawSubpixelDot from pixel pointer");
		}
	}//end DrawSubpixelDot(float xDot, float yDot, byte* lpbySrcPixel)
	void GBuffer32BGRA::DrawSubpixelLine(float xStart, float yStart, float xEnd, float yEnd,
			Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement) {
		int iLoops=0;
		static int iMaxLoops=1000000; //debug hard-coded limitation
		static float xNow, yNow, xRelMax, yRelMax, rRelMax, theta, rRel;
		xNow=xStart;
		yNow=yStart;
		xRelMax=xEnd-xStart;
		yRelMax=yEnd-yStart;
		rRelMax=ROFXY(xRelMax,yRelMax);
		theta=THETAOFXY(xRelMax,yRelMax);
		rRel=0;
		rRel-=fPrecisionIncrement; //the "0th" value
		static Pixel pixelColor;
		pixelColor.r=pixelStart.r;
		pixelColor.g=pixelStart.g;
		pixelColor.b=pixelStart.b;
		pixelColor.a=pixelStart.a;
		while (rRel<rRelMax && iLoops<iMaxLoops) {
			rRel+=fPrecisionIncrement;
			if (pixelEndOrNull!=NULL) {
				pixelColor.r=SafeByte(APPROACH(pixelStart.r,pixelEndOrNull->r,rRel/rRelMax));
				pixelColor.g=SafeByte(APPROACH(pixelStart.g,pixelEndOrNull->g,rRel/rRelMax));
				pixelColor.b=SafeByte(APPROACH(pixelStart.b,pixelEndOrNull->b,rRel/rRelMax));
				pixelColor.a=SafeByte(APPROACH(pixelStart.a,pixelEndOrNull->a,rRel/rRelMax));
			}
			xNow=(XOFRTHETA(rRel,theta))+xStart;
			yNow=(YOFRTHETA(rRel,theta))+yStart;
			if (xNow>0&&yNow>0&&xNow<iWidth&&yNow<iHeight)
				DrawSubpixelDot(xNow, yNow, pixelColor);
			iLoops++;
			if (iLoops>=iMaxLoops) break;
		}//end while drawing line
		if (iLoops>=iMaxLoops && iErrors<iMaxErrors) {
			cerr<<"In DrawSubpixelLine: loop overflow!"<<endl;
			iErrors++;
		}
	}//end DrawSubpixelLine

	void GBuffer32BGRA::DrawSubpixelLine(FPOINT &pointStart, FPOINT &pointEnd,
			Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement) {
		DrawSubpixelLine(pointStart.x, pointStart.y, pointEnd.x, pointEnd.y,
			pixelStart, pixelEndOrNull, fPrecisionIncrement);
	}//DrawSubpixelLine
	void GBuffer32BGRA::DrawSubpixelArc(float xCenter, float yCenter,
			float fRadius, float fWidthMultiplier, float fRotate,
			float fDegStart, float fDegEnd,
			Pixel &pixelColor,
			float fPrecisionIncrement,float fPushSpiralPixPerRotation) {
		static float fTemp,xNow,yNow;
		//TODO: make the fPrecisionIncrement a pixel increment to match other subpixel draw functions
		if (fDegStart>fDegEnd) {
			fTemp=fDegStart;
			fDegStart=fDegEnd;
			fDegEnd=fTemp;
		}
		int iLoops=0;
		static int iMaxLoops=1000000;
		for (float fNow=fDegStart; fNow<fDegEnd; fNow+=fPrecisionIncrement) {
			xNow=(XOFRTHETA(fRadius,fNow));
			yNow=-(YOFRTHETA(fRadius,fNow));//negative to flip to non-cartesian monitor loc
			xNow*=fWidthMultiplier;
			Rotate(xNow,yNow,fRotate);
			xNow+=xCenter;
			yNow+=yCenter;
	        if (xNow>0&&yNow>0&&xNow<iWidth&&yNow<iHeight)
				DrawSubpixelDot(xNow, yNow, pixelColor);
			iLoops++;
			if (iLoops>=iMaxLoops) break;
			fRadius+=fPushSpiralPixPerRotation/360.0f;
		}
		if (iLoops>=iMaxLoops && iErrors<iMaxErrors) {
			cerr<<"In DrawSubpixelArc: loop overflow!"<<endl;
			iErrors++;
		}
	}//DrawSubpixelArc
	bool GBuffer32BGRA::Fill(byte byGrayVal) {
		bool bGood=false;
		int iNow=0;
		try {
			if (byarrData!=null) {
				memset(byarrData,byGrayVal,iBytesTotal);
				//while (iNow<iBytesTotal) {
				//	iNow+=
				//}
				bGood=true;
			}
			else {
                bGood=false;
				ShowError("null buffer! {"+ToString(iWidth)+"x"+ToString(iHeight)+"x"+ToString(iBytesPP*8)+"}","GBuffer32BGRA::Fill");
			}
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"Fill");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("Fill");
		}
		return bGood;
	}//end Fill
	bool GBuffer32BGRA::IsLoaded() {
		return byarrData!=null;
	}
	bool GBuffer32BGRA::IsLike(GBuffer32BGRA &gbDest) {
		return gbDest.iBytesPP==iBytesPP
			&& gbDest.iStride==iStride
			&& gbDest.iHeight==iHeight
			&& gbDest.iBytesTotal==iBytesTotal;
	}
	bool GBuffer32BGRA::NeedsToCrop(GBuffer32BGRA &gbDest,int xDest, int yDest) {
		static bool bFirstRun=true;
		bool bReturn=true;
		if (bFirstRun) Console.Write("check NeedsToCrop...");
		if (iBytesTotal==0) {
			ShowError("Getting status on zero-length buffer!","NeedsToCrop");
		}
		try {
			if (xDest>=0 && yDest>=0
				&& xDest+iWidth<gbDest.iWidth
				&& yDest+iHeight<gbDest.iHeight) bReturn=false;
		}
		catch (...) {bReturn=true;}
		if (bFirstRun) Console.Write(((bReturn)?"yes...":"no..."));
		bFirstRun=false;
		return bReturn;
	}
	//bool GBuffer32BGRA::DrawToSmallerWithoutVoidAreasElseCancel(GBuffer32BGRA gbDest, int xSrc, int ySrc, int iDrawMode) {
	//}
	bool GBuffer32BGRA::DrawToLargerWithoutCropElseCancel(GBuffer32BGRA &gbDest, int xDest, int yDest, int iDrawMode) {
		bool bGood=true;
		static bool bFirstRun=true;
		if (byarrData==null) {
			ShowError("Tried to draw null buffer!","DrawToLargerWithoutCropElseCancel");
			return false;
		}
		try {
			if (bFirstRun) Console.Write("DrawToLargerWithoutCropElseCancel...");
			if (NeedsToCrop(gbDest,xDest,yDest)) {
				if (bFirstRun) {
					Console.Write("failed since not in bounds("+ToString(iWidth)+"x"+ToString(iHeight)+" to "+ToString(gbDest.iWidth)+"x"+ToString(gbDest.iHeight)+" at ("+ToString(xDest)+","+ToString(yDest)+") )...");
				}
				bGood=false;
			}
			if (bGood) {
                if (bFirstRun) Console.Write("offset...");
				byte* lpDestLine=&gbDest.byarrData[yDest*gbDest.iStride+xDest*gbDest.iBytesPP];
				byte* lpDestPix;
				byte* lpSrcLine=byarrData;
				byte* lpSrcPix;
				int iStrideMin=(iStride<gbDest.iStride)?iStride:gbDest.iStride;
				if (gbDest.iBytesPP==4 && iBytesPP==4) {
					switch (iDrawMode) {
					case DrawModeCopyAlpha:
                		if (bFirstRun) Console.Write("DrawModeCopyAlpha...");
						if (IsLike(gbDest) && xDest==0 && yDest==0) {
							memcpy(gbDest.byarrData,byarrData,iBytesTotal);
						}
						else {
							for (int y=0; y<iHeight; y++) {
								memcpy(lpDestLine,lpSrcLine,iStrideMin);
								lpDestLine+=gbDest.iStride;
								lpSrcLine+=iStride;
							}
						}
						break;
					case DrawModeBlendAlpha:
                		if (bFirstRun) Console.Write("DrawModeBlendAlpha(...)");
                		//if (bFirstRun) Console.Write("("+ToString(iWidth)+"x"+ToString(iHeight)+"x"+ToString(iBytesPP)+" to "+ToString(gbDest.iWidth)+"x"+ToString(gbDest.iHeight)+")...");
						//alpha result: ((Source-Dest)*alpha/255+Dest)
						float fCookedAlpha;
                        //if (bFirstRun) Console.WriteLine();
                        register int x,y;
						for (y=0; y<iHeight; y++) {
                            //if (bFirstRun) Console.Write(ToString(y));
							lpDestPix=lpDestLine;
							lpSrcPix=lpSrcLine;
							for (x=0; x<iWidth; x++) {
								//if (bFirstRun) Console.Write(".");
								if (lpSrcPix[3]==0) {
									lpSrcPix+=4; lpDestPix+=4;//assumes 32-bit
								}
								else if (lpSrcPix[3]==255) {
									memcpy(lpDestPix, lpSrcPix, 3);
									lpSrcPix+=4; lpDestPix+=4;//assumes 32-bit
								}
								else {
									fCookedAlpha=(float)lpSrcPix[3]/255.0f;
									*lpDestPix=BYROUNDF(((float)(*lpSrcPix-*lpDestPix))*fCookedAlpha+*lpDestPix); //B
									lpSrcPix++; lpDestPix++;
									*lpDestPix=BYROUNDF(((float)(*lpSrcPix-*lpDestPix))*fCookedAlpha+*lpDestPix); //G
									lpSrcPix++; lpDestPix++;
									*lpDestPix=BYROUNDF(((float)(*lpSrcPix-*lpDestPix))*fCookedAlpha+*lpDestPix); //R
									lpSrcPix+=2; lpDestPix+=2;//assumes 32-bit
								}
							}
							lpDestLine+=gbDest.iStride;
							lpSrcLine+=iStride;
                            //if (bFirstRun) Console.WriteLine();
						}
						break;
					case DrawModeKeepGreaterAlpha:
                		if (bFirstRun) Console.Write("DrawModeKeepGreaterAlpha...");
						//alpha result: ((Source-Dest)*alpha/255+Dest)
						for (int y=0; y<iHeight; y++) {
							lpDestPix=lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int x=0; x<iWidth; x++) {
								*lpDestPix=(*lpSrcPix>*lpDestPix)?*lpSrcPix:*lpDestPix; //B
								lpSrcPix++; lpDestPix++;
								*lpDestPix=(*lpSrcPix>*lpDestPix)?*lpSrcPix:*lpDestPix; //G
								lpSrcPix++; lpDestPix++;
								*lpDestPix=(*lpSrcPix>*lpDestPix)?*lpSrcPix:*lpDestPix; //R
								lpSrcPix+=2; lpDestPix+=2; //assumes 32-bit
							}
							lpDestLine+=gbDest.iStride;
							lpSrcLine+=iStride;
						}
						break;
					case DrawModeKeepDestAlpha:
                		if (bFirstRun) Console.Write("DrawModeKeepDestAlpha...");
						for (int y=0; y<iHeight; y++) {
							lpDestPix=lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int x=0; x<iWidth; x++) {
								memcpy(lpDestPix,lpSrcPix,3);
								lpDestPix+=4; lpSrcPix+=4; //assumes 32-bit
							}
							lpDestLine+=gbDest.iStride;
							lpSrcLine+=iStride;
						}
						break;
					}//end switch
				}
				else {
					ShowError("Can't Draw unless both GBuffers are 32-bit BGRA.  The GBuffer32BGRA class is designed for speed only.","GBuffer32BGRA::Draw");
				}
			}//end if does not need to crop
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"GBuffer32BGRA.DrawToLargerWithoutCropElseCancel");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("GBuffer32BGRA.DrawToLargerWithoutCropElseCancel");
		}
		if (bFirstRun) Console.WriteLine(bGood?"DrawToLargerWithoutCropElseCancel Success...":"DrawToLargerWithoutCropElseCancel failed...");
		bFirstRun=false;
		return bGood;
	}//end DrawToLargerWithoutCropElseCancel

	//#endregion class GBuffer32BGRA methods
	//end class
	//#region GBuffer32BGRA functions
	/// <summary>
	/// gbDest must be true color 24- or 32-bit for the raw source
	/// to be represented correctly.
	/// </summary>
	/// <param name="byarrSrc"></param>
	/// <param name="gbDest"></param>
	/// <param name="iSrcWidth"></param>
	/// <param name="iSrcHeight"></param>
	/// <param name="iSrcBytesPP"></param>
	/// <returns></returns>
	bool RawOverlayNoClipToBig(GBuffer32BGRA &gbDest, IPoint &ipAt, byte* byarrSrc, int iSrcWidth, int iSrcHeight, int iSrcBytesPP) {
		int iSrcByte;
		int iDestByte;
		bool bGood=true;
		int iDestAdder;
		try {
			if (iSrcBytesPP==16) {
				ShowError("16-bit source isn't implemented in this function","RawOverlayNoClipToBig");
				bGood=false;
			}
			if (bGood) {
				iDestByte=ipAt.y*gbDest.iStride+ipAt.x*gbDest.iBytesPP;
				GBuffer32BGRA gbSrc;
				gbSrc.Init(iSrcWidth, iSrcHeight, iSrcBytesPP, false);
				gbSrc.byarrData=byarrSrc;
				iDestAdder=gbDest.iStride - gbSrc.iWidth*gbDest.iBytesPP;//intentionally gbDest.iBytesPP
				iSrcByte=0;
				int iSlack=(gbSrc.iBytesPP>gbDest.iBytesPP)?(gbSrc.iBytesPP-gbDest.iBytesPP):1;
						//offset of next source pixel after loop
				for (int ySrc=0; ySrc<gbSrc.iHeight; ySrc++) {
					for (int xSrc=0; xSrc<gbSrc.iWidth; xSrc++) {
						for (int iChannel=0; iChannel<gbDest.iBytesPP; iChannel++) {
							gbDest.byarrData[iDestByte]=gbSrc.byarrData[iSrcByte];
							if ((iChannel+1)<gbSrc.iBytesPP) iSrcByte++;//don't advance to next pixel
							iDestByte++;
						}
				        iSrcByte+=iSlack;
					}
					iDestByte+=iDestAdder;
				}
				if (bGood==false) {
					ShowError("Error copying graphics buffer data","OverlayNoClipToBigCopyAlpha(...)");
				}
			}//end if bGood
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"RawOverlayNoClipToBig");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("RawOverlayNoClipToBig");
		}
		return bGood;
	}//end RawOverlayNoClipToBig
	/// <summary>
	/// Gradient version of Alpha overlay
	/// </summary>
	/// <param name="gbDest"></param>
	/// <param name="gbSrc"></param>
	/// <param name="ipDest"></param>
	/// <param name="gradNow"></param>
	/// <param name="iSrcChannel"></param>
	/// <returns></returns>
	bool OverlayNoClipToBig(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, IPoint &ipDest, Gradient &gradNow, int iSrcChannel) {
		int iSrcByte;
		int iDestByte;
		int iDestAdder;
		bool bGood=true;
		try {
			iDestByte=ipDest.y*gbSrc.iStride+ipDest.x*gbSrc.iBytesPP;
			iSrcByte=(iSrcChannel<gbSrc.iBytesPP)?iSrcChannel:gbSrc.iBytesPP-1;
			iDestAdder=gbDest.iStride - gbDest.iBytesPP*gbSrc.iWidth;//intentionally the dest BytesPP
			for (int ySrc=0; ySrc<gbSrc.iHeight; ySrc++) {
				for (int xSrc=0; xSrc<gbSrc.iWidth; xSrc++) {
					if (false==gradNow.Shade(gbDest.byarrData, iDestByte, gbSrc.byarrData[iSrcByte])) {
						//TODO: change above to ShadeAlpha
						bGood=false;
					}
					iSrcByte+=gbSrc.iBytesPP;
					iDestByte+=gbDest.iBytesPP;
				}
				iDestByte+=iDestAdder;
			}
			if (bGood==false) {
				ShowError("Error shading","OverlayNoClipToBig gradient to "+ipDest.ToString());
			}
		}
		catch (exception& exn) {
            bGood=false;
            if (&ipDest==null) ShowException(exn,"OverlayNoClipToBig gradient to NULL point");
			else ShowException(exn,"OverlayNoClipToBig gradient to "+ipDest.ToString());
		}
		catch (...) {
            bGood=false;
            if (&ipDest==null) ShowUnknownException("OverlayNoClipToBig gradient to NULL point");
			else ShowUnknownException("OverlayNoClipToBig gradient to "+ipDest.ToString());
		}
		return bGood;
	}//end OverlayNoClipToBig
	/// <summary>
	/// Gradient version of CopyAlpha (no blending) overlay
	/// </summary>
	/// <param name="gbDest"></param>
	/// <param name="ipAt"></param>
	/// <param name="gbSrc"></param>
	/// <param name="gradNow"></param>
	/// <param name="iSrcChannel"></param>
	/// <returns></returns>
	bool OverlayNoClipToBigCopyAlpha(GBuffer32BGRA &gbDest, IPoint &ipAt, GBuffer32BGRA &gbSrc, Gradient &gradNow, int iSrcChannel) {
		int iSrcByte;
		int iDestByte;
		int iDestAdder;
		bool bGood=true;
		try {
			iDestByte=ipAt.y*gbDest.iStride+ipAt.x*gbDest.iBytesPP;
			iSrcByte=(iSrcChannel<gbSrc.iBytesPP)?iSrcChannel:gbSrc.iBytesPP-1;
			iDestAdder=gbDest.iStride - gbSrc.iWidth*gbDest.iBytesPP;//intentionally the dest BytesPP
			for (int ySrc=0; ySrc<gbSrc.iHeight; ySrc++) {
				for (int xSrc=0; xSrc<gbSrc.iWidth; xSrc++) {
					if (false==gradNow.Shade(gbDest.byarrData, iDestByte, gbSrc.byarrData[iSrcByte])) {
						bGood=false;
					}
					iSrcByte+=gbSrc.iBytesPP;
					iDestByte+=gbDest.iBytesPP;
				}
				iDestByte+=iDestAdder;
			}
			if (bGood==false) {
				ShowError("Error copying graphics buffer data","OverlayNoClipToBigCopyAlpha(...) gradient");
			}
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"OverlayNoClipToBigCopyAlpha gradient");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("OverlayNoClipToBigCopyAlpha gradient");
		}
		return bGood;
	} //end OverlayNoClipToBigCopyAlpha gradient
	/// <summary>
	/// CopyAlpha overlay function.
	/// "ToBig" functions must overlay small
	/// image to big image without cropping else unexpected results occur.
	/// </summary>
	/// <param name="gbDest"></param>
	/// <param name="ipAt"></param>
	/// <param name="gbSrc"></param>
	/// <returns></returns>
	bool OverlayNoClipToBigCopyAlpha(GBuffer32BGRA &gbDest, IPoint &ipAt, GBuffer32BGRA &gbSrc) {
		int iSrcByte;
		int iDestByte;
		bool bGood=true;
		try {
			iDestByte=ipAt.y*gbDest.iStride+ipAt.x*gbDest.iBytesPP;
			iSrcByte=0;
			for (int ySrc=0; ySrc<gbSrc.iHeight; ySrc++) {
				if (false==CopySafe(gbDest.byarrData, gbSrc.byarrData, iDestByte, iSrcByte, gbSrc.iStride)) {
					bGood=false;
				}
				iSrcByte+=gbSrc.iStride;
				iDestByte+=gbDest.iStride;
			}
			if (bGood==false) {
				ShowError("Error copying graphics buffer data","OverlayNoClipToBigCopyAlpha(...)");
			}
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"OverlayNoClipToBigCopyAlpha(...)");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("OverlayNoClipToBigCopyAlpha(...)");
		}
		return bGood;
	} //end OverlayNoClipToBigCopyAlpha
	bool MaskFromChannel(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int iByteInPixel) {
		int iDestByte=0;
		int iSrcByte=iByteInPixel;
		int iBytesCopy;
		int iBytesPPOffset;
		bool bGood=true;
		try {
			if (gbDest.byarrData==NULL) {
				gbDest.Init((int)gbSrc.iWidth, (int)gbSrc.iHeight, 1);
			}
			iBytesCopy=gbDest.iBytesTotal;
			iBytesPPOffset=gbSrc.iBytesPP;
			for (iDestByte=0; iDestByte<iBytesCopy; iDestByte++) {
				gbDest.byarrData[iDestByte]=gbSrc.byarrData[iSrcByte];
				iDestByte++;
				iSrcByte+=iBytesPPOffset;
			}
		}
		catch (exception& exn) {
			string sMsg="{\n";
			sMsg+="  iByteInPixel:"; sMsg+=ToString(iByteInPixel);
				//+"; iDestCharPitch:"+iDestCharPitch.ToString()
				//+"; iChar1:"+iChar1.ToString()
				//+"; iCharNow:"+iCharNow.ToString()
				//+"; yNow:"+yNow.ToString()
				//+"; xNow:"+xNow.ToString()
			sMsg+="; iSrcByte:"; sMsg+=ToString(iSrcByte);
			sMsg+="; iDestByte:"+ToString(iDestByte) +"}";
			ShowException(exn,"MaskFromChannel()",sMsg);
			bGood=false;
		}
		catch (...) {
			string sMsg="{\n  iByteInPixel:"+ToString(iByteInPixel)
				//+"; iDestCharPitch:"+iDestCharPitch.ToString()
				//+"; iChar1:"+iChar1.ToString()
				//+"; iCharNow:"+iCharNow.ToString()
				//+"; yNow:"+yNow.ToString()
				//+"; xNow:"+xNow.ToString()
				+"; iSrcByte:"+ToString(iSrcByte)
				+"; iDestByte:"+ToString(iDestByte) +"}";
			ShowUnknownException("MaskFromChannel()",sMsg);
			bGood=false;
		}
		return bGood;
	}
	bool MaskFromValue(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc) {
		int iDestByte=0;
		int iSrcByte=0;
		int iPixels;
		int iBytesPPOffset;
		bool bGood=true;
		try {
			if (gbDest.byarrData==NULL) {
				gbDest.Init((int)gbSrc.iWidth, (int)gbSrc.iHeight, 1);
			}
			iPixels=gbSrc.iWidth*gbSrc.iHeight;
			iBytesPPOffset=gbSrc.iBytesPP;
			for (iDestByte=0; iDestByte<iPixels; iDestByte++) {
				gbDest.byarrData[iDestByte]=(byte)(((float)gbSrc.byarrData[iSrcByte]
						+(float)gbSrc.byarrData[iSrcByte+1]
						+(float)gbSrc.byarrData[iSrcByte+2])/3.0f);
				iSrcByte+=iBytesPPOffset;
			}
		}
		catch (exception& exn) {
			string sLastErr="make sure source bitmap is 24-bit or 32-bit {\n";
				//+"  "+"iByteInPixel:"+iByteInPixel.ToString()
				//+"; iDestCharPitch:"+iDestCharPitch.ToString()
				//+"; iChar1:"+iChar1.ToString()
				//+"; iCharNow:"+iCharNow.ToString()
				//+"; yNow:"+yNow.ToString()
				//+"; xNow:"+xNow.ToString()
			sLastErr+="; iSrcByte:"+ToString(iSrcByte);
			sLastErr+="; iDestByte:"+ToString(iDestByte) +"}";
			ShowException(exn,"MaskFromValue()",sLastErr);
			bGood=false;
		}
		catch (...) {
			string sLastErr="make sure source bitmap is 24-bit or 32-bit {\n";
				//+"  "+"iByteInPixel:"+iByteInPixel.ToString()
				//+"; iDestCharPitch:"+iDestCharPitch.ToString()
				//+"; iChar1:"+iChar1.ToString()
				//+"; iCharNow:"+iCharNow.ToString()
				//+"; yNow:"+yNow.ToString()
				//+"; xNow:"+xNow.ToString()
			sLastErr+="; iSrcByte:"+ToString(iSrcByte)
				+"; iDestByte:"+ToString(iDestByte) +"}";
			ShowUnknownException("MaskFromValue()",sLastErr);
			bGood=false;
		}
		return bGood;
	}//end MaskFromValue
	bool InterpolatePixel(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int iDest, DPoint &dpSrc) {
		bool bGood=false;
		bool bOnX;
		bool bOnY;
		double dWeightNow;
		double dWeightTotal;
		double dHeavyChannel;
		DPoint dparrQuad[4]; //rounded
		int iSrcRoundX;
		int iSrcRoundY;
		double dSrcRoundX;
		double dSrcRoundY;
		int iSampleQuadIndex;
		double dMaxX;
		double dMaxY;
		int iQuad;
		int iChan;
		int iDestNow;
		int iTotal=0;
		int iarrLocOfQuad[4];
		try {
			//iarrLocOfQuad=(int*)malloc(4*sizeof(int));
			dMaxX=(double)gbSrc.iWidth-1.0;
			dMaxY=(double)gbSrc.iHeight-1.0;
			//iDest=gbDest.iStride*ipDest.y+gbDest.iBytesPP*ipDest.x;
			dWeightNow=0;
			dWeightTotal=0;
			//dparrQuad=new DPoint[4];
			iSrcRoundX=(int)(dpSrc.x+.5);
			iSrcRoundY=(int)(dpSrc.y+.5);
			dSrcRoundX=(double)iSrcRoundX;
			dSrcRoundY=(double)iSrcRoundY;
			if (dSrcRoundX<dpSrc.x) {
				if (dSrcRoundY<dpSrc.y) {
					iSampleQuadIndex=0;
					dparrQuad[0].x=dSrcRoundX;		dparrQuad[0].y=dSrcRoundY;
					dparrQuad[1].x=dSrcRoundX+1.0;	dparrQuad[1].y=dSrcRoundY;
					dparrQuad[2].x=dSrcRoundX;		dparrQuad[2].y=dSrcRoundY+1.0;
					dparrQuad[3].x=dSrcRoundX+1.0;	dparrQuad[3].y=dSrcRoundY+1.0;
				}
				else {
					iSampleQuadIndex=2;
					dparrQuad[0].x=dSrcRoundX;		dparrQuad[0].y=dSrcRoundY-1.0;
					dparrQuad[1].x=dSrcRoundX+1.0;	dparrQuad[1].y=dSrcRoundY-1.0;
					dparrQuad[2].x=dSrcRoundX;		dparrQuad[2].y=dSrcRoundY;
					dparrQuad[3].x=dSrcRoundX+1.0;	dparrQuad[3].y=dSrcRoundY;
				}
			}
			else {
				if (dSrcRoundY<dpSrc.y) {
					iSampleQuadIndex=1;
					dparrQuad[0].x=dSrcRoundX-1.0;	dparrQuad[0].y=dSrcRoundY;
					dparrQuad[1].x=dSrcRoundX;		dparrQuad[1].y=dSrcRoundY;
					dparrQuad[2].x=dSrcRoundX-1.0;	dparrQuad[2].y=dSrcRoundY+1.0;
					dparrQuad[3].x=dSrcRoundX;		dparrQuad[3].y=dSrcRoundY+1.0;
				}
				else {
					iSampleQuadIndex=3;
					dparrQuad[0].x=dSrcRoundX-1.0;	dparrQuad[0].y=dSrcRoundY-1.0;
					dparrQuad[1].x=dSrcRoundX;		dparrQuad[1].y=dSrcRoundY-1.0;
					dparrQuad[2].x=dSrcRoundX-1.0;	dparrQuad[2].y=dSrcRoundY;
					dparrQuad[3].x=dSrcRoundX;		dparrQuad[3].y=dSrcRoundY;
				}
			}
			if (dpSrc.x<0) {
				for (iQuad=0; iQuad<4; iQuad++) {
					if (dparrQuad[iQuad].x<0) dparrQuad[iQuad].x=0;
				}
			}
			else if (dpSrc.x>dMaxX) {
				for (iQuad=0; iQuad<4; iQuad++) {
					if (dparrQuad[iQuad].x>dMaxX) dparrQuad[iQuad].x=dMaxX;
				}
			}
			if (dpSrc.y<0) {
				for (iQuad=0; iQuad<4; iQuad++) {
					if (dparrQuad[iQuad].y<0) dparrQuad[iQuad].y=0;
				}
			}
			else if (dpSrc.y>dMaxY) {
				for (iQuad=0; iQuad<4; iQuad++) {
					if (dparrQuad[iQuad].y>dMaxY) dparrQuad[iQuad].y=dMaxY;
				}
			}
			if (dpSrc.x==(double)iSrcRoundX) bOnX=true;
			else bOnX=false;
			if (dpSrc.y==(double)iSrcRoundY) bOnY=true;
			else bOnY=false;

			if (bOnY&&bOnX) {
				CopySafe(gbDest.byarrData, gbSrc.byarrData, iDest, iSrcRoundY*gbSrc.iStride+iSrcRoundX*gbSrc.iBytesPP, gbDest.iBytesPP);
			}
			else {
				iDestNow=iDest;
				for (iQuad=0; iQuad<4; iQuad++) {
					iarrLocOfQuad[iQuad]=gbSrc.iStride*(int)dparrQuad[iQuad].y + gbSrc.iBytesPP*(int)dparrQuad[iQuad].x;
				}
				for (iChan=0; iChan<gbSrc.iBytesPP; iChan++, iTotal++) {
					dHeavyChannel=0;
					dWeightTotal=0;
					for (iQuad=0; iQuad<4; iQuad++) {
						dWeightNow=dDiagonalUnit-DDist(dpSrc, dparrQuad[iQuad]);
						dWeightTotal+=dWeightNow; //debug performance, this number is always the same theoretically
						dHeavyChannel+=(double)gbSrc.byarrData[iarrLocOfQuad[iQuad]+iChan]*dWeightNow;
					}
					gbDest.byarrData[iDestNow]=(byte)(dHeavyChannel/dWeightTotal);
					iDestNow++;
				}
			}
			bGood=true;
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"InterpolatePixel");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("InterpolatePixel");
		}
		return bGood;
	}//end InterpolatePixel
	/// <summary>
	/// Fakes motion blur.
	///   Using a byDecayTotal of 255 makes the blur trail fade to transparent.
	/// </summary>
	bool EffectMoBlurSimModWidth(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int xOffsetTotal, byte byDecayTotal) {
		bool bGood=true;
		int xDirection;
		int xLength;
		int iDestByteStart;
		if (xOffsetTotal<0) {
			xDirection=-1;
			xLength=xOffsetTotal*-1;
		}
		else {
			xDirection=1;
			xLength=xOffsetTotal;
		}
		try {
			gbDest.iWidth=gbSrc.iWidth+xLength;
			gbDest.iBytesPP=gbSrc.iBytesPP;
			gbDest.iStride=gbSrc.iStride;
			gbDest.iHeight=gbSrc.iHeight;
			gbDest.iBytesTotal=gbDest.iStride*gbDest.iHeight;
			if (gbDest.byarrData==NULL || (gbDest.iBytesTotal!=gbDest.iBytesTotal))
				gbDest.Init((int)gbSrc.iWidth,(int)gbSrc.iHeight,(int)gbSrc.iBytesPP);
			int iHeight2=gbDest.iHeight;
			int iWidth2=gbDest.iWidth;
			int iHeight1=gbSrc.iHeight;
			int iWidth1=gbSrc.iWidth;
			int iStride=gbSrc.iStride;
			int iStride2=gbDest.iStride;
			int iSrcByte=0;
			iDestByteStart=0;
			if (xDirection<0) {
				iDestByteStart=xLength;
			}
			int iDestByte=iDestByteStart;
			bool bTest=true;
			int yNow;
			for (yNow=0; yNow<iHeight1; yNow++) {
				bTest=CopySafe(gbDest.byarrData,
							 	gbSrc.byarrData,
							  	iDestByte, iSrcByte, iStride);
				if (bTest==false) {
					ShowError("Error precopying blur data.","EffectMoBlurSimModWidth(...)");
					break;
				}
				iSrcByte+=iStride;
				iDestByte+=iStride2;
			}
			int iOffsetEnder=xLength;
			if (xDirection<0) {
				iOffsetEnder=-1;
			}
			//debug float precision error on super-high res?
			float fMultiplier=1.0f;
			float fPixNow=0;
			float fMaxPix=(float)(xLength-1);
			float fDecayTotal=(float)byDecayTotal;
			//bTest=true;
			for (int iOffsetNow=iDestByteStart; fPixNow<=fMaxPix; iOffsetNow+=xDirection) {
				if (bTest==false) break;
				iSrcByte=0;
				iDestByte=iOffsetNow;
				for (yNow=0; yNow<iHeight1; yNow++) {
					bTest=EffectLightenOnly(gbDest.byarrData,
						gbSrc.byarrData,iDestByte, iSrcByte, iStride, fMultiplier);
					if (bTest==false) {
						ShowError("Error overlaying blur data.","EffectMoBlurSimModWidth(...)");
						break;
					}
					iSrcByte+=iStride;
					iDestByte+=iStride2;
				}
				fPixNow++;
				fMultiplier=(fDecayTotal/255.0f)*(fPixNow/fMaxPix);
			}
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"EffectMoBlurSimModWidth");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("EffectMoBlurSimModWidth");
		}
		return bGood;
	}//EffectMoBlurSimModWidth
	bool EffectSkewModWidth(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int xOffsetBottom) {
		bool bGood=true;
		int iDestLine;
		double xDirection;
		double dHeight;
		double yNow;
		double dWidthDest;
		//double dWidthSrc;
		double xNow;
		double xAdd;
		double dMaxY;
		int iDestIndex;
		DPoint dpSrc;
		int iDestByte;
		int iSrcByte;
		if (xOffsetBottom<0) {
			xDirection=-1;
			xAdd=(double)(xOffsetBottom*-1);
		}
		else {
			xDirection=1;
			xAdd=(double)xOffsetBottom;
		}
		try {
			gbDest.iWidth=gbSrc.iWidth+((xOffsetBottom<0)?xOffsetBottom*-1:xOffsetBottom);
			gbDest.iBytesPP=gbSrc.iBytesPP;
			gbDest.iStride=gbSrc.iStride;
			gbDest.iHeight=gbSrc.iHeight;
			gbDest.iBytesTotal=gbDest.iStride*gbDest.iHeight;
			if (gbDest.byarrData==NULL || (gbDest.iBytesTotal!=gbDest.iBytesTotal))
				gbDest.Init((int)gbSrc.iWidth,(int)gbSrc.iHeight,(int)gbSrc.iBytesPP);
			iSrcByte=0;
			iDestByte=0;//iDestByteStart;//TODO: Uncomment, and separate the blur code here and make alpha overlay version
			bool bTest=true;
			iDestLine=0;
			//dpSrc=new DPoint();
			dpSrc.y=0;
			dHeight=(double)gbDest.iHeight;
			dWidthDest=(double)gbDest.iWidth;
			//dWidthSrc=(double)gbSrc.iWidth;
			dMaxY=dHeight-1.0;
			iDestIndex=0;
			for (yNow=0; yNow<dHeight; yNow+=1.0) {
				dpSrc.x=(yNow/dMaxY)*xAdd;
				if (xOffsetBottom<0) dpSrc.x=(xAdd-dpSrc.x);
				for (xNow=0; xNow<dWidthDest; xNow+=1.0) {
					if (dpSrc.x>-1.0) {
						if (dpSrc.x<dWidthDest)
							bTest=InterpolatePixel(gbDest, gbSrc, iDestIndex, dpSrc);
					}
					if (bTest==false) {
						bGood=false;
						break;
					}
					iDestIndex+=gbSrc.iBytesPP;
				}
				if (bGood==false) break;
				//iDestLine+=gbDest.iStride;
				dpSrc.y+=1.0;
			}
			if (bGood==false) {
				ShowError("Error calculating skew data.","EffectSkewModWidth(...)");
			}
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"EffectSkewModWidth");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("EffectSkewModWidth");
		}
		return bGood;
	}//end EffectSkewModWidth
	bool EffectLightenOnly(byte* byarrDest, byte* byarrSrc, int iDestByte, int iSrcByte, int iBytes) {
		bool bGood=true;
		try {
			for (int iByteNow=0; iByteNow<iBytes; iByteNow++) {
				if (byarrSrc[iSrcByte]>byarrDest[iDestByte]) byarrDest[iDestByte]=byarrSrc[iSrcByte];
				iDestByte++;
				iSrcByte++;
			}
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"EffectLightenOnly regular");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("EffectLightenOnly regular");
		}
		return bGood;
	}//end EffectLightenOnly
	bool EffectLightenOnly(byte* byarrDest, byte* byarrSrc, int iDestByte, int iSrcByte, int iBytes, float fMultiplySrc) {
		bool bGood=true;
		if (fMultiplySrc>1.0f) fMultiplySrc=1.0f;
		byte bySrc;
		float fVal;
		try {
			for (int iByteNow=0; iByteNow<iBytes; iByteNow++) {
				fVal=((float)byarrSrc[iSrcByte]*fMultiplySrc);
				if (fVal>255.0) fVal=255;
				bySrc=(byte)fVal;
				if (bySrc>byarrDest[iDestByte]) byarrDest[iDestByte]=bySrc;
				iDestByte++;
				iSrcByte++;
			}
		}
		catch (exception& exn) {
            bGood=false;
			ShowException(exn,"EffectLightenOnly multiplied source");
		}
		catch (...) {
            bGood=false;
			ShowUnknownException("EffectLightenOnly multiplied source");
		}
		return bGood;
	}//end EffectLightenOnly

	//#endregion GBuffer32BGRA functions

}//end namespace
#endif
