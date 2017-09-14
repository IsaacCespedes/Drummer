#include "stdafx.h"
#include "DirectXSound.h"

DirectXSound::DirectXSound()
{
	gSoundPlayer=new CSoundPlayer;
}

DirectXSound::~DirectXSound()
{
	delete gSoundPlayer;
}

bool DirectXSound::Initialize(HWND hwnd)
{
	if (FAILED(gSoundPlayer->Initialise(hwnd)))
		return FALSE;

	int id;
	if (!gSoundPlayer->AddWav(L"sound/Win/crash.wav",&id))
		return FALSE;
	
	if (!gSoundPlayer->AddWav(L"sound/Win/hihat.wav",&id))
		return FALSE;

	if (!gSoundPlayer->AddWav(L"sound/Win/snare.wav",&id))
		return FALSE;

	if (!gSoundPlayer->AddWav(L"sound/Win/hitom.wav",&id))
		return FALSE;

	if (!gSoundPlayer->AddWav(L"sound/Win/midtom.wav",&id))
		return FALSE;

	if (!gSoundPlayer->AddWav(L"sound/Win/lotom.wav",&id))
		return FALSE;

	if (!gSoundPlayer->AddWav(L"sound/Win/kick.wav",&id))
		return FALSE;

	return true;
}

void DirectXSound::playSound(int i)
{
	gSoundPlayer->PlaySoundW(i, false);
}

void DirectXSound::shutdown()
{
	return;
}