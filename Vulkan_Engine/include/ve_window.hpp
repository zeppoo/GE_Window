#pragma once

#include "ve_configuration.hpp"

namespace ve {

class ve_window {

public:
    ve_window(ve_configuration& config);
    ~ve_window();

    void windowLoop();

private:

    void createWindow();
    void createWindowSurface();
    
    ve_configuration& config;
};
}

