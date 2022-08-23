#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <iostream>
#include <immintrin.h>
#include <stdio.h>

#define USE_SIMD

class Vec3
{
public:
#ifdef USE_SIMD:
	__m128 sseVector;
#else
	float x, y, z;
#endif

	Vec3(float x, float y, float z);
	Vec3(__m128);

	Vec3 operator+(const Vec3& other) const;
	void operator+=(const Vec3& other);
	Vec3 operator-(const Vec3& other) const;
	void operator-=(const Vec3& other);
	Vec3 operator*(const float scale) const;   // multiple vector by scalar
	float operator*(const Vec3& other) const;  // dot product

	float dot(const Vec3& other) const;

	float magnitudeSquared() const;

	float magnitude() const;

	Vec3 normalized() const;

	float getX() const; // This one is fine
	float getY() const; // These might not be optimal in SIMD
	float getZ() const; // But shouldn't be called too much anyway

	void setX(const float x);  // These are also also a bit slow in SIMD
	void setY(const float y);  // And also shouldn't be neccesary too often
	void setZ(const float z);

	static bool usingSimd();

	Vec3() = default;
};

#endif