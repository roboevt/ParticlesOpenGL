# CMakeList.txt : CMake project for Particles, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (Particles "main.cpp"  "Particle.h" "Vec3.h" "Vec3.cpp" "Vec.h" "ParticleSystem.h" "ParticleSystem.cpp" "Engine.h" "Engine.cpp")

# TODO: Add tests and install targets if needed.
