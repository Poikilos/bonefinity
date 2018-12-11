#ifndef MAIN_C
#define MAIN_C

#include <cstdlib>
#include <iostream>
#include <string>
#include <SDL/SDL.h>

#include <sprite.cpp>
#include <world.cpp>
#include <anim32bgra.cpp>
#include <gbuffer32bgra.cpp>
#include <targa.cpp>
#include <byter.cpp>
#include <base.cpp>
#include <frameworkdummy.cpp>

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

	SDL_Surface *screen=NULL;
	Anim32BGRA animTest;
	GBuffer32BGRA gbScreen;
	GBuffer32BGRA gbOverlays;
	bool bFirstRun=true;
	bool bDone=false;
	int iTickLastIteration=0;
	int iSetScreenW=640;
	int iSetScreenH=480;
	int iSetScreenBytesPP=4;


	static bool Init() {
		Console::Write("Loading data...");
		bool bGood=true;
		int iFrames=16;
		animTest.LoadSeq("C:\\My Documents\\Projects-cpp\\DXMan\\bin\\images\\hero*.tga");
		iFrames=animTest.lFramesCached;
		Console::Write("Found "+ToString(iFrames)+" frames...");
		Console::WriteLine();
		Console::WriteLine(animTest.Dump(true));
		Console::Write("Init screen GBuffer...");
		gbScreen.Init(iSetScreenW,iSetScreenH,4);//assumes 32-bit GBuffer is needed
		Console::WriteLine("Done.");
		Console::Write("Loading test ini and running dump test...");
		Variables vsNow;
		Variable* pvNow=null;
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
				if (pvNow!=null) bGood=pvNow->Get(sTemp,i1st);
				else { bGood=false; sTemp="(VAR NAME NOT FOUND)"; }
				ofNow<<sTemp<<endl;
			}
			else {
				for (int i2nd=0; i2nd<i2ndDimLenNow; i2nd++) {
					pvNow=vsNow.PointerOf(sName);
					if (pvNow!=null) bGood=pvNow->Get(sTemp,i1st,i2nd);
					else { bGood=false; sTemp="(VAR NAME NOT FOUND)"; }
					ofNow<<sTemp;
					if (i2nd+1<i2ndDimLenNow) ofNow<<", ";
				}
				ofNow<<endl;
			}
		}
		Console::WriteLine("Done.");
		Console::Write("Closing test ini...");
		ofNow.close();
		Console::WriteLine("Done.");
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
			if (gbScreen.BytePtrStart()==null) {
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
				if (bFirstRun) Console::Write("debug test...");

				//Uint8* lpbyScreen=(Uint8*)screen->pixels;//debug only
				//*((Uint32*)lpbyScreen)=0xFFFFFF; //debug only
				//*(Uint32*)(&lpbyScreen[32*iDestStride+32*iDestBytesPP])=0xFFFFFF; //debug only
				//gbScreen.Fill(255);//debug only

				if (bFirstRun) Console::Write("copy...");
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
		catch (char* szExn) {
			ShowAndDeleteException(szExn,"DrawScreen()");
			bGood=false;
		}
		catch (...) {
			ShowUnknownException("DrawScreen()");
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

		if (bFirstRun) Console::Write("Starting Draw background color...");
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
		SDL_FillRect (screen, &sdlrectPixelNow, u32Color);
		if (bFirstRun) Console::WriteLine("Done.");
		if (bFirstRun) Console::Write("Clearing screen...");
		bool bTest=gbScreen.Fill(0);
		if (bFirstRun) Console::WriteLine((bTest)?"Success.":"Failed.");
		if (bFirstRun) Console::Write("Drawing frame...");
		bTest=animTest.DrawToLargerWithoutCropElseCancel(gbScreen,gbScreen.iWidth/2,gbScreen.iHeight/2,DrawModeBlendAlpha);
		if (bFirstRun) Console::WriteLine((bTest)?"Success.":"Failed.");
		if (bFirstRun) Console::Write("Drawing screen...");
		bTest=DrawScreen();
		if (bFirstRun) Console::WriteLine((bTest)?"Success.":"Failed.");
		if (bFirstRun) Console::Write("Advancing to next frame...");
		bTest=animTest.GotoNextFrame(true);
		if (bFirstRun) Console::WriteLine((bTest)?"Success.":"Failed.");
		if (bFirstRun) Console::Write("Flip backbuffer...");
		SDL_Flip(screen);
		if (bFirstRun) Console::WriteLine("Done.");
		SDL_Delay(1);//TODO: remove this and do timing outside of iterate
	}//end Iterate
}//end namespace
using namespace ExpertMultimediaBase;
//int main(int argc, char *argv[]) {
int main(int iArgs, char** lpsArg) {
	Console::Write("Initializing display...");
	if (SDL_Init (SDL_INIT_VIDEO) < 0) {
		string sMsg="Couldn't initialize SDL: ";
		string sTemp="";
		sTemp.assign(SDL_GetError());
		sMsg+=sTemp;
		if (ShowError()) cerr<<sMsg<<endl;
	exit (1);
	}
	atexit(SDL_Quit);

	Console::WriteLine("Done.");
	Console::Write("Setting video mode...");
	screen=SDL_SetVideoMode(iSetScreenW, iSetScreenH, iSetScreenBytesPP*8, SDL_SWSURFACE | SDL_DOUBLEBUF);//commented for debugonly: | SDL_RESIZABLE);//SDL_FULLSCREEN
	if (screen==NULL) {
		Console::WriteLine("Fail!");
		string sMsg="Couldn't set "+ToString(iSetScreenW)+"x"
			+ToString(iSetScreenH)+"x"
			+ToString(iSetScreenBytesPP*8);
		sMsg+=" video mode: ";
		string sTemp="";
		sTemp.assign(SDL_GetError());
		sMsg+=sTemp;
		cerr<<sMsg<<endl;
		ShowError(sMsg,"main init");
		exit(2);
	}
	else Console::WriteLine("Done.");
	//SDL_WM_SetCaption ("SDL MultiMedia Application", NULL);
	Init();
	bDone=false;
	iTickLastIteration=SDL_GetTicks();
	while (!bDone) {
		if (bFirstRun) Console::WriteLine("Entered main event loop.");
		SDL_Event event;
		int iSetWidth;
		int iSetHeight;
		static bool bFirstResize=true;
		while (SDL_PollEvent (&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					break;
				case SDL_QUIT:
					bDone=true;
					break;
				case SDL_VIDEORESIZE:
					/* commented for debug only
					iSetWidth=event.resize.w;
					iSetHeight=event.resize.h;
					if ( abs(iSetWidth -gbScreen.iWidth)>2
						 || abs(iSetHeight-gbScreen.iHeight)>2 ) {
						if (bFirstResize) Console::Write("Resizing screen...");
						screen=SDL_SetVideoMode(iSetWidth, iSetHeight, iSetScreenBytesPP*8, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);//, SDL_OPENGL)
						gbScreen.Init(iSetWidth,iSetHeight,gbScreen.iBytesPP,true);
						if (bFirstResize) Console::WriteLine("Done");
						bFirstResize=false;
					}
					*/
					break;
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
