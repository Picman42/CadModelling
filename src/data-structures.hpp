#pragma once

#include <glm/glm.hpp>

class Solid;
class Face;
class Loop;
class HalfEdge;
class Edge;
class Vertex;

class Solid
{
public:
	Solid *prevS, *nextS;
	Face* sFace;
	Edge* sEdge;
	Solid(): prevS(nullptr), nextS(nullptr), sFace(nullptr), sEdge(nullptr) {}
};

class Face
{
public:
	Face *prevF, *nextF;
	Solid* fSolid;
	Loop* fLoop;
	Face() : prevF(nullptr), nextF(nullptr), fSolid(nullptr), fLoop(nullptr) {}
};

class HalfEdge
{
public:
	HalfEdge *prev, *next;
	HalfEdge* sibling;
	Loop* hLoop;
	Edge* hEdge;
	Vertex *startV, *endV;
	HalfEdge() : prev(nullptr), next(nullptr), sibling(nullptr), hLoop(nullptr), hEdge(nullptr), startV(nullptr), endV(nullptr) {}
	int loopSize()
	{
		// Return the size of the loop of this halfedge.
		int count = 1;
		HalfEdge* he = this;
		while (he)
		{
			he = he->next;
			if (he == this) break;
			count++;
		}
		return count;
	}
};

class Loop
{
public:
	Loop* prevL, * nextL;
	Face* lFace;
	HalfEdge* lHalfEdge;
	//int size();
	bool isInner;
	Loop() : prevL(nullptr), nextL(nullptr), lFace(nullptr), lHalfEdge(nullptr), isInner(false) {}
	void setInner(bool inner) { isInner = inner; }
	int size() { return lHalfEdge ? lHalfEdge->loopSize() : 0; }
};

class Edge
{
public:
	Edge *prevE, *nextE;
	HalfEdge *he1, *he2;
	Edge() : prevE(nullptr), nextE(nullptr), he1(nullptr), he2(nullptr) {}
};

class Vertex
{
public:
	Vertex* prevV, * nextV;
	glm::vec3 color;
	glm::vec3 pos;
	Vertex() : prevV(nullptr), nextV(nullptr), pos(0, 0, 0)
	{
		color = glm::vec3(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f);
	}
};