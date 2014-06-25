#include "AABB.h"

bool AABB::intersect(Ray& ray, float* hitt0, float* hitt1) const 
{
	float t0 = ray.tmin, t1 = ray.tmax;
	for (int i = 0; i < 3; ++i) {
		// Update interval for _i_th bounding box slab
		float invRayDir = 1.f / ray.direction[i];
		float tNear = (min[i] - ray.source[i]) * invRayDir;
		float tFar  = (max[i] - ray.source[i]) * invRayDir;

		// Update parametric interval from slab intersection $t$s
		if (tNear > tFar) std::swap(tNear, tFar);
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar  < t1 ? tFar  : t1;
		if (t0 > t1) return false;
	}
	if (hitt0) *hitt0 = t0;
	if (hitt1) *hitt1 = t1;
	return true;
}