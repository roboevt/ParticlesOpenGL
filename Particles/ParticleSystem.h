#pragma once
#include <vector>
#include <chrono>

#include "Particle.h"

class ParticleSystem {
public:
	std::vector<Particle> particles;
	void update();
	std::vector<Particle>& getParticles();
	void damp();
	void addRandomParticle();
	void removeRandomParticle();
	void setG(float Gx1000) { this->G = Gx1000 / 1000.0f; }
	float getG() { return G * 1000.0f; }
	void clearTrails();
	ParticleSystem();
	~ParticleSystem();
private:
	std::chrono::high_resolution_clock::time_point previousFrameTime;
	float G;
	
};