#pragma once
#include <vector>
#include <chrono>

#include "Particle.h"

struct Cell {
	std::vector<int> particleIndexes;
	float mass;
};

struct Grid {
	static constexpr float CELL_PITCH = 0.01f;
	static constexpr int GRID_WIDTH_CELLS = 1.0f / CELL_PITCH;
	std::array<std::array<Cell, GRID_WIDTH_CELLS>, GRID_WIDTH_CELLS> cells;
};

class ParticleSystem {
public:
	std::vector<Particle> particles;
	void update();
	std::vector<Particle>& getParticles();
	void addRandomParticle();
	void addParticle(Particle part);
	void removeRandomParticle();
	void setG(float G) { this->G = G; }
	float getG() const { return G; }
	void setSubSteps(int subSteps) { this->subSteps = subSteps; }
	int getSubSteps() const { return subSteps; }
	void setAttractor(float x, float y);
	void removeAttractor();
	void setRepulsor(float x, float y);
	void removeRepulsor();

	ParticleSystem();
	~ParticleSystem();
private:
	void applyGravity(Particle& part);
	void applyCollision(Particle& part1, Particle& part2);
	void applyNBodyGravity(Particle& part1, Particle& part2);
	void applyAttractor(Particle& part);
	void applyRepulsor(Particle& part);

	void assignParticlesToGrid();
	void applyCollisionsGrid();
	void applyCollisionsCell(const Cell& cell1, const Cell& cell2);

	Grid grid;
	Vec2 attractor;
	Vec2 repulsor;
	bool attractorActive;
	bool repulsorActive;

	std::chrono::high_resolution_clock::time_point previousFrameTime;
	float G;
	float bounceDamp;
	int subSteps;
};
