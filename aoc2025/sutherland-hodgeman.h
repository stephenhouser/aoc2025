#include <span>
#include <vector>
#include <print>

#include "point.h"

constexpr point_t operator*(const point_t& a, float b) {
	return {(float)a.x * b, (float)a.y * b};
}

constexpr float dot(const point_t& a, const point_t& b) {
	return (float)a.x * (float)b.x + (float)a.y * (float)b.y;
}

constexpr float cross(const point_t& a, const point_t& b) {
	return (float)a.x * (float)b.y - (float)b.x * (float)a.y;
}

// check if a point is on the LEFT side of an edge
constexpr bool is_inside(const point_t& point, const point_t& a, const point_t& b) {
    return (cross(a - b, point) + cross(b, a)) < 0.0f;
}

// calculate intersection point
constexpr point_t intersection(const point_t& a1, const point_t& a2, const point_t& b1, const point_t& b2) {
    return ((b1 - b2) * cross(a1, a2) - (a1 - a2) * cross(b1, b2)) *
           (1.0f / cross(a1 - a2, b1 - b2));
}

// Sutherland-Hodgman clipping
std::vector<point_t> sutherland_hodgman(std::span<point_t const> subject, std::span<point_t const> clip);