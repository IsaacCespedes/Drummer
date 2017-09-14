/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameGUI.cpp

	See GameGUI.h for a class description.
*/

#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\game\IllegalArgumentException.h"
#include "SSSF_SourceCode\gsm\state\GameState.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gui\Button.h"
#include "SSSF_SourceCode\gui\Cursor.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\gui\ScreenGUI.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\graphics\RenderList.h"
#include "SSSF_SourceCode\graphics\TextureManager.h"
#include "SSSF_SourceCode\input\ButtonEventHandler.h"
#include "SSSF_SourceCode\input\GameInput.h"
#include "SSSF_SourceCode\text\BufferedTextFileReader.h"

/*
	GameGUI - Default Constructor, it sets up the data structure for storing
	all ScreenGUIs. These should subsequently be added using addScreenGUI
*/
GameGUI::GameGUI()
{
	customCursor = NULL;
}

/*
	~GameGUI - Destructor, this would be called when the application is closing,
	it will clean up all memory for the GUI, including the cursor and all
	ScreenGUIs.
*/
GameGUI::~GameGUI()
{
	delete customCursor;

	map<GameState, ScreenGUI*>::iterator sgIterator = screens.begin();

	map<GameState, ScreenGUI*>::iterator sgItToErase;

	while(sgIterator != screens.end())
	{
		ScreenGUI *sg = (*sgIterator).second;
		sgItToErase = sgIterator;
		sgIterator++;
		screens.erase(sgItToErase);
		delete sg;
	}
	screens.clear();
}

/*
	addRenderItemsToRenderList - This method cascades this call to the
	current ScreenGUI. That object will add the appropriate items to
	the render list. This method should be called once per frame to make
	sure the current GUI is always rendered.

	Make sure the mode you provide matches the index of this GUI's screen.
	If an illegal mode value is provided, an exception is thrown.
*/
void GameGUI::addRenderItemsToRenderList(Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameState gameState = gsm->getCurrentGameState();
	
	GameGraphics *graphics = game->getGraphics();
	RenderList *guiRenderList = graphics->getGUIRenderList();

	if (gsm->isAppActive())
	{
		// CASCADE THIS CALL TO THE APPROPRIATE SCREEN
		screens.at(gameState)->addRenderItemsToRenderList(guiRenderList);
	}

	// CASCADE THIS CALL TO THE CURSOR, IF THERE IS ONE
	if (customCursor != NULL)
		customCursor->addRenderItemToRenderList(guiRenderList);

}

/*
	addScreenGUI - This method adds a constructed ScreenGUI to the
	game's gui.
*/
void GameGUI::addScreenGUI(GameState gameState, ScreenGUI *screenToAdd)
{
	screens[gameState] = screenToAdd;
}

/*
	checkCurrentScreenForAction - This method should be called
	once per frame. It checks with the current screen to see if
	a button event needs to be fired, and fires it if necessary.

	If a mode is provided that does not match a ScreenGUI index,
	an exception is thrown.
*/
bool GameGUI::checkCurrentScreenForAction(Game *game)
{
	// IF THE GAME STATE DOESN'T HAVE A CORRESPONDING,
	// SCREEN DON'T DO ANYTHING
	GameStateManager *gsm = game->getGSM();
	GameState gameState = gsm->getCurrentGameState();
	ScreenGUI *gui = screens[gameState];
	if (gui != NULL)
	{
		// CASCADE THE CHECK TO THE CURRENT SCREEN
		return gui->fireButtonCommand(game);
	}
	return false;
}

/*
	registerButtonEventHandler - This method tells all of the screens in
	this gui the class that will handle button click events.
*/
void GameGUI::registerButtonEventHandler(ButtonEventHandler *eventHandler)
{
	map<GameState,ScreenGUI*>::iterator it = screens.begin();
	while (it != screens.end())
	{
		// NOTE THAT THE "second" VALUE IN A MAP ELEMENT IS THE DATA,
		// THE "first" WOULD BE THE KEY
		(*it).second->registerButtonEventHandler(eventHandler);
		it++;
	}
}

/*
	updateGUIState - This method sends mousePoint, the current cursor
	location, to the current GUI screen such that all of its button
	states can be updated. This method should be called once per frame
	after mouse position has been updated but before we check for fired
	events.

	If an illegal mode is provided, meaning a screen index that does not
	exist, an exception is thrown.
*/
void GameGUI::updateGUIState(long mouseX, long mouseY, GameState gameState)
{
	ScreenGUI *gui = screens[gameState];
	if (gui != NULL)
	{
		// LET THE CURRENT SCREEN UPDATE IT'S BUTTON STATES
		gui->updateAllButtons(mouseX, mouseY);
	}
}

void GameGUI::updateViewportState(long mouseX, long mouseY)
{
	if (viewport.areViewportCoordinatesInViewport(mouseX, mouseY, 0, 0))
		mouseOverViewport = true;
	else
		mouseOverViewport = false;
}