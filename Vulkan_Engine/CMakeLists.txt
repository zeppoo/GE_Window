cmake_minimum_required (VERSION 3.8)
if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Vulkan_Engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add scripts
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(Vulkan_Engine ${SOURCES})

# Find SubFolders
add_subdirectory ("lib/glfw")
add_subdirectory ("lib/glm")
add_subdirectory("lib/imgui")

# Find Vulkan package
find_package(Vulkan REQUIRED)

# Include directories for Vulkan, GLFW, and GLM
target_include_directories(Vulkan_Engine PRIVATE
    include
    libs/glfw/include
    libs/glm
    libs/imgui
    ${Vulkan_INCLUDE_DIRS}
)

# Link libraries to Project
target_link_libraries(Vulkan_Engine PRIVATE glfw glm imgui Vulkan::Vulkan)

# Give imgui access to libraries
target_include_directories(imgui PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(imgui PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE ${Vulkan_LIBRARIES})
target_link_libraries(imgui PRIVATE glfw)