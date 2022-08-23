#include "Engine.h"
#include "ParticleSystem.h"

int main() {
    ParticleSystem system;
    Engine engine(system.getParticles());
    
    while (!engine.renderFrame()) {
        system.update();
    }
}