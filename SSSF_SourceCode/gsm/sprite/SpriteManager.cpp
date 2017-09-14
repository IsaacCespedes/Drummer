/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.cpp

	See SpriteManager.h for a class description.
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\sound\GameSound.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSpriteType.h"
#include "SSSF_SourceCode\gsm\sprite\SpriteManager.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"

const char* SpriteManager::sounds[] = {"crash", "hihat", "snare", "hitom", "midtom", "lotom", "kick"};
const wstring SpriteManager::playedStates[] = {L"NOTE_1_PLAYED", L"NOTE_2_PLAYED", L"NOTE_3_PLAYED", L"NOTE_4_PLAYED", L"NOTE_5_PLAYED", L"NOTE_6_PLAYED", L"NOTE_7_PLAYED"};
const wstring SpriteManager::confirmedStates[] = {L"NOTE_1_CONFIRMED", L"NOTE_2_CONFIRMED", L"NOTE_3_CONFIRMED", L"NOTE_4_CONFIRMED", L"NOTE_5_CONFIRMED", L"NOTE_6_CONFIRMED", L"NOTE_7_CONFIRMED"};
/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	PhysicalProperties *pp = sprite->getPhysicalProperties();

	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	
									pp->getX(),
									pp->getY(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(	sprite->getCurrentImageID(),
									pp->round(pp->getX()-viewport->getViewportX()),
									pp->round(pp->getY()-viewport->getViewportY()),
									pp->round(pp->getZ()),
									sprite->getAlpha(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight());
	}
}

/*
	addSpriteItemsToRenderList - This method goes through all of the sprites,
	including the player sprite, and adds the visible ones to the render list.
	This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		// ADD THE PLAYER SPRITE
		//addSpriteToRenderList(&player, renderList, viewport);

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = (*botIterator);
			addSpriteToRenderList(bot, renderList, viewport);
			botIterator++;
		}
	}
}

/*
	addSprite - This method is for adding a new sprite to 
	this sprite manager. Once a sprite is added it can be 
	scheduled for rendering.
*/
void SpriteManager::addBot(Bot *botToAdd)
{
	bots.push_back(botToAdd);
}

/*
	addSpriteType - This method is for adding a new sprite
	type. Note that one sprite type can have many sprites. For
	example, we may say that there may be a "Bunny" type of
	sprite, and specify properties for that type. Then there might
	be 100 different Bunnies each with their own properties, but that
	share many things in common according to what is defined in
	the shared sprite type object.
*/
unsigned int SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd)
{
	spriteTypes.push_back(spriteTypeToAdd);
	return spriteTypes.size()-1;
}

void SpriteManager::animateNote(Game * game, int & l, int & r)
{
	if(bots.empty())
		return;

	int totalNotes = game->getGSM()->getWorld()->getWorldWidth()/16;

	if(currentNote >= totalNotes)
	{
		currentCol = bots.begin();
		lastCol = bots.begin();
		currentNote = 0;
		return;
	}
	if(currentCol == bots.end())
		return;

	//int totalNotes = game->getGSM()->getWorld()->getWorldWidth()/16;
	list<Bot*>::iterator noteAnimation = currentCol;

	/*if(currentNote >= totalNotes)
	{
		currentCol = bots.begin();
		lastCol = bots.begin();
		currentNote = 0;
		return;
	}*/

	while(noteAnimation != bots.end())
		{
			NoteBot * nb =  static_cast <NoteBot *>(*noteAnimation);
			if(currentNote == nb->getRow())
			{
				int col = nb->getCol();

				if(col == 0 || col == 2 || col == 4)
					l = col;
				if(col == 1 || col == 3 || col == 5)
					r = col;
			}
			if(currentNote < nb->getRow())
				return;
			noteAnimation++;
		}


	//reset if all the notes have been played
	/*if(lastCol == bots.end())
	{
		currentCol = bots.begin();
		lastCol = bots.begin();
		currentNote = 0;
		return false;
	}*/
}

