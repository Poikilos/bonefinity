#ifndef WORLD_CPP
#define WORLD_CPP

#include <RWorld.h>


using namespace std;

namespace ExpertMultimediaBase {
	//private methods:
	void World::InitNull() {
		arranim=null;
	}
	void World::SafeFree() {
		ExpertMultimediaBase::SafeFree(arranim);
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
		bool bGood=false;
		Console::Error.WriteLine("NOT YET IMPLEMENTED: World::Load");
		return bGood;
	}
	bool World::Pause(bool val) {
		bool bGood=false;
		Console::Error.WriteLine("NOT YET IMPLEMENTED: World::Pause");
		return bGood;
	}
}//end namespace
#endif
