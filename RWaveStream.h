#ifndef WAVER_H
#define WAVER_H

#include <SDL.h>
#include <SDL_mixer.h>
//#include "E:\Projects-cpp\Base\anim32bgra.h"

using namespace std;

namespace ExpertMultimediaBase {

	class Waver {
	private:
		//runtime vars (to save):
		//save-only (non-runtime) vars (commented purposely)
		//runtime-only vars:
		void InitNull();
		void SafeFree();
	public:
		//runtime vars (to save):
		//string sBasePath;
		//runtime-only vars:
		Waver();
	};
}//end namespace
#endif
