# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "/home/roboevt/dev/cuda/ParticlesOpenGL/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_ROOT "/home/roboevt/dev/cuda/ParticlesOpenGL/vcpkg")
project ("Particles")

set(CMAKE_CXX_FLAGS "-msse4.1 -Ofast")

# Include sub-projects.
add_subdirectory ("Particles")

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})

set_property(TARGET Particles PROPERTY CXX_STANDARD 20)

target_link_libraries(Particles PRIVATE imgui::imgui glfw GL GLEW::GLEW)
