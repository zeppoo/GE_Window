#pragma once

#include "ve_globals.hpp"

namespace ve
{
    void initWindow();

    void windowLoop();

    static void createWindow();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    static void createWindowSurface();
}

