#ifndef MAIN_C
#define MAIN_C

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <SDL2/SDL.h>

#include <RSprite_bgra32.h>
//#include <RWorld.h>
#include <RAnim_bgra32.h>
#include <RImage_bgra32.h>
#include <RFont_bgra32.h>
#include <RMath.h>
#include <Targa.h>
#include <RFile.h>
#include <camera.h> //only needed for using 3D features of ExpertMultimediaBase
#include <limits> //std::numeric_limits<T>::min() etc
#include <base.h> //RString_iDecimalPlacesForToString etc
//#include <base.cpp>
//#include <frameworkdummy.cpp>
#include <frameworkdummy.h>

using namespace std;

namespace ExpertMultimediaBase {

//using namespace ExpertMultimediaBase;

	//using namespace std;

	typedef unsigned short USHORT;
	typedef unsigned short WORD;
	//typedef unsigned char BYTE;

	//#ifndef BYTE
	//typedef unsigned __int8 BYTE;
	//#endif
	//typedef unsigned __int8 DWORD;
	//typedef unsigned __int16 USHORT;
	//typedef unsigned __int8 BYTE;
	//typedef ULARGE_INTEGER __int64);


	/*
	 PLEASE NOTE: the program will require SDL.dll which is located in
				dev-c++'s dll directory. You have to copy it to you
				program's home directory or the path.
	*/
	bool MoveThePoint=true;
	SDL_Surface *screen=NULL;
	SDL_Window *sdlWindow=NULL;
	SDL_Renderer *sdlRenderer=NULL;
	SDL_Texture *sdlTexture=NULL;
	Camera camera;
	Anim animTest;
	GBuffer gbScreen;
	GBuffer gbOverlays;
	GFont gfontDefault;
	//GFont gfontBlue;
	bool bFirstRun=true;
	bool bDone=false;
	int iTickLastIteration=0;
	int iSetScreenW=640;
	int iSetScreenH=480;
	int iSetScreenBytesPP=4;
	float xPlayer=1.0f;
	float yPlayer=1.0f;
	float xDest=std::numeric_limits<float>::min();
	float yDest=std::numeric_limits<float>::min();
	//int yTextLine=16;
	//int LineOffset=16;//pixels between bottom of each line and the next

	//#region program-related input vars
	int xCursorDown=-1;
	int yCursorDown=-1;
	float xfCursorDown=-1.0f;
	float yfCursorDown=-1.0f;
	bool bMouseDown=false;
	bool bMouseDownR=false;
	int xCursor=-1;
	int yCursor=-1;
	float xfCursor=-1.0f;
	float yfCursor=-1.0f;
	bool bMouseMove=false;
	Pixel pixelRed(255,0,0,255);
	Pixel pixelGreen(0,255,0,255);
	Pixel pixelDarkGreen(0,128,0,255);
	Pixel pixelBlue(0,0,255,255);
	Pixel pixelCyan(0,255,255,255);
	Pixel pixelDarkCyan(0,64,64,255);
	Pixel pixelMagenta(255,0,255,255);
	Pixel pixelYellow(255,255,0,255);
	Pixel pixelWhite(255,255,255,255);
	Pixel pixelLightGray(192,192,192,255);
	Pixel pixelDarkGray(64,64,64,255);
	//#endregion program-related input vars

	//#region debugging vars
	ExpertMultimediaBase::Pixel debugStartPixel(128,128,128,255); //r,g,b,a
	ExpertMultimediaBase::Pixel debugEndPixel(255,255,255,255); //r,g,b,a
	IPoint textPoint;
	int iLastLineRelationshipType=RMath::LineRelationshipNone;
	int iLastIntersection=RMath::IntersectionNo;
	bool playerIsMoving=false;
	Mass3d cursor3d;
	FPoint startPoint;
	Mass3d startMass;
	FPoint endPoint;
	Mass3d endMass;
	//Mass3d startTrail;
	//Mass3d endTrail;
	Mass3d followTrailMass;
	Mass3d followMass;
	float followUnitsPerFrame=.01f;
	FPoint angleDiffPositivePoint;
	FPoint anglediffStartAngleIntersectsRadiusAtPoint;
	FPoint anglediffEndAngleIntersectsRadiusAtPoint;
	float anglediffpositiveStartAngle=0.0f;
	float anglediffpositiveEndAngle=0.0f;
	float anglediffpositiveRadius=1.0f;//changed later
	//#endregion debugging vars

	int AssumedTextHeight=12;

