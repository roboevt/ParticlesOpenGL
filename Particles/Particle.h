#pragma once

#include "Vec.h"

struct Particle {
	Vec2 position;
	Vec2 velocity;
	float remainingLife;
	Particle(Vec2 pos, Vec2 vel, float life): position(pos), velocity(vel), remainingLife(life) {}
};