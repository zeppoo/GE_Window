#include "ve_window.hpp"

namespace ve {

    ve_window::ve_window(const int width, const int height, const std::string& name, VkInstance* pvkInstance, VkSurfaceKHR* psurface) : width{width}, height{height}, windowName{name}, pvkInstance{pvkInstance}, psurface{psurface}
    {
        try {
            createWindow();
            createWindowSurface(*pvkInstance, psurface);
        }
        catch (const std::exception&) {
            // Clean up GLFW resources before rethrowing
            if (window) {
                glfwDestroyWindow(window);
            }
            glfwTerminate();
            throw; // Rethrow the current exception
        }
    }

    ve_window::~ve_window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void ve_window::createWindow()
    {
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return;
        }
    }

    void ve_window::windowLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }

    void ve_window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) 
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to create window surface");
        }
    }
}
