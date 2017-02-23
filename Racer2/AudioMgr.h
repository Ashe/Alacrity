// by Mark Featherstone (C) 2015 All Rights Reserved.
#ifndef AUDIOMGR_H
#define AUDIOMGR_H

#include <vector>

#include "UtilsDefines.h"
#include "Singleton.h"


class IAudioMgr;

/*
a collection of sounds i.e. sfx or music, one 'sound' can be
playing on many 'channels' at the same time. The interface
means we can implement a group using fmod, bass, openAL
*/
class IAudioGroup
{
public:
	//explicit constructor prevent any auto compiler conversion of parameters
	//C++ has a nasty habit of quietly trying to convert the wrong type into the right one
	explicit IAudioGroup( const UDEF::utf8string &grpName ) 
		: m_grpName(grpName), m_volume(1), m_channelCutOffVol(1.f) {};
	virtual ~IAudioGroup() {};
	///stop everything in the group
	virtual void Stop() =0;
	///stop this particular channel
	virtual void Stop( const unsigned int channelHandle ) =0;
	///mute everything or not
	virtual void Mute( const bool state ) =0;
	///set things up either as all streaming sounds or preloaded
	virtual bool Initialise( const bool asStreams ) =0;
	///grab all sound files in this folder
	virtual bool Load( const UDEF::utf8string &folder ) =0;
	///how many channels are going
	virtual unsigned int NumChannelsPlaying() =0;
	///how many sounds were loaded
	virtual unsigned int NumSoundsLoaded() =0;
	///grab a channel's handle
	virtual unsigned int GetChannelHandle( const int idx ) =0;
	///set volume of everything
	virtual void SetVolume( const float vol ) { m_volume = vol; }
	//change frequency
	virtual float GetFrequency(const unsigned int channelHandle) = 0;
	virtual void SetFrequency(const float freq, const unsigned int channelHandle) = 0;
	///set volume of one channel
	virtual void SetVolume( const float vol, const unsigned int channelHandle ) =0;
	///pan one channel
	virtual void SetPan(  const float pan, const unsigned int channelHandle ) =0;
	///pause one channel
	virtual void SetPause( const bool state, const unsigned int channelHandle = UDEF::MAX_UINT ) =0;
	///volume of entire group
	virtual float GetVolume() const { return m_volume; }
	///play a sound by its short name
	///@param name - name of sound to play, no path, no extension, just the filename
	///@param loop - loop forever
	///@param paused - don't immediately start it playing
	///@param pChannelHandle - optional pointer will have the channel handle written into it
	virtual bool Play( const UDEF::utf8string &name, const bool loop, const bool paused, unsigned int * pChannelHandle = NULL, 
						const float vol = 1.f ) =0;
	///play a sound given a 'sound handle'
	///@param soundhandle - all the loaded sounds can be referred to either by name or handle
	virtual bool Play( const unsigned int soundHandle, const bool loop, const bool paused, unsigned int * pChannelHandle = NULL, 
						const float vol = 1.f ) =0;
	///does the named sound exist in the group
	virtual bool Exists( const UDEF::utf8string &name, int *pSoundIndex = NULL ) =0;
	///is the channel currently playing, could be paused or finished
	virtual bool IsPlaying( const unsigned int channelHandle ) =0;
	///get the short name of the sound playing through this channel
	virtual const UDEF::utf8string &GetName( const unsigned int channelHandle ) =0;
	///get the sound index for the sound playing through this channel
	virtual const unsigned int &GetSoundIndex( const unsigned int channelHandle ) =0;
	///set a limit to channel volumes 
	void SetChannelVolCutoff( const float cutOffVol ) { m_channelCutOffVol = cutOffVol; }
protected:
	UDEF::utf8string m_grpName;			///< friendly name for group
	float m_channelCutOffVol;		///< no channel can be louder than this
private:
	float m_volume;	

	const IAudioGroup &operator=( const IAudioGroup & );  //prevent an audio group being assigned
};


/*
holds a song and sfx manager and controls all audio, as an
interface it means we can replace the implementation with 
fmod, bass, opanAL and the game code won't be touched
*/
class IAudioMgr : public Singleton<IAudioMgr>
{
public:
	IAudioMgr() : m_pSfxMgr(NULL), m_pSongMgr(NULL) { } 
	virtual ~IAudioMgr() {
		Shutdown();
	}
	//call it before using the audio manager
	virtual bool Initialise(void) =0;
	//call once per frame
	virtual void Update() =0 {
		NewFrame();
	}
	//stop everything
	virtual void Shutdown() =0;
	//do something with streamed music
	IAudioGroup * const GetSongMgr() { return m_pSongMgr; }
	//do something with loaded sfx
	IAudioGroup * const GetSfxMgr() { return m_pSfxMgr; }
	///see if this handle has already been started once this frame
	static bool CheckDuplicates( const unsigned int handle );
protected:
	IAudioGroup *m_pSongMgr;	//streamed audio
	IAudioGroup *m_pSfxMgr;		//memory loaded audio, small clips
	static std::vector<unsigned int> s_hdlStartedNow;	///<we shouldn't have the same handle starting more than once in a frame
	///reset our handle record each time the frame changes
	static void NewFrame() { s_hdlStartedNow.clear(); }   //static means there is only one of these functions regardless of instantiation
};
SINGLETON_GET(IAudioMgr);

#endif
