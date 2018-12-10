#ifndef MAIN_C
#define MAIN_C

#include <cstdlib>
#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <anim.cpp>
#include <base.cpp>
#include <byter.cpp>
#include <dotnetfake.cpp>
#include <gbuffer.cpp>
#include <gradient.cpp>
#include <targa.cpp>
#include <gbuffer.cpp>
using namespace std;

namespace ExpertMultimediaBase {

//using namespace ExpertMultimediaBase;

	using namespace std;

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
	Anim animTest;
	

	static bool Init() {
		bool bGood=true;
		animTest.LoadSeq("C:\\Documents and Settings\\Owner\\My Documents\\Projects-cpp\\DXMan\\bin\\images\\expart0000.tga",16);
		return bGood;
	}
	static bool Shutdown() {
		bool bGood=true;
		return bGood;
	}
	/* This function draws to the screen; replace this with your own code! */
	static void Iterate() {
	    static int direction = 0;
	    static int value = 0;
	    static int which = 0;
	    SDL_Rect sdlrectPixelNow;
	    Uint32 dwColor;
		animTest.Goto//TODO: finish this I'm tired now
	    /* Create a black background */
	    dwColor = SDL_MapRGB (screen->format, 0, 0, 0);
	    SDL_FillRect (screen, NULL, dwColor);

	    /* Determine which dwColor the layer should have */
	    if (direction == 0) {
	        value += 2;
	        if (value >= 256) {
	            value = 255;
	            direction = 1;
	        }
	    }
	    else {
	        value -= 2;
	        if (value <= 5) {
	            value = 0;
	            direction = 0;
	            which++;
	            if (which == 5)
	                which = 0;
	        }
	    }

	    /* Draw a layer with variable dwColor */
	    switch (which) {
	      case 0:
	          dwColor = SDL_MapRGB (screen->format, value, 0, 0);
	          break;
	      case 1:
	          dwColor = SDL_MapRGB (screen->format, 0, value, 0);
	          break;
	      case 2:
	          dwColor = SDL_MapRGB (screen->format, 0, 0, value);
	          break;
	      case 3:
	          dwColor = SDL_MapRGB (screen->format, value, value, value);
	          break;
	      case 4:
	          dwColor = SDL_MapRGB (screen->format, value, 0, value);
	          break;
	    }

	    sdlrectPixelNow.w = screen->w / 2;
	    sdlrectPixelNow.h = screen->h / 2;
	    sdlrectPixelNow.x = (screen->w / 2) - (sdlrectPixelNow.w / 2);
	    sdlrectPixelNow.y = (screen->h / 2) - (sdlrectPixelNow.h / 2);
	    SDL_FillRect (screen, &sdlrectPixelNow, dwColor);


	    /* Make sure everything is displayed on screen */
	    SDL_Flip (screen);
	    /* Don't run too fast */
	    SDL_Delay (1);
	}


//	int main(int argc, char *argv[]) {
}//end namespace
using namespace ExpertMultimediaBase;
int main(int iArgs, char** lpsArg) {
	    char *msg;
	    bool bDone;

	    /* Initialize SDL */
	    if (SDL_Init (SDL_INIT_VIDEO) < 0) {
			string sMsg="Couldn't initialize SDL: ";
			string sTemp="";
			sTemp.assign(SDL_GetError());
			sMsg+=sTemp;
			if (ShowError()) cerr<<sMsg<<endl;
	        exit (1);
	    }
	    atexit(SDL_Quit);

	    /* Set 640x480 16-bits video mode */
	    screen=SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);
	    if (screen == NULL) {
			string sMsg="Couldn't set 640x480x16 video mode: ";
			string sTemp="";
			sTemp.assign(SDL_GetError());
			sMsg+=sTemp;
			if (ShowError()) cerr<<sMsg<<endl;
	        exit (2);
	    }
	    //SDL_WM_SetCaption ("SDL MultiMedia Application", NULL);
		Init();
	    bDone=false;
	    while (!bDone) {
	        SDL_Event event;

	        /* Check for events */
	        while (SDL_PollEvent (&event)) {
	            switch (event.type) {
	            case SDL_KEYDOWN:
	                break;
	            case SDL_QUIT:
	                bDone=true;
	                break;
	            default:
	                break;
	            }
	        }
	        Iterate();
	    }
	    Shutdown();
	    return 0;
	}


#endif