/*
	clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	spriteTypes.clear();
	bots.clear();
}
//given mouse coordinates, either add or delete a note 
void SpriteManager::editNote(int x, int y)
{
	list<Bot*>::iterator botIterator = getBotsIterator();

	int row = x/16;
	int col = y/16;
	int position = row*7 + col;

	
	//if mouse clicked on an active note, delete it
	while(botIterator != getEndOfBotsIterator())
	{
		NoteBot * nb = static_cast<NoteBot *>(*botIterator);
		if(nb->getLocation() == position)
		{
			recyclableBots.recycleBot(L"notebot", nb);
			bots.remove(*botIterator);
			currentCol = bots.begin();
			lastCol = bots.begin();
			return;
		}
		botIterator++;
	}

	//otherwise add an active note, maintaining the order of notes based on when they should be played

	botIterator = getBotsIterator();

	NoteBot * noteBot = static_cast<NoteBot *>(recyclableBots.retrieveBot(L"notebot"));
	noteBot->setSpriteType(spriteTypes.at(0));
	noteBot->setLocation(row, col);

	while(botIterator != getEndOfBotsIterator())
	{
		NoteBot * nb = static_cast<NoteBot *>(*botIterator);
		if(nb->getLocation() > position)
		{
			break;
		}
		botIterator++;
	}

	noteBot->setCurrentState(confirmedStates[col]);

	bots.insert(botIterator, noteBot);	

	currentNote = 0;
	lastCol = bots.begin();
	currentCol = bots.begin();
}

//play the sound for each active note
bool SpriteManager::playNotes(Game * game)
{
	if(bots.empty())
		return false;

	int totalNotes = game->getGSM()->getWorld()->getWorldWidth()/16;

	if(currentNote >= totalNotes)
	{
		currentCol = bots.begin();
		lastCol = bots.begin();
		currentNote = 0;
		return false;
	}

	//deactivate the active notes (played in the last cycle)
	if(currentCol != bots.begin())
	{
		while(lastCol != currentCol)
		{
			NoteBot * nb =  static_cast <NoteBot *>(*lastCol);
			nb->setCurrentState(confirmedStates[nb->getCol()]);
			lastCol++;
		}
	}

	//if it is time for a note to be active (and have the sound played) then do it
	while(currentCol != bots.end())
		{
			NoteBot * nb =  static_cast <NoteBot *>(*currentCol);
			if(currentNote == nb->getRow())
			{
				nb->getCurrentState();
				nb->setCurrentState(playedStates[nb->getCol()]);
				game->getSound()->playSound(nb->getCol());
			}
			if(currentNote < nb->getRow())
			{
				currentNote++;
				return true;
			}
			currentCol++;
		}

	//reset if all the notes have been played
	if(lastCol == bots.end())
	{
		currentCol = bots.begin();
		lastCol = bots.begin();
		currentNote = 0;
		return false;
	}

	return true;
}

/*
	getSpriteType - This gets the sprite type object that corresponds
	to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(unsigned int typeIndex)
{
	if (typeIndex < spriteTypes.size())
		return spriteTypes.at(typeIndex);
	else
		return NULL;
}

/*
	unloadSprites - This method removes all artwork from memory that
	has been allocated for game sprites.
*/
void SpriteManager::unloadSprites()
{
	while(!bots.empty())
	{
		delete bots.front();
		bots.pop_front();
	}

	for(int i = 0; i < spriteTypes.size(); ++i) {
		delete spriteTypes[i];
	}

	clearSprites();
}

Bot* SpriteManager::removeBot(Bot *botToRemove)
{
	return NULL;
	// @TODO
}

/*
	update - This method should be called once per frame. It
	goes through all of the sprites, including the player, and calls their
	update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{
	// UPDATE THE PLAYER SPRITE
	//player.updateSprite();

	// NOW UPDATE THE REST OF THE SPRITES
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		bot->updateSprite();
		botIterator++;
	}
}