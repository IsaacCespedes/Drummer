#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\sound\GameSound.h"
#include <shellapi.h>
#include <commdlg.h>
#include <strsafe.h>
#define _WIN32_DCOM
#pragma warning( disable : 4996 ) // disable deprecated warning 
#pragma warning( default : 4996 )


//-----------------------------------------------------------------------------
// Struct to hold audio game state
//-----------------------------------------------------------------------------
struct AUDIO_STATE
{
    IXACT3Engine* pEngine;
	IXACT3SoundBank* pSoundBank;
    IXACT3WaveBank* pInMemoryWaveBank;
	IXACT3WaveBank* pStreamingWaveBank;

	IXACT3Cue* pZeroLatencyRevCue;

	HANDLE hStreamingWaveBankFile;
    VOID* pbInMemoryWaveBank; // Handle to wave bank data.  Its memory mapped so call UnmapViewOfFile() upon cleanup to release file
    VOID* pbSoundBank; // Pointer to sound bank data.  Call delete on it when the sound bank is destroyed

	CRITICAL_SECTION cs;
    bool bHandleStreamingWaveBankPrepared;
    bool bHandleZeroLatencyCueStop;
    bool bHandleSongStopped;
};

class XactSound : public GameSound
{
private:
	bool DoesCommandLineContainAuditionSwitch();
	HRESULT prepareXACT();
	AUDIO_STATE g_audioState;
public:
	XactSound();
	~XactSound();
	HRESULT createBank();
	HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );
	bool	isThereEngine();
	void play();
	void playSound(PCSTR soundName);
	void	pause();
	void	resume();
	void shutdown();
	void stop();
	void work();
	void UpdateAudio();
	static void WINAPI XACTNotificationCallback( const XACT_NOTIFICATION* pNotification );
	void HandleNotification( const XACT_NOTIFICATION* pNotification );
};