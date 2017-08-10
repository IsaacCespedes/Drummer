#pragma once

#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

class Bot : public AnimatedSprite
{
public:
	Bot()	{}
	~Bot()	{}

	// TO BE DEFINED BY BOT AI CLASSES
	virtual Bot* clone()=0;
};