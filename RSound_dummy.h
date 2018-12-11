#ifndef RSOUND_H
#define RSOUND_H

//#include <cstdlib>
#include <string>
#include <climits>
#include <IRSound.h>
//#include <SFML/Audio.hpp>
//#ifdef EMM_USE_AUDIERE
//#include "audiere.h"
//#endif

//see implementation tips in IRSound.h

namespace ExpertMultimediaBase {
	class RSoundBuffer : public IRSoundBuffer
	{
	public:
		/** Default constructor */
		RSoundBuffer();

		/** Default destructor */
		virtual ~RSoundBuffer();

		/** Unload the sound */
		void Dispose();

		/** Access IsLoaded
		 * \return The current value of IsLoaded
		 */
		bool GetIsLoaded();

		/** Access FilePath
		 * \return The current value of FilePath
		 */
		std::string GetFilePath();

		/** Set FilePath
		 * \param val New value to set
		 */
		//void SetFilePath(std::string val);
		void LoadFile(std::string load_FilePath, std::string SoundName);
		bool enable;

	protected:
	private:
		bool IsLoaded; //!< Member variable "IsLoaded"
		std::string Name; //!< Member variable "Name"
		bool IsMusic; //!< Member variable "IsMusic"
		std::string FilePath; //!< Member variable "FilePath"

		void InitNull();
	};//end class RSoundBuffer


	class RSoundBufferInstance : public IRSoundBufferInstance
	{
	public:
		/** Default constructor */
		RSoundBufferInstance();

		/** Default destructor */
		virtual ~RSoundBufferInstance();

		/** Access X
		 * \return The current value of X
		 */
		float GetX();

		/** Set X
		 * \param val New value to set
		 */
		void SetX(float val);

		/** Access Y
		 * \return The current value of Y
		 */
		float GetY();

		/** Set Y
		 * \param val New value to set
		 */
		void SetY(float val);
		/** Access Z
		 * \return The current value of Z
		 */
		float GetZ();
		/** Set Z
		 * \param val New value to set
		 */
		void SetZ(float val);
		/** Access VolumeRatio
		 * \return The current value of VolumeRatio
		 */
		float GetVolumeRatio();
		/** Set VolumeRatio
		 * \param val New value to set
		 */
		void SetVolumeRatio(float val);
		/** Access IPlayBufferIndex
		 * \return The current value of IPlayBufferIndex
		 */
		int GetIPlayBufferIndex();
		/** Set IPlayBufferIndex
		 * \param val New value to set
		 */
		void SetIPlayBufferIndex(int val);
		std::string getName();

		/** Unload the RSoundBufferInstance */
		void Dispose();
		void play();
	protected:
	private:
		std::string Name;
		float Position[3];
		float Velocity[3];
		bool IsLooped; //!< Member variable "IsLooped"
		float VolumeRatio; //!< Member variable "VolumeRatio"
		float PitchRatio; //!< Member variable "PitchRatio"
		long MillisecondsLife; //!< Member variable "MillisecondsLife"
		int IPlayBufferIndex; //!< Member variable "IPlayBufferIndex"
		float RevolveAroundX; //!< Member variable "RevolveAroundX"
		float RevolveAroundY; //!< Member variable "RevolveAroundY"
		float RevolveAroundZ; //!< Member variable "RevolveAroundZ"
		float RevolveXPerMS; //!< Member variable "RevolveXPerMS"
		float RevolveYPerMS; //!< Member variable "RevolveYPerMS"
		float RevolveZPerMS; //!< Member variable "RevolveZPerMS"
		float AngleX; //!< Member variable "AngleX"
		float AngleY; //!< Member variable "AngleY"
		float AngleZ; //!< Member variable "AngleZ"

		void InitNull();
	};//end class RSoundBufferInstance


