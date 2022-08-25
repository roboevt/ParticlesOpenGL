#pragma once

#include "Vec.h"

struct Particle {
	Vec2 position;
	Vec2 velocity;
	float mass;
	Particle(Vec2 pos, Vec2 vel, float mass): position(pos), velocity(vel), mass(mass) {}
};