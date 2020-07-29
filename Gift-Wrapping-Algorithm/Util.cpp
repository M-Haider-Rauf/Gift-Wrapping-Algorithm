#include "Util.hpp"

#include <raylib.h>

size_t least_x(const Vector2* points, size_t len)
{
	size_t least = 0;
	for (size_t i = 1; i < len; ++i) {
		if (points[i].x < points[least].x) {
			least = i;
		}
	}

	return least;
}

float get_orientation(Vector2 p, Vector2 q, Vector2 r)
{
	return (q.x - p.x) * (r.y - q.y) - (q.y - p.y) * (r.x - q.x);
}

float points_distance_sqrd(Vector2 a, Vector2 b)
{
	float x = b.x - a.x;
	float y = b.y - a.y;

	return x * x + y * y;
}
