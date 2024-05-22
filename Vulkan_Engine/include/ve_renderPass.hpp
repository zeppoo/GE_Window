#pragma once

#include "ve_configuration.hpp"
#include "ve_swapChain.hpp"

namespace ve {

class ve_renderPass {
public:
    ve_renderPass(ve_configuration& config);

    void createRenderPass();

private:
    ve_configuration& config;
};

}