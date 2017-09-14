#include "stdafx.h"

// drummerboy GAME INCLUDES
#include "DBG_SourceCode\DrummerBoyButtonEventHandler.h"
#include "DBG_SourceCode\DrummerBoyDataLoader.h"
#include "DBG_SourceCode\DrummerBoyGame.h"
#include "DBG_SourceCode\DrummerBoyKeyEventHandler.h"
#include "DBG_SourceCode\DrummerBoyTextGenerator.h"

// GAME OBJECT INCLUDES
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\gsm\state\GameState.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"
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
//#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\XACTSound.h"
//#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\XAudioSound.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXSound.h"
/*
	loadGame - This method loads the setup game data into the game and
	constructs all the needed objects for the game to work.
*/
void DrummerBoyDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[DG_TITLE];
	wstring useFullscreenProp = (*properties)[DG_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[DG_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[DG_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *drummerBoyOS = new WindowsOS(	hInstance, 
										nCmdShow,
										useFullscreen,
										titleProp,
										screenWidth, screenHeight,
										game);
	
	int textFontSize;
	wstring textFontSizeProp = (*properties)[DG_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *drummerBoyGraphics = new DirectXGraphics(game);
	drummerBoyGraphics->init(screenWidth, screenHeight);
	drummerBoyGraphics->initGraphics(drummerBoyOS->getWindowHandle(), useFullscreen);
	drummerBoyGraphics->initTextFont(textFontSize);

	DirectXSound *drummerBoySound = new DirectXSound();
	drummerBoySound->Initialize(drummerBoyOS->getWindowHandle());

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(drummerBoyGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *drummerBoyInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *drummerBoyTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins(	(GameGraphics*)drummerBoyGraphics,
								(GameSound*)drummerBoySound,
								(GameInput*)drummerBoyInput,
								(GameOS*)drummerBoyOS,
								(GameTimer*)drummerBoyTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	DrummerBoyTextGenerator *drummerBoyTextGenerator = new DrummerBoyTextGenerator();
	drummerBoyTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(DG_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)drummerBoyTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);	

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

/*
	initColors - this helper method loads the color key, used for loading
	images, and the font color, used for rendering text.
*/
void DrummerBoyDataLoader::initColors(	GameGraphics *graphics,
									map<wstring,wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[DG_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[DG_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[DG_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[DG_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[DG_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[DG_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
	loadGUI - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void DrummerBoyDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
} 

/*
loads music notes as bots
*/
void DrummerBoyDataLoader::loadSprites(Game* game)
{
	TextureManager *worldTextureManager = game->getGraphics()->getWorldTextureManager();

	AnimatedSpriteType *ast = new AnimatedSpriteType();
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();

	int noteImageID0 = worldTextureManager->loadTexture(L"textures/world/sprites/note0.png");
	int noteImageID1 = worldTextureManager->loadTexture(L"textures/world/sprites/note1.png");
	int noteImageID2 = worldTextureManager->loadTexture(L"textures/world/sprites/note2.png");
	int noteImageID3 = worldTextureManager->loadTexture(L"textures/world/sprites/note3.png");
	int noteImageID4 = worldTextureManager->loadTexture(L"textures/world/sprites/note4.png");
	int noteImageID5 = worldTextureManager->loadTexture(L"textures/world/sprites/note5.png");
	int noteImageID6 = worldTextureManager->loadTexture(L"textures/world/sprites/note6.png");
	int noteImageID7 = worldTextureManager->loadTexture(L"textures/world/sprites/note7.png");
	int noteImageID8 = worldTextureManager->loadTexture(L"textures/world/sprites/note8.png");
	int noteImageID9 = worldTextureManager->loadTexture(L"textures/world/sprites/note9.png");
	int noteImageID10 = worldTextureManager->loadTexture(L"textures/world/sprites/note10.png");
	int noteImageID11 = worldTextureManager->loadTexture(L"textures/world/sprites/note11.png");
	int noteImageID12 = worldTextureManager->loadTexture(L"textures/world/sprites/note12.png");
	int noteImageID13 = worldTextureManager->loadTexture(L"textures/world/sprites/note13.png");
	int noteImageID14 = worldTextureManager->loadTexture(L"textures/world/sprites/note14.png");
	ast->setTextureSize(16, 16);
	ast->addAnimationSequence(NOTE_HIDDEN);
	ast->addAnimationSequence(NOTE_1_CONFIRMED);
	ast->addAnimationSequence(NOTE_1_PLAYED);
	ast->addAnimationSequence(NOTE_2_CONFIRMED);
	ast->addAnimationSequence(NOTE_2_PLAYED);
	ast->addAnimationSequence(NOTE_3_CONFIRMED);
	ast->addAnimationSequence(NOTE_3_PLAYED);
	ast->addAnimationSequence(NOTE_4_CONFIRMED);
	ast->addAnimationSequence(NOTE_4_PLAYED);
	ast->addAnimationSequence(NOTE_5_CONFIRMED);
	ast->addAnimationSequence(NOTE_5_PLAYED);
	ast->addAnimationSequence(NOTE_6_CONFIRMED);
	ast->addAnimationSequence(NOTE_6_PLAYED);
	ast->addAnimationSequence(NOTE_7_CONFIRMED);
	ast->addAnimationSequence(NOTE_7_PLAYED);

	ast->addAnimationFrame(NOTE_HIDDEN, noteImageID0, 5);
	ast->addAnimationFrame(NOTE_1_CONFIRMED, noteImageID1, 5);
	ast->addAnimationFrame(NOTE_1_PLAYED, noteImageID2, 5);
	ast->addAnimationFrame(NOTE_2_CONFIRMED, noteImageID3, 5);
	ast->addAnimationFrame(NOTE_2_PLAYED, noteImageID4, 5);
	ast->addAnimationFrame(NOTE_3_CONFIRMED, noteImageID5, 5);
	ast->addAnimationFrame(NOTE_3_PLAYED, noteImageID6, 5);
	ast->addAnimationFrame(NOTE_4_CONFIRMED, noteImageID7, 5);
	ast->addAnimationFrame(NOTE_4_PLAYED, noteImageID8, 5);
	ast->addAnimationFrame(NOTE_5_CONFIRMED, noteImageID9, 5);
	ast->addAnimationFrame(NOTE_5_PLAYED, noteImageID10, 5);
	ast->addAnimationFrame(NOTE_6_CONFIRMED, noteImageID11, 5);
	ast->addAnimationFrame(NOTE_6_PLAYED, noteImageID12, 5);
	ast->addAnimationFrame(NOTE_7_CONFIRMED, noteImageID13, 5);
	ast->addAnimationFrame(NOTE_7_PLAYED, noteImageID14, 5);
	
	unsigned int spriteTypeID = spriteManager->addSpriteType(ast);
	ast->setSpriteTypeID(spriteTypeID);

	//CONSTRUCT BOTS
		NoteBot *noteBot = new NoteBot(0, 0);
		noteBot->setSpriteType(ast);
		noteBot->setCurrentState(NOTE_HIDDEN);

		spriteManager->getBotRecycler()->registerBotType(L"notebot", noteBot);
		spriteManager->getBotRecycler()->initRecyclableBots(L"notebot", 10);		
}

/*
	loadLevel - This method should load the data the level described by the
	levelInitFile argument in to the Game's game state manager.
*/
void DrummerBoyDataLoader::loadWorld(Game *game, wstring levelInitFile)	
{
	// FIRST SETUP THE GAME WORLD DIMENSIONS
	GameStateManager *gsm = game->getGSM();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *worldTextureManager = graphics->getWorldTextureManager();

	// NOTE THAT THE WORLD WILL BE THE SAME DIMENSIONS AS OUR TILED LAYER,
	// WE COULD GET A PARALLAX SCROLLING EFFECT IF WE MADE A LAYER SMALLER,
	// AND THEN SCROLLED IT SLOWER
	World *world = gsm->getWorld();
	world->setWorldWidth(NUM_COLUMNS * TILE_WIDTH);
	world->setWorldHeight((NUM_ROWS + 1) * TILE_HEIGHT);

	// NOW LOAD OUR TILED BACKGROUND
	TiledLayer *tiledLayer = new TiledLayer(	NUM_COLUMNS,	NUM_ROWS, 
												TILE_WIDTH,		TILE_HEIGHT, 
												0, true, 
												NUM_COLUMNS * TILE_WIDTH,
												NUM_ROWS * TILE_HEIGHT);
	int imageID = worldTextureManager->loadTexture(IMAGE_PATH);
	for (int i = 0; i < (NUM_COLUMNS * NUM_ROWS); i++)
	{
		Tile *tileToAdd = new Tile();
		tileToAdd->collidable = false;
		tileToAdd->textureID = imageID;
		tiledLayer->addTile(tileToAdd);
	}
	world->addLayer(tiledLayer);

	loadSprites(game);
	/*// AND NOW LET'S MAKE A MAIN CHARACTER SPRITE
	AnimatedSpriteType *ast = new AnimatedSpriteType();
	int spriteImageID0 = worldTextureManager->loadTexture(PLAYER_IDLE0_PATH);
	int spriteImageID1 = worldTextureManager->loadTexture(PLAYER_IDLE1_PATH);
	int spriteImageID2 = worldTextureManager->loadTexture(PLAYER_IDLE2_PATH);

	// SIZE OF SPRITE IMAGES
	ast->setTextureSize(PLAYER_WIDTH, PLAYER_HEIGHT);

	// NOW LET'S ADD AN ANIMATION STATE
	// FIRST THE NAME
	ast->addAnimationSequence(IDLE_STATE);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID0, 50);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID1, 20);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID2, 40);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID0, 100);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID1, 20);

	SpriteManager *spriteManager = gsm->getSpriteManager();
	unsigned int spriteTypeID = spriteManager->addSpriteType(ast);
	ast->setSpriteTypeID(spriteTypeID);

//	AnimatedSprite *player = spriteManager->getPlayer();
//	player->setSpriteType(ast);
//	PhysicalProperties *playerProps = player->getPhysicalProperties();
//	playerProps->setX(10);
//	playerProps->setY(10);
//	playerProps->setVelocity(0.0f, 0.0f);
//	playerProps->setAccelerationX(0);
//	playerProps->setAccelerationY(0);

	// WE WILL SET LOTS OF OTHER PROPERTIES ONCE
	// WE START DOING COLLISIONS AND PHYSICS

//	player->setAlpha(255);
//	player->setCurrentState(IDLE_STATE);*/
}

/*
	initCursor - initializes a simple little cursor for the gui.
*/
void DrummerBoyDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> imageIDs;
	int imageID;

// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(DBG_CURSOR_PATH);
	imageIDs.push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs.begin()),
						0,
						0,
						0,
						255,
						16,
						16);
	gui->setCursor(cursor);
}

/*
	initSplashScreen - initializes the game's splash screen gui.
*/
void DrummerBoyDataLoader::initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(DG_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							game->getGraphics()->getScreenWidth(),
							game->getGraphics()->getScreenHeight(),
							false,
							DG_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd, DG_GO_TO_MM_COMMAND);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
*/
void DrummerBoyDataLoader::initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(DG_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd, imageID);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(DG_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(DG_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							500,
							0,
							255,
							200,
							100,
							false,
							DG_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd, DG_EXIT_COMMAND);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(DG_START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(DG_START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							350,
							0,
							255,
							200,
							100,
							false,
							DG_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd, DG_START_COMMAND);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU,		mainMenuGUI);
}

/*
	initInGameGUI - initializes the game's in-game gui.
*/
void DrummerBoyDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
		// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int scrollBarID = guiTextureManager->loadTexture(DBG_SCROLL_BAR_IMAGE_PATH);
	unsigned int playButtonID = guiTextureManager->loadTexture(DBG_PLAY_IMAGE_PATH);
	unsigned int scrollLeftButtonID = guiTextureManager->loadTexture(DBG_SCROLL_LEFT_IMAGE_PATH);
	unsigned int scrollRightButtonID = guiTextureManager->loadTexture(DBG_SCROLL_RIGHT_IMAGE_PATH);
	unsigned int scrollLeftDButtonID = guiTextureManager->loadTexture(DBG_SCROLL_LEFT_D_IMAGE_PATH);
	unsigned int scrollRightDButtonID = guiTextureManager->loadTexture(DBG_SCROLL_RIGHT_D_IMAGE_PATH);
	unsigned int scrollMarkID = guiTextureManager->loadTexture(DBG_SCROLL_MARK_IMAGE_PATH);
	unsigned int extendButtonID = guiTextureManager->loadTexture(DBG_EXTEND_IMAGE_PATH);
	unsigned int extendMoButtonID = guiTextureManager->loadTexture(DBG_EXTEND_MO_IMAGE_PATH);


	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 176;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 16;
	imageToAdd->imageID = scrollBarID;
	inGameGUI->addOverlayImage(imageToAdd, scrollBarID);

	// INIT THE play BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(playButtonID, 
							playButtonID,
							0,
							32,
							0,
							255,
							32,
							32,
							false,
							DBG_PLAY_COMMAND);
	inGameGUI->addButton(buttonToAdd, DBG_PLAY_COMMAND);

	Button *scrollLeftButton = new Button();
	scrollLeftButton->initButton(scrollLeftButtonID, 
							scrollLeftButtonID,
							0,
							176,
							0,
							255,
							16,
							16,
							false,
							DBG_SCROLL_LEFT_COMMAND);
	scrollLeftButton->setDeactivatedTextureID(scrollLeftDButtonID);
	inGameGUI->addButton(scrollLeftButton, DBG_SCROLL_LEFT_COMMAND);

	Button *scrollRightButton = new Button();
	scrollRightButton->initButton(scrollRightButtonID, 
							scrollRightButtonID,
							1008,
							176,
							0,
							255,
							16,
							16,
							false,
							DBG_SCROLL_RIGHT_COMMAND);
	scrollRightButton->setDeactivatedTextureID(scrollRightDButtonID);
	inGameGUI->addButton(scrollRightButton, DBG_SCROLL_RIGHT_COMMAND);

	Button *extendButton = new Button();
	extendButton->initButton(extendButtonID, 
							extendMoButtonID,
							944,
							48,
							0,
							255,
							64,
							16,
							false,
							DBG_EXTEND_COMMAND);
	inGameGUI->addButton(extendButton, DBG_EXTEND_COMMAND);

	/*OverlayImage *scrollMarkImage = new OverlayImage();
	scrollMarkImage ->x = 17;
	scrollMarkImage ->y = 175;
	scrollMarkImage ->z = 0;
	scrollMarkImage ->alpha = 255;
	scrollMarkImage ->width = 32;
	scrollMarkImage ->height = 18;
	scrollMarkImage ->imageID = scrollMarkID;
	inGameGUI->setScrollMarkImage(scrollMarkImage);*/

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS,	inGameGUI);
}

/*
	initViewport - initializes the game's viewport.
*/
void DrummerBoyDataLoader::initViewport(GameGUI *gui, map<wstring,wstring> *properties)
{
		Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY;
	wstring viewportOffsetXProp = (*properties)[DG_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[DG_VIEWPORT_OFFSET_Y];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
	viewport->setViewportWidth(1024);
	viewport->setViewportHeight(112);
}


