#include "Ray.h"
//bool Ray::intersectsTriangle(const vec3& v0, const vec3& v1, const vec3& v2, float& t, float &u, float &v)
bool Ray::intersectsTriangle(const vec3& v0, const vec3& v1, const vec3& v2)
{
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
    vec3 s1 = glm::cross(direction, e2);
    float divisor = glm::dot(s1, e1);
    
    if (divisor == 0.)
        return false;
    float invDivisor = 1.f / divisor;

    // Compute first barycentric coordinate
    vec3 d = source - v0;
    float b1 = glm::dot(d, s1) * invDivisor;
    if (b1 < 0. || b1 > 1.)
        return false;

    // Compute second barycentric coordinate
    vec3 s2 = glm::cross(d, e1);
    float b2 = glm::dot(direction, s2) * invDivisor;
    if (b2 < 0. || b1 + b2 > 1.)
        return false;

    // Compute _t_ to intersection point
    float t = glm::dot(e2, s2) * invDivisor;
    if (t < tmin || t > tmax)
        return false;
	//cout << "Ray::intersectsTriangle return true" << endl;
	return true;
}
