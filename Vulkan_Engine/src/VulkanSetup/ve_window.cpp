#include "ve_window.hpp"
namespace ve {

    ve_window::ve_window(ve_configuration& config) : config{config}
    {
        try {
            createWindow();
            createWindowSurface();
        }
        catch (const std::exception&) {
            // Clean up GLFW resources before rethrowing
            if (config.window) {
                glfwDestroyWindow(config.window);
            }
            glfwTerminate();
            throw; // Rethrow the current exception
        }
    }

    ve_window::~ve_window() {
        glfwDestroyWindow(config.window);
        glfwTerminate();
    }

    void ve_window::createWindow()
    {
        config.window = glfwCreateWindow(config.WIN_WIDTH, config.WIN_HEIGHT, config.NAME.c_str(), nullptr, nullptr);
        if (!config.window) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return;
        }
    }

    void ve_window::windowLoop(ve_device device) {
        while (!glfwWindowShouldClose(config.window)) {
            glfwPollEvents();
            device.drawFrame();
        }
    }

    void ve_window::createWindowSurface() 
    {
        if (glfwCreateWindowSurface(config.vkInstance, config.window, nullptr, &config.surface) != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to create window surface");
        }
    }




}
