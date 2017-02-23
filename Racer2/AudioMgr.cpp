// by Mark Featherstone (C) 2015 All Rights Reserved.

#include <string.h>
#include <stdio.h>
#include <algorithm>

#include "AudioMgr.h"


using namespace std;
using namespace UDEF;


//**************************************************************************************************
//just think of vectors as safer arrays, you can add things, search them, access an element, etc.
std::vector<unsigned int> IAudioMgr::s_hdlStartedNow;	

/*
see if the handle has already been started this frame
sometimes you might trigger a sound more than once by accident
in the same frame. It would sound wrong.
*/
bool IAudioMgr::CheckDuplicates( const unsigned int handle )
{
	if( !s_hdlStartedNow.empty() && (find( s_hdlStartedNow.begin(), s_hdlStartedNow.end(), handle ) != s_hdlStartedNow.end()) )
		return true;
	s_hdlStartedNow.push_back(handle);
	return false;
}

//release all sounds
void IAudioMgr::Shutdown()
{
	//if this was called accidentally twice, it would be fine
	//deleting NULL doesn't do anything
	delete m_pSongMgr;
	m_pSongMgr=NULL;
	delete m_pSfxMgr;
	m_pSfxMgr=NULL;
}



