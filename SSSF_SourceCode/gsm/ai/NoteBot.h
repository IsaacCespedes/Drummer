#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"

class NoteBot : public Bot
{
private:
	int x, y;
public:
	NoteBot(int initX, int initY);
	~NoteBot();
	Bot*	clone();
	int getCol(){return y;}
	int  getLocation(){return (x * 7) + y;}
	int  getRow(){return x;}
	void setLocation(int i, int j);
	void think(Game *game){}
};