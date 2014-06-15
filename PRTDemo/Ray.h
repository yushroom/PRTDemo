#ifndef _RAY_H_
#define _RAY_H_
#include "global.h"
#include "AABB.h"

class Ray
{
public:
	Ray() : tmax(INT_MAX), tmin(0) {};
	~Ray() {};
	Ray(vec3 _source, vec3 _direction) : source(_source), direction(_direction), tmax(INT_MAX), tmin(0) {
		direction = glm::normalize(direction);
	}

	void set(const vec3& newSource, const vec3& newDirection) {
		source = newSource;
		direction = newDirection;
	}

	bool intersectsTriangle(const vec3& v1, const vec3& v2, const vec3& v3);
	bool intersectsAABB(const AABB& aabb);

	vec3 source;
	vec3 direction;
	float tmax;
	float tmin;
};

#endif