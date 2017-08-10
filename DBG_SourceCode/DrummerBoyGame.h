#pragma once
#include "stdafx.h"
#include "vld.h"

/*
	DrummerBoyGame.h

	This file contains setup data for making the drummerBoy game. Note that nearly
	all of this data would better serve us if loaded from files.
*/

// HERE IS THE DUMMY GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
static const wstring	DBG_INIT_FILE			= L"data/DrummerBoyGameInit.txt";
static const wstring	DBG_GUI_INIT_FILE		= L"data/DrummerBoyGameGUIInit.txt";
static const string		DBG_DEBUG_FILE			= "DebugOutput.txt";


// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const wstring	DBG_TITLE				= L"TITLE";
static const wstring	DBG_SCREEN_WIDTH			= L"SCREEN_WIDTH";
static const wstring	DBG_SCREEN_HEIGHT		= L"SCREEN_HEIGHT";
static const wstring	DBG_USE_FULLSCREEN_MODE	= L"USE_FULLSCREEN_MODE";
static const wstring	DBG_TEXT_FONT_SIZE		= L"TEXT_FONT_SIZE";
static const wstring	DBG_VIEWPORT_OFFSET_X	= L"VIEWPORT_OFFSET_X";
static const wstring	DBG_VIEWPORT_OFFSET_Y	= L"VIEWPORT_OFFSET_Y";
static const wstring	DBG_FONT_COLOR_RED		= L"FONT_COLOR_RED";
static const wstring	DBG_FONT_COLOR_GREEN		= L"FONT_COLOR_GREEN";
static const wstring	DBG_FONT_COLOR_BLUE		= L"FONT_COLOR_BLUE";
static const wstring	DBG_COLOR_KEY_RED		= L"COLOR_KEY_RED";
static const wstring	DBG_COLOR_KEY_GREEN		= L"COLOR_KEY_GREEN";
static const wstring	DBG_COLOR_KEY_BLUE		= L"COLOR_KEY_BLUE";

// CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
static const wstring DBG_GO_TO_MM_COMMAND	= L"Go To Main Menu";
static const wstring DBG_START_COMMAND		= L"Start";
static const wstring DBG_EXIT_COMMAND		= L"Exit";
static const wstring DBG_QUIT_COMMAND		= L"Quit";
static const wstring DBG_PLAY_COMMAND		= L"Play";
static const wstring DBG_SCROLL_LEFT_COMMAND		= L"Scroll Left";
static const wstring DBG_SCROLL_RIGHT_COMMAND		= L"Scroll Right";
static const wstring DBG_EXTEND_COMMAND		= L"Extend";
static const wstring DBG_CURSOR_PATH	= L"textures/gui/cursor/cursor.png";
static const wstring DBG_PLAY_IMAGE_PATH		= L"textures/gui/buttons/play.png";
static const wstring DBG_SCROLL_BAR_IMAGE_PATH		= L"textures/gui/overlays/scrollbar.png";
static const wstring DBG_SCROLL_LEFT_IMAGE_PATH		= L"textures/gui/buttons/scrollleft.png";
static const wstring DBG_SCROLL_RIGHT_IMAGE_PATH		= L"textures/gui/buttons/scrollright.png";
static const wstring DBG_SCROLL_LEFT_D_IMAGE_PATH		= L"textures/gui/buttons/scrollleft_d.png";
static const wstring DBG_SCROLL_RIGHT_D_IMAGE_PATH		= L"textures/gui/buttons/scrollright_d.png";
static const wstring DBG_SCROLL_MARK_IMAGE_PATH		= L"textures/gui/overlays/scrollmark.png";
static const wstring DBG_EXTEND_IMAGE_PATH		= L"textures/gui/buttons/extend.png";
static const wstring DBG_EXTEND_MO_IMAGE_PATH		= L"textures/gui/buttons/extend_mo.png";
/*static const wstring DBG_SPLASH_SCREEN_PATH	= L"textures/gui/overlays/spiked_acid_splash_screen_overlay.png";
static const wstring DBG_MAIN_MENU_PATH		= L"textures/gui/overlays/spiked_acid_main_menu_overlay.png";
static const wstring DBG_START_IMAGE_PATH	= L"textures/gui/buttons/start_game.png";
static const wstring DBG_START_IMAGE_MO_PATH	= L"textures/gui/buttons/start_game_mo.png";
static const wstring DBG_EXIT_IMAGE_PATH		= L"textures/gui/buttons/exit_game.png";
static const wstring DBG_EXIT_IMAGE_MO_PATH	= L"textures/gui/buttons/exit_game_mo.png";
static const wstring DBG_QUIT_IMAGE_PATH		= L"textures/gui/buttons/quit.png";
static const wstring DBG_QUIT_IMAGE_MO_PATH	= L"textures/gui/buttons/quit_mo.png";
*/


// EXAMPLE LEVEL DATA FOR THE HARD-CODED loadLevelExample

// FOR BACKGROUND TILES
/*static const wstring	GRASS_IMAGE_PATH = L"textures/world/tiles/grass.png";
static const wstring	WALL_IMAGE_PATH = L"textures/world/tiles/wall.png";*/
static const wstring	IMAGE_PATH = L"textures/world/tiles/image.png";



// FOR OUR SPRITES
static const wstring	NOTE_HIDDEN = L"NOTE_HIDDEN";
static const wstring	NOTE_1_CONFIRMED = L"NOTE_1_CONFIRMED";
static const wstring	NOTE_2_CONFIRMED = L"NOTE_2_CONFIRMED";
static const wstring	NOTE_3_CONFIRMED = L"NOTE_3_CONFIRMED";
static const wstring	NOTE_4_CONFIRMED = L"NOTE_4_CONFIRMED";
static const wstring	NOTE_5_CONFIRMED = L"NOTE_5_CONFIRMED";
static const wstring	NOTE_6_CONFIRMED = L"NOTE_6_CONFIRMED";
static const wstring	NOTE_7_CONFIRMED = L"NOTE_7_CONFIRMED";
static const wstring	NOTE_1_PLAYED = L"NOTE_1_PLAYED";
static const wstring	NOTE_2_PLAYED = L"NOTE_2_PLAYED";
static const wstring	NOTE_3_PLAYED = L"NOTE_3_PLAYED";
static const wstring	NOTE_4_PLAYED = L"NOTE_4_PLAYED";
static const wstring	NOTE_5_PLAYED = L"NOTE_5_PLAYED";
static const wstring	NOTE_6_PLAYED = L"NOTE_6_PLAYED";
static const wstring	NOTE_7_PLAYED = L"NOTE_7_PLAYED";

/*static const wstring	IDLE_STATE = L"IDLE_STATE";
static const wstring	PLAYER_IDLE0_PATH = L"textures/world/sprites/tros/tros_3_0.png";
static const int		ANIM_SPEED = 6;
static const int		PLAYER_WIDTH = 32;
static const int		PLAYER_HEIGHT = 18;*/

// FOR OUR WORLD
static const int		NUM_COLUMNS = 64;
static const int		NUM_ROWS = 7;
static const int		TILE_WIDTH = 16;
static const int		TILE_HEIGHT = 16;
static const int		Z_LAYER = 0;
