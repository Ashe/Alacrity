// by Mark Featherstone (C) 2015 All Rights Reserved.
#ifndef AUDIOMGRFMOD_H
#define AUDIOMGRFMOD_H

#include <vector>

#include "UtilsDefines.h"
#include "AudioMgr.h"

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
	class ChannelGroup;
}

/*
Take our abstract IAudioGroup and IAudioMgr and provide concrete classes
which implement all the missing functions from the base classes. 
*/
class AudioMgrFMOD;
class AudioGroupFMOD : public IAudioGroup
{
public:
	//a data structure only used by the fmod audio group and its manager
	struct ChannelData
	{
		ChannelData( FMOD::Channel * const pChannel, const unsigned int soundIdx, const unsigned int channelHandle )
			:_pChannel(pChannel), _soundIdx(soundIdx), _channelHandle(channelHandle) {};
		FMOD::Channel *_pChannel;	
		unsigned int _soundIdx;
		unsigned int _channelHandle;
	};
	//see base class for descriptions
	AudioGroupFMOD( AudioMgrFMOD &audioMgr, const UDEF::utf8string &grpName ) 
		: IAudioGroup(grpName), m_audioMgr(audioMgr), m_pMyGroup(NULL) {};
	~AudioGroupFMOD();
	virtual void Stop();
	virtual void Stop( const unsigned int channelHandle );
	virtual void Mute( const bool state );
	virtual void SetVolume( const float vol );
	virtual void SetVolume( const float vol, const unsigned int channelHandle );
	virtual float GetFrequency(const unsigned int channelHandle);
	void SetFrequency(const float freq, const unsigned int channelHandle);
	virtual void SetPan(  const float pan, const unsigned int channelHandle );
	virtual void SetPause( const bool state, const unsigned int channelHandle = UDEF::MAX_UINT );
	virtual bool Initialise( const bool asStreams );
	virtual bool Load( const UDEF::utf8string &folder );
	virtual unsigned int NumChannelsPlaying();
	virtual unsigned int NumSoundsLoaded() { return static_cast<unsigned int>(m_sounds.size()); }
	virtual bool Play( const UDEF::utf8string &name, const bool loop, const bool paused, unsigned int *pChannelHandle = NULL, 
						const float vol = 1.f );
	virtual bool Play( const unsigned int soundHandle, const bool loop, const bool paused, unsigned int *pChannelHandle = NULL,
						const float vol = 1.f );
	virtual bool Exists( const UDEF::utf8string &name, int *pSoundIndex = NULL );
	virtual bool IsPlaying( const unsigned int channelHandle );
	virtual const UDEF::utf8string &GetName( const unsigned int channelHandle );
	virtual unsigned int GetChannelHandle( const int idx );
	virtual const unsigned int &GetSoundIndex( const unsigned int channelHandle );

	//fmod specific access provided for the audioMgr
	FMOD::Channel *GetChannel( const unsigned int channelHandle ) const;
	FMOD::Sound * const GetSound( const unsigned int soundHandle ) const { return m_sounds.at(soundHandle)._pSound; }
	FMOD::ChannelGroup * const GetChannelGroup() { return m_pMyGroup; }
private:
	//this sound data isn't used by any code outside the audioMgr so its private
	struct SoundData 
	{
		FMOD::Sound *_pSound;
		UDEF::utf8string _name;	//lets us link a sound to its friendly name
	};

	typedef std::vector<ChannelData> Channels;  //take the ugly complete type definition
	typedef std::vector<SoundData> Sounds;		//and give it a nice simple name

	Sounds m_sounds;	//an array of all the sounds and their friendly names
	Channels m_channels;//an array of all the channels we are using
	FMOD::ChannelGroup *m_pMyGroup;
	bool m_asStreams;	//is this group streamed (music) or loaded into memory (sfx)
	AudioMgrFMOD &m_audioMgr;	//keep track of our audioMgr
	static unsigned int m_uniqueChannelCounter;	//a single source of unique channel handles, it just counts up

	///get the group index for the given sound
	///@param name - filename or short name for the sound
	///@param pSD - optional pointer that can be hooked up to this sounds data object
	int GetSoundData( const UDEF::utf8string &name, SoundData ** pSD = NULL );
	///get channel data
	const ChannelData &GetChannelData( const unsigned int channelHandle );
};

///fmod version of the abstract base class
class AudioMgrFMOD : public IAudioMgr
{
public:
	AudioMgrFMOD();
	bool Initialise(void);
	void Shutdown();
	void Update();
	FMOD::System * const GetSystem() { return m_pSystem; }
private:
	FMOD::System *m_pSystem;	//the main fmod system is owned by us

	AudioMgrFMOD( const IAudioMgr & );
	const AudioMgrFMOD &operator=( const IAudioMgr & );
	bool InitFMOD();

};
	 

#endif
