#pragma once

#include "ve_pipeline.hpp"

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