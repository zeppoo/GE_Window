#pragma once

#include "GLFW/glfw3.h"
#include <string>

namespace ve {

class ve_window {

public:
    ve_window(const int height, const int width, const std::string& name);
    ~ve_window();

    void mainLoop();


private:

    void initWindow();

    const int width;
    const int height;

    std::string windowName;
    GLFWwindow *window;

};


}

