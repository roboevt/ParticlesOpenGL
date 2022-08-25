#include "Engine.h"
#include "ParticleSystem.h"

int main() {
    ParticleSystem system;
    Engine engine(&system);
    
    while (!engine.renderFrame()) {
        system.update();
    }
}