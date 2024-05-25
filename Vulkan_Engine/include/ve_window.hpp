#pragma once

#include "ve_configuration.hpp"
#include "ve_device.hpp"

namespace ve {

class ve_window {

public:
    ve_window(ve_configuration& config);
    ~ve_window();

    void windowLoop(ve_device device);

private:

    void createWindow();
    void createWindowSurface();

    ve_configuration& config;
};
}

