#ifndef WORLD_H
#define WORLD_H

#include <anim32bgra.h>
//#include "C:\My Documents\Projects-cpp\Base\anim32bgra.h"

using namespace std;

namespace ExpertMultimediaBase {

class World {
private:
	//runtime vars (to save):
	string sAnimsFile;
	string sSpritesFile;
	string sWorldFile;
	string sBasePath;
	//save-only (non-runtime) vars (commented purposely)
	//runtime-only vars:

	void InitNull();
	void SafeFree();
public:
	//runtime vars (to save):
	//string sBasePath;
	//runtime-only vars:
	Anim32BGRA* animarr;
	Sprite* spritearr;

	World();
	~World();
	//bool Init();
	bool Load(string sWorldFileRelToBasePath, string sBasePathWithEndSlash);
	bool Pause(bool val);
};

World world;

}//end namespace
#endif
