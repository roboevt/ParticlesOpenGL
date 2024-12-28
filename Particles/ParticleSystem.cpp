#include "ParticleSystem.h"

#include <iostream>
#include <cmath>

float randf(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

ParticleSystem::ParticleSystem() : G(1.f), numMain(0), bounceDamp(0.95f), subSteps(8) {
	srand(time(0));
	particles.reserve(numMain);
	for (int i = 0; i < numMain; i++) {
		particles.push_back(Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f) }, { randf(-.001f, .001f), randf(-.001f, .001f) }));
	}

	previousFrameTime = std::chrono::high_resolution_clock::now();
}

std::vector<Particle>& ParticleSystem::getParticles() {
	return particles;
}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::applyGravity(Particle& part) {
	part.accelerate(Vec2(0, -G * part.mass));
}

static void applyBorderConstraint(Particle& part) {
	// Circular border
	const Vec2 to_center = Vec2(0.0f, 0.0f) - part.position; // hopefully compiler optimizes this
	const float distance = to_center.magnitude();
	if(distance > 1.0f) {
		const Vec2 n = to_center / distance;
		part.position = Vec2(0.0f, 0.0f) - n;
	}

	// Square border
	// if (part.position.x() > 1.0f) part.position.x(1.0f);
	// if (part.position.x() < -1.0f) part.position.x(-1.0f);
	// if (part.position.y() < -1.0f) part.position.y(-1.0f);
	// if (part.position.y() > 1.0f) part.position.y(1.0f);
}

void ParticleSystem::applyCollision(Particle& part1, Particle& part2) {
	const Vec2 direction = part1.position - part2.position;
	const float distance = direction.magnitude();
	if (distance < 0.01f) {
		// std::cout << "Collision: Part1: " << part1.position << " Part2: " << part2.position << std::endl;
		const Vec2 n = direction / distance;
		const float delta = 0.01f - distance;
		// std::cout << "Delta: " << delta << std::endl;
		// std::cout << "N: " << n << std::endl;
		// std::cout << "Distance: " << distance << std::endl;
		// std::cout << "Direction: " << direction << std::endl;
		// endl(std::cout);
		part1.position += n * delta / 2;
		part2.position -= n * delta / 2;
	}
}

void ParticleSystem::update() {
	const auto start = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> elapsedTime = start - previousFrameTime;
	previousFrameTime = start;
	float frameTime = elapsedTime.count();
	// if (frameTime > 0.01f) {
	// 	// min calculation rate of 100hz - goes to "slow motion" if computer can't keep up. Also eliminates hitches from resizing.
	// 	frameTime = 0.01f;
	// 	std::cout << "Slow frame" << std::endl;
	// }

	frameTime /= ((float)subSteps);

	for (int substep = 0; substep < subSteps; substep++) {
		for (int i = 0; i < numMain; i++) {
			Particle& part1 = particles[i];

			// applyGravity(part1);
			applyBorderConstraint(part1);

			for(int j = i + 1; j < numMain; j++) {
				Particle& part2 = particles[j];
				applyCollision(part1, part2);
			}

			part1.updatePosition(frameTime);

			// Between each particle
			for (int j = 0; j < numMain; j++) {
				if (j == i) continue;
				Particle part2 = particles[j];
				const float fudge = 0.01f;
				Vec2 direction = part2.position - part1.position;
				float force = (G * part1.mass * part2.mass) / (direction.magnitudeSquared() + fudge);
				part1.accelerate(direction.normalized() * force / 8192.);
			}
		}
	}
	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> elapsed = end - start;
	// std::cout << "Particle Elapsed time: " << elapsed.count() * 1000 << "ms" << std::endl;
}

void ParticleSystem::damp() {
	for (Particle& part : particles) {
		// part.velocity /= 5;
	}
}

void ParticleSystem::addParticle(Particle part) {
	numMain++;
	particles.push_back(part);
}

void ParticleSystem::addRandomParticle() {
	numMain++;
	particles.push_back(Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f) }, { randf(-.001f, .001f), randf(-.001f, .001f) }));
	//particles.at(numMain - 1) = (Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f) }, { randf(-0.5f, 0.5f), randf(-0.5f, 0.5f) }, randf(2.0f, 100.0f)));	
}

void ParticleSystem::removeRandomParticle() {
	//clearTrails();
	particles.pop_back();
	numMain--;
}
