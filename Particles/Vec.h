#pragma once
#include <math.h>
#include <initializer_list>
#include <algorithm>
#include <iostream>

template<int N> class Vec {
protected:
    float components[N];
public:
    Vec operator+(const Vec& other) const {
        float next[N];
        for (int i = 0; i < N; i++) {
            next[i] = components[i] + other[i];
        }
        return Vec<N>(next);
    }
    void operator+=(const Vec& other) { *this = *this + other; }

    Vec operator-(const Vec& other) const {
        float next[N];
        for (int i = 0; i < N; i++) {
            next[i] = components[i] - other[i];
        }
        return Vec<N>(next);
    }
    void operator-=(const Vec& other) { *this = *this - other; }

    float operator*(const Vec& other) const {  // dot product
        float sum = 0;
        for (int i = 0; i < N; i++) {
            sum += components[i] * other.components[i];
        }
        return sum;
    }

    Vec operator*(float scale) const {
        float next[N];
        for (int i = 0; i < N; i++) {
            next[i] = components[i] * scale;
        }
        return Vec<N>(next);
    }
    void operator*=(float scale) { *this = *this * scale; }

    Vec operator/(float scale) const {
        return *this * (1.0f / scale);
    }
    void operator/=(float scale) { *this = *this / scale; }

    float operator[](unsigned int index) const { return components[index]; }
    float magnitudeSquared() const { return (*this) * (*this); }
    float magnitude() const { return sqrt(this->magnitudeSquared()); }
    Vec normalized() const { return *this / magnitude(); }
    void normalize() { *this = *this / magnitude(); }

    Vec() {
        for (int i = 0; i < N; i++) {
            this->components[i] = 0.0f;
        }
    }
    Vec(float input[N]) {
        for (int i = 0; i < N; i++) {
            components[i] = input[i];
        }
    }
    Vec(std::initializer_list<float> componentsIn) {
        std::copy(std::begin(componentsIn), std::end(componentsIn), this->components);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec& vec) {
        os << "(";
        for (int i = 0; i < N; i++) {
            os << vec[i];
            if (i < N - 1) os << ", ";
        }
        os << ")";
        return os;
    }

    bool operator==(const Vec& other) const {
        for (int i = 0; i < N; i++) {
            if (components[i] != other.components[i]) return false;
        }
        return true;
    }
};

class Vec3 : public Vec<3> {
private:
    enum axis { xPos, yPos, zPos };
public:
    Vec3() : Vec<3>{ {0,0,0} } {}
    Vec3(float x, float y, float z) : Vec<3>{ {x,y,z} } {}
    float x() const { return components[xPos]; }
    float y() const { return components[yPos]; }
    float z() const { return components[zPos]; }
    void x(float x) { components[xPos] = x; }
    void y(float y) { components[yPos] = y; }
    void z(float z) { components[zPos] = z; }
};

class Vec2 : public Vec<2> {
private:
    enum axis { xPos, yPos };
public:
    Vec2() : Vec<2>{ {0,0} } {}
    Vec2(float x, float y) : Vec<2>{ {x,y} } {}
    Vec2(const Vec<2>& other) { components[xPos] = other[xPos]; components[yPos] = other[yPos]; }
    float x() const { return components[xPos]; }
    float y() const { return components[yPos]; }
    void x(float x) { components[xPos] = x; }
    void y(float y) { components[yPos] = y; }
};

class Color : public Vec3 {
private:
    enum axis { rPos, gPos, bPos };
public:
    Color() {}
    Color(float r, float g, float b) : Vec3(r, g, b) {}
    float r() const { return components[rPos]; }
    float b() const { return components[bPos]; }
    float g() const { return components[gPos]; }
    void r(float r) { components[rPos] = r; }
    void g(float g) { components[gPos] = g; }
    void b(float b) { components[bPos] = b; }
};