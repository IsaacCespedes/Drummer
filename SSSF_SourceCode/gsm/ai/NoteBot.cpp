#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\ai\NoteBot.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"


/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
NoteBot::NoteBot(int initX, int initY)
{
	PhysicalProperties *pp = getPhysicalProperties();
	x = initX;
	y = initY;
	pp->setX(initX * 16);
	pp->setY(initY * 16);
	pp->setZ(0);
	setAlpha(255);
}

Bot* NoteBot::clone()
{
	NoteBot *botClone = new NoteBot(x, y);
	return botClone;
}


void NoteBot::setLocation(int i, int j)
{
	PhysicalProperties *pp = getPhysicalProperties();
	x = i;
	y = j;
	pp->setPosition(i*16, j*16);
}