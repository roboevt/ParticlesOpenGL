#pragma once

#include "Vec.h"

#include <iostream>

struct Particle {
	static constexpr float mass = 1.0f;
	static constexpr float RADIUS = 0.012f;
	Vec2 position;
	Vec2 last_position;
	Vec2 acceleration;
	Particle(Vec2 pos): position(pos), last_position(pos) {}
	Particle(Vec2 pos, Vec2 vel): position(pos), last_position(pos + vel) {}

	void accelerate(const Vec2 force) {
		acceleration += force / mass;
	}

	void updatePosition(const float timestep) {
		Vec2 velocity = position - last_position;
		last_position = position;
		position += velocity + acceleration * timestep * timestep;
		acceleration = Vec2(0, 0);
	}

	bool operator==(const Particle& other) const {
		return position == other.position;
	}
};