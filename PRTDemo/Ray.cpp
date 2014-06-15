#include "Ray.h"
//bool Ray::intersectsTriangle(const vec3& v0, const vec3& v1, const vec3& v2, float& t, float &u, float &v)
bool Ray::intersectsTriangle(const vec3& v0, const vec3& v1, const vec3& v2)
{
	//cout << "Ray::intersectsTriangle()" << endl;
	vec3 edge1 = v1 - v0;
	vec3 edge2 = v2 - v0;
	vec3 pvec = glm::cross(direction, edge2);
	float det = glm::dot(edge1, pvec);
	if (det > -EPSILON && det < EPSILON) return false;
	float invDet = 1 / det;
	vec3 tvec = source - v0;
	float u = glm::dot(tvec, pvec) * invDet;		//
	if (u < 0 || u > 1) return false;
	vec3 qvec = glm::cross(tvec, edge1);
	float v = glm::dot(direction, qvec) * invDet;	//
	if (v < 0 || u + v > 1) return false;
	float t = glm::dot(edge2, qvec) * invDet;		//
	return true;
}


// error not fixed!
bool Ray::intersectsAABB(const AABB& aabb)
{
	if (aabb.isPointInside(source)) {
		return true;
	}
	const vec3& min = aabb.min;
	const vec3& max = aabb.max;
	float tmin = (min.x - source.x) / direction.x;
    float tmax = (max.x - source.x) / direction.x;
    if (tmin > tmax) std::swap(tmin, tmax);
    float tymin = (min.y - source.y) / direction.y;
    float tymax = (max.y - source.y) / direction.y;
    if (tymin > tymax) std::swap(tymin, tymax);
    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    float tzmin = (min.z - source.z) / direction.z;
    float tzmax = (max.z - source.z) / direction.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);
    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    if ((tmin > this->tmax) || (tmax < this->tmin)) return false;
    if (this->tmin < tmin) this->tmin = tmin;
    if (this->tmax > tmax) this->tmax = tmax;
    return true;
}