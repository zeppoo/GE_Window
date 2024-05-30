#include "ve_window.hpp"
#include "ve_device.hpp"

namespace ve
{
  void initWindow()
  {
    createWindow();
    createWindowSurface();
  }

  void createWindow()
  {
    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, NAME.c_str(), nullptr, nullptr);
    if (!window)
    {
      std::cerr << "Failed to create GLFW window\n";
      glfwTerminate();
      return;
    }
  }

  void createWindowSurface()
  {
    if (glfwCreateWindowSurface(vkInstance, window, nullptr, &surface) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create window surface");
    }
    glfwSetWindowUserPointer(window, window);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
  }

  static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    framebufferResized = true;
  }

  void windowLoop()
  {
    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
      drawFrame();
    }
  }
}
