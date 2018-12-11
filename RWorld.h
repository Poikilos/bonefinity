#ifndef WORLD_H
#define WORLD_H

#include <RAnim_bgra32.h>
#include <RSprite_bgra32.h>
//#include "E:\Projects-cpp\Base\RAnim_bgra32.h"

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
	Anim* arranim;
	Sprite* arrsprite;

	World();
	~World();
	//bool Init();
	bool Load(string sWorldFileRelToBasePath, string sBasePathWithEndSlash);
	bool Pause(bool val);
};

World world;

}//end namespace
#endif
