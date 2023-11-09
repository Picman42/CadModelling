#include "euler-operations.hpp"

#include <iostream>

void swapLoopsIfLarger(Loop*& loop1, Loop*& loop2)
{
    // If loop1.size > loop2.size, swap them.
    int size1 = loop1->size(), size2 = loop2->size();
    if (loop1->size() > loop2->size())
    {
        auto tmp = loop1;
        loop1 = loop2;
        loop2 = tmp;
    }
}

Solid* mvfs(Vertex *&outVertex, glm::vec3 pos)
{
    Solid* solid = new Solid();
    Face* face = new Face();
    Loop* loop = new Loop();

    outVertex = new Vertex();
    outVertex->pos = pos;

    solid->sFace = face;
    face->fSolid = solid;
    face->fLoop = loop;
    loop->lFace = face;

    return solid;
}

HalfEdge* mev(Vertex* v1, glm::vec3 v2_pos, Loop* loop)
{
    HalfEdge* he1 = new HalfEdge();
    HalfEdge* he2 = new HalfEdge();
    Edge* edge = new Edge();
    he1->hEdge = he2->hEdge = edge;
    he1->sibling = edge->he2 = he1->next = he2;
    he2->sibling = edge->he1 = he2->prev = he1;

    Vertex* v2 = new Vertex();
    v2->pos = v2_pos;
    he1->startV = he2->endV = v1;
    he1->endV = he2->startV = v2;

    he1->hLoop = he2->hLoop = loop;
    if (loop->lHalfEdge == nullptr)
    {
        he2->next = he1;
        he1->prev = he2;
        loop->lHalfEdge = he1;
    }
    else
    {
        HalfEdge* he;
        // Traverse to the last
        for (he = loop->lHalfEdge; he->next->startV != v1; he = he->next) {}
        he2->next = he->next;
        he->next->prev = he2;
        he1->prev = he;
        he->next = he1;
    }

    Solid* solid = loop->lFace->fSolid;
    Edge* e = solid->sEdge;
    if (e == nullptr)
    {
        solid->sEdge = edge;
    }
    else
    {
        while (e->nextE != nullptr)     e = e->nextE;
        e->nextE = edge;
        edge->prevE = e;
    }

    return he1;
}

Loop* mef(Vertex* v1, Vertex* v2, Loop*& loop)
{
    HalfEdge* he1 = new HalfEdge();
    HalfEdge* he2 = new HalfEdge();
    Edge* edge = new Edge();
    he1->hEdge = he2->hEdge = edge;
    he1->sibling = edge->he2 = he2;
    he2->sibling = edge->he1 = he1;
    he1->startV = he2->endV = v1;
    he1->endV = he2->startV = v2;

    auto findHalfEdgeStartWithVertex = [](Loop* loop, Vertex* vertex) -> HalfEdge*
    {
        // Find the halfedge in the loop that starts with the given vertex.
        // If the given vertex is not found in the loop, returns nullptr.
        HalfEdge* hStart = loop->lHalfEdge;
        HalfEdge* he = hStart;
        while (he->startV != vertex)
        {
            he = he->next;
            if (he == hStart)
            {
                std::cout << "Error: mef(): cannot find vertex in the given loop." << std::endl;
                return nullptr;
            }
        }
        return he;
    };

    HalfEdge* tmpHe1 = findHalfEdgeStartWithVertex(loop, v1);
    HalfEdge* tmpHe2 = findHalfEdgeStartWithVertex(loop, v2);
    if (!tmpHe1 || !tmpHe2)
    {
        return nullptr;
    }

    // Add new halfedges to the loop.
    he2->next = tmpHe1;
    he2->prev = tmpHe2->prev;
    he1->next = tmpHe2;
    he1->prev = tmpHe1->prev;
    tmpHe1->prev->next = he1;
    tmpHe1->prev = he2;
    tmpHe2->prev->next = he2;
    tmpHe2->prev = he1;

    loop->lHalfEdge = he1;
    Loop* loopNew = new Loop();
    loopNew->lHalfEdge = he2;

    Solid* solid = loop->lFace->fSolid;

    Face* faceNew = new Face();
    faceNew->fLoop = loopNew;
    loopNew->lFace = faceNew;
    faceNew->fSolid = solid;

    // Add face to the solid.
    Face* fLast;
    for (fLast = solid->sFace; fLast->nextF != nullptr; fLast = fLast->nextF) {}
    fLast->nextF = faceNew;
    faceNew->prevF = fLast;

    // Add edge to the solid.
    Edge* e = solid->sEdge;
    if (e == nullptr)
    {
        solid->sEdge = edge;
    }
    else
    {
        while (e->nextE)    e = e->nextE;
        e->nextE = edge;
        edge->prevE = e;
    }

    swapLoopsIfLarger(loopNew, loop);

    return loopNew;
}

