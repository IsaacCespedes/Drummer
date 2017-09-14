#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\sound\GameSound.h"
#include "SoundPlayer.h"

class DirectXSound : GameSound
{
private:
	CSoundPlayer *gSoundPlayer;
public:
	DirectXSound();
	~DirectXSound();

	bool Initialize(HWND hwnd);
	void playSound(int i);
	void shutdown();

};