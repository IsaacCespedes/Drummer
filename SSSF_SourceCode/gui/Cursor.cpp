/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Cursor.cpp

	See Cursor.h for a class description.
*/

#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\graphics\RenderList.h"
#include "SSSF_SourceCode\graphics\TextureManager.h"
#include "SSSF_SourceCode\gui\Cursor.h"
#include "SSSF_SourceCode\input\GameInput.h"
#include "SSSF_SourceCode\os\GameOS.h"

/*
	Cursor - Default constructor, this method constructs the imageIDs vector
	and sets all numeric variables to zero. This method does not setup a usable
	cursor. To do so, either call mutator methods or use the init method, which
	initializes all necessary parameters at once.
*/
Cursor::Cursor()
{
	activeCursorID = 0;
	x = 0;
	y = 0;
	z = 0;
	alpha = 0;
	width = 0;
	height = 0;
}

/*
	~Cursor - Destructor, it cleans up our vector pointer.
*/
Cursor::~Cursor()	
{
	imageIDs.clear();
}

/*
	addRenderItemToRenderList - Called once per frame, this method makes a RenderItem
	representing the current cursor and adds it to the render list. The result being
	that the cursor will be rendered according to its current state. The cursor should
	be rendered last, after the game world and the rest of the GUI.
*/
void Cursor::addRenderItemToRenderList(RenderList *renderList)
{
	if(0<x && (x+16)<1024 && 0<y && (y+16)<768)
	renderList->addRenderItem(	activeCursorID,
								x,
								y,
								z,
								alpha,
								width,
								height);
}

/*
	initCursor - This method can be used to initialize all important 
	cursor variables at once.
*/
void Cursor::initCursor(vector<unsigned int> initImageIDs,
						unsigned int initActiveCursorID,
						int initX,
						int initY,
						int initZ,
						int initAlpha,
						int initWidth,
						int initHeight)
{
	imageIDs = initImageIDs;
	activeCursorID = initActiveCursorID;
	x = initX;
	y = initY;
	z = initZ;
	alpha = initAlpha;
	width = initWidth;
	height = initHeight;
}
