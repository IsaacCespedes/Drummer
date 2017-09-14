/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	ScreenGUI.h

	This class represents a single screen GUI, like the main
	menu. In a game application, there is always a GUI of some sort,
	whether it is application controls or game controls. So even while
	the game is being played, we can interact with a different GUI.

	This class manages a single GUI, and a game application may have many.
	Each GUI can have many Buttons, many OverlayImages, and a screen name.
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\graphics\RenderList.h"
#include "SSSF_SourceCode\gui\Button.h"
#include "SSSF_SourceCode\gui\OverlayImage.h"

class ScreenGUI
{
private:
	// THE BUTTONS FOR THIS SCREEN
	//list<Button*>		*buttons;
	map<const wstring, Button*> *buttons;

	// ADDITIONAL IMAGES FOR THIS SCREEN, LIKE BORDERS OR 
	//list<OverlayImage*> *overlayImages;
	map<const int, OverlayImage*> *overlayImages;
	// SCREEN NAME, LIKE "MAIN MENU"
	wchar_t				*screenName;

public:
	// INLINED ACCESSOR METHODS
	int getNumButtons()			{ return buttons->size();		}
	int getNumOverlayImages()	{ return overlayImages->size(); }
	wstring getScreenName()			{ return screenName;			}

	// INLINED MUTATOR METHODS
	void setScreenName(wchar_t *initScreenName)
	{
		screenName = initScreenName;
	}

	// METHODS DEFINED IN ScreenGUI.cpp
	ScreenGUI();
	~ScreenGUI();
	void addButton(Button *buttonToAdd, wstring cmd);
	void addOverlayImage(OverlayImage *imageToAdd, int id);
	void addRenderItemsToRenderList(RenderList *renderList);
	bool fireButtonCommand(Game *game);
	void registerButtonEventHandler(ButtonEventHandler *eventHandler);
	void updateAllButtons(long mouseX, long mouseY);
};