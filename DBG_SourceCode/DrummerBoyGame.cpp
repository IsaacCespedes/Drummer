/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DrummerBoyGame.cpp

	This is a test game application, a game that demonstrates use of the 
	SideScrollerFramework to make a little scrolling, interactive demo. It
	demonstrates the rendering of images and text as well as responding to
	key presses and button cicks. Students should make their own named game
	applictions using a similar pattern, gradually adding other components,
	like additional gui controls, tiles, sprites, ai, and physics.
 */

#include "stdafx.h"

// DUMMY GAME INCLUDES
#include "DBG_SourceCode\DrummerBoyButtonEventHandler.h"
#include "DBG_SourceCode\DrummerBoyDataLoader.h"
#include "DBG_SourceCode\DrummerBoyGame.h"
#include "DBG_SourceCode\DrummerBoyKeyEventHandler.h"
#include "DBG_SourceCode\DrummerBoyTextGenerator.h"

// GAME OBJECT INCLUDES
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\gsm\state\GameState.h"
#include "SSSF_SourceCode\gui\Cursor.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\gui\ScreenGUI.h"
#include "SSSF_SourceCode\input\GameInput.h"
#include "SSSF_SourceCode\os\GameOS.h"
#include "SSSF_SourceCode\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsOS.h"
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsInput.h"
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

// DIRECTX INCLUDES
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXGraphics.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\XactSound.h"

/*
	WinMain - This is the application's starting point. In this method we will construct a Game object, 
	then initialize all the platform-dependent technologies, then construct all the custom data for our 
	game, and then initialize the Game with	our custom data. We'll then start the game loop.
*/
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// CREATE THE GAME
	Game *drummerBoyGame = new Game();

	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ IN DATA TO SETUP OTHER STUFF
	DrummerBoyDataLoader *drummerBoyDataLoader = new DrummerBoyDataLoader();
	drummerBoyDataLoader->initWinHandle(hInstance, nCmdShow);
	drummerBoyGame->setDataLoader(drummerBoyDataLoader);
	drummerBoyDataLoader->loadGame(drummerBoyGame, DBG_INIT_FILE);
	
	// WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
	// MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
	// FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

	// LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	drummerBoyDataLoader->loadGUI(drummerBoyGame, DBG_GUI_INIT_FILE);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	DrummerBoyButtonEventHandler *drummerBoyButtonHandler = new DrummerBoyButtonEventHandler();
	GameGUI *gui = drummerBoyGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)drummerBoyButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	DrummerBoyKeyEventHandler *drummerBoyKeyHandler = new DrummerBoyKeyEventHandler();
	drummerBoyGame->getInput()->registerKeyHandler((KeyEventHandler*)drummerBoyKeyHandler);

	// AND UPDATE THE GAME STATE OF COURSE
	drummerBoyGame->getGSM()->goToLoadLevel();	// NOTE THAT CURRENTLY HTERE IS NO LEVEL FILE,
	
	// THAT'S ONE THING YOU'LL BE DOING
	drummerBoyDataLoader->loadWorld(drummerBoyGame, L"THERE IS NO FILE");
	drummerBoyDataLoader->loadSprites(drummerBoyGame);
	drummerBoyDataLoader->loadModel(drummerBoyGame);

	// START THE GAME LOOP
	drummerBoyGame->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)drummerBoyGame->getOS();
	delete (WindowsInput*)drummerBoyGame->getInput();
	delete (WindowsTimer*)drummerBoyGame->getTimer();
	delete (DirectXGraphics*)drummerBoyGame->getGraphics();
	//delete (DirectXTextureManager*) drummerBoyGame->getGraphics()->getGUITextureManager();
	//delete (DirectXTextureManager*) drummerBoyGame->getGraphics()->getWorldTextureManager();
	delete (XactSound*)drummerBoyGame->getSound();
	delete (DrummerBoyTextGenerator*)drummerBoyGame->getText()->getTextGenerator();
	delete drummerBoyDataLoader;
	delete drummerBoyButtonHandler;
	delete drummerBoyKeyHandler;
	delete drummerBoyGame;

	// AND RETURN
	return 0;
}