#include "Vec3.h"
using namespace std;

#define FIRST_MASK 0x71
#define ALL_MASK 0x77

Vec3::Vec3(float x, float y, float z) {
#ifdef USE_SIMD
	this->sseVector = _mm_setr_ps(x, y, z, 0.0f);
#else
	this->x = x;
	this->y = y;
	this->z = z;
#endif
}

#ifdef USE_SIMD
Vec3::Vec3(__m128 inputVector) {
	this->sseVector = inputVector;

}
#endif

float Vec3::dot(const Vec3& other) const {
#ifdef USE_SIMD
	return _mm_cvtss_f32(_mm_dp_ps(this->sseVector, other.sseVector, FIRST_MASK));
#else 
	return this->x * other.x + this->y * other.y + this->z * other.z;
#endif
}

float Vec3::magnitudeSquared() const {
#ifdef USE_SIMD
	return _mm_cvtss_f32(_mm_dp_ps(this->sseVector, this->sseVector, FIRST_MASK));
#else
	return this->x * this->x + this->y * this->y + this->z * this->z;
#endif
}

float Vec3::magnitude() const {
#ifdef USE_SIMD
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(this->sseVector, this->sseVector, FIRST_MASK)));
#else
	return sqrt(this->magnitudeSquared());
#endif
}

Vec3 Vec3::normalized() const {
#ifdef USE_SIMD
	__m128 inverseSqrt = _mm_rsqrt_ps(_mm_dp_ps(this->sseVector, this->sseVector, ALL_MASK));
	return Vec3(_mm_mul_ps(this->sseVector, inverseSqrt));
#else
	float magnitude = this->magnitude();
	return Vector(this->x / magnitude, this->y / magnitude, this->z / magnitude);
#endif
}

Vec3 Vec3::operator+(const Vec3& other) const {
#ifdef USE_SIMD
	return Vec3(_mm_add_ps(this->sseVector, other.sseVector));
#else
	return Vector(this->x + other.x, this->y + other.y, this->z + other.z);
#endif
}

void Vec3::operator+=(const Vec3& other) {
#ifdef USE_SIMD
	this->sseVector = _mm_add_ps(this->sseVector, other.sseVector);
#else
	this.x += other.x;
	this.y += other.y;
	this.z += other.z;
#endif
}

Vec3 Vec3::operator-(const Vec3& other) const {
#ifdef USE_SIMD
	return Vec3(_mm_sub_ps(this->sseVector, other.sseVector));
#else
	return Vector(this->x - other.x, this->y - other.y, this->z - other.z);
#endif
}

void Vec3::operator-=(const Vec3& other) {
#ifdef USE_SIMD
	this->sseVector = _mm_sub_ps(this->sseVector, other.sseVector);
#else
	this.x -= other.x;
	this.y -= other.y;
	this.z -= other.z;
#endif
}

Vec3 Vec3::operator*(const float scale) const {
#ifdef USE_SIMD
	return Vec3(_mm_mul_ps(this->sseVector, _mm_set1_ps(scale)));
#else
	return Vector(this->x * scale, this->y * scale, this->z * scale);
#endif
}

float Vec3::operator*(const Vec3& other) const {
	return dot(other);
}

float Vec3::getX() const {
#ifdef USE_SIMD
	return _mm_cvtss_f32(this->sseVector);
#else
	return this->x;
#endif
}

float Vec3::getY() const {
#ifdef USE_SIMD
	float result;
	_MM_EXTRACT_FLOAT(result, this->sseVector, 1);
	return result;
#else
	return this->y;
#endif
}

float Vec3::getZ() const {
#ifdef USE_SIMD
	float result;
	_MM_EXTRACT_FLOAT(result, this->sseVector, 2);
	return result;
#else
	return this->z;
#endif
}

void Vec3::setX(const float x) {
#ifdef USE_SIMD
	float y = this->getY();
	float z = this->getZ();
	this->sseVector = _mm_setr_ps(x, y, z, 0.0f);
#else
	this->x = x;
#endif
}

void Vec3::setY(const float y) {
#ifdef USE_SIMD
	float x = this->getX();
	float z = this->getZ();
	this->sseVector = _mm_setr_ps(x, y, z, 0.0f);
#else
	this->y = y;
#endif
}

void Vec3::setZ(const float z) {
#ifdef USE_SIMD
	float x = this->getX();
	float y = this->getY();
	this->sseVector = _mm_setr_ps(x, y, z, 0.0f);
#else
	this->z = z;
#endif
}

bool Vec3::usingSimd() {
#ifdef USE_SIMD
	return true;
#else
	return false;
#endif
}