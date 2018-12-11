#include <RSound_audiere.h>

using namespace std;
using namespace audiere;

namespace ExpertMultimediaBase {

	///#region RSoundBuffer

	RSoundBuffer::RSoundBuffer() {
		InitNull();
	}

	void RSoundBuffer::InitNull() {
		this->source=nullptr;
		this->enable=false;
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
		//unload the sound
		if (source!=(audiere::SampleSourcePtr)nullptr) { //TODO: is this ok?
			//delete source; //TODO: is not deleting ok?
			source=nullptr;
		}
		this->InitNull();
		this->enable=false;
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
		if (source!=(SampleSourcePtr)nullptr) { //TODO: is this ok?
			cout << "WARNING: source was unexpectedly present in LoadFile '"<<load_FilePath<<"', so manually deleting."<<endl;
			//delete source; //TODO: is not deleting ok?
			source = nullptr;
		}
		source = audiere::OpenSampleSource(load_FilePath.c_str()); //for unicode, use wx--see https://github.com/kg/Audiere/blob/master/examples/wxPlayer/DeviceFrame.cpp
		if (source) {
			FilePath = load_FilePath;
			IsLoaded=true;
			enable=true;
		}
		else {
			cout << "WARNING: sound source could not be loaded: '"<<load_FilePath<<"'"<<endl;
		}
	}

	SampleSourcePtr RSoundBuffer::get_source() {
		return this->source;
	}

	///#endregion RSoundBuffer


	///#region RSoundBufferInstance

	RSoundBufferInstance::RSoundBufferInstance() {
		//ctor
		InitNull();
	}

	void RSoundBufferInstance::InitNull() {
		this->source = nullptr; //managed elsewhere
		this->device = nullptr; //managed elsewhere
		this->stream = nullptr; //managed by this class
		this->enable=false;
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
		this->Dispose();
	}

