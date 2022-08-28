#pragma once
#include <vector>
#include <chrono>

#include "Particle.h"

class ParticleSystem {
public:
	std::vector<Particle> particles;
	std::vector<Particle> trail;
	void update();
	std::vector<Particle>& getParticles();
	std::vector<Particle>& getTrail();
	void damp();
	void addRandomParticle();
	void removeRandomParticle();
	void setG(float Gx1000) { this->G = Gx1000 / 1000.0f; }
	float getG() { return G * 1000.0f; }
	void setTrailLength(int numTrail);
	int getTrailLength() { return numTrail; }
	void clearTrails();
	ParticleSystem();
	~ParticleSystem();
private:
	std::chrono::high_resolution_clock::time_point previousFrameTime;
	float G;
	int numMain;
	int numTrail;
	int currentTrailEnd;
	float bounceDamp;
};