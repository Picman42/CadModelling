#pragma once

#include "data-structures.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Solid* mvfs(Vertex *&outVertex, glm::vec3 pos);
HalfEdge* mev(Vertex* v1, glm::vec3 v2_pos, Loop* loop);
Loop* mef(Vertex* v1, Vertex* v2, Loop*& loop);
Loop* kemr(Edge* edge, Loop* loop);
void kfmrh(Loop* outerLoop, Loop* innerLoop);

Solid* sweep(Face* face, glm::vec3 direction, float distance);