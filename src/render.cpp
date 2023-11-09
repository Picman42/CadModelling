#include "render.hpp"

void drawSolid(Solid* solid)
{
    Face* startF = solid->sFace;
    Face* face = solid->sFace;
    do
    {
        if (face != nullptr)
        {
            drawFace(face);
        }
        face = face->nextF;
    } while (face != nullptr && face != startF);
}

void drawFace(Face* face)
{
    // Render the inner loop.
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_ALWAYS, 1, ~0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    Loop* startL = face->fLoop;
    Loop* l = face->fLoop;
    do
    {
        if (l != nullptr && l->isInner)
        {
            drawLoop(l);
        }
        l = l->nextL;
    } while (l != nullptr && l != startL);

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, 0, ~0);
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Render the outer loop.
    startL = face->fLoop;
    l = face->fLoop;
    do
    {
        if (l != nullptr && !l->isInner)
        {
            drawLoop(l);
        }
        l = l->nextL;
    } while (l != nullptr && l != startL);

    glDisable(GL_STENCIL_TEST);
}

struct TessData
{
    GLdouble data[6];
    TessData(glm::vec3 point, glm::vec3 color)
    {
        data[0] = point.x;
        data[1] = point.y;
        data[2] = point.z;
        data[3] = color.r;
        data[4] = color.g;
        data[5] = color.b;
    }
};

void drawLoop(Loop* loop)
{
    GLuint id = glGenLists(1); // create a display list
    if (!id)
        return; // failed to create a list, return 0

    GLUtesselator* tess = gluNewTess(); // create a tessellator
    if (!tess)
        return; // failed to create tessellation object, return 0

    GLdouble quad1[6] = { -1, 3, 0, 1, 1, 1 };
    GLdouble quad2[6] = { 0, 0, 0, 1, 1, 1 };
    GLdouble quad3[6] = { 1, 3, 0, 1, 1, 1 };
    GLdouble quad4[6] = { 0, 2, 0, 1, 1, 1 };
    GLdouble quad5[6] = { -1, 5, 0, 1, 1, 1 };

    // register callback functions
    gluTessCallback(tess, GLU_TESS_BEGIN, (void(CALLBACK*)())tessBeginCB);
    gluTessCallback(tess, GLU_TESS_END, (void(CALLBACK*)())tessEndCB);
    gluTessCallback(tess, GLU_TESS_VERTEX, (void(CALLBACK*)())tessVertexCB);

    glNewList(id, GL_COMPILE);
    glColor3f(1, 1, 1);
    gluTessBeginPolygon(tess, 0); // with NULL data
    gluTessBeginContour(tess);
    HalfEdge* startHe = loop->lHalfEdge;
    HalfEdge* he = loop->lHalfEdge;
    Vertex* startV = he->startV;
    vector<TessData> quadList;
    do
    {
        glm::vec3 point = he->startV->pos;
        glm::vec3 color = he->startV->color;
        TessData data(point, color);
        quadList.push_back(data);
        he = he->next;
    } while (nullptr != he && he != startHe);
    for (int i = 0; i < quadList.size(); i++)
    {
        gluTessVertex(tess, quadList[i].data, quadList[i].data);
    }
    // gluTessVertex(tess, quad1, quad1);
    // gluTessVertex(tess, quad2, quad2);
    // gluTessVertex(tess, quad3, quad3);
    // gluTessVertex(tess, quad4, quad4);
    // gluTessVertex(tess, quad5, quad5);

    gluTessEndContour(tess);
    gluTessEndPolygon(tess);
    glEndList();

    gluDeleteTess(tess); // delete after tessellation

    glCallList(id);
}



void CALLBACK tessBeginCB(GLenum which)
{
    glBegin(which);
}

void CALLBACK tessEndCB()
{
    glEnd();
}

void CALLBACK tessVertexCB(const GLvoid* data)
{
    // cast back to double type
    const GLdouble* ptr = (const GLdouble*)data;

    glColor3dv(ptr + 3);
    glVertex3dv(ptr);
}