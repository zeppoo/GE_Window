# Add .cpp
add_executable (Vulkan_Engine   
"main.cpp" 
"ve_engine.cpp"
"ve_engine.hpp"
"ve_window.cpp" 
"ve_window.hpp" 
"ve_physicDevice.cpp" 
"ve_physicDevice.hpp"
"ve_logicDevice.cpp"
"ve_logicDevice.hpp" 
"ve_queueFamily.cpp" 
"ve_queueFamily.hpp" 
"ve_debugMessenger.cpp"
"ve_debugMessenger.hpp")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Vulkan_Engine PROPERTY CXX_STANDARD 20)
endif()