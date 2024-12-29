// cmake --build build
// ./build/Particles/Particles

#include "Engine.h"
#include "ParticleSystem.h"

int main() {
    ParticleSystem system;
    Engine engine(&system);
    int frames = 0;
    while (!engine.renderFrame()) {
        frames++;
        system.update();
        if(!(frames % 4) && system.getParticles().size() < 4096) {
            system.addParticle(Particle(Vec2(0, 0), Vec2(-0.001f, -0.001f)));
            system.addParticle(Particle(Vec2(0.02, 0), Vec2(-0.001f, -0.001f)));
            system.addParticle(Particle(Vec2(0.04, 0), Vec2(-0.001f, -0.001f)));
            system.addParticle(Particle(Vec2(0.06, 0), Vec2(-0.001f, -0.001f)));
        }
    }
}