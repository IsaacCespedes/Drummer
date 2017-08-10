/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	This class provides a framework for technology-specific
	classes that will manage the rendering of all game textures and text.
	It provides methods for the initialization of objects necessary
	for rendering images and text.

	Rendering should be done by generating a list of RenderItem
	objects each frame and putting them into a list. These
	objects store information about the textures that have
	to be drawn to the screen.
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\game\WStringTable.h"
#include "SSSF_SourceCode\graphics\RenderList.h"
#include "SSSF_SourceCode\graphics\VertexList.h"
#include "SSSF_SourceCode\graphics\TextureManager.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\input\GameInput.h"
#include "SSSF_SourceCode\text\GameText.h"
#include "SSSF_SourceCode\text\TextFileWriter.h"

class GameGraphics
{
protected:
	// WE PERIODICALLY NEED ACCESS TO GAME STUFF
	Game *game;

	// WE'LL SORE THE SCREEN DIMENSIONS HERE
	int screenWidth;
	int screenHeight;

	// EACH FRAME, RENDER LISTS ARE CREATED, FILLED WITH
	// DATA, THEN USED FOR RENDERING THE FRAME, AND DESTROYED
	RenderList				*guiRenderList;
	RenderList				*worldRenderList;
	//VertexList				*modelVertexList;
	

	// NOTE THAT IF WE REALLY WANTED TO BE SLICK, WE COULD PROVIDE
	// MULTIPLE WORLD RENDER LISTS, ONE FOR EACH Z-LEVEL

	// THE TextureManagers STORE ALL THE IMAGES FOR OUR GAME
	TextureManager			*guiTextureManager;
	TextureManager			*worldTextureManager;

public:	
	// INLINED ACCESSOR METHODS
	RenderList*				getGUIRenderList()			{ return guiRenderList;			}
	TextureManager*			getGUITextureManager()		{ return guiTextureManager;		}
	RenderList*				getWorldRenderList()		{ return worldRenderList;		}
	TextureManager*			getWorldTextureManager()	{ return worldTextureManager;	}
	VertexList*				getVertexList()				{return NULL;}//modelVertexList;}

	// VIRTUAL METHODS TO BE OVERRIDDEN BY TECHNOLOGY
	// SPECIFIC CLASSES
	virtual	TextureManager* createTextureManager()=0;
	virtual int				getScreenHeight()=0;
	virtual int				getScreenWidth()=0;
	virtual	void			initGraphics(HWND hWnd, bool isFullscreen)=0;
	virtual void			initTextFont(int fontSize)=0;
	virtual void			initVertices()=0;
	virtual void			reloadGraphics()=0;
	virtual void			renderGame(Game *game)=0;
	virtual void			renderTextToDraw(TextToDraw *textToDraw)=0;		
	virtual void			setColorKey(int r, int g, int b)=0;
	virtual void			setFontColor(int r, int g, int b)=0;
	virtual void			shutdown()=0;

	// CONCRETE METHODS DEFINED IN GameGraphics.cpp
	GameGraphics();
	~GameGraphics();
	void clearWorldTextures();
	void fillRenderLists(Game *game);
	void init(int screenWidth, int screenHeight);
	void renderText(GameText *text);
};