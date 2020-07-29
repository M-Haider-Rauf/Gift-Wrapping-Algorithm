#ifndef UTIL_HPP
#define UTIL_HPP

struct Vector2;

size_t least_x(const Vector2* points, size_t len); //to find index of point with least x coord
float get_orientation(Vector2 p, Vector2 q, Vector2 r); // get orientation of triplet (p, q, r)
float points_distance_sqrd(Vector2 a, Vector2 b); 

#endif // !UTIL_HPP
