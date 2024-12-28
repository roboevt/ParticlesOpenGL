#include "Engine.h"
#include "ParticleSystem.h"

int main() {
    ParticleSystem system;
    Engine engine(&system);
    int frames = 0;
    while (!engine.renderFrame()) {
        frames++;
        system.update();
        if(!(frames % 8) && system.getParticles().size() < 1000) {
            system.addParticle(Particle(Vec2(0, 0), Vec2(-0.0001f, -0.001f)));
            system.addParticle(Particle(Vec2(0.01, 0), Vec2(-0.0001f, -0.001f)));
            system.addParticle(Particle(Vec2(0.02, 0), Vec2(-0.0001f, -0.001f)));
            system.addParticle(Particle(Vec2(0.03, 0), Vec2(-0.0001f, -0.001f)));
        }
    }
}