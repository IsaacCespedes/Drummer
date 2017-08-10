#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\graphics\VertexList.h"
#include "SSSF_SourceCode\gsm\world\Model.h"
#include "SSSF_SourceCode\gui\Viewport.h"

Model::Model()
{
	vertices = new vector<ModelVertex*>();
	cameraPosition.x = 0.0f;
	cameraPosition.y = 0.0f;
	cameraPosition.z = 10.0f;
	focusPosition.x = 0.0f;
	focusPosition.y = 0.0f;
	focusPosition.z = 0.0f;
	upDirection.x = 0.0;
	upDirection.y = 1.0;
	upDirection.z = 0.0;
}

Model::~Model()
{
	delete vertices;
}

void Model::addVerticesToVertexList(VertexList* vertexList, Viewport* viewport)
{
	// WE'LL USE THIS IN OUR CALCULATIONS
	float viewportX = (float)(viewport->getViewportX());
	float viewportY = (float)(viewport->getViewportY());

	// TILES THAT WE PUT IN THE RENDER LIST ARE GOING TO BE
	// RENDERED, SO WE ONLY WANT TO PUT TILES IN THE LIST
	// THAT OVERLAP THE CURRENT VIEWPORT. TO ENSURE PARALLAX
	// SCROLLING, SUBTRACT THE CORRECTED parallaxViewportX 
	// & parallaxViewportY FROM THE TILE X & Y VALUES TO 
	// SCALE SCROLLING.
	int adjustedX, adjustedY;
	vector<ModelVertex*>::iterator iterator;

	// GO THROUGH ALL THE TILES, STARTING WITH THE FIRST ONE
	iterator = vertices->begin();
	while (iterator != vertices->end())
	{
		// GET THE CURRENT TILE
		ModelVertex* mv = (*iterator);
		adjustedX = mv->x - (int)viewportX;
		adjustedY = mv->y - (int)viewportY;


			// IT'S VISIBLE, SO RENDER IT
			vertexList->addVertexItem(	adjustedX,
										adjustedY,
										mv->z,
										mv->normal);


		// ONTO THE NEXT ONE
		iterator++;
	}
}

void Model::randomizeVertices()
{
	vector<ModelVertex*>::iterator iterator;
	srand(time(NULL));
	// GO THROUGH ALL THE TILES, STARTING WITH THE FIRST ONE
	iterator = vertices->begin();
	while (iterator != vertices->end())
	{
		ModelVertex* mv = (*iterator);
		mv->x = rand() % 200;
		mv->y = rand() % 200;
		iterator++;
	}
}