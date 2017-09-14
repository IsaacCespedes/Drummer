/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DrummerBoyKeyEventHandler.cpp

	See DrummerBoyKeyEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "DBG_SourceCode\DrummerBoyKeyEventHandler.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\game\WStringTable.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "SSSF_SourceCode\gsm\state\GameState.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gui\Cursor.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\input\GameInput.h"
#include "SSSF_SourceCode\timer\GameTimer.h"
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

// USED FOR MOVEMENT
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
const unsigned int UP_KEY = VK_UP;
const unsigned int DOWN_KEY = VK_DOWN;
const unsigned int LEFT_KEY = VK_LEFT;
const unsigned int RIGHT_KEY = VK_RIGHT;

// THIS IS JUST FOR SHOWING HOW THE CURSOR CAN BE CHANGED
const unsigned int C_KEY = (unsigned int)'C';

const float PLAYER_SPEED = 5.0f;
const int MIN_FPS = 5;
const int MAX_FPS = 100;
const int FPS_INC = 1;

/*
	handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
	gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
	key states since the last frame, which can allow us to respond to key presses, including when keys
	are held down for multiple frames.
*/
void DrummerBoyKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
//	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
//	PhysicalProperties *pp = player->getPhysicalProperties();
	
	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		// CHECK FOR WASD KEYS FOR MOVEMENT
		int incX = 0;
		int incY = 0;
		bool moveViewport = false;

		// WASD AND DIRECTION KEY PRESSES WILL CONTROL THE PLAYER,
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
//		float vX = pp->getVelocityX();
//		float vY = pp->getVelocityY();

		if (input->isKeyDown(W_KEY))
		{
	//		vY = -PLAYER_SPEED;
		}
		if (input->isKeyDown(A_KEY))
		{
		//	vX = -PLAYER_SPEED;
		}
		if (input->isKeyDown(S_KEY))
		{
		//	vY = PLAYER_SPEED;
		}
		if (input->isKeyDown(D_KEY))
		{
		//	vX = PLAYER_SPEED;
		}
		
		// NOW SET THE ACTUAL VELOCITY
//		Physics *physics = gsm->getPhysics();
//		pp->setVelocitySafely(physics, vX, vY);

		// NOTE THAT THE VIEWPORT SHOULD FOLLOW THE PLAYER, AND SO SHOULD BE CORRECTED AFTER PHYSICS
		// ARE APPLIED. I HAVE PROVIDED A SIMPLE WAY OF DOING SO, WHICH SHOULD BE IMPROVED, DEPENDING
		// ON THE GAME'S NEED
	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;		
		cursor->setActiveCursorID(id);
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);
}