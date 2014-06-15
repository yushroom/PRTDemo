#ifndef _ACCELERATIONSTRUCTURE_H_
#define _ACCELERATIONSTRUCTURE_H_
#include <climits>
#include "global.h"
#include "Ray.h"
//#include "Object.h"

class Object;

class Grid
{
	struct Cell {
		// TODO remove model
		Cell(Object* model) : model(model) {}
		void insert(uint32_t idx) { triangles.push_back(idx); };
		bool intersect(Ray& ray) const;
		// tris id in model->indices
		vector<uint32_t> triangles;
		Object* model;
	};
public:
	Grid(Object* model);
	~Grid() {
		// TODO
	}
	bool intersect(Ray& ray) const;
	uint32_t resolution[3];
	vec3 cellDimension;
	int ncell;
	AABB bbox;
	Cell** cells;
	Object* model;
};

#endif