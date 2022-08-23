# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.20)

project ("Particles")

# Include sub-projects.
add_subdirectory ("Particles")

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

set_property(TARGET Particles PROPERTY CXX_STANDARD 20)

target_link_libraries(Particles PRIVATE imgui::imgui glfw opengl32.lib GLEW::GLEW)
