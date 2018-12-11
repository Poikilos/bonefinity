#ifndef WORLD_H
#define WORLD_H

#include <RAnim_bgra32.h>
#include <RSprite_bgra32.h>


namespace ExpertMultimediaBase {

	class World {
	private:
		//runtime vars (to save):
		std::string sAnimsFile;
		std::string sSpritesFile;
		std::string sWorldFile;
		std::string sBasePath;
		//save-only (non-runtime) vars (commented purposely)
		//runtime-only vars:

		void InitNull();
		void SafeFree(std::string DebugNote);
	public:
		//runtime vars (to save):
		//string sBasePath;
		//runtime-only vars:
		Anim* arranim;
		Sprite* arrsprite;

		World();
		~World();
		//bool Init();
		bool Load(std::string sWorldFileRelToBasePath, std::string sBasePathWithEndSlash);
		bool Pause(bool val);
	};

	World world;

}//end namespace
#endif
