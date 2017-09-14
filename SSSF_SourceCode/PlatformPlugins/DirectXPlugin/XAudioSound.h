#pragma once
#include "stdafx.h"
#include "XAudio2.h"
#include "SSSF_SourceCode\sound\GameSound.h"


class XAudioSound : public GameSound
{
private:
	static const WCHAR * soundNames [];
	IXAudio2* pXAudio2;
	IXAudio2SourceVoice* pSourceVoices [7];
	IXAudio2MasteringVoice* pMasteringVoice;
	HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );
	void    playSound(int i);
	HRESULT prepareXAudio();
	HRESULT preparePCM(IXAudio2* pXaudio2, LPCWSTR szFilename, int sourceNum);
	void	shutdown();
public:
	XAudioSound();
	~XAudioSound();
	
};