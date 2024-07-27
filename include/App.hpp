#pragma once

#include <Grid.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class App {
public:
    Grid* grid;
    void run();
private:
    GLFWwindow* window;

    bool paused = false;

    void drawMenuBar();
};