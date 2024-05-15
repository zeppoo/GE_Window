#include "ve_window.hpp"
#include <iostream>

namespace ve {

    ve_window::ve_window(int height, int width, const std::string& name) : width{width}, height{height}, windowName{name}
    {
        initWindow();
    }

    ve_window::~ve_window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void ve_window::initWindow() {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);
    }

    void ve_window::mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }
}
