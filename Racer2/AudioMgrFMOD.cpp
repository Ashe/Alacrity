// by Mark Featherstone (C) 2015 All Rights Reserved.
#include <string.h>
#include <stdio.h>

#include "fmod.hpp"
#include "fmod_errors.h"

#include "AudioMgrFMOD.h"
#include "Error.h"
#include "File.h"
#include "UtilsDefines.h"

using namespace std;
using namespace UDEF;

/*
FMOD doesn't know if its on a PC or an Xbox, so it needs us
to provide functions load data in (songs and sfx files). It
will call them, it just needs us to provide them.
*/
class FileBridge
{
public:
	//open an audio file
	static FMOD_RESULT F_CALLBACK userOpen(	const char *  name, int  unicode, unsigned int *  filesize, void **  handle, void **  userdata );
	//close an audio file
	static FMOD_RESULT F_CALLBACK userClose( void *  handle, void *  userdata );
	//read some binary data from a file
	static FMOD_RESULT F_CALLBACK userRead( void *  handle, void *  buffer, unsigned int  sizebytes, unsigned int *  bytesread, void *  userdata );
	//move the file pointer to read data from a different offset in the file
	static FMOD_RESULT F_CALLBACK userSeek( void *  handle, unsigned int  pos, void *  userdata );
	//keep a single array of file handle to open files, if its streaming
	//it will stay open a long time
	static vector<File*> s_openFiles;
};


vector<File*> FileBridge::s_openFiles;

