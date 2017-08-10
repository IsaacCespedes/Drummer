#include "stdafx.h"
#include "SSSF_SourceCode\graphics\VertexList.h"


VertexList::VertexList(int initMaxItems)
{
	maxItems = initMaxItems;
	vertexList = new VertexItem[maxItems];
	iteratorIndex = 0;
	size = 0;
}

VertexList::~VertexList()
{
	delete [] vertexList;
}

void VertexList::addVertexItem(	int x,
								int y,
								int z,
								D3DVECTOR norm)
{

	if (size < maxItems)
	{
		vertexList[size].x = x;
		vertexList[size].y = y;
		vertexList[size].z = z;

		vertexList[size].normal = norm;
		size++;
	}
}

void VertexList::clear()
{
	size = 0;
	iteratorIndex = 0;
}

bool VertexList::hasNext()
{
	return (iteratorIndex < size);
}

VertexItem* VertexList::next()
{
	if(hasNext())
	{
		VertexItem vItem = vertexList[iteratorIndex];
		iteratorIndex++;
		return &vItem;
	}
}

void VertexList::resetIterator()
{
	iteratorIndex = 0;
}