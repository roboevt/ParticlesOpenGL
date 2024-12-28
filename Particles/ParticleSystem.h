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
	void addParticle(Particle part);
	void removeRandomParticle();
	void setG(float G) { this->G = G; }
	float getG() { return G; }
	ParticleSystem();
	~ParticleSystem();
private:
	void applyGravity(Particle& part);
	void applyCollision(Particle& part1, Particle& part2);

	std::chrono::high_resolution_clock::time_point previousFrameTime;
	float G;
	int numMain;
	float bounceDamp;
	int subSteps;
};