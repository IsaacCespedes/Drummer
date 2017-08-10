#pragma once
#include "stdafx.h"

struct VertexItem
{
	FLOAT x, y, z; 
	D3DVECTOR normal; 
};


class VertexList
{
private:
	VertexItem *vertexList;
	int size;
	int	maxItems;
	int iteratorIndex;

public:
	VertexList(int initMaxItems);
	~VertexList();
	void		addVertexItem(	int x,
								int y,
								int z,
								D3DVECTOR norm);
	void		clear();
	VertexItem* getList(){return vertexList;}
	int			getSize(){return size;}
	bool		hasNext();
	VertexItem*	next();
	void		resetIterator();
};