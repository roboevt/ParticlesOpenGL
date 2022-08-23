#include <vector>
#include <chrono>

#include "Particle.h"

class ParticleSystem {
public:
	std::vector<Particle> particles;
	void update();
	std::vector<Particle>& getParticles();
	ParticleSystem();
	~ParticleSystem();
private:
	std::chrono::high_resolution_clock::time_point previousFrameTime;
};