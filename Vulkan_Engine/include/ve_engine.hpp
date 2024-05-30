#pragma once

#include "ve_globals.hpp"
#include "ve_debugMessenger.hpp"
#include "ve_window.hpp"
#include "ve_device.hpp"
#include "ve_swapChain.hpp"
#include "ve_pipeline.hpp"
#include <cstring>

namespace ve
{
  class ve_engine
  {
  public:
    void initEngine();

  private:
    void Cleanup();
    void initGLFW();
    void initVULKAN();
    bool checkValidationLayerSupport();
    std::vector<const char *> getRequiredExtensions();
    void setupDebugMessenger();
  };

}