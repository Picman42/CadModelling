#include "controller.hpp"
#include <iostream>

using namespace std;

double Controller::cameraY = 0.0;
double Controller::cameraDistance = 10.0;
unsigned int Controller::count = 0;
bool Controller::keys[1024] = {false};

bool Controller::Initialize()
{
    return true;
}

void Controller::Release() {}

void Controller::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        count++;
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void Controller::Movement(float deltaTime)
{
    const double MAX_HEIGHT = 20.0;
    const double MIN_DISTANCE = 0.5;
    const double MAX_DISTANCE = 20.0;
    
    deltaTime *= 50;
    // Camera controls
    if (keys[GLFW_KEY_W])
        cameraY -= 0.3 * deltaTime;
    if (keys[GLFW_KEY_S])
        cameraY += 0.3 * deltaTime;
    if (keys[GLFW_KEY_A])
        cameraDistance += 0.2 * deltaTime;
    if (keys[GLFW_KEY_D])
        cameraDistance -= 0.2 * deltaTime;

    if (cameraY < -MAX_HEIGHT)
        cameraY = -MAX_HEIGHT;
    if (cameraY >  MAX_HEIGHT)
        cameraY =  MAX_HEIGHT;
    if (cameraDistance < MIN_DISTANCE)
        cameraDistance = MIN_DISTANCE;
    if (cameraDistance > MAX_DISTANCE)
        cameraDistance = MAX_DISTANCE;

}
