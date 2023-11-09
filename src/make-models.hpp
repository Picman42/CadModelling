#pragma once

#include "euler-operations.hpp"
#include "render.hpp"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

Solid* makeShape()
{
    std::vector<glm::vec3> outerRing
    {
        glm::vec3(1.0,  0.5, 2.0),
        glm::vec3(-1.0, 0.5, -0.5),
        glm::vec3(-0.5, 0.5, -0.5),
        glm::vec3(-1.0, 0.5, -2.0),
        glm::vec3(1.0,  0.5, 0.5),
        glm::vec3(0.5,  0.5, 0.5),
    };
    
    Solid* solid;
    Vertex *V0, *prevV;
    solid = mvfs(V0, outerRing[0]);
    prevV = V0;
    Loop* topLoop = solid->sFace->fLoop;

    for (int i = 1; i < outerRing.size(); i++)
    {
        HalfEdge* he = mev(prevV, outerRing[i], topLoop);
        prevV = he->endV;
    }

    Loop* downLoop = mef(V0, prevV, topLoop);

    sweep(topLoop->lFace, glm::vec3(0, -1, 0), 1);

    return solid;
}

Solid* makeShapeWithHoles()
{
    std::vector<glm::vec3> outerRing
    {
        glm::vec3(-1.0, 0.5, 2.0),
        glm::vec3(1.0,  0.5, 2.0),
        glm::vec3(2.0,  0.5, 0.0),
        glm::vec3(1.0,  0.5, -2.0),
        glm::vec3(-1.0, 0.5, -2.0),
        glm::vec3(-2.0, 0.5, 0.0),
    };

    std::vector < std::vector<glm::vec3> > innerRings
    {
        std::vector<glm::vec3> {
            glm::vec3(0.0,  0.5, 1.5),
            glm::vec3(1.0,  0.5, 0.5),
            glm::vec3(0.0,  0.5, 1.0),
            glm::vec3(-1.0, 0.5, 0.5),
        },
        std::vector<glm::vec3> {
            glm::vec3(0.0,  0.5, -1.5),
            glm::vec3(1.0,  0.5, -0.5),
            glm::vec3(0.0,  0.5, -1.0),
            glm::vec3(-1.0, 0.5, -0.5),
        },
        std::vector<glm::vec3> {
            glm::vec3(0.0,  0.5,  0.5),
            glm::vec3(0.5,  0.5,  0.0),
            glm::vec3(0.0,  0.5, -0.5),
            glm::vec3(-0.5, 0.5,  0.0),
        },
    };
    
    // Make outer rings.
    int outerRingSize = outerRing.size();
    std::vector<Vertex*> topVertices;
    std::vector<Vertex*> downVertices;
    Vertex* V0;
    
    Solid* solid = mvfs(V0, outerRing[0]);
    topVertices.push_back(V0);

    Loop* topLoop = solid->sFace->fLoop;
    for (int i = 1; i < outerRingSize; i++)
    {
        HalfEdge* he = mev(topVertices[i-1], outerRing[i], topLoop);
        topVertices.push_back(he->endV);
    }
    
    Loop* downLoop = mef(V0, topVertices[outerRingSize - 1], topLoop);
    for (int i = 0; i < outerRingSize; i++)
    {
        glm::vec3 downPosition = outerRing[i] + glm::vec3(0.0, -1.0, 0.0);
        HalfEdge* he = mev(topVertices[i], downPosition, downLoop);
        downVertices.push_back(he->endV);
    }

    for (int i = 0; i < outerRingSize; i++)
    {
        mef(downVertices[i], downVertices[(i + 1) % outerRingSize], downLoop);
    }

    // Make inner rings.
    for (auto& innerRing : innerRings)
    {
        topVertices.clear();
        downVertices.clear();

        int innerRingSize = innerRing.size();
        HalfEdge* bridge = mev(V0, innerRing[0], topLoop);
        Vertex* innerV0 = bridge->endV;
        topVertices.push_back(innerV0);

        for (int i = 1; i < innerRingSize; i++)
        {
            HalfEdge* he = mev(topVertices[i-1], innerRing[i], topLoop);
            topVertices.push_back(he->endV);
        }

        Loop* downInnerLoop = mef(innerV0, topVertices[innerRingSize - 1], topLoop);
        Loop* topInnerLoop = kemr(bridge->hEdge, topLoop);
        topInnerLoop->setInner(true);
        downInnerLoop->setInner(true);

        for (int i = 0; i < innerRingSize; i++)
        {
            glm::vec3 downPosition = innerRing[i] + glm::vec3(0.0, -1.0, 0.0);
            HalfEdge* he = mev(topVertices[i], downPosition, downInnerLoop);
            downVertices.push_back(he->endV);
        }

        for (int i = 0; i < innerRingSize; i++)
        {
            mef(downVertices[i], downVertices[(i + 1) % innerRingSize], downInnerLoop);
        }

        kfmrh(downLoop, downInnerLoop);
    }

    return solid;
}
