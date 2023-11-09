// STL
#include <vector>
#include <iostream>
#include <algorithm>
// OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render.hpp"
#include "controller.hpp"
#include "make-models.hpp"

const glm::ivec2 SCREEN_SIZE(1280, 720);
GLFWwindow* window;

int InitGLFW()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Create window
    window = glfwCreateWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "CAD Project", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    // Windowed
    glfwMakeContextCurrent(window);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    return 0;
}

void InitController()
{
    glfwSetKeyCallback(window, Controller::KeyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ReleaseController()
{
    Controller::Release();
}

int main()
{
    double cameraRotationPerSecond = 0.5;
    
    InitGLFW();
    InitController();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    vector<Solid*> solids;
    solids.push_back(makeShapeWithHoles());
    solids.push_back(makeShape());

    GLfloat deltaTime = 0, currentTime = 0;
    GLfloat startTime = glfwGetTime();
    GLfloat lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        Solid* solid = solids[Controller::count % solids.size()];
        glfwPollEvents();
        GLfloat newTime = glfwGetTime();
        Controller::Movement(newTime - lastTime);
        lastTime = newTime;
        GLfloat time = newTime - startTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
        glViewport(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y);

        // Perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (double)SCREEN_SIZE.x / (double)SCREEN_SIZE.y, 0.1f, 100.0f);

        // Get camera position.
        double angle = cameraRotationPerSecond * time * 3.14159265358979323846 * 2.0;
        double cameraX = Controller::cameraDistance * cos(angle);
        double cameraZ = Controller::cameraDistance * sin(angle);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); // Reset the model-view matrix
        gluLookAt(cameraX, Controller::cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);

        glRotatef(180.0, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0, 1.0f, 0.0f, 0.0f);

        drawSolid(solid);

        glfwSwapBuffers(window);
    }

    ReleaseController();

    glfwTerminate();
    return 0;
}