Loop* kemr(Edge* edge, Loop* loop)
{
    HalfEdge* he1 = edge->he1;
    HalfEdge* he2 = edge->he2;
    he1->next->prev = he2->prev;
    he2->prev->next = he1->next;
    he1->prev->next = he2->next;
    he2->next->prev = he1->prev;

    Loop* loopNew = new Loop();
    loop->lHalfEdge = he1->prev;
    loopNew->lHalfEdge = he1->next;

    // Add loop to the face.
    Face* face = loop->lFace;
    loopNew->lFace = face;
    Loop* l;
    for (l = face->fLoop; l->nextL != nullptr; l = l->nextL) {}
    l->nextL = loopNew;
    loopNew->prevL = l;

    // Delete edge from the solid.
    Solid* solid = face->fSolid;
    Edge* e = solid->sEdge;
    if (e == edge)
    {
        solid->sEdge = edge->nextE;
        solid->sEdge->prevE = e->prevE;
    }
    else
    {
        while (e->nextE != edge)    e = e->nextE;
        e->nextE = edge->nextE;
        e->nextE->prevE = e;
    }

    swapLoopsIfLarger(loopNew, loop);

    return loopNew;
}

void kfmrh(Loop* outerLoop, Loop* innerLoop)
{
    Face* face1 = outerLoop->lFace;
    Face* face2 = innerLoop->lFace;

    Loop* l;
    for (l = face1->fLoop; l->nextL != nullptr; l = l->nextL) {}
    l->nextL = innerLoop;
    innerLoop->prevL = l;
    innerLoop->lFace = face1;

    Solid* solid = face1->fSolid;
    Face* f = solid->sFace;
    if (f == face2)
    {
        solid->sFace = f->nextF;
        solid->sFace->prevF = f->prevF;
    }
    else
    {
        while (f->nextF != face2)   f = f->nextF;
        f->nextF = face2->nextF;
        if (f->nextF) f->nextF->prevF = f;
    }
}

Solid* sweep(Face* face, glm::vec3 direction, float distance)
{
    // Normalize the direction.
    glm::vec3 directionNorm = glm::normalize(direction);

    Solid* solid = face->fSolid;
    for (Loop* loop = face->fLoop; loop != nullptr; loop = loop->nextL)
    {
        HalfEdge* he = loop->lHalfEdge;
        Vertex* startV = he->startV;
        glm::vec3 pos = startV->pos + distance * directionNorm;
        //std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
        HalfEdge* heStart = mev(startV, pos, loop);
        Vertex* vStart = heStart->endV;

        he = he->next;
        Vertex* v = he->startV;
        while (v != startV)
        {
            pos = v->pos + distance * directionNorm;
            HalfEdge* heNew = mev(v, pos, loop);

            Vertex* vNew = heNew->endV;
            
            mef(vNew, vStart, loop);
            vStart = vNew;
            he = he->next;
            v = he->startV;

        }

        /*h = loop->lHalfEdge;
        for (int i = 0; i < 10; i++)
        {
            pos = h->startV->pos;
            std::cout << "LOOP: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
            h = h->next;
        }*/

        mef(heStart->endV, vStart, loop);
    }

    return solid;
}