	void RSoundBufferInstance::Dispose() {
		//unload the sound
		if (stream!=(OutputStreamPtr)nullptr) { //TODO: is this ok?
			//delete stream; //TODO: is not deleting ok?
			stream=nullptr;
		}
		this->InitNull();
		this->enable=false;
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
	bool RSoundBufferInstance::GetEnable() {
		return this->enable;
	}
	void RSoundBufferInstance::SetEnable(bool val) {
		this->enable = val;
	}

	string RSoundBufferInstance::getName() {
		return this->Name;
	}
	void RSoundBufferInstance::play() {
		if (this->stream==(OutputStreamPtr)nullptr) { //TODO: is this ok?
			if (this->device!=(AudioDevicePtr)nullptr) { //TODO: is this ok?
				if (this->source!=(SampleSourcePtr)nullptr) { //TODO: is this ok?
					this->stream = audiere::OpenSound(this->device, this->source, true);
				}
				//else cerr << "ERROR: tried play() but source was null" << endl;
			}
			//else cerr << "ERROR: tried play() but device was null" << endl;
		}
		if (this->stream) {
			this->stream->play();
		}
		//else cerr << "ERROR: tried play() but buffer could not be instantiated as stream" << endl;
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
		///TODO: exception handling and null checks
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
		this->has_device = false;
		this->listener = nullptr;
		this->device = nullptr;
		this->buffers = nullptr;
		this->instances = nullptr;
		SoundCount = 0;
		InstanceCount = 0;
		MaxInstanceCount = 20;
		MaxSoundCount = 15;
		IsMuted=false;
		SoundCount=0;
		IsLoaded=false;
	}

	RSoundListener* RSound::GetListener() {
		return listener;
	}

	void RSound::Start(float cameraOrientation[], float cameraPosition[], float cameraVelocity[]) {
		if (IsLoaded) {
			cerr << "WARNING in RSound::Start: audio already loaded. Disposing...";
			cerr.flush();
			this->Dispose();
			cerr << "done disposing RSound." << endl;
		}
		cerr << "RSound::Start...";
		cerr.flush();
		if (this->listener==nullptr) this->listener = new RSoundListener();
		else this->listener->Dispose();
		///TODO: dispose all RSoundBuffer objects if any
		///TODO: dispose all RSoundBufferInstance objects if any
		this->listener->Init(cameraOrientation, cameraPosition, cameraVelocity);
		const char* device_name = "hw:0,0"; //""; //specify, else leave blank
		//if blank, results is "/dev/dsp: No such file or directory. Correct device is:
		//hw:0,0
		//(determined via cat /proc/asound/pcm)
		//see https://bbs.archlinux.org/viewtopic.php?id=99234
		//this->device = OpenDevice(device_name, "buffer=100,rate=44100");
		//this->device = new AudioDevicePtr(OpenDevice());
		//below is based on wxPlayer (Audiere example)
		if (this->device = audiere::OpenDevice("pulse")) { //, parameters.c_str()
			this->has_device = true;
			cerr << "found device " << this->device->getName() << "...";
			cerr.flush();
		}
		if (!this->has_device) {
			if (this->device = audiere::OpenDevice("autodetect")) { //, parameters.c_str()
				this->has_device = true;
				cerr << "found device " << this->device->getName() << "...";
				cerr.flush();
			}
		}
		if (!this->has_device) {
			cerr << "autodetect failed, trying null device...";
			cerr.flush();
			if (this->device = audiere::OpenDevice("null")) { //, parameters.c_str()
				this->has_device = true;
				cerr << "found device " << this->device->getName() << "...";
				cerr.flush();
			}
		}

		if (this->device) {
			buffers = new RSoundBuffer *[MaxSoundCount];
			for (int i=0; i<MaxSoundCount; i++) buffers[i] = nullptr;
			instances = new RSoundBufferInstance *[MaxInstanceCount];
			for (int i=0; i<MaxInstanceCount; i++) instances[i] = nullptr;
			IsLoaded=true;//TODO: only say loaded if really loaded
			cerr << "OK" << endl;
		}
		else {
			cerr << "FAILED at audiere OpenDevice()" << endl;
			//return EXIT_FAILURE;
		}
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
			if (buffers!=nullptr) {
				for (int i=0; i<SoundCount; i++) {
					if (buffers[i]!=nullptr) {
						buffers[i]->Dispose();
						delete buffers[i];
						buffers[i] = nullptr;
					}
				}
				delete buffers;
				buffers = nullptr;
			}
			if (instances!=nullptr) {
				for (int i=0; i<InstanceCount; i++) {
					if (instances[i]!=nullptr) {
						instances[i]->Dispose();
						delete instances[i];
						instances[i] = nullptr;
					}
				}
				delete instances;
				instances = nullptr;
			}
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
	bool LoadFile_out_enable = true;
	void RSound::LoadFile(string SoundName, string FileName, bool IsMusic) {
		RSoundBuffer* this_buffer = nullptr;
		if (buffers!=nullptr) {
			if (this->SoundCount < this->MaxSoundCount) {
				buffers[this->SoundCount] = new RSoundBuffer();
				this_buffer = buffers[this->SoundCount];
				this->SoundCount++;
			}
			else {
				for (int i=0; i<SoundCount; i++) {
					if (buffers[i]==nullptr || !buffers[i]->enable) {
						if (buffers[i]==nullptr) buffers[i] = new RSoundBuffer();
						this_buffer = buffers[i];
					}
				}
			}
			if (this_buffer!=nullptr) {
				this_buffer->LoadFile(FileName, SoundName);
			}
			else {
				if (LoadFile_out_enable) {
					cerr << "debug message: no free buffers so sound file '"+FileName+"' wasn't loaded (this error will only be shown once)" << endl;
					LoadFile_out_enable = false;
				}
			}
			//this_instance->sound = OpenSound(RSound::device, load_FilePath.c_str());
			//if (!this_instance->sound) {
			//	cerr << "OpenSound() failed for " << load_FilePath << endl;
				//return EXIT_FAILURE;
			//}
			//use like:
			//sound->play();
			//if (sound->isSeekable()) {
			// 	//int pos = sound->getPosition();
			//	if (IsLooped && !sound->isPlaying()) {sound->setPosition(0); sound->play(); }
			//}
		}
		else cerr<<"ERROR: nullptr buffers in LoadFile"<<endl;
	}
	void RSound::PlayAsMusic(string SoundName) {
		PlayAsMusic(SoundName,true,1.0);
	}
	void RSound::StopMusic() {
		///TODO: actually stop music
		cerr << "Not Yet Implemented: StopMusic" << endl;
	}
	void RSound::PlayAsMusic(string SoundName, bool IsLooped, float VolumeRatio) {
		if (buffers!=nullptr) {
			RSoundBuffer* this_buffer = nullptr;
			for (int i=0; i<SoundCount; i++) {
				if (buffers[i]!=nullptr && buffers[i]->enable) {
					if (buffers[i]->Name==SoundName) {
						this_buffer = buffers[i];
						break;
					}
				}
			}
			if (this_buffer!=nullptr) {
				RSoundBufferInstance* this_instance = nullptr;
				//TODO: check if instance already exists where its Name is InstanceName
				if (this->InstanceCount < this->MaxInstanceCount) {
					instances[this->InstanceCount] = new RSoundBufferInstance();
					this_instance = instances[this->InstanceCount];
					this->InstanceCount++;
				}
				else {
					for (int i=0; i<InstanceCount; i++) {
						if (instances[i]==nullptr || !instances[i]->enable) {
							if (instances[i]==nullptr) instances[i] = new RSoundBufferInstance();
							this_instance = instances[i];
						}
					}
				}
				if (this_instance!=nullptr) {
					this_instance->Name = SoundName;
					this_instance->source = this_buffer->get_source();
					this_instance->device = this->device;
					this_instance->play();
				}
				//else no free instances, so ignore
			}
			else {
				cerr << "ERROR in PlayAsMusic: No sound buffer was found named " << SoundName << endl;
			}
		}
		else cerr << "ERROR: nullptr for buffers during PlayAsMusic" <<endl;
	}
	void RSound::PlayAsMusicOnceThenResumePrevious(string SoundName, float VolumeRatio) {
		///TODO: actually play a RSoundBufferInstance now if one by SoundName exists else output error
		///TODO: actually add previously playing song to queue (if none then don't)
		cerr << "Not Yet Implemented: PlayAsMusicOnceThenResumePrevious" << endl;
	}
	void RSound::PlayAsSound(string SoundName, string EmitterName, string InstanceName, float positionVector3[], float velocityVector3[]) {
		RSound::PlayAsSound(SoundName, EmitterName, InstanceName, positionVector3, velocityVector3, false);
	}
	void RSound::PlayAsSound(string SoundName, string EmitterName, string InstanceName, float positionVector3[], float velocityVector3[], bool IsLooped) {
		if (buffers!=nullptr) {
			RSoundBuffer* this_buffer = nullptr;
			for (int i=0; i<SoundCount; i++) {
				if (buffers[i]!=nullptr && buffers[i]->enable) {
					if (buffers[i]->Name==SoundName) {
						this_buffer = buffers[i];
						break;
					}
				}
			}
			if (this_buffer!=nullptr) {
				RSoundBufferInstance* this_instance = nullptr;
				//TODO: check if instance already exists where its Name is InstanceName
				if (this->InstanceCount < this->MaxInstanceCount) {
					instances[this->InstanceCount] = new RSoundBufferInstance();
					this_instance = instances[this->InstanceCount];
					this->InstanceCount++;
				}
				else {
					for (int i=0; i<InstanceCount; i++) {
						if (instances[i]==nullptr || !instances[i]->enable) {
							if (instances[i]==nullptr) instances[i] = new RSoundBufferInstance();
							this_instance = instances[i];
						}
					}
				}
				if (this_instance!=nullptr) {
					this_instance->EmitterName = EmitterName;
					this_instance->Name = InstanceName;
					this_instance->source = this_buffer->get_source();
					this_instance->device = this->device;
					this_instance->play();
				}
				//else no free instances, so ignore
			}
			else {
				cerr << "ERROR in PlaySound: No sound buffer was found named " << SoundName << endl;
			}
		}
		else cerr << "ERROR: nullptr for buffers during PlaySound" <<endl;
	}
	bool MoveEmitter_out_enable = true;
	void RSound::MoveEmitter(string EmitterName, float positionVector3[], float velocityVector3[]) {
		///TODO: actually move a RSoundBufferInstance now if one by SoundName exists else output error
		if (MoveEmitter_out_enable) {
			cerr << "Not Yet Implemented: MoveEmitter (this error will only be shown once)" << endl;
			MoveEmitter_out_enable = false;
		}
	}
	void RSound::StopInstance(string InstanceName) {
		///TODO: actually stop a RSoundBufferInstance using loop that checks thisInstance.getName()
		cerr << "Not Yet Implemented: StopInstance" << endl;
	}

	void RSound::ClearAllInstances() {
		ClearSoundInstances();
		ClearMusicInstances();
	}
	void RSound::ClearSoundInstances() {
		///TODO: actually clear sound instances
		cerr << "Not Yet Implemented: ClearSoundInstances" << endl;
	}
	void RSound::ClearMusicInstances() {
		///TODO: actually clear music instances
		cerr << "Not Yet Implemented: ClearMusicInstances" << endl;
	}

	///#endregion RSound

}//end namespace ExpertMultimediaBase
