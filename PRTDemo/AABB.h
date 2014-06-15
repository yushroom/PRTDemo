#ifndef _AABB_H_
#define _AABB_H_
#include "global.h"
#include <climits>

class AABB
{
public:
	vec3 min;
	vec3 max;
	AABB() : max(INT_MIN, INT_MIN, INT_MIN), min(INT_MAX, INT_MAX, INT_MAX) {}
	
	void expand(const vec3& p) {
		if (max.x < p.x) max.x = p.x;
		if (max.y < p.y) max.y = p.y;
		if (max.z < p.z) max.z = p.z;
		if (min.x > p.x) min.x = p.x;
		if (min.y > p.y) min.y = p.y;
		if (min.z > p.z) min.z = p.z;
	}

	int majorAxis() const {
        vec3 diag = max - min;
        if (diag.x > diag.y && diag.x > diag.z)
            return 0;
        else if (diag.y > diag.z)
            return 1;
        else
            return 2;
    }

	bool isPointInside(const vec3& p) const {
		return (max.x >= p.x && p.x >= min.x && max.y >= p.y && p.y >= min.y && max.z >= p.z && p.z >= min.z);
	}
};

#endif // _AABB_H_