	static bool Init() {
		Console::Error.WriteLine("Date and time:"+System::DateTime_Now.ToString("yyyy-MM-dd HH:mm:ss"));
		string tryNumberString="-998877";
		Console::Error.WriteLine("Convert.ToInt32(\""+tryNumberString+"\"):"+System::Convert::ToString(System::Convert::ToInt32(tryNumberString)));
		Console::Error.Write("main Init...");
		bool bGood=true;
		int iFrames=16;
		//animTest.LoadSeq("D:\\Projects-cpp\\dxman\\dxman-crossplatform\\bin\\images\\hero*.tga");
		animTest.LoadSeq("images/spin*.tga");
		iFrames=animTest.IFrames(); //TODO: change to lFramesCached?
		Console::Error.Write("Found "+RString_ToString(iFrames)+" frames...");
		Console::Error.WriteLine();
		Console::Error.WriteLine(animTest.Dump(true));
		followMass.X=0.0f;
		followMass.Y=0.0f;
		followMass.Z=5.0f;
		followMass.CopyTo(followTrailMass);

		Console::Error.Write("Init screen GBuffer...");
		gbScreen.Init(iSetScreenW,iSetScreenH,4);//assumes 32-bit GBuffer is needed
		xPlayer=(float)gbScreen.iWidth/4.0f;
		yPlayer=(float)gbScreen.iHeight/4.0f-30.0f;

		angleDiffPositivePoint.X=(float)gbScreen.iWidth/6.0f*5.0f;
		angleDiffPositivePoint.Y=(float)gbScreen.iHeight/2.0f;

		anglediffpositiveRadius=(float)gbScreen.iWidth/6.0f*.5f;

		Console::Error.WriteLine("Done.");
		Console::Error.Write("Loading test ini and running dump test...");
		Variables vsNow;
		Variable* pvNow=nullptr;
		vsNow.Load("data/test.ini");
		ofstream ofNow("1.data-test_ini-Vars-output-dump.txt");
		string sTemp;
		string sName="test";
		ofNow<<sName<<" dump:"<<endl;
		int i1stDimLen=vsNow.Indeces(sName);
		ofNow<<i1stDimLen<<" indeces found for dimension 1 in data \""<<vsNow.GetForcedString(sName)<<"\""<<endl;
		for (int i1st=0; i1st<i1stDimLen; i1st++) {
			int i2ndDimLenNow=vsNow.Indeces(sName,i1st);
			ofNow<<i2ndDimLenNow<<" indeces found for dimension 2 at dimension 1 index "<<i1st<<endl;
			if (i2ndDimLenNow==0) {
				pvNow=vsNow.PointerOf(sName);
				if (pvNow!=nullptr) bGood=pvNow->Get(sTemp,i1st);
				else { bGood=false; sTemp="(VAR NAME NOT FOUND)"; }
				ofNow<<sTemp<<endl;
			}
			else {
				for (int i2nd=0; i2nd<i2ndDimLenNow; i2nd++) {
					pvNow=vsNow.PointerOf(sName);
					if (pvNow!=nullptr) bGood=pvNow->Get(sTemp,i1st,i2nd);
					else { bGood=false; sTemp="(VAR NAME NOT FOUND)"; }
					ofNow<<sTemp;
					if (i2nd+1<i2ndDimLenNow) ofNow<<", ";
				}
				ofNow<<endl;
			}
		}
		Console::Error.WriteLine("Done.");
		Console::Error.Write("Closing test ini...");
		Console::Error.Flush();
		ofNow.close();
		Console::Error.WriteLine("Done.");
		Console::Error.Write("Loading font...");
		Console::Error.Flush();
		gfontDefault.FromImageValue("images/ascii256-dos-fixed.tga",7,AssumedTextHeight,16,16); //gfontDefault.FromImageAsPredefinedColorFont("images/ascii256-dos-fixed.png",8,12,16,16);
		//gfontBlue.FromImageValue("images/ascii256-dos-fixed.tga",7,AssumedTextHeight,16,16);
		//gfontBlue.SetColor(0,0,255);
		//gfontGreen.FromImageValue("images/ascii256-dos-fixed.tga",7,AssumedTextHeight,16,16);
		//gfontGreen.SetColor(0,255,0);
		//gfontDefault.FromImageValue("images/ascii256-dos-fixed.tga",7,AssumedTextHeight,16,16);
		//gfontDefault.SetColor(255,255,0);
		Console::Error.WriteLine("Done.");
		return bGood;
	}
	static bool Shutdown() {
		bool bGood=true;
		return bGood;
	}
	static bool DrawScreen() {
		bool bGood=true;
		if (SDL_MUSTLOCK(screen)!=0) {
			if (SDL_LockSurface(screen)<0) {
				bGood=false;
				ShowError("Can't lock screen surface","DrawScreen");
			}
		}
		int iDestBytesPP=0;
		int iDestStride=0;
		int iSrcStride=0;
		int iStrideMin=0;
		if (bGood) {
			iDestBytesPP=screen->format->BytesPerPixel;
			iDestStride=screen->pitch;
			iSrcStride=gbScreen.iStride;
			iStrideMin=(iSrcStride<iDestStride)?iSrcStride:iDestStride;
			if (gbScreen.BytePtrStart()==nullptr) {
				ShowError("Tried to draw uninitialized GBuffer!","DrawScreen");
				bGood=false;
			}
		}
		try {
			byte* lpSrcLine;
			byte* lpSrcPix;
			Uint8* lpDestPix;
			Uint8* lpDestLine;
			if (bGood) {
				lpSrcLine=gbScreen.BytePtrStart();
				if (bFirstRun) Console::Error.Write("debug test...");

				//Uint8* lpbyScreen=(Uint8*)screen->pixels;//debug only
				//*((Uint32*)lpbyScreen)=0xFFFFFF; //debug only
				//*(Uint32*)(&lpbyScreen[32*iDestStride+32*iDestBytesPP])=0xFFFFFF; //debug only
				//gbScreen.Fill(255);//debug only

				if (bFirstRun) Console::Error.Write("copy...");
				lpDestLine=(Uint8*)screen->pixels;//8bit in order to use stride
				switch (iDestBytesPP) {
					case 1:
						for (int y=0; y<gbScreen.iHeight; y++) {
							lpDestPix=lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int x=0; x<gbScreen.iWidth; x++) {
								*lpDestPix = (Uint8)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
								lpSrcPix+=4;//assumes 32-bit GBuffer
								lpDestPix++;
							}
							lpDestLine+=iDestStride;
							lpSrcLine+=iSrcStride;
						}
						break;
					case 2:
						Uint16* lpwDestPix;
						for (int y=0; y<gbScreen.iHeight; y++) {
							lpwDestPix=(Uint16*)lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int x=0; x<gbScreen.iWidth; x++) {
								*lpwDestPix = (Uint16)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
								lpSrcPix+=4;//assumes 32-bit GBuffer
								lpwDestPix++;//ok since Uint16 ptr
							}
							lpDestLine+=iDestStride;
							lpSrcLine+=iSrcStride;
						}
						break;
					case 3:
						Uint32 color;
						if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
							for (int y=0; y<gbScreen.iHeight; y++) {
								lpDestPix=lpDestLine;
								lpSrcPix=lpSrcLine;
								for (int x=0; x<gbScreen.iWidth; x++) {
									color = (Uint32)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
									*lpDestPix=(byte)color;
									lpDestPix[1]=(byte)(color>>8);
									lpDestPix[2]=(byte)(color>>16);
									lpSrcPix+=4;//assumes 32-bit GBuffer
									lpDestPix+=3;
								}
								lpDestLine+=iDestStride;
								lpSrcLine+=iSrcStride;
							}
						}
						else {
							for (int y=0; y<gbScreen.iHeight; y++) {
								lpDestPix=lpDestLine;
								lpSrcPix=lpSrcLine;
								for (int x=0; x<gbScreen.iWidth; x++) {
									color = (Uint32)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
									lpDestPix[2]=(byte)color;
									lpDestPix[1]=(byte)(color>>8);
									*lpDestPix=(byte)(color>>16);
									lpSrcPix+=4;//assumes 32-bit GBuffer
									lpDestPix+=3;
								}
								lpDestLine+=iDestStride;
								lpSrcLine+=iSrcStride;
							}
						}
						break;
					case 4:
						Uint32* lpu32DestPix;
						for (int y=0; y<gbScreen.iHeight; y++) {
							lpu32DestPix=(Uint32*)lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int x=0; x<gbScreen.iWidth; x++) {
								//*lpu32DestPix=*((Uint32*)lpSrcPix);
								*lpu32DestPix = (Uint32)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
								lpSrcPix+=4;//assumes 32-bit GBuffer
								lpu32DestPix++;//ok since Uint32*
							}
							lpDestLine+=iDestStride;
							lpSrcLine+=iSrcStride;
						}
						/*
						if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
							for (int iLine=0; iLine<gbScreen.iHeight; iLine++) {
								memcpy(lpDestLine,lpSrcLine,iStrideMin);
								lpDestLine+=iDestStride;
								lpSrcLine+=iSrcStride;
							}
						}
						*/
						break;
				}//end switch
				if (SDL_MUSTLOCK(screen)!=0) {
					SDL_UnlockSurface(screen);
				}
			}//end if bGood
			//else already showed an error
		}
		catch (exception& exn) {
			ShowExn(exn, "DrawScreen");
			bGood=false;
		}
		catch (...) {
			ShowUnknownExn("DrawScreen()");
			bGood=false;
		}
		return bGood;
	}//end DrawScreen

	static void Iterate() {
		iTickLastIteration=SDL_GetTicks();
		static int iFakeValueVel = 0;
		static int iFakeValue = 128;
		static int iFakeHue = 0;
		SDL_Rect sdlrectPixelNow;
		Uint32 u32Color;
		/* Create a black background */
		u32Color = SDL_MapRGB (screen->format, 0, 0, 0);
		SDL_FillRect (screen, NULL, u32Color);

		/* Determine which u32Color the layer should have */
		if (iFakeValueVel == 0) {
			iFakeValue += 2;
			if (iFakeValue >= 256) {
				iFakeValue = 255;
				iFakeValueVel = 1;
			}
		}
		else {
			iFakeValue -= 2;
			if (iFakeValue <= 5) {
				iFakeValue = 0;
				iFakeValueVel = 0;
				iFakeHue++;
				if (iFakeHue == 5)
					iFakeHue = 0;
			}
		}

		if (bFirstRun) Console::Error.Write("Starting Draw background color...");
		switch (iFakeHue) {
			case 0:
				u32Color = SDL_MapRGB (screen->format, iFakeValue, 0, 0);
				break;
			case 1:
				u32Color = SDL_MapRGB (screen->format, 0, iFakeValue, 0);
				break;
			case 2:
				u32Color = SDL_MapRGB (screen->format, 0, 0, iFakeValue);
				break;
			case 3:
				u32Color = SDL_MapRGB (screen->format, iFakeValue, iFakeValue, iFakeValue);
				break;
			case 4:
				u32Color = SDL_MapRGB (screen->format, iFakeValue, 0, iFakeValue);
				break;
		}
		sdlrectPixelNow.w = screen->w / 2;
		sdlrectPixelNow.h = screen->h / 2;
		sdlrectPixelNow.x = (screen->w / 2) - (sdlrectPixelNow.w / 2);
		sdlrectPixelNow.y = (screen->h / 2) - (sdlrectPixelNow.h / 2);
		//SDL_FillRect (screen, &sdlrectPixelNow, u32Color);
		if (bFirstRun) Console::Error.WriteLine("Done.");
		if (bFirstRun) Console::Error.Write("Clearing gbScreen...");
		bool bTest=gbScreen.Fill(0);
		if (bFirstRun) Console::Error.WriteLine((bTest)?"Success (on "+RString_ToString(gbScreen.iBytesPP)+"-BytesPP).":("Failed (on "+RString_ToString(gbScreen.iBytesPP)+"-BytesPP)."));
		if (bFirstRun) Console::Error.Write("Drawing debug info...");

		int AssumedIntersectionDebugStringWidth=gbScreen.iWidth/2;

		//draw a line 3/8 from the left:
		float line0_x1=(float)gbScreen.iWidth/8.0f*3.0f;
		float line0_y1=(float)gbScreen.iHeight/2.0f - (float)gbScreen.iHeight/4.0f;
		float line0_x2=(float)gbScreen.iWidth/8.0f*3.0f;
		float line0_y2=(float)gbScreen.iHeight/2.0f + (float)gbScreen.iHeight/4.0f;
		float xInter=0.0f;
		float yInter=0.0f;

		gbScreen.DrawSubpixelLine(line0_x1,line0_y1,line0_x2,line0_y2,pixelBlue,NULL,1.0f);

		double xInterD;
		double yInterD;
		iLastLineRelationshipType=RMath::IntersectionAndRelationship(xInterD,yInterD, (double)line0_x1,(double)line0_y1,(double)line0_x2,(double)line0_y2, (double)xfCursorDown,(double)yfCursorDown,(double)xfCursor,(double)yfCursor);
		xInter=(float)xInterD;
		yInter=(float)yInterD;
		//iLastLineRelationshipType=RMath::IntersectionAndRelationship(xInter,yInter, line0_x1,line0_y1,line0_x2,line0_y2, xfCursorDown,yfCursorDown,xfCursor,yfCursor);

		float RelationshipText_X=xInter;
		float RelationshipText_Y=yInter;
		if ((int)RelationshipText_X<0) RelationshipText_X=0;
		else if ((int)RelationshipText_X+AssumedIntersectionDebugStringWidth>gbScreen.iWidth) RelationshipText_X=gbScreen.iWidth-AssumedIntersectionDebugStringWidth;
		if ((int)RelationshipText_Y<0) RelationshipText_Y=0;
		else if ((int)RelationshipText_Y+AssumedTextHeight*2+gfontDefault.lineGap>gbScreen.iHeight) RelationshipText_Y=gbScreen.iHeight-AssumedTextHeight*2-gfontDefault.lineGap;
		textPoint.Set((int)RelationshipText_X,int(RelationshipText_Y));
		gfontDefault.SetColor(192,192,192);//Set right away so you aren't left with the color from the previous frame!
		if (bFirstRun) Console::Error.WriteLine("main about to call TypeFast (to "+RString_ToString(gbScreen.iBytesPP)+"-BytesPP).");
		//    GFont::TypeFast(GBuffer*, IPoint&,   bool,         string,                                                                                                                                     int iGlyphType,       int iDrawMode) {
		gfontDefault.TypeFast(&gbScreen, textPoint, MoveThePoint, "("+RString_ToString(xInter)+","+RString_ToString(yInter)+")[relationship:"+RMath::LineRelationshipToString(iLastLineRelationshipType)+"]", GFont_GlyphTypePlain, DrawModeCopyAlpha);
		float fRadius=2.5;
		float fWidthMultiplier1UnlessEllipse=1.0f;
		float fRotate=0.0f;
		float fPrecisionIncrement=1.0f;
		float fPushSpiralPixPerRotation=0.0f;
		gbScreen.DrawSubpixelArc(xInter,yInter,fRadius,fWidthMultiplier1UnlessEllipse,fRotate,0.0f,360.0f,pixelGreen,fPrecisionIncrement,fPushSpiralPixPerRotation);


		bool bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1 = true;//line relationship method calls it with FALSE
		iLastIntersection=RMath::Intersection(xInter, yInter, line0_x1, line0_y1, line0_x2, line0_y2, xfCursorDown,yfCursorDown,xfCursor,yfCursor, bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1);
		//iLastIntersection=RMath::Intersection(xInter, yInter, Line1_x1, Line1_y1, Line1_x2, Line1_y2, Line2_x1, Line2_y1, Line2_x2, Line2_y2, bReturn1IfWithinSegmentElseIfNotThen2_IfThisVarIsFalseOrMissingAndDoesIntersectThenReturn1);
		//iLastIntersection=RMath::Intersection(xInter, yInter, Line1_x1, Line1_y1, Line1_x2, Line1_y2, Line2_x1, Line2_y1, Line2_x2, Line2_y2)
		float IntersectionText_X=xInter;
		float IntersectionText_Y=yInter+AssumedTextHeight+gfontDefault.lineGap;
		if ((int)IntersectionText_X<0) IntersectionText_X=0;
		else if ((int)IntersectionText_X+AssumedIntersectionDebugStringWidth>gbScreen.iWidth) IntersectionText_X=gbScreen.iWidth-AssumedIntersectionDebugStringWidth;
		if ((int)IntersectionText_Y-(AssumedTextHeight+gfontDefault.lineGap)<0) IntersectionText_Y=(AssumedTextHeight+gfontDefault.lineGap);
		else if ((int)IntersectionText_Y+AssumedTextHeight>gbScreen.iHeight) IntersectionText_Y=gbScreen.iHeight-AssumedTextHeight;
		textPoint.Set((int)IntersectionText_X,(int)IntersectionText_Y);
		ExpertMultimediaBase::RString_iDecimalPlacesForToString=1;
		gfontDefault.TypeFast(&gbScreen, textPoint, MoveThePoint, "("+RString_ToString(xInter)+","+RString_ToString(yInter)+")[intersection:"+RMath::IntersectionToString(iLastIntersection)+"]", GFont_GlyphTypePlain, DrawModeCopyAlpha);
		if (iLastIntersection==RMath::IntersectionYes)
			gbScreen.DrawSubpixelArc(xInter,yInter,fRadius,fWidthMultiplier1UnlessEllipse,fRotate,0.0f,360.0f,pixelWhite,fPrecisionIncrement,fPushSpiralPixPerRotation);
		else
			gbScreen.DrawSubpixelArc(xInter,yInter,fRadius,fWidthMultiplier1UnlessEllipse,fRotate,0.0f,360.0f,pixelDarkGray,fPrecisionIncrement,fPushSpiralPixPerRotation);


		//bTest=DrawDebug();
		bTest=true;
		try {
			//gbScreen.SetBrushColor(255,255,255);
			//if (xCursorDown>0 && yCursorDown>0) gbScreen.DrawRect(xCursorDown,yCursorDown,xCursor-xCursorDown,yCursor-yCursorDown);
			//DEBUG THIS--previous line crashes without "if" statement
			gbScreen.DrawSubpixelLine(xfCursorDown,yfCursorDown,xfCursor,yfCursor,debugStartPixel,&debugEndPixel,.5);
		}
		catch (exception& exn) {
			bTest=false;
			ShowExn(exn, "drawing regression tests");
		}
		catch (...) {
			bTest=false;
			ShowUnknownExn("drawing regression tests");
		}

		if (bFirstRun) Console::Error.WriteLine((bTest)?"Success.":"Failed.");

		if (bFirstRun) Console::Error.Write("Drawing animation frame...");
		if (xDest==std::numeric_limits<float>::min()) {
			xDest=(float)gbScreen.iWidth/2.0f;
			yDest=(float)gbScreen.iHeight/2.0f;
		}
		float fRadiansFacingDest=0.0f;
		float xToMove=0.0f;
		float yToMove=0.0f;
		if (FPDIST(xPlayer,yPlayer,xDest,yDest)>=1.0f) {
			playerIsMoving=true;
			fRadiansFacingDest=RMath::ThetaOfXY_Rad(xDest-xPlayer,yDest-yPlayer);
			xToMove=RMath::XOfRTheta_Rad(1.0f,fRadiansFacingDest);
			yToMove=RMath::YOfRTheta_Rad(1.0f,fRadiansFacingDest);
			xPlayer+=xToMove;
			yPlayer+=yToMove;
		}
		else playerIsMoving=false;
		//GBuffer* this_frame_ptr = animTest.GetFramePtr();
		if (animTest.frame_ptr!=nullptr) bTest=animTest.DrawToLargerWithoutCropElseCancel(&gbScreen,IROUNDF(xPlayer-((float)animTest.frame_ptr->iWidth/2.0f)),IROUNDF(yPlayer-((float)animTest.frame_ptr->iHeight/2.0f)),DrawModeBlendAlpha);
		else {
			bTest=false;
			Console::Error.WriteLine("ERROR: current frame is nullptr in animTest, draw not attempted.");
		}
		if (bFirstRun) Console::Error.WriteLine((bTest)?"Success.":"Failed.");

		if (bFirstRun) Console::Error.Write("Drawing extra debug info...");
		//yTextLine=32;


		stringstream ss;
		textPoint.Set(16,32);

		gfontDefault.SetColor(128,128,128);
		ss << "debugStartPixel:" << debugStartPixel.ToString()<<"; debugEndPixel:"<<debugEndPixel.ToString();
		gfontDefault.TypeFast(&gbScreen,textPoint,MoveThePoint, ss.str());


		//yTextLine+=(AssumedTextHeight+gfontDefault.lineGap);
		//textPoint.Set(16,yTextLine);
		//ss.str(std::string());//same as ("") but faster
		//ss << "line:((" << RString_ToString(line0_x1)<<","<<RString_ToString(line0_y1)<<"),"
		//	<<"(" << RString_ToString(line0_x2)<<","<<RString_ToString(line0_y2)<<"))";
		//gfontBlue.TypeFast(gbScreen,textPoint,MoveThePoint, ss.str());

		IPoint tempPoint;

		tempPoint.Set(line0_x1,line0_y1);
		ss.str(std::string());//same as ss.str("") but faster
		ss << "(" << RString_ToString(line0_x1)<<","<<RString_ToString(line0_y1)<<")";
		gfontDefault.SetColor(0,0,255);
		gfontDefault.TypeFast(&gbScreen,tempPoint,MoveThePoint, ss.str());

		tempPoint.Set(line0_x2,line0_y2);
		ss.str(std::string());//same as ss.str("") but faster
		ss <<"(" << RString_ToString(line0_x2)<<","<<RString_ToString(line0_y2)<<")";
		gfontDefault.TypeFast(&gbScreen,tempPoint,MoveThePoint, ss.str());
		gfontDefault.SetColor(255,255,255);

		gfontDefault.marginLeftX=textPoint.X;
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=LineOffset;
		//textPoint.Set(16,yTextLine);
		ss.str(std::string());//same as ss.str("") but faster
		ss << "cursor:(" << RString_ToString(xfCursor)<<","<<RString_ToString(yfCursor)<<")";
		ss << " cursorDown:(" << RString_ToString(xCursorDown)<<","<<RString_ToString(yCursorDown)<<")";
		ss << " cursorDownR:(" << RString_ToString(xDest)<<","<<RString_ToString(yDest)<<")";
		gfontDefault.TypeFast(&gbScreen,textPoint,MoveThePoint, ss.str());

		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=LineOffset;
		//textPoint.Set(16,yTextLine);
		ss.str(std::string());//same as ss.str("") but faster
		ss << "bMouseDown:" << RString_ToString(bMouseDown);
		ss << " bMouseDownR:" << RString_ToString(bMouseDownR);
		gfontDefault.TypeFast(&gbScreen,textPoint,MoveThePoint, ss.str());

		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=LineOffset;
		//textPoint.Set(16,yTextLine);
		ss.str(std::string());//same as ss.str("") but faster
		ss << "RConvert_THETAOFXY_DEG(clicked.x,clicked.y,x,y):" << RString_ToString(RConvert_THETAOFXY_DEG(xfCursor-(float)xCursorDown,yfCursor-(float)yCursorDown));
		gfontDefault.TypeFast(&gbScreen,textPoint,MoveThePoint, ss.str());


		cursor3d.HardLocation(0.0f,0.0f,0.0f);
		float apertureDegrees=40.0f;
		///default is 		640,							480,								0.0f,		-14.836f,5.376f,	0.0f,		-11.77,	90.0f,39.2025f);
		///																						x			y			z			xRot,		yRot,		zRot,	fApertureAngleHeight
		bool Z_UP=true;
		bool X_FORWARD=true;
		camera.SetCamera((float)gbScreen.iWidth,	(float)gbScreen.iHeight,	0.0f,		-14.836f,5.376f,	0.0f,		-11.77f,	90.0f,apertureDegrees, Z_UP, X_FORWARD);

		///DRAW GRID:
		//startMass.HardLocation(-2.0f,0.0f,0.0f);
		for (float yGridIndex=-10.0f; yGridIndex<11.0f; yGridIndex+=1.0f) {
			for (float xGridIndex=-10.0f; xGridIndex<11.0f; xGridIndex+=1.0f) {
				///ACROSS:
				endMass.HardLocation(xGridIndex,yGridIndex,0.0f);
				startMass.HardLocation(xGridIndex+1.0f,yGridIndex,0.0f);
				camera.Point2dFrom3d(startPoint, startMass);
				camera.Point2dFrom3d(endPoint, endMass);
				gbScreen.DrawSubpixelLine(startPoint,endPoint,pixelLightGray,NULL,1.0f);
				///UP:
				endMass.HardLocation(xGridIndex,yGridIndex,0.0f);
				startMass.HardLocation(xGridIndex,yGridIndex+1.0f,0.0f);
				camera.Point2dFrom3d(startPoint, startMass);
				camera.Point2dFrom3d(endPoint, endMass);
				gbScreen.DrawSubpixelLine(startPoint,endPoint,pixelLightGray,NULL,1.0f);
			}
		}

		///Move & Draw follower (3D one):
		camera.DrawBox(&gbScreen, followMass, pixelLightGray, pixelDarkGray); //gbScreen.DrawMass(followMass);
		float followPitch=0.0f;
		float followYaw=0.0f;
		float followTrailUnitsPerFrame=followUnitsPerFrame*.1f;
		bool make_orientation_Z_UP = camera.make_orientation_Z_UP;
		bool make_orientation_X_FORWARD = camera.make_orientation_X_FORWARD;
		Travel3d(followMass.X,      followMass.Y,      followMass.Z,      (double)followPitch,(double)followYaw,(double)followUnitsPerFrame,      make_orientation_Z_UP, make_orientation_X_FORWARD);
		Travel3d(followTrailMass.X, followTrailMass.Y, followTrailMass.Z, (double)followPitch,(double)followYaw,(double)followTrailUnitsPerFrame,	make_orientation_Z_UP, make_orientation_X_FORWARD);

		///DRAW 3D cursor:
		float maximumPitchDegrees=-5.0f; //behavior of Point3dFrom2dAssumingHeight changes according to whether past this value (maximum is zero)
		float targetingHeight=0.0f;
		int iLineRelationshipType=RMath::LineRelationshipIntersectionNotCrossingInRangeCouldNotFinish;
		camera.Point3dFrom2dAssumingHeight(cursor3d,iLineRelationshipType,xfCursor,yfCursor,targetingHeight,maximumPitchDegrees);

		float fMetersFromCamera=15;
		//camera.Point3dFrom2d(Mass3d &pointReturn, float x2D, float y2D, float fMetersFromCamera) //debug only
		//camera.Point3dFrom2d(cursor3d, xfCursor, yfCursor, fMetersFromCamera); //debug only

		camera.DrawBox(&gbScreen,cursor3d,pixelCyan,pixelDarkCyan);
		//cursor3d.HardLocation(cursor3d.X+1.0f,cursor3d.Y,cursor3d.Z); //debug only
		//camera.DrawBox(gbScreen,cursor3d,pixelCyan,pixelDarkCyan);//debug only
		//cursor3d.HardLocation(cursor3d.X-2.0f,cursor3d.Y,cursor3d.Z); //debug only
		//camera.DrawBox(gbScreen,cursor3d,pixelCyan,pixelDarkCyan);//debug only
		camera.DrawDebug(&gbScreen);

		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=LineOffset;
		//textPoint.Set(16,yTextLine);
		gfontDefault.SetColor(0,255,255);//Cyan
		gfontDefault.TypeFast(&gbScreen,textPoint,MoveThePoint,"cursor3d:"+cursor3d.ToString(true));

		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=gfontDefault.arranimGlyphType[GFont_GlyphTypePlain].gbFrame.iHeight*2+LineOffset;
		//textPoint.Set(16,yTextLine);
		gfontDefault.SetColor(255,255,255);//White
		gfontDefault.TypeFast(&gbScreen,textPoint,MoveThePoint,"camera:"+camera.ToString(true));

		gfontDefault.SetColor(255,255,0);//Yellow
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=gfontDefault.arranimGlyphType[GFont_GlyphTypePlain].gbFrame.iHeight*10+LineOffset;
		//textPoint.Set(16,yTextLine);
		gfontDefault.TypeFast(&gbScreen,textPoint, MoveThePoint, "startAngle:"+RString_ToString(anglediffpositiveStartAngle) );
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=LineOffset;
		//textPoint.Set(16,yTextLine);
		gfontDefault.TypeFast(&gbScreen,textPoint, MoveThePoint, "endAngle:SafeAngle360("+RString_ToString(anglediffpositiveEndAngle) +"):"+RString_ToString(RMath::SafeAngle360(anglediffpositiveEndAngle)) );
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=LineOffset;
		//textPoint.Set(16,yTextLine);
		gfontDefault.TypeFast(&gbScreen,textPoint, MoveThePoint, "FANGLEDIFF(startAngle,endAngle):"+RString_ToString(FANGLEDIFF(anglediffpositiveStartAngle,anglediffpositiveEndAngle)) );
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=LineOffset;
		//textPoint.Set(16,yTextLine);
		gfontDefault.TypeFast(&gbScreen,textPoint, MoveThePoint, "ANGLEDIFFPOSITIVE((float)startAngle, (float)endAngle):"+RString_ToString( ANGLEDIFFPOSITIVE((float)anglediffpositiveStartAngle, (float)anglediffpositiveEndAngle) ) );
		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=LineOffset;
		//textPoint.Set(16,yTextLine);
		gfontDefault.TypeFast(&gbScreen,textPoint, MoveThePoint, "ANGLEDIFFPOSITIVE((double)startAngle, (double)endAngle):"+RString_ToString( ANGLEDIFFPOSITIVE((double)anglediffpositiveStartAngle, (double)anglediffpositiveEndAngle) ) );

		anglediffStartAngleIntersectsRadiusAtPoint.X=angleDiffPositivePoint.X;
		anglediffStartAngleIntersectsRadiusAtPoint.Y=angleDiffPositivePoint.Y;
		Travel2d_Polar_Deg(anglediffStartAngleIntersectsRadiusAtPoint.X,anglediffStartAngleIntersectsRadiusAtPoint.Y,anglediffpositiveRadius,anglediffpositiveStartAngle,true);
		anglediffEndAngleIntersectsRadiusAtPoint.X=angleDiffPositivePoint.X;
		anglediffEndAngleIntersectsRadiusAtPoint.Y=angleDiffPositivePoint.Y;
		Travel2d_Polar_Deg(anglediffEndAngleIntersectsRadiusAtPoint.X,anglediffEndAngleIntersectsRadiusAtPoint.Y,anglediffpositiveRadius,anglediffpositiveEndAngle,true);

		gfontDefault.TypeFast_NewLine(&gfontDefault, textPoint);
		//yTextLine+=LineOffset;
		//textPoint.Set(16,yTextLine);
		gfontDefault.SetColor(0,255,0);//Green
		///Green arcs:
		gbScreen.DrawSubpixelArc(angleDiffPositivePoint.X,angleDiffPositivePoint.Y,fRadius,fWidthMultiplier1UnlessEllipse,fRotate,0.0f,360.0f,pixelDarkGreen,fPrecisionIncrement,fPushSpiralPixPerRotation);
		gbScreen.DrawSubpixelArc(anglediffStartAngleIntersectsRadiusAtPoint.X,anglediffStartAngleIntersectsRadiusAtPoint.Y,fRadius,fWidthMultiplier1UnlessEllipse,fRotate,0.0f,360.0f,pixelGreen,fPrecisionIncrement,fPushSpiralPixPerRotation);
		gfontDefault.TypeFast(&gbScreen,textPoint,MoveThePoint,"result of AngleToward resulting start point (should be same as StartAngle%360):"+RString_ToString(
				AngleTowardDestFromSrc_Deg(anglediffStartAngleIntersectsRadiusAtPoint.X,anglediffStartAngleIntersectsRadiusAtPoint.Y,angleDiffPositivePoint.X,angleDiffPositivePoint.Y,true)) );
		gfontDefault.SetColor(255,255,0);//Yellow

		///Yellow lines:
		gbScreen.DrawSubpixelLine(angleDiffPositivePoint.X,angleDiffPositivePoint.Y,anglediffStartAngleIntersectsRadiusAtPoint.X,anglediffStartAngleIntersectsRadiusAtPoint.Y,pixelYellow,nullptr,1.0f);
		gbScreen.DrawSubpixelLine(angleDiffPositivePoint.X,angleDiffPositivePoint.Y,anglediffEndAngleIntersectsRadiusAtPoint.X,anglediffEndAngleIntersectsRadiusAtPoint.Y,pixelYellow,nullptr,1.0f);
		///Yellow arc:
		gbScreen.DrawSubpixelArc(angleDiffPositivePoint.X,angleDiffPositivePoint.Y,anglediffpositiveRadius,1.0f,0.0f,anglediffpositiveStartAngle,anglediffpositiveEndAngle,pixelYellow,1.0f,0.0f);

		anglediffpositiveStartAngle+=.1f;
		anglediffpositiveEndAngle-=.2f;




		if (bFirstRun) Console::Error.WriteLine((bTest)?"Success.":"Failed.");


		if (bFirstRun) Console::Error.Write("Drawing screen...");
		bTest=DrawScreen();

		if (bFirstRun) Console::Error.WriteLine((bTest)?"Success.":"Failed.");

		if (playerIsMoving) {
			if (bFirstRun) Console::Error.Write("Advancing to next frame...");
			bTest=animTest.GotoNextFrame(true);
			if (bFirstRun) Console::Error.WriteLine((bTest)?"Success.":"Failed.");
		}
		if (bFirstRun) Console::Error.Write("Flip backbuffer...");
		//SDL_Flip(screen); //deprecated in SDL2
		SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
		SDL_RenderPresent(sdlRenderer);
		if (bFirstRun) Console::Error.WriteLine("Done.");
		SDL_Delay(1);//TODO: remove this and do timing outside of iterate
	}//end Iterate
	}//end namespace
	using namespace ExpertMultimediaBase;
	//int main(int argc, char *argv[]) {
	int main(int iArgs, char** lpsArg) {
	Console::Error.Write("Initializing display...");
	if (SDL_Init (SDL_INIT_VIDEO) < 0) {
		string sMsg="Couldn't initialize SDL: ";
		string sTemp="";
		sTemp.assign(SDL_GetError());
		sMsg+=sTemp;
		if (ShowError()) cerr<<sMsg<<endl;
		exit (1);
	}
	atexit(SDL_Quit);

	Console::Error.WriteLine("Done.");
	Console::Error.Write("Setting video mode...");

	//screen=SDL_SetVideoMode(iSetScreenW, iSetScreenH, iSetScreenBytesPP*8, SDL_SWSURFACE | SDL_DOUBLEBUF);//deprecated in SDL2 //commented for debug only: | SDL_RESIZABLE);//SDL_FULLSCREEN
	/* Set iSetScreenW x iSetScreenH 32-bits video mode */
	Console::Error.Write("Setting video mode...");
	Console::Error.Flush();
	string sMsg="";
	bool bGood=true;//debug unused except by video initialization
	//screen=SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF); //deprecated in SDL2
	sdlWindow = SDL_CreateWindow("ExpertMultimediaBase Regression Suite",
						  SDL_WINDOWPOS_UNDEFINED,
						  SDL_WINDOWPOS_UNDEFINED,
						  iSetScreenW, iSetScreenH,
						  SDL_WINDOW_OPENGL);//SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
	if (sdlWindow == NULL) {
		sMsg="Couldn't set "+RString_ToString(iSetScreenW)+"x"+RString_ToString(iSetScreenH)+"x32 video mode: \n";
		sMsg+=RString_ToString(SDL_GetError());
		Console::Error.WriteLine("In GameInit -- "+sMsg);
		iErrors++;
		//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
		bGood=false;
		return bGood;
	}
	//SDL_WM_SetCaption ("DXMan", NULL); //deprecated in SDL2
	Console::Error.WriteLine("done.");
	Console::Error.Write("Loading SDL renderer...");
	Console::Error.Flush();
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);//window, index, flags
	Console::Error.WriteLine("done.");

	Console::Error.Write("Creating SDL screen surface (screen)...");
	Console::Error.Flush();
	// "if all this hex scares you, check out SDL_PixelFormatEnumToMasks()!" -- http://wiki.libsdl.org/MigrationGuide#Moving_from_SDL_1.2_to_2.0
	//The four last params below are			RED mask,	GREEN mask,	BLUE mask,	ALPHA mask:
	screen = SDL_CreateRGBSurface(0, iSetScreenW, iSetScreenH, 32,
														0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000);
	if (screen!=NULL) {
		Console::Error.WriteLine("OK (little endian)");
	}
	else {
		Console::Error.Write("trying big endian...");
		Console::Error.Flush();
		screen = SDL_CreateRGBSurface(0, iSetScreenW, iSetScreenH, 32,
														0x0000FF00,	0x00FF0000,	0xFF000000,	0x000000FF);
		if (screen!=NULL) {
			Console::Error.WriteLine("OK (big endian)");
		}
		else {
			Console::Error.WriteLine("FAIL");
		}
	}

	Console::Error.Write("Creating SDL screen texture (sdlTexture)...");
	Console::Error.Flush();
	sdlTexture = SDL_CreateTexture(sdlRenderer,
															  SDL_PIXELFORMAT_ARGB8888,
															  SDL_TEXTUREACCESS_STREAMING,
															  iSetScreenW, iSetScreenH);
	Console::Error.WriteLine("done.");

	if (screen==NULL) {
		Console::Error.WriteLine("Fail!");
		string sMsg="Couldn't set "+RString_ToString(iSetScreenW)+"x"
			+RString_ToString(iSetScreenH)+"x"
			+RString_ToString(iSetScreenBytesPP*8);
		sMsg+=" video mode: ";
		string sTemp="";
		sTemp.assign(SDL_GetError());
		sMsg+=sTemp;
		cerr<<sMsg<<endl;
		ShowError(sMsg,"main init");
		exit(2);
	}
	else Console::Error.WriteLine("Done.");
	//SDL_WM_SetCaption ("SDL MultiMedia Application", NULL);
	Init();
	bDone=false;
	iTickLastIteration=SDL_GetTicks();
	while (!bDone) {
		if (bFirstRun) Console::Error.WriteLine("Entered main event loop.");
		SDL_Event event;
		int iSetWidth;
		int iSetHeight;
		static bool bFirstResize=true;
		while (SDL_PollEvent (&event)) { //can also do OnEvent(SDL_Event* Event) //then later use if (Event->type == SDL_Quit) {} //etc
			switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym==SDLK_a) {
						//DirKeyDown();
						//u32Pressing|=GAMEKEY_LEFT;
					}
					else if (event.key.keysym.sym==SDLK_d) {
						//DirKeyDown();
						//u32Pressing|=GAMEKEY_RIGHT;
					}
					else if (event.key.keysym.sym==SDLK_w) {
						//DirKeyDown();
						//u32Pressing|=GAMEKEY_UP;
					}
					else if (event.key.keysym.sym==SDLK_s) {
						//DirKeyDown();
						//u32Pressing|=GAMEKEY_DOWN;
					}
					else if (event.key.keysym.sym==SDLK_DELETE) {
						////laser keydown
						//u32Pressing|=GAMEKEY_FIRE;
					}
					else if (event.key.keysym.sym==SDLK_END) {
						//u32Pressing|=GAMEKEY_JUMP;
					}
					else if (event.key.keysym.sym==SDLK_ESCAPE) {
						//u32Pressing|=GAMEKEY_EXIT;
					}
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym==SDLK_a) {
						//DirKeyUp();
						//u32Pressing&=(GAMEKEY_LEFT^0xFFFFFFFF);
					}
					else if (event.key.keysym.sym==SDLK_d) {
						//DirKeyUp();
						//u32Pressing&=(GAMEKEY_RIGHT^0xFFFFFFFF);
					}
					else if (event.key.keysym.sym==SDLK_w) {
						//DirKeyUp();
						//u32Pressing&=(GAMEKEY_UP^0xFFFFFFFF);
					}
					else if (event.key.keysym.sym==SDLK_s) {
						//DirKeyUp();
						//u32Pressing&=(GAMEKEY_DOWN^0xFFFFFFFF);
					}
					else if (event.key.keysym.sym==SDLK_DELETE) {
						////iChanLaser=Mix_PlayChannel(2, mcLaser, 0);//chan, sound, num of loops
						//u32Pressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
					}
					else if (event.key.keysym.sym==SDLK_END) {
						//u32Pressing&=(GAMEKEY_JUMP^0xFFFFFFFF);
					}
					else if (event.key.keysym.sym==SDLK_ESCAPE) {
						//u32Pressing&=(GAMEKEY_EXIT^0xFFFFFFFF);
					}
					else if (event.key.keysym.sym==SDLK_F2) {
						stringstream timeSS;
						time_t t = time(0);   // get time now
						struct tm * now = localtime( & t );
						timeSS << (now->tm_year + 1900) << '-'
							<<  ((now->tm_mon<10)?"0":"") << now->tm_mon << "-"
							<<  ((now->tm_mday<10)?"0":"") << now->tm_mday << " "
							<<  ((now->tm_hour<10)?"0":"") << now->tm_hour << "_"
							<<  ((now->tm_min<10)?"0":"") << now->tm_min << "_"
							<<  ((now->tm_sec<10)?"0":"") << now->tm_sec;
						string screenShotFileName=timeSS.str()+".tga";
						Console::Error.Write("Saving screenshot to "+screenShotFileName+"...");
						Console::Error.Flush();
						gbScreen.FillAlpha(255);
						bool bSaved=gbScreen.Save(screenShotFileName);
						Console::Error.WriteLine(bSaved?"OK":"FAIL");
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					//SDL_GetRelativeMouseState(&xCursorDown, &yCursorDown);//SDL_GetMouseState(&m3dEnt.X, &m3dEnt.Y);
					if (event.button.button == SDL_BUTTON_LEFT) {
						xCursorDown = event.motion.x;
						yCursorDown = event.motion.y;
						xfCursorDown=xCursorDown;
						yfCursorDown=yCursorDown;
						bMouseDown=true;
					}
					else {
						bMouseDownR=true;
						xDest=(float)event.motion.x;
						yDest=(float)event.motion.y;
						//u32Pressing|=GAMEKEY_JUMP;
						static bool bFirstRightClick=true;
						if (bFirstRightClick) Console::Error.WriteLine("Detected the first right-click.");
						bFirstRightClick=false;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {
						bMouseDown=false;
					}
					else {
						bMouseDownR=false;
						//u32Pressing&=GAMEKEY_JUMP^0xFFFFFFFF;
					}
					break;
				case SDL_MOUSEMOTION:
					xCursor=event.motion.x;
					yCursor=event.motion.y;
					xfCursor=xCursor;
					yfCursor=yCursor;
					bMouseMove=true;
					break;
				case SDL_QUIT:
					bDone=true;
					break;
				//case SDL_VIDEORESIZE:
					/* commented for debug only
					iSetWidth=event.resize.w;
					iSetHeight=event.resize.h;
					if ( abs(iSetWidth -gbScreen.iWidth)>2
						 || abs(iSetHeight-gbScreen.iHeight)>2 ) {
						if (bFirstResize) Console::Error.Write("Resizing screen...");
						screen=SDL_SetVideoMode(iSetWidth, iSetHeight, iSetScreenBytesPP*8, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);//, SDL_OPENGL)
						gbScreen.Init(iSetWidth,iSetHeight,gbScreen.iBytesPP,true);
						if (bFirstResize) Console::Error.WriteLine("Done");
						bFirstResize=false;
					}
					*/
				//	break;
				default:
					break;
			}
		}
		Iterate();
		bFirstRun=false;
	}//end while !bDone
	Shutdown();
	return 0;
}//end main (using ExpertMultimediaBase [see above for part of it])
#endif
