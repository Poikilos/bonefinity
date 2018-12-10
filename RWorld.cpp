#ifndef WORLD_CPP
#define WORLD_CPP

#include <world.h>
//#include "C:\My Documents\Projects-cpp\Base\world.h"

using namespace std;

namespace ExpertMultimediaBase {
	//private methods:
	void World::InitNull() {
		animarr=null;
	}
	void World::SafeFree() {
		ExpertMultimediaBase::SafeFree(animarr);
	}
	//public methods:
	World::World() {
		InitNull();
		//Init();
	}
	World::~World() {
		SafeFree();
	}
	//bool World::Init() {
	//	return Load("world","data");
	//}
	bool World::Load(string sWorldFileRelToBasePath, string sBasePathWithEndSlash) {

	}
    bool World::Pause(bool val) {

	}
}//end namespace
#endif
