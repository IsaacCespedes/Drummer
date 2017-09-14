//**************************************************************************************************
//
// SoundPlayer.h: interface for the CSoundPlayer class.
//
// Description: Uses DirectSound to play back sounds
// Note: Makes use of the DirectX framework files dsutil.cpp and dsutil.h
// These define classes CSoundManager and CSound used by this program
//
// Created by: Keith Ditchburn 30/01/2004
//
//**************************************************************************************************

#if !defined(AFX_SOUND1_H__9E6A20A2_156A_47D8_9063_4CEC8193874E__INCLUDED_)
#define AFX_SOUND1_H__9E6A20A2_156A_47D8_9063_4CEC8193874E__INCLUDED_

#pragma once

#include "stdafx.h"

class CSoundManager;
class CSound;

class CSoundPlayer  
{
private:
	CSoundManager* m_soundManager;
	std::vector <CSound*> m_soundVector;
public:
	void StopSound(int id);
	bool PlaySound(int id,bool loop);
	bool AddWav(WCHAR* filename,int *id);
	HRESULT Initialise(HWND hWnd);
	CSoundPlayer();
	virtual ~CSoundPlayer();
};

#endif // !defined(AFX_SOUND1_H__9E6A20A2_156A_47D8_9063_4CEC8193874E__INCLUDED_)
