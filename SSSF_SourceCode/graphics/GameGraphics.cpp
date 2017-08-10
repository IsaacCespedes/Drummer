/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameGraphics.cpp

	See GameGraphics.h for a class description.
*/

#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\game\IllegalArgumentException.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\graphics\TextureManager.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\world\World.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\os\GameOS.h"
#include "SSSF_SourceCode\text\GameText.h"
#include "SSSF_SourceCode\text\TextFileWriter.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"

/*
	GameGraphics - Default constructor, nothing to initialize.
*/
GameGraphics::GameGraphics()
{
}

/*
	~GameGraphics - Destructor, it cleans up the render lists and texture
	managers. This should only be called when the application is closing.
*/
GameGraphics::~GameGraphics()
{
	delete guiRenderList;
	delete worldRenderList;
}

/*
	clearWorldTextures - When the game leaves a level we have to clear
	out these data structures. Calling clear on these will delete
	all the objects inside.
*/
void GameGraphics::clearWorldTextures()
{	
	// CLEAR LEVEL DATA STRUCURES
	//worldTextureManager->clear();
	//guiTextureManager->clear();
	worldRenderList->clear();
	guiRenderList->clear();
	//modelVertexList->clear();
}

/*
	fillRenderLists - This method causes the render lists to be 
	filled with the things that have to be drawn this frame.
*/
void GameGraphics::fillRenderLists(Game *game)
{
	// GENERATE RENDER LISTS FOR GAME WORLD AND GUI
	GameStateManager *gsm = game->getGSM();
	gsm->addGameRenderItemsToRenderList(game);
	GameGUI *gui = game->getGUI();
	gui->addRenderItemsToRenderList(game);
}

/*
	init - This method constructs the data structures for managing textures
	and render lists. It calls the createTextureManager, which is technology
	specific, and so is implemented only by child classes.
*/
void GameGraphics::init(int initScreenWidth, int initScreenHeight)
{
	// INIT SCREEN DIMENSIONS
	screenWidth = initScreenWidth;
	screenHeight = initScreenHeight;

	// GUI TEXTURES (like buttons, cursor, etc.)
	guiRenderList = new RenderList(100);
	guiTextureManager = createTextureManager();

	// LEVEL TEXTURES (like sprites, tiles, particles, etc.)
	worldRenderList = new RenderList(1000);
	worldTextureManager = createTextureManager();

//	modelVertexList = new VertexList(1000);
}

/*
	renderText - This method will go through the GameText argument,
	pull out each TextToDraw object, and use a technology-specific
	method in a child class, renderTextToDraw, to render each
	piece of text.
*/
void GameGraphics::renderText(GameText *text)
{
	int numTextObjects = text->getNumTextObjectsToDraw();
	for (int i = 0; i < numTextObjects; i++)
	{
		TextToDraw *textToDraw = text->getTextToDrawAtIndex(i);
		renderTextToDraw(textToDraw);
	}
}