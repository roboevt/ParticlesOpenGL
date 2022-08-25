#include "ParticleSystem.h"

#include <iostream>
#include <cmath>

float randf(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

ParticleSystem::ParticleSystem() : G(0.001f) {
	srand(time(0));
	int count = 3;
	particles.reserve(count);
	for (int i = 0; i < count; i++) {
		addRandomParticle();
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
	for (int i = 0; i < particles.size(); i++) {
		Particle& part1 = particles[i];

		//Gravitational force calculation
		Vec2 netForce(0, 0);
		for (int j = 0; j < particles.size(); j++) {
			if (j == i) continue;
			Particle part2 = particles[j];
			const float fudge = 0.02f;
			Vec2 direction = part2.position - part1.position;
			float force = (G * part1.mass * part2.mass) / (direction.magnitudeSquared() + fudge);
			netForce += direction.normalized() * force;
		}
		
		//Integration
		Vec2 acceleration = netForce / part1.mass;
		part1.velocity += acceleration * frameTime;
		part1.position += part1.velocity * frameTime;

		//Bounds checking
		if (part1.position.x() > 1.0f) part1.velocity.x(-std::abs(part1.velocity.x()));
		if (part1.position.x() < -1.0f) part1.velocity.x(std::abs(part1.velocity.x()));
		if (part1.position.y() < -1.0f) part1.velocity.y(std::abs(part1.velocity.y()));
		if (part1.position.y() > 1.0f) part1.velocity.y(-std::abs(part1.velocity.y()));
	}
}

void ParticleSystem::damp() {
	for (Particle& part : particles) {
		part.velocity /= 5;
	}
}

void ParticleSystem::addRandomParticle() {
	particles.push_back(Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f) }, { randf(-0.5f, 0.5f), randf(-0.5f, 0.5f) }, randf(2.0f, 100.0f)));
}

void ParticleSystem::removeRandomParticle() {
	particles.pop_back();
}