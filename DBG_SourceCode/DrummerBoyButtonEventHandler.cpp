/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	drummerBoyButtonEventHandler.cpp

	See DrummerBoyButtonEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "DBG_SourceCode\DrummerBoyGame.h"
#include "DBG_SourceCode\DrummerBoyButtonEventHandler.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"
#include "SSSF_SourceCode\gsm\ai\NoteBot.h"

void DrummerBoyButtonEventHandler::handleButtonEvents(	Game *game, 
													wstring command)
{
	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(DBG_EXIT_COMMAND) == 0)
	{
		game->shutdown();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(DBG_GO_TO_MM_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToMainMenu();
	}
	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	else if (command.compare(DBG_START_COMMAND) == 0)
	{
		game->startGame();
	}
	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(DBG_QUIT_COMMAND) == 0)
	{
		game->quitGame();
	}
	else if (command.compare(DBG_PLAY_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->beginTrack();
	}
	else if (command.compare(DBG_SCROLL_LEFT_COMMAND) == 0)
	{
		GameGUI * gui = game->getGUI();
		GameStateManager *gsm = game->getGSM();
		World * world = gsm->getWorld();
		if(gsm->isLeftScroll())
			gui->getViewport()->moveViewport(-10, 0, world->getWorldWidth(), world->getWorldHeight());
	}
	else if (command.compare(DBG_SCROLL_RIGHT_COMMAND) == 0)
	{
		GameGUI * gui = game->getGUI();
		GameStateManager *gsm = game->getGSM();
		World * world = gsm->getWorld();
		if(gsm->isRightScroll())
			gui->getViewport()->moveViewport(10, 0, world->getWorldWidth(), world->getWorldHeight());
	}
	else if (command.compare(DBG_EXTEND_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		World * world = gsm->getWorld();
		world->setWorldWidth(world->getWorldWidth() + (64 * 16));
		TiledLayer * tl = dynamic_cast<TiledLayer*>(world->getLayers()->at(0));
		int gridSize = tl->getColumns() * tl->getRows();

		tl->setLayerWidth(tl->getLayerWidth() + (64 * 16));
		tl->setColumns(tl->getColumns() + 64);
		
		

		for (int i = gridSize; i < gridSize + (NUM_COLUMNS * NUM_ROWS); i++)
		{
			Tile *tileToAdd = new Tile();
			tileToAdd->collidable = false;
			tileToAdd->textureID = 0;
			tl->addTile(tileToAdd);
		}
	}
}