	class RSoundListener : public IRSoundListener
	{
	public:
		/** Specify the "At" coordinates then the "Up" coordinates */
		RSoundListener();
		static RSoundListener* new_RSoundListener(float atX, float atY, float atZ, float upX, float upY, float upZ);
		bool GetIsLoaded();
		void Init(float atX, float atY, float atZ, float upX, float upY, float upZ, float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ);
		void Init(float cameraOrientation[], float cameraPosition[], float cameraVelocity[]);
		/** Default destructor */
		virtual ~RSoundListener();
		void Dispose();
	protected:
	private:
		bool IsLoaded;
		void InitNull();
		float Orientation[6]; //!< Member variable "Orientation[3]" (1st 3 are "UP" coordinates, next 3 are "AT" coordinates, like OpenAL)
		float Position[3]; //!< Member variable "Position[3]"
		float Velocity[3]; //!< Member variable "Velocity[3]"
	};//end RSoundListener

	///<summary>
	///To use RSound, just do RSound::Start(whatever) where whatever are your parameters (rsound object is created automatically)
	///</summary>
	class RSound : public IRSound
	{
	public:
		/** Default constructor */
		RSound();

		/** Start
		 * \param cameraOrientation 6-long array, where first 3D point being "at" and where second 3D point being "up" of the listener.
		 * \param cameraPosition 3-long array which is a 3D point being position of the listener.
		 * \param cameraVelocity 3-long array which is a 3D point that, by its relationship to the position, would imply the direction the listener is facing.
		 */
		void Start(float cameraOrientation[], float cameraPosition[], float cameraVelocity[]);

		/** Default destructor */
		virtual ~RSound();

		void Dispose();

		RSoundListener* GetListener();

		/** Access SoundCount
		 * \return The current value of SoundCount
		 */
		int GetSoundCount();

		/** Set SoundCount
		 * \param val New value to set
		 */
		void SetSoundCount(int val);

		/** Access IsLoaded
		 * \return The current value of IsLoaded
		 */
		bool GetIsLoaded();

		/** Set IsLoaded
		 * \param val New value to set
		 */
		void SetIsLoaded(bool val);

		/** Access IsMuted
		 * \return The current value of IsMuted
		 */
		bool GetIsMuted();

		/** Set IsMuted
		 * \param val New value to set
		 */
		void SetIsMuted(bool val);


		///<summary>
		///Loads a file.
		///SoundName is just for convenience, as shorthand for filename. You can also use filename for SoundName. You can also load a sound file twice with different names, though that would normally be considered an unnecessary use of memory.
		///</summary>
		void LoadFile(std::string SoundName, std::string FileName, bool IsMusic);
		void PlayAsMusic(std::string SoundName);
		void PlayAsMusic(std::string SoundName, bool IsLooped, float VolumeRatio);
		void PlayAsMusicOnceThenResumePrevious(std::string SoundName, float VolumeRatio);
		void StopMusic();
		void PlayAsSound(std::string SoundName, std::string EmitterName, std::string InstanceName, float positionVector3[], float velocityVector3[]);
		void StopInstance(std::string InstanceName);
		///<summary>
		///EmitterName is only needed if you want to move the object later. It is not a unique instance of sound just object--later, more than one instance can be moved by calling MoveObject(EmitterName)
		///</summary>
		void PlayAsSound(std::string SoundName, std::string EmitterName, std::string InstanceName, float positionVector3[], float velocityVector3[], bool IsLooped);
		///<summary>
		///Moves all sound instances using the given EmitterName that you specified when calling PlayAsSound
		///</summary>
		void MoveEmitter(std::string EmitterName, float positionVector3[], float velocityVector3[]);

		void ClearAllInstances();
		void ClearSoundInstances();
		void ClearMusicInstances();

	protected:
	private:
		bool IsMuted;
		int SoundCount; //!< Member variable "SoundCount"
		bool IsLoaded; //!< Member variable "IsLoaded"
		RSoundListener* listener;
		void InitNull();
	};//end RSound

}//end namespace ExpertMultimediaBase
#endif // RSOUND_H
