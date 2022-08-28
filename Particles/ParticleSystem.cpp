#include "ParticleSystem.h"

#include <iostream>
#include <cmath>

float randf(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

ParticleSystem::ParticleSystem() : G(0.001f), numMain(3), numTrail(1000), bounceDamp(0.95f) {
	srand(time(0));
	currentTrailEnd = numMain;
	particles.reserve(numMain);
	for (int i = 0; i < numMain; i++) {
		particles.push_back(Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f) }, { randf(-0.5f, 0.5f), randf(-0.5f, 0.5f) }, randf(5.0f, 100.0f)));
	}
	//For testing trails atm, not final
	trail.reserve(numTrail);
	for (int i = 0; i < numTrail; i++) {
		trail.push_back(Particle(Vec2(), Vec2(), 00.0f));
	}
	previousFrameTime = std::chrono::high_resolution_clock::now();
}

std::vector<Particle>& ParticleSystem::getParticles() {
	return particles;
}

std::vector<Particle>& ParticleSystem::getTrail() {
	return trail;
}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::update() {
	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsedTime = now - previousFrameTime;
	previousFrameTime = now;
	float frameTime = elapsedTime.count();
	if (frameTime > 0.01f) frameTime = 0.01f; // min calculation rate of 100hz - goes to "slow motion" if computer can't keep up. Also eliminates hitches from resizing.

	for (int i = 0; i < numMain; i++) {
		Particle& part1 = particles[i];

		//Gravitational force calculation
		Vec2 netForce(0, 0);
		for (int j = 0; j < numMain; j++) {
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
		if (part1.position.x() > 1.0f) part1.velocity.x(-std::abs(part1.velocity.x()) * bounceDamp);
		if (part1.position.x() < -1.0f) part1.velocity.x(std::abs(part1.velocity.x()) * bounceDamp);
		if (part1.position.y() < -1.0f) part1.velocity.y(std::abs(part1.velocity.y()) * bounceDamp);
		if (part1.position.y() > 1.0f) part1.velocity.y(-std::abs(part1.velocity.y()) * bounceDamp);

		if (currentTrailEnd >= numTrail) currentTrailEnd = 0;
		trail[currentTrailEnd++] = Particle(part1.position, Vec2(), 2.5f);
		
		//particles.at(currentPart++) = Particle(part1.position, Vec2(), 2.5f);
	}
}

void ParticleSystem::damp() {
	for (Particle& part : particles) {
		part.velocity /= 5;
	}
}

void ParticleSystem::addRandomParticle() {
	clearTrails();
	numMain++;
	particles.push_back(Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f) }, { randf(-0.5f, 0.5f), randf(-0.5f, 0.5f) }, randf(2.0f, 100.0f)));
	//particles.at(numMain - 1) = (Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f) }, { randf(-0.5f, 0.5f), randf(-0.5f, 0.5f) }, randf(2.0f, 100.0f)));	
}

void ParticleSystem::removeRandomParticle() {
	//clearTrails();
	particles.pop_back();
	numMain--;
}

void ParticleSystem::clearTrails() {
	//particles.erase(particles.begin() + numMain, particles.end());
}

void ParticleSystem::setTrailLength(int newTrail) {
	if (newTrail > numTrail) {
		trail.reserve(newTrail);
		for (int i = numTrail; i < newTrail; i++) {
			trail.push_back(Particle(Vec2(), Vec2(), 00.0f));
		}
	}
	else {
		trail.erase(trail.begin() + newTrail, trail.end());
	}
	numTrail = newTrail;
	
}