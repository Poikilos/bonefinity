#ifndef IRSOUND_H
#define IRSOUND_H

//#include <cstdlib>
#include <string>
#include <climits>

//This is the Interface. This file should be included and implemented by your implementation which should be included by your project (but this should only be included from there)
//Implementation tips:
//* Name your implementation files using naming convention such as: RSound_audiere.h and RSound_audiere.cpp
//  (where audiere is the name of the sound library you plan to use)
//  but make the defines (for ifndef macro) RSOUND_H and RSOUND_CPP to ensure that no other implementations are allowed simultaneously.
//  (Therefore your object can always be named same as Interface but without I, such as `class RSound : public IRSound`)
//* To keep RSound stable, create only one RSound object, and no objects from other classes.

namespace ExpertMultimediaBase {
	class IRSoundBuffer
	{
	public:
		/** Default constructor */
		//IRSoundBuffer();

		/** Default destructor */
		//virtual ~RSoundBuffer();

		/** Unload the sound */
		virtual void Dispose() = 0;

		/** Access IsLoaded
		 * \return The current value of IsLoaded
		 */
		virtual bool GetIsLoaded() = 0;

		/** Access FilePath
		 * \return The current value of FilePath
		 */
		virtual std::string GetFilePath() = 0;

		/** Set FilePath
		 * \param val New value to set
		 */
		//void SetFilePath(std::string val);
		virtual void LoadFile(std::string load_FilePath, std::string SoundName) = 0;

	protected:
	private:
		//void InitNull();
	};//end class RSoundBuffer


	class IRSoundBufferInstance
	{
	public:
		/** Default constructor */
		//RSoundBufferInstance();

		/** Default destructor */
		//virtual ~RSoundBufferInstance();
		virtual void Dispose() = 0;

		/** Access X
		 * \return The current value of X
		 */
		virtual float GetX() = 0;

		/** Set X
		 * \param val New value to set
		 */
		virtual void SetX(float val) = 0;

		/** Access Y
		 * \return The current value of Y
		 */
		virtual float GetY() = 0;

		/** Set Y
		 * \param val New value to set
		 */
		virtual void SetY(float val) = 0;
		/** Access Z
		 * \return The current value of Z
		 */
		virtual float GetZ() = 0;
		/** Set Z
		 * \param val New value to set
		 */
		virtual void SetZ(float val) = 0;
		/** Access VolumeRatio
		 * \return The current value of VolumeRatio
		 */
		virtual float GetVolumeRatio() = 0;
		/** Set VolumeRatio
		 * \param val New value to set
		 */
		virtual void SetVolumeRatio(float val) = 0;
		/** Access IPlayBufferIndex
		 * \return The current value of IPlayBufferIndex
		 */
		virtual int GetIPlayBufferIndex() = 0;
		/** Set IPlayBufferIndex
		 * \param val New value to set
		 */
		virtual void SetIPlayBufferIndex(int val) = 0;
		/** Access enable
		 * \return The current value of enable
		 */
		virtual bool GetEnable() = 0;
		/** Set enable
		 * \param val New value to set
		 */
		virtual void SetEnable(bool val) = 0;
		virtual std::string getName() = 0;
		virtual void play() = 0;
	protected:
		//void InitNull();
	};//end class RSoundBufferInstance


	class IRSoundListener
	{
	public:
		/** Specify the "At" coordinates then the "Up" coordinates */
		//RSoundListener();
		//static IRSoundListener* new_RSoundListener(float atX, float atY, float atZ, float upX, float upY, float upZ);
		virtual bool GetIsLoaded() = 0;
		virtual void Init(float atX, float atY, float atZ, float upX, float upY, float upZ, float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ) = 0;
		virtual void Init(float cameraOrientation[], float cameraPosition[], float cameraVelocity[]) = 0;
		/** Default destructor */
		//virtual ~RSoundListener();
		virtual void Dispose() = 0;
	protected:
	private:
		//void InitNull();
	};//end RSoundListener

	///<summary>
	///To use RSound, just do RSound::Start(whatever) where whatever are your parameters (rsound object is created automatically)
	///</summary>
	class IRSound
	{
	public:
		/** Default constructor */
		//RSound();

		/** Start
		 * \param cameraOrientation 6-long array, where first 3D point being "at" and where second 3D point being "up" of the listener.
		 * \param cameraPosition 3-long array which is a 3D point being position of the listener.
		 * \param cameraVelocity 3-long array which is a 3D point that, by its relationship to the position, would imply the direction the listener is facing.
		 */
		virtual void Start(float cameraOrientation[], float cameraPosition[], float cameraVelocity[]) = 0;

		/** Default destructor */
		//virtual ~RSound();


		virtual void Dispose() = 0;

		virtual IRSoundListener* GetListener() = 0;

		/** Access SoundCount
		 * \return The current value of SoundCount
		 */
		virtual int GetSoundCount() = 0;

		/** Set SoundCount
		 * \param val New value to set
		 */
		virtual void SetSoundCount(int val) = 0;

		/** Access IsLoaded
		 * \return The current value of IsLoaded
		 */
		virtual bool GetIsLoaded() = 0;

		/** Set IsLoaded
		 * \param val New value to set
		 */
		//NEVER DO THIS//virtual void SetIsLoaded(bool val) = 0;

		/** Access IsMuted
		 * \return The current value of IsMuted
		 */
		virtual bool GetIsMuted() = 0;

		/** Set IsMuted
		 * \param val New value to set
		 */
		virtual void SetIsMuted(bool val) = 0;


		///<summary>
		///Loads a file.
		///SoundName is just for convenience, as shorthand for filename. You can also use filename for SoundName. You can also load a sound file twice with different names, though that would normally be considered an unnecessary use of memory.
		///</summary>
		virtual void LoadFile(std::string SoundName, std::string FileName, bool IsMusic) = 0;
		virtual void PlayAsMusic(std::string SoundName) = 0;
		virtual void PlayAsMusic(std::string SoundName, bool IsLooped, float VolumeRatio) = 0;
		virtual void PlayAsMusicOnceThenResumePrevious(std::string SoundName, float VolumeRatio) = 0;
		virtual void StopMusic() = 0;
		virtual void PlayAsSound(std::string SoundName, std::string EmitterName, std::string InstanceName, float positionVector3[], float velocityVector3[]) = 0;
		virtual void StopInstance(std::string InstanceName) = 0;
		///<summary>
		///EmitterName is only needed if you want to move the object later. It is not a unique instance of sound just object--later, more than one instance can be moved by calling MoveObject(EmitterName)
		///</summary>
		virtual void PlayAsSound(std::string SoundName, std::string EmitterName, std::string InstanceName, float positionVector3[], float velocityVector3[], bool IsLooped) = 0;
		///<summary>
		///Moves all sound instances using the given EmitterName that you specified when calling PlayAsSound
		///</summary>
		virtual void MoveEmitter(std::string EmitterName, float positionVector3[], float velocityVector3[]) = 0;

		virtual void ClearAllInstances() = 0;
		virtual void ClearSoundInstances() = 0;
		virtual void ClearMusicInstances() = 0;

	protected:
	private:
		//void InitNull();
	};//end RSound

}//end namespace ExpertMultimediaBase
#endif // IRSOUND_H