/*
Callback functions - we create a function because we know
how to implement it, fmod does not. Fmod knows when they
need calling, but we don't. Function pointers to these
are passed to FMOD.
*/
FMOD_RESULT F_CALLBACK FileBridge::userOpen(
	const char *  name, 
	int  unicode, 
	unsigned int *  filesize, 
	void **  handle, 
	void **  userdata
	)
{
	//open a file for reading and save the file pointer
	File *pF = new File( name, File::MPF_READ );
	*filesize = pF->getSize();
	size_t i;
	for( i = 0; i < s_openFiles.size(); ++i )
	{
		if( s_openFiles[i] == 0 )
		{
			s_openFiles[i] = pF;
			break;
		}
	}
	if( s_openFiles.empty() || i == s_openFiles.size() )
		s_openFiles.push_back(pF);

	*handle = (void*)i;

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FileBridge::userClose(
	void *  handle, 
	void *  userdata
	)
{
	//close an already open file
	File *pF = s_openFiles[(size_t)handle];
	pF->close();
	delete pF;
	s_openFiles[(size_t)handle] = 0;
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FileBridge::userRead(
	void *  handle, 
	void *  buffer, 
	unsigned int  sizebytes, 
	unsigned int *  bytesread, 
	void *  userdata
	)
{
	//just read a few bytes from an open file
	File *pF = s_openFiles[(size_t)handle];
	pF->read( buffer, sizebytes, *bytesread );
	if( sizebytes > *bytesread )
		return FMOD_ERR_FILE_EOF;
	return FMOD_OK;
}

//move to a different part of the file
FMOD_RESULT F_CALLBACK FileBridge::userSeek(
	void *  handle, 
	unsigned int  pos, 
	void *  userdata
	)
{
	File *pF = s_openFiles[(size_t)handle];
	pF->seek(pos);
	return FMOD_OK;
}



unsigned int AudioGroupFMOD::m_uniqueChannelCounter(0);


AudioMgrFMOD::AudioMgrFMOD() 
	: m_pSystem(NULL), IAudioMgr()
{

}

/*
Set up FMOD and, link it to our file system callbacks
Create a sonf and sfx manager, use them to load any and all
audio files from the two default directories "sfx" and "music".
The music folder has large files for streaming, the sfx folder
has small files played often that need loading into memory.
*/
bool AudioMgrFMOD::Initialise(void)
{
	MPOD_ASSERT( !m_pSystem );
	if( !InitFMOD() )
		return false;

	MPOD_VERIFY( m_pSystem->setFileSystem( FileBridge::userOpen, FileBridge::userClose, FileBridge::userRead, FileBridge::userSeek, -1 ) == FMOD_OK );
	
	MPOD_ASSERT( !m_pSongMgr && !m_pSfxMgr );
	m_pSongMgr = new AudioGroupFMOD(*this,"song");
	if( !m_pSongMgr->Initialise(true) )
		return false;
	m_pSfxMgr = new AudioGroupFMOD(*this,"sfx");
	if( !m_pSfxMgr->Initialise(false) )
		return false;
	
	if(!GetSongMgr()->Load( "music"))
		MPOD_ASSERT_MSG(false,"No music folder?");
	GetSongMgr()->SetVolume(1);

	if(!GetSfxMgr()->Load( "sfx" ))
		MPOD_ASSERT_MSG(false,"No sfx folder");
	GetSfxMgr()->SetVolume(1);

	return true;
}

/*
Comes from the FMOD manual, it's one standard way
to set up FMOD.
Note the use of goto here, the one valid use for it. To
prevent code duplication when trying to leave a function from
multple locations.
*/
bool AudioMgrFMOD::InitFMOD()
{
	FMOD_RESULT      result;
	unsigned int     version;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS        caps;

	/*
	Create a System object and initialize.						    
	*/
	if( FMOD::System_Create(&m_pSystem) != FMOD_OK )
	{
		m_pSystem = NULL;
		return false;
	}

	if( m_pSystem->getVersion(&version) != FMOD_OK )
		goto shutdown_error;

	if (version < FMOD_VERSION)
	{
		MPOD_ASSERT_MSG( 0, "Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		goto shutdown_error;
	}

	if( m_pSystem->getDriverCaps(0, &caps, 0, 0, &speakermode) != FMOD_OK )
		goto shutdown_error;

	if( m_pSystem->setSpeakerMode(speakermode) != FMOD_OK )       /* Set the user selected speaker mode. */
		goto shutdown_error;

	if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
	{                                                   /* You might want to warn the user about this. */
		if( m_pSystem->setDSPBufferSize(1024, 10) != FMOD_OK )    /* At 48khz, the latency between issuing an fmod command and hearing it will now be about 213ms. */
			goto shutdown_error;
	}

	result = m_pSystem->init(100, FMOD_INIT_NORMAL, 0);    /* Replace with whatever channel count and flags you use! */
	if( result == FMOD_ERR_OUTPUT_CREATEBUFFER )         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
	{
		if( m_pSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO) != FMOD_OK )
			goto shutdown_error;

		if( m_pSystem->init(100, FMOD_INIT_NORMAL, 0) != FMOD_OK ) /* Replace with whatever channel count and flags you use! */
			goto shutdown_error;
	} 


	return true;

shutdown_error:
	Shutdown();
	return false;
}


void AudioMgrFMOD::Update()
{
	//let fmod update
	if( !m_pSystem || (m_pSystem->update() != FMOD_OK) )
		return;
	//call the base class update as it has its own work to do
	IAudioMgr::Update();

}

void AudioMgrFMOD::Shutdown()
{
	//first let the base class do its thing
	IAudioMgr::Shutdown();

	//shut down fmod
	if( !m_pSystem )
		return;
	m_pSystem->release();
	m_pSystem = NULL;
}






//**********************************************************************************
//a way for fmod to let us know a sound has finished playing
FMOD_RESULT F_CALLBACK fmodChannelCallback( FMOD_CHANNEL *  channel, 
											FMOD_CHANNEL_CALLBACKTYPE  type, 
											int  command, 
											unsigned int  commanddata1, 
											unsigned int  commanddata2 )
{
	if( type == FMOD_CHANNEL_CALLBACKTYPE_END )
	{
		//tell the audio manager this channel is finished
		//we gave it an instance of one of our channel data objects, it's not nice, but
		//but it has to be passed as "void*" because fmod knows nothing about it
		void *pCD;
		FMOD::Channel *pCh = (FMOD::Channel*)channel;
		MPOD_VERIFY( pCh->getUserData( &pCD ) == FMOD_OK );
		static_cast<AudioGroupFMOD::ChannelData *>(pCD)->_channelHandle = UDEF::MAX_UINT;
	}
	return FMOD_OK;	
}


 
//stop everything and release all sounds 
AudioGroupFMOD::~AudioGroupFMOD()
{
	Stop();
	for( size_t i = 0; i < m_sounds.size(); ++i )
		m_sounds[i]._pSound->release();
}

/*
Given a folder name, look through for any audio files, ignore everything
else. If we don't already have it, either create a stream to it (music) or
load it in (sfx).
*/
bool AudioGroupFMOD::Load( const utf8string &folder )
{
	if( !File::folderExists( folder ) )
		return false;
	vector<utf8string> fileSpec;
	fileSpec.push_back( "*.wav" );
	fileSpec.push_back( "*.ogg" );
	fileSpec.push_back( "*.mp3" );
	fileSpec.push_back( "*.wma" );

	bool allLoaded = true;
	for( size_t i = 0; i < fileSpec.size(); ++i )
	{
		const File::Names &names = File::findFiles( folder, fileSpec[i].c_str() );
		if( names.empty() )
			continue;
		File::setCurrentFolder( folder );
		for( size_t ii = 0; ii < names.size(); ++ii )
		{
			SoundData data;
			//data._name = names[ii];
			if(!UDEF::splitFileName(names[ii],NULL,NULL,NULL,&data._name))
				continue;
			//if( !CMN::getSubStr( names[ii], "(.*)\\....", data._name ) )
			//	continue;
			if( Exists( data._name ) )
				continue;
			bool success = true;
			FMOD_RESULT res;
			if( m_asStreams )
			{
				res = m_audioMgr.GetSystem()->createStream( names[ii].c_str(), FMOD_DEFAULT, 0, &data._pSound );
				if( res != FMOD_OK ) 
				{ 
					MPOD_ASSERT_MSG(0, "FMOD ERROR (%d) %s", res, FMOD_ErrorString(res));
					success = false; 
				}
			}
			else
			{
				res = m_audioMgr.GetSystem()->createSound( names[ii].c_str(), FMOD_DEFAULT, 0, &data._pSound );
				if( res != FMOD_OK ) 
				{ 
					MPOD_ASSERT_MSG(0, "FMOD ERROR (%d) %s", res, FMOD_ErrorString(res));
					success = false; 
				}
			}
			if( success )
				m_sounds.push_back( data );
			else
				allLoaded = false;
		}
		File::setCurrentFolder( File::getFirstRunDirectory() ); //scanning through folders moves the "current directory" so put it back
	}
	return allLoaded;
} 

/*
Create the potential in this group for 100 sounds
*/
bool AudioGroupFMOD::Initialise( const bool asStreams )
{
	m_channels.reserve(100);
	m_sounds.reserve(100);
	m_asStreams = asStreams;
	if( m_audioMgr.GetSystem()->createChannelGroup( m_grpName.c_str(), &m_pMyGroup ) != FMOD_OK )
		return false;
	
	FMOD::ChannelGroup *pMasterGrp;
	if( m_audioMgr.GetSystem()->getMasterChannelGroup( &pMasterGrp ) != FMOD_OK )
		return false;

	if( pMasterGrp->addGroup( m_pMyGroup ) != FMOD_OK )
		return false;


	return true;
}


unsigned int AudioGroupFMOD::NumChannelsPlaying()
{
	int n = 0;
	if( m_pMyGroup && m_pMyGroup->getNumChannels( &n ) != FMOD_OK ) { 
		MPOD_ASSERT(0); 
	}
	return n;
}

const utf8string &AudioGroupFMOD::GetName( const unsigned int channelHandle )
{
	const ChannelData &dat = GetChannelData( channelHandle );
	return m_sounds.at(dat._soundIdx)._name;
}

unsigned int AudioGroupFMOD::GetChannelHandle( const int idx )
{
	MPOD_ASSERT( idx >= 0 && idx < static_cast<int>(m_channels.size()) );
	return m_channels[idx]._channelHandle;
}

const unsigned int &AudioGroupFMOD::GetSoundIndex( const unsigned int channelHandle )
{
	const ChannelData &dat = GetChannelData(channelHandle);
	return dat._soundIdx;
}

int AudioGroupFMOD::GetSoundData( const UDEF::utf8string &name, SoundData ** pSD )
{
	for( int i = 0; i <static_cast<int>(m_sounds.size()); ++i )
	{
		if( m_sounds[i]._name == name )
		{
			if( pSD )
				*pSD = &m_sounds[i];
			return i;
		}
	}
	if( pSD )
		*pSD = NULL;
	return -1;
}


bool AudioGroupFMOD::Exists( const UDEF::utf8string &name, int *pSoundIndex )
{
	utf8string sname;
	int hdl;
	//if( !CMN::getSubStr( name, "(.*)\\....", sname ) )
	if(!UDEF::splitFileName(name,NULL,NULL,NULL,&sname))
	{
		if( (hdl=GetSoundData( name )) < 0 )
			return false;
		if( pSoundIndex )
			*pSoundIndex = hdl;
		return true;
	}
	if( (hdl=GetSoundData( sname )) < 0 )
		return false;
	if( pSoundIndex )
		*pSoundIndex = hdl;
	return true;
}

//we know the name, not the channel, so play it
bool AudioGroupFMOD::Play( const UDEF::utf8string &name, const bool loop, const bool paused, unsigned int *pChannelHandle, const float vol )
{
	unsigned int handle = GetSoundData(name);
	if( handle == UDEF::MAX_UINT )
		return false;
	return Play( handle, loop, paused, pChannelHandle, vol ); 	
}

//start playing a previously loaded sound
bool AudioGroupFMOD::Play( const unsigned int handle, const bool loop, const bool paused, unsigned int *pChannelHandle, const float vol )
{
	//only one of each sound is started per frame - otherwise it sounds wierd
	if( IAudioMgr::CheckDuplicates(handle) )
		return false;
	//let hte audioMgr know what we want
	FMOD::Channel *pCh;
	if( m_audioMgr.GetSystem()->playSound( FMOD_CHANNEL_FREE, GetSound( handle ), paused, &pCh ) != FMOD_OK ) 
		return false;
	if( pCh->setChannelGroup( GetChannelGroup() ) != FMOD_OK )
		return false;

	if( loop )
		pCh->setLoopCount(-1);
		//MPOD_VERIFY( pCh->setLoopCount(-1) != FMOD_OK );

	//keep track of the new sound playing
	Channels::iterator it;
	int idx = 0;
	for( it = m_channels.begin(); it != m_channels.end(); ++it )
	{
		if( (*it)._channelHandle != UDEF::MAX_UINT )
		{
			MPOD_ASSERT( idx++ < 100 );
			continue;
		}

		(*it)._pChannel = pCh;
		(*it)._soundIdx = handle;
		(*it)._channelHandle = m_uniqueChannelCounter++;
		if( pChannelHandle )
			*pChannelHandle = (*it)._channelHandle;
		break;
	}
	//if you don't find it, add it
	if( it == m_channels.end() )
	{
		m_channels.push_back( ChannelData(pCh,handle,m_uniqueChannelCounter++) );
		if( pChannelHandle )
			*pChannelHandle = m_channels.back()._channelHandle;
		it = m_channels.end();
		it--;
	}
	//setup the callback so we know when it finishes
	pCh->setUserData( &(*it) );
	pCh->setCallback( FMOD_CHANNEL_CALLBACKTYPE_END, fmodChannelCallback, 0 ); 

	if( vol > m_channelCutOffVol )
		pCh->setVolume(m_channelCutOffVol);
	else
		pCh->setVolume( vol );// 1.f );//getVolume() );

	return true;
}

//stop everything and forget all channel handles as they've all now gone
void AudioGroupFMOD::Stop()
{
	m_pMyGroup->stop();
	Channels::iterator it;
	for( it = m_channels.begin(); it != m_channels.end(); ++it )
		(*it)._channelHandle = UDEF::MAX_UINT;
}

//search through the channels array for a specific one
const AudioGroupFMOD::ChannelData &AudioGroupFMOD::GetChannelData( const unsigned int channelHandle )
{
	Channels::iterator it;
	for( it = m_channels.begin(); it != m_channels.end(); ++it )
	{
		if( (*it)._channelHandle == channelHandle )
			return (*it);
	}
	MPOD_VERIFY(0);
	return m_channels.front();
}

//stop one channel from playing
void AudioGroupFMOD::Stop( const unsigned int channelHandle )
{
	Channels::iterator it;
	for( it = m_channels.begin(); it != m_channels.end(); ++it )
	{
		if( (*it)._channelHandle != channelHandle )
			continue;
		(*it)._pChannel->stop();
		(*it)._channelHandle = UDEF::MAX_UINT;
	}
}

void AudioGroupFMOD::SetVolume( const float vol )
{
	IAudioGroup::SetVolume( vol );
	m_pMyGroup->setVolume( vol );
}

//is a sound currently playing back
bool AudioGroupFMOD::IsPlaying( const unsigned int channelHandle )
{
	FMOD::Channel *pCh = GetChannel(channelHandle);
	bool playing;						 
	pCh->isPlaying( &playing );
	return playing;
}

FMOD::Channel *AudioGroupFMOD::GetChannel( const unsigned int channelHandle ) const
{
	for( size_t i = 0; i < m_channels.size(); ++i )
	{
		if( channelHandle == m_channels[i]._channelHandle )
			return m_channels[i]._pChannel;
	}
	return NULL;
}

void AudioGroupFMOD::SetVolume( const float vol, const unsigned int channelHandle )
{
	MPOD_ASSERT( vol >= 0 && vol <= 1 );

	FMOD::Channel *pCh = GetChannel(channelHandle);
	if( pCh )
	{
		if( vol > m_channelCutOffVol )
			pCh->setVolume(m_channelCutOffVol);
		else
			pCh->setVolume( vol );// * getVolume() );
	}
}

float AudioGroupFMOD::GetFrequency(const unsigned int channelHandle)
{
	FMOD::Channel *pCh = GetChannel(channelHandle);
	float freq=0;
	if (pCh)
	{
		if (pCh->getFrequency(&freq) != FMOD_OK)
			MPOD_VERIFY(0);
	}
	return freq;
}

void AudioGroupFMOD::SetFrequency(const float freq, const unsigned int channelHandle)
{
	FMOD::Channel *pCh = GetChannel(channelHandle);
	if (pCh)
	{
		if (pCh->setFrequency(freq) != FMOD_OK)
			MPOD_VERIFY(0);
		return;
	}
	MPOD_VERIFY(0);
}

void AudioGroupFMOD::SetPan(  const float pan, const unsigned int channelHandle )
{
	MPOD_ASSERT( pan >= -1 && pan <= 1 );
	FMOD::Channel *pCh = GetChannel(channelHandle);
	if( pCh )
		pCh->setPan( pan );
}

void AudioGroupFMOD::SetPause( const bool state, const unsigned int channelHandle )
{
	if( channelHandle != UDEF::MAX_UINT )
	{
		FMOD::Channel *pCh = GetChannel(channelHandle);
		if( pCh )
			pCh->setPaused(state);
		return;
	}
	m_pMyGroup->setPaused(state);
}

void AudioGroupFMOD::Mute( const bool state )
{
	m_pMyGroup->setMute(state);
}

