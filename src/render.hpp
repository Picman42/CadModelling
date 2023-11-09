#pragma once

#include "euler-operations.hpp"
#include <string>
#include <vector>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// GLFW
#include <GLFW/glfw3.h>
// GLUT
#include <GL/glut.h>

using namespace std;

void drawSolid(Solid* solid);
void drawFace(Face* face);
void drawLoop(Loop* loop);

void CALLBACK tessBeginCB(GLenum which);
void CALLBACK tessEndCB();
void CALLBACK tessErrorCB(GLenum errorCode);
void CALLBACK tessVertexCB(const GLvoid* data);