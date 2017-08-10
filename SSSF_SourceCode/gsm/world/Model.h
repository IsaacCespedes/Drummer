#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\graphics\VertexList.h"
#include "SSSF_SourceCode\gui\Viewport.h"



struct ModelVertex{
	 FLOAT x, y, z;
	D3DVECTOR normal;
};

struct Vertex3D{
	FLOAT x, y, z;
};

class Model {
private:
	vector <ModelVertex*>* vertices;
	Vertex3D cameraPosition;
	Vertex3D focusPosition;
	Vertex3D upDirection;

public:
	Model();
	~Model();

	void		addVertex(ModelVertex* mv){vertices->push_back(mv);}
	ModelVertex* getVertex(int index){return vertices->at(index);}
	Vertex3D getCamera(){return cameraPosition;}
	Vertex3D getFocus(){return focusPosition;}
	Vertex3D getUp(){return upDirection;}
	void		setVertex(ModelVertex* mv, int index){vertices->at(index) = mv;}
	void		addVerticesToVertexList(VertexList* vertexList, Viewport* viewport);
	void			randomizeVertices();
};