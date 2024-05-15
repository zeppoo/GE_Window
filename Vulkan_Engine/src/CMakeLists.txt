# Add executables
add_executable (Vulkan_Engine   "ve_window.hpp" "ve_window.cpp" "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Vulkan_Engine PROPERTY CXX_STANDARD 20)
endif()