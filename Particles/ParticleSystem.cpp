#include "ParticleSystem.h"

#include <iostream>

float randf(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

ParticleSystem::ParticleSystem() {
	int count = 1000000;
	particles.reserve(count);
	for (int i = 0; i < count; i++) {
		particles.push_back(Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f)}, {randf(-0.5f, 0.5f), randf(-0.5f, 0.5f)}, 10.0f));
	}
	previousFrameTime = std::chrono::high_resolution_clock::now();
}

std::vector<Particle>& ParticleSystem::getParticles() {
	return particles;
}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::update() {
	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsedTime = now - previousFrameTime;
	previousFrameTime = now;
	float frameTime = elapsedTime.count();
	for (Particle& part : particles) {
		part.position += part.velocity * frameTime;
		if (part.position.x() > 1.0f || part.position.x() < -1.0f) part.velocity.x(-part.velocity.x());
		if (part.position.y() > 1.0f || part.position.y() < -1.0f) part.velocity.y(-part.velocity.y());

	}
}