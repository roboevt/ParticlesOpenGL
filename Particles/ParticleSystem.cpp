#include "ParticleSystem.h"

#include <iostream>
#include <cmath>

float randf(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

ParticleSystem::ParticleSystem() : G(1.f), bounceDamp(0.95f), subSteps(8) {
	srand(time(0));
	for (int i = 0; i < 0; i++) {
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
	// const Vec2 to_center = Vec2(0.0f, 0.0f) - part.position; // hopefully compiler optimizes this
	// const float distance = to_center.magnitude();
	// if(distance > 1.0f) {
	// 	const Vec2 n = to_center / distance;
	// 	part.position = Vec2(0.0f, 0.0f) - n;
	// }

	// Square border
	if (part.position.x() > 1.0f) part.position.x(1.0f); 
	if (part.position.x() < -1.0f) part.position.x(-1.0f);
	if (part.position.y() < -1.0f) part.position.y(-1.0f);
	if (part.position.y() > 1.0f) part.position.y(1.0f);
}

void ParticleSystem::applyCollision(Particle& part1, Particle& part2) {
	const Vec2 direction = part1.position - part2.position;
	const float distance = direction.magnitude();
	if (distance < Particle::RADIUS) {
		const Vec2 n = direction / distance;
		const float delta = Particle::RADIUS - distance;
		part1.position += n * delta / 2;
		part2.position -= n * delta / 2;
		// 		std::cout << "Collision between " << part1.position << " and " << part2.position << std::endl;
		// std::cout << "Distance: " << distance << std::endl;
		// std::cout << "Delta: " << delta << std::endl;
		// std::cout << "Direction: " << direction << std::endl;
		// std::cout << "n: " << n << std::endl;
		// endl(std::cout);
		// Approximate velocities using position change
	}
}

void ParticleSystem::applyAttractor(Particle& part) {
	if (attractorActive) {
		const Vec2 direction = attractor - part.position;
		float force = ((G + .5f) * part.mass * 0.1f / (direction.magnitudeSquared() + 0.01f));
		if(force > 1e3) force = 1e3;  // Clip force to limit explosions
		part.accelerate(direction.normalized() * force);
	}
}

void ParticleSystem::applyNBodyGravity(Particle& part1, Particle& part2) {
	constexpr float fudge = 0.01f;
	Vec2 direction = part2.position - part1.position;
	float force = (G * part1.mass * part2.mass) / (direction.magnitudeSquared() + fudge);
	if(force > 1e3) force = 1e3;  // Clip force to limit explosions
	force /= 8192;
	direction.normalize();
	part1.accelerate(direction * force);
	part2.accelerate(direction * -force);
}

void ParticleSystem::applyCollisionsCell(const Cell& cell1, const Cell& cell2) {
	for(const int i : cell1.particleIndexes) {
		for(const int j : cell2.particleIndexes) {
			if(i == j) continue;
			applyCollision(particles[i], particles[j]);
		}
	}
}

static constexpr inline float clip(float x, float lower, float upper) {
	return std::max(lower, std::min(x, upper));
}

void ParticleSystem::assignParticlesToGrid() {
	// First clear out previous assignment
	for(auto& cells : grid.cells) {
		for(auto& cell : cells) {
			cell.particleIndexes.clear();
		}
	}

	// Next assign particles to cells
	for(int i = 0; i < particles.size(); i++) {
		const Particle& part = particles[i];
		const int gx = clip(((part.position.x() + 1.f) / 2) / grid.CELL_PITCH, 0, grid.GRID_WIDTH_CELLS - 1);
		const int gy = clip(((part.position.y() + 1.f) / 2) / grid.CELL_PITCH, 0, grid.GRID_WIDTH_CELLS - 1);
		// std::cout << "Assigning " << part.position << " to " << gx << ", " << gy << std::endl;
		grid.cells.at(gx).at(gy).particleIndexes.push_back(i);
	}
}

void ParticleSystem::applyCollisionsGrid() {
	for(int gx = 0; gx < grid.GRID_WIDTH_CELLS; gx++) {
		for(int gy = 0; gy < grid.GRID_WIDTH_CELLS; gy++) {
			auto& currentCell = grid.cells[gx][gy];
			// Find collisions within the cell
			for(int i = 0; i < currentCell.particleIndexes.size(); i++) {
				for(int j = i + 1; j < currentCell.particleIndexes.size(); j++) {
					applyCollision(particles[currentCell.particleIndexes[i]], particles[currentCell.particleIndexes[j]]);
				}
			}
			// Find collisions with neighboring cells
			for(int dx = -1; dx < 2; dx++) {
				for(int dy = -1; dy < 2; dy++) {
					if(gx + dx < 0 || gx + dx >= grid.GRID_WIDTH_CELLS || gy + dy < 0 || gy + dy >= grid.GRID_WIDTH_CELLS) continue;
					auto& otherCell = grid.cells.at(gx+dx).at(gy+dy);
					applyCollisionsCell(currentCell, otherCell);
				}
			}
		}
	}
	// I think this version doubles up sometimes
	// for(int gx = 1; gx < grid.GRID_WIDTH_CELLS - 1; gx++) {
	// 	for(int gy = 1; gy < grid.GRID_WIDTH_CELLS - 1; gy++) {
	// 		auto& currentCell = grid.cells[gx][gy];
	// 		for(int dx = -1; dx < 1; dx++) {
	// 			for(int dy = -1; dy < 1; dy++) {
	// 				auto& otherCell = grid.cells.at(gx+dx).at(gy+dy);
	// 				applyCollisionsCell(currentCell, otherCell);
	// 			}
	// 		}
	// 	}
	// }
}

void ParticleSystem::update() {
	const auto start = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> elapsedTime = start - previousFrameTime;
	previousFrameTime = start;
	float frameTime = elapsedTime.count();
	// std::cout << "Frame time: " << frameTime << std::endl;
	// if (frameTime > 0.01f) {
	// 	// min calculation rate of 100hz - goes to "slow motion" if computer can't keep up. Also eliminates hitches from resizing.
	// 	frameTime = 0.01f;
	// 	std::cout << "Slow frame" << std::endl;
	// }

	frameTime /= ((float)subSteps);
	
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = end - start;
	// std::cout << "Assign to grid Elapsed time: " << elapsed.count() * 1000 << "ms" << std::endl;
	for (int substep = 0; substep < subSteps; substep++) {
		assignParticlesToGrid();  // TODO needs to be in substep?
		applyCollisionsGrid();
		for (int i = 0; i < particles.size(); i++) {
			Particle& part1 = particles[i];
			applyGravity(part1);
			applyBorderConstraint(part1);
			applyAttractor(part1);
			part1.updatePosition(frameTime);
		}
	}

	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	// std::cout << "Particle Elapsed time: " << elapsed.count() * 1000 << "ms" << std::endl;
}

void ParticleSystem::setAttractor(float x, float y) {
	attractor = Vec2(x, y);
	attractorActive = true;
}

void ParticleSystem::removeAttractor() {
	attractorActive = false;
}

void ParticleSystem::addParticle(Particle part) {
	particles.push_back(part);
}

void ParticleSystem::addRandomParticle() {
	particles.push_back(Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f) }, { randf(-.001f, .001f), randf(-.001f, .001f) }));
	//particles.at(particles.size() - 1) = (Particle({ randf(-1.0f, 1.0f), randf(-1.0f, 1.0f) }, { randf(-0.5f, 0.5f), randf(-0.5f, 0.5f) }, randf(2.0f, 100.0f)));	
}

void ParticleSystem::removeRandomParticle() {
	particles.pop_back();
}
