//**************************************************************************************************
//
// SoundPlayer.cpp: implementation of the CSoundPlayer class.
//
// Description: Uses DirectSound to play back sounds
// Note: Makes use of the DirectX framework files dsutil.cpp and dsutil.h
// These define classes CSoundManager and CSound used by this program
//
// Created by: Keith Ditchburn 30/01/2004
//
//**************************************************************************************************

#include "stdafx.h"
#include "SoundPlayer.h"
#include <assert.h>
#include "dsutil.h"

/**************************************************************************************************
**************************************************************************************************/
CSoundPlayer::CSoundPlayer()
{
	m_soundManager=NULL;
}

/**************************************************************************************************
**************************************************************************************************/
CSoundPlayer::~CSoundPlayer()
{
	for(int i = 0; i < m_soundVector.size(); i++)
		if (m_soundVector[i])
			delete m_soundVector[i];
	if (m_soundManager)
		delete m_soundManager;
}

/**************************************************************************************************
Desc: Initialise DirectSound using the dsound utility class CSoundManager
Date: 30/01/2004
**************************************************************************************************/
HRESULT CSoundPlayer::Initialise(HWND hWnd)
{
	HRESULT hr=S_OK;

    m_soundManager = new CSoundManager();

	// Set coop level to DSSCL_PRIORITY, 
    if( FAILED( hr = m_soundManager->Initialize( hWnd, DSSCL_PRIORITY) ) )
    {
        MessageBox( hWnd, L"Error initializing DirectSound.  Sample will now exit.", 
                            L"DirectSound Sample", MB_OK | MB_ICONERROR );
        return hr;
    }

    	
    // set primary buffer format to stereo, 22kHz and 16-bit output.
    if( FAILED( hr = m_soundManager->SetPrimaryBufferFormat( 2, 22050, 16 ) ) )
    {
        MessageBox( hWnd, L"Error initializing DirectSound.  Sample will now exit.", 
                          L"DirectSound Sample", MB_OK | MB_ICONERROR );
        return hr;
    }

	return hr;
}

/**************************************************************************************************
Desc: Add a wave sound
Returns FALSE on error
If succeeded returns TRUE and fills id with vector index of this sound
Date: 02/02/2004
**************************************************************************************************/
bool CSoundPlayer::AddWav(WCHAR *filename,int *id)
{
	assert(filename);
	assert(id);

	// Create a sound file
	CSound *newSound=NULL;

	HRESULT hr;

	// Load the wave file into a DirectSound buffer
    if( FAILED( hr = m_soundManager->Create( &newSound, filename, 0, GUID_NULL ) ) )
		return FALSE;

	// Succeeded so add to our vector and fill id for return
	m_soundVector.push_back(newSound);

	*id=m_soundVector.size()-1;

	return TRUE;
}

/**************************************************************************************************
Desc: Play a sound, Requires id passed in, If loop is TRUE the sound will loop until stopped
Date: 06/02/2004
**************************************************************************************************/
bool CSoundPlayer::PlaySound(int id, bool loop)
{
	assert(id>=0 && id<m_soundVector.size());

    DWORD flags=0;
	if (loop)
		flags=DSBPLAY_LOOPING;

	HRESULT hr = m_soundVector[id]->Reset();
    hr = m_soundVector[id]->Play( 0, flags );
	

	return FAILED(hr);
}

/**************************************************************************************************
Desc: Stop a sound playing
Date: 06/02/2004
**************************************************************************************************/
void CSoundPlayer::StopSound(int id)
{
	assert(id>=0 && id<m_soundVector.size());

	m_soundVector[id]->Stop();
}
