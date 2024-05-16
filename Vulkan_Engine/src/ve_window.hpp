#pragma once

#include "main.hpp"

namespace ve {

class ve_window {

public:
    ve_window(const int height, const int width, const std::string& name, VkInstance* pvkInstance, VkSurfaceKHR* surface);
    ~ve_window();

    void windowLoop();

private:

    void createWindow();
    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);


    const int width;
    const int height;
    const std::string windowName;

    GLFWwindow* window;
    VkInstance* pvkInstance;
    VkSurfaceKHR* psurface;
};
}

