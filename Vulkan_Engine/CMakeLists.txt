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

# Find SubFolders
add_subdirectory ("src")
add_subdirectory ("include/glfw")
add_subdirectory ("include/glm")
add_subdirectory("include/imgui")

# Find Vulkan package
find_package(Vulkan REQUIRED)

# Link libraries to Project
target_link_libraries (Vulkan_Engine glfw)
target_link_libraries (Vulkan_Engine glm)
target_link_libraries (Vulkan_Engine imgui)
target_link_libraries(Vulkan_Engine Vulkan::Vulkan)

# Give imgui access to libraries
target_include_directories(imgui PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(imgui PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE ${Vulkan_LIBRARIES})
target_link_libraries(imgui PRIVATE glfw)