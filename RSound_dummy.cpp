#include <RSound_dummy.h>

using namespace std;
//using namespace audiere;

namespace ExpertMultimediaBase {

	///#region RSoundBuffer

	RSoundBuffer::RSoundBuffer() {
		InitNull();
	}

	void RSoundBuffer::InitNull() {
		IsLoaded=false;
		Name="";
		IsMusic=false;
		FilePath="";
	}

	RSoundBuffer::~RSoundBuffer() {
		//dtor
		Dispose();
	}

	void RSoundBuffer::Dispose() {
		///TODO: unload the sound
	}

	bool RSoundBuffer::GetIsLoaded() {
		return IsLoaded;
	}

	string RSoundBuffer::GetFilePath() {
		return FilePath;
	}

	//void RSoundBuffer::SetFilePath(string val) {
		//FilePath = val;
	//}
	void RSoundBuffer::LoadFile(string load_FilePath, string SoundName) {
		Name = SoundName;
		if (IsLoaded) {
			Dispose();
		}
		FilePath = load_FilePath;
		///TODO: load a file into the buffer
		IsLoaded=true;
	}

	///#endregion RSoundBuffer


	///#region RSoundBufferInstance

	RSoundBufferInstance::RSoundBufferInstance() {
		//ctor
		InitNull();
	}

	void RSoundBufferInstance::InitNull() {
		Position[0]=0.0;
		Position[1]=0.0;
		Position[2]=0.0;

		Velocity[0]=0.0;
		Velocity[1]=0.0;
		Velocity[2]=0.0;

		IsLooped=false;
		VolumeRatio=0.0;
		PitchRatio=0.0;
		MillisecondsLife=LONG_MAX;
		IPlayBufferIndex=-1;
		RevolveAroundX=0.0;
		RevolveAroundY=0.0;
		RevolveAroundZ=0.0;
		RevolveXPerMS=0.0;
		RevolveYPerMS=0.0;
		RevolveZPerMS=0.0;
		AngleX=0.0;
		AngleY=0.0;
		AngleZ=0.0;
	}

	RSoundBufferInstance::~RSoundBufferInstance() {
		//dtor
	}
	void RSoundBufferInstance::Dispose() {
		///TODO: unload the sound
	}

	float RSoundBufferInstance::GetX() {
		return Position[0];
	}

	void RSoundBufferInstance::SetX(float val) {
		Position[0] = val;
	}

	float RSoundBufferInstance::GetY() {
		return Position[1];
	}

	void RSoundBufferInstance::SetY(float val) {
		Position[1] = val;
	}

	float RSoundBufferInstance::GetZ() {
		return Position[2];
	}

	void RSoundBufferInstance::SetZ(float val) {
		Position[2] = val;
	}

	float RSoundBufferInstance::GetVolumeRatio() {
		return VolumeRatio;
	}

	void RSoundBufferInstance::SetVolumeRatio(float val) {
		VolumeRatio = val;
	}

	int RSoundBufferInstance::GetIPlayBufferIndex() {
		return IPlayBufferIndex;
	}

	void RSoundBufferInstance::SetIPlayBufferIndex(int val) {
		IPlayBufferIndex = val;
	}
	string RSoundBufferInstance::getName() {
		return this->Name;
	}

	///#endregion RSoundBufferInstance


	///#region RSoundListener
	RSoundListener::RSoundListener() {
		//ctor
		InitNull();
	}
	RSoundListener* RSoundListener::new_RSoundListener(float atX, float atY, float atZ, float upX, float upY, float upZ) {
		RSoundListener* result = new RSoundListener();
		result->InitNull();
		result->Init(atX, atY, atZ, upX, upY, upZ, 0,0,0, 0,0,0);
		return result;
	}

	void RSoundListener::Init(float atX, float atY, float atZ, float upX, float upY, float upZ, float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ) {
		float newOrientation[6];
		float newPosition[3];
		float newVelocity[3];

		newOrientation[0]=atX;
		newOrientation[1]=atY;
		newOrientation[2]=atZ;
		newOrientation[3]=upX;
		newOrientation[4]=upY;
		newOrientation[5]=upZ;

		newPosition[0]=positionX;
		newPosition[1]=positionY;
		newPosition[2]=positionZ;

		newVelocity[0]=velocityX;
		newVelocity[1]=velocityY;
		newVelocity[2]=velocityZ;
		Init(newOrientation, newPosition, newVelocity);
	}

	void RSoundListener::Init(float setOrientation[], float setPosition[], float setVelocity[]) {
		///TODO: exception handling
		Orientation[1]=setOrientation[1];
		Orientation[2]=setOrientation[2];
		Orientation[3]=setOrientation[3];
		Orientation[4]=setOrientation[4];
		Orientation[5]=setOrientation[5];

		Position[0]=setPosition[0];
		Position[1]=setPosition[1];
		Position[2]=setPosition[2];

		Velocity[0]=setVelocity[0];
		Velocity[1]=setVelocity[1];
		Velocity[2]=setVelocity[2];
		///TODO: actually create a listener here
		IsLoaded=true;
	}

	void RSoundListener::InitNull() {
		IsLoaded=false;
		Orientation[0]=0.0;
		Orientation[1]=0.0;
		Orientation[2]=0.0;
		Orientation[3]=0.0;
		Orientation[4]=0.0;
		Orientation[5]=0.0;

		Position[0]=0.0;
		Position[1]=0.0;
		Position[2]=0.0;

		Velocity[0]=0.0;
		Velocity[1]=0.0;
		Velocity[2]=0.0;
	}

	void RSoundListener::Dispose() {
		if (IsLoaded) {
			///TODO: actually unload anything where necessary
			IsLoaded=false;
		}
		this->InitNull();
	}

	RSoundListener::~RSoundListener() {
		//dtor
		Dispose();
	}

	bool RSoundListener::GetIsLoaded() {
		return IsLoaded;
	}
	///#endregion RSoundListener

	///#region RSound
	RSound::RSound() {
		//ctor
		InitNull();

		//audiere example:
		//bool soundIsStreamed=false; //false to load sounds into memory
		//AudioDevicePtr soundDeviceADP(OpenDevice());

		//OutputStreamPtr introOSP=null;
		//OutputStreamPtr endingOSP=null;
		//OutputStreamPtr level1OSP=null;

		//OutputStreamPtr introOSP(OpenSound(soundDeviceADP, "music/MixMystery-DXMan-Intro.ogg", soundIsStreamed));
		//OutputStreamPtr endingOSP(OpenSound(soundDeviceADP, "music/MixMystery-DXMan-Ending.ogg", soundIsStreamed));
		//OutputStreamPtr level1OSP(OpenSound(soundDeviceADP, "music/MixMystery-DXMan-Invasion.ogg", soundIsStreamed));

		//AudioDevicePtr device(OpenDevice());
		//OutputStreamPtr sound(OpenSound(device, filename.c_str(), false));
	}

	void RSound::InitNull() {
		this->listener = nullptr;
		IsMuted=false;
		SoundCount=0;
		IsLoaded=false;
	}

	RSoundListener* RSound::GetListener() {
		return listener;
	}

	void RSound::Start(float cameraOrientation[], float cameraPosition[], float cameraVelocity[]) {
		if (IsLoaded) cerr << "WARNING in RSound::Start: audio already loaded" << endl;
		this->Dispose();
		if (this->listener==nullptr) this->listener = new RSoundListener();
		else this->listener->Dispose();
		///TODO: dispose all RSoundBuffer objects if any
		///TODO: dispose all RSoundBufferInstance objects if any
		this->listener->Init(cameraOrientation, cameraPosition, cameraVelocity);
		IsLoaded=true;//TODO: only say loaded if really loaded
	}

	RSound::~RSound() {
		//dtor
		Dispose();
	}

	void RSound::Dispose() {
		if (IsLoaded) {
			IsLoaded=false;
			if (this->listener!=nullptr) {
				this->listener->Dispose();
				delete this->listener;
				this->listener = nullptr;
			}
			///TODO: actually unload anything where necessary
		}
		else cerr << "WARNING: RSound was already disposed" << endl;
	}

	///#region RSound get/set
	int RSound::GetSoundCount() {
		return SoundCount;
	}

	void RSound::SetSoundCount(int val) {
		SoundCount = val;
	}

	bool RSound::GetIsLoaded() {
		return IsLoaded;
	}

	//void RSound::SetIsRunning(bool val) {
	//	IsRunning = val;
	//}

	bool RSound::GetIsMuted() {
		return IsMuted;
	}

	void RSound::SetIsMuted(bool val) {
		IsMuted = val;
	}
	///#endregion get/set

	void RSound::LoadFile(string SoundName, string FileName, bool IsMusic) {
		///TODO: actually load a music file here

	}
	void RSound::PlayAsMusic(string SoundName) {
		PlayAsMusic(SoundName,true,1.0);
	}
	void RSound::StopMusic() {
		///TODO: actually stop music
	}
	void RSound::PlayAsMusic(string SoundName, bool IsLooped, float VolumeRatio) {
		///TODO: actually play a RSoundBufferInstance now if one by SoundName exists else output error
	}
	void RSound::PlayAsMusicOnceThenResumePrevious(string SoundName, float VolumeRatio) {
		///TODO: actually play a RSoundBufferInstance now if one by SoundName exists else output error
		///TODO: actually add previously playing song to queue (if none then don't)
	}
	void RSound::PlayAsSound(string SoundName, string EmitterName, string InstanceName, float positionVector3[], float velocityVector3[]) {
		RSound::PlayAsSound(SoundName, EmitterName, InstanceName, positionVector3, velocityVector3, false);
	}
	void RSound::PlayAsSound(string SoundName, string EmitterName, string InstanceName, float positionVector3[], float velocityVector3[], bool IsLooped) {
		///TODO: actually play a RSoundBufferInstance now if one by SoundName exists else output error
	}
	void RSound::MoveEmitter(string EmitterName, float positionVector3[], float velocityVector3[]) {
		///TODO: actually move a RSoundBufferInstance now if one by SoundName exists else output error
	}
	void RSound::StopInstance(string InstanceName) {
		///TODO: actually stop a RSoundBufferInstance using loop that checks thisInstance.getName()
	}

	void RSound::ClearAllInstances() {
		ClearSoundInstances();
		ClearMusicInstances();
	}
	void RSound::ClearSoundInstances() {
		///TODO: actually clear sound instances
	}
	void RSound::ClearMusicInstances() {
		///TODO: actually clear music instances
	}

	///#endregion RSound

}//end namespace ExpertMultimediaBase
