#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\game\WStringTable.h"
#include "SSSF_SourceCode\graphics\RenderList.h"
#include "SSSF_SourceCode\graphics\TextureManager.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\input\GameInput.h"
#include "SSSF_SourceCode\text\GameText.h"
#include "SSSF_SourceCode\text\TextFileWriter.h"

class GameSound
{
protected:
	wstring soundFile;

public:	
	virtual void			shutdown()=0;
	virtual bool			Initialize(HWND hwnd)=0;
	//virtual bool			isThereEngine()=0;
	//virtual void			work()=0;
	//virtual void			playSound(PCSTR soundName)=0;
	virtual void			playSound(int i)=0;
	//virtual void			play()=0;
	//virtual void			pause()=0;
	//virtual void			resume()=0;
	//virtual void			stop()=0;

	// CONCRETE METHODS DEFINED IN GameSound.cpp
	GameSound();
	~GameSound();
};