#include "AccelerationStructure.h"
#include "Object.h"

uint32_t clamp(uint32_t v, uint32_t lo, uint32_t hi)
{
	return std::max(lo, std::min(v, hi));
}

Grid::Grid(Object* model) : model(model), cells(NULL) //, cellMemoryPool(NULL)
{
	// compute bound of the scene
	int totalNumTriangles = model->indices.size() / 3;
	bbox.expand(model->aabb.max);
	bbox.expand(model->aabb.min);
	// create the grid
	vec3 size = bbox.max - bbox.min;
	cout << "bbox.min = (" << bbox.min.x << ", " << bbox.min.y << ", " << bbox.min.z << ") max = (" << bbox.max.x << ", " << bbox.max.y << ", " << bbox.max.z << ")\n";
#if 1
	uint8_t maxAxis = bbox.majorAxis();
	float invMaxSize = 1.f / size[maxAxis];
	assert(invMaxSize >0);
	float cubeRoot = 3 * powf(float(totalNumTriangles), 1.f / 3.f);
	float cellsPerUnitDist = cubeRoot * invMaxSize;
	for (uint8_t i = 0; i < 3; ++i) {
		resolution[i] = std::floor(size[i] * cellsPerUnitDist);
		resolution[i] = clamp(uint32_t(1), resolution[i], uint32_t(128));
	}
#else
	float cubeRoot = powf(5 * totalNumTriangles / (size[0] * size[1] * size[2]), 1 / 3.f);
	cout << "cubeRoot = " << cubeRoot << endl;
	for (uint8_t i = 0; i < 3; ++i) {
		resolution[i] = std::floor(size[i] * cubeRoot);
		resolution[i] = std::max(uint32_t(1), std::min(resolution[i], uint32_t(128)));
	}
#endif
	cellDimension = vec3(size.x/resolution[0], size.y/resolution[1], size.z/resolution[2]);
	cout << "resolution" << resolution[0] << ' ' << resolution[1] << ' ' << resolution[2] << endl; 
	cout << "cellDimension" << cellDimension.x << ' ' << cellDimension.x << ' ' << cellDimension.x << endl; 
	// allocate memory
	ncell = resolution[0] * resolution[1] * resolution[2];
	cout << "ncell:" << ncell << endl;
	cells = new Cell*[ncell];
	//for (int i = 0; i < ncell; i++)
	//	cells[i] = new Cell(model);
	// set all pointers to NULL
	 memset(cells, 0x0, sizeof(Grid::Cell*) * ncell);

	// insert all the triangles in the cells
	for (int i = 0; i < model->indices.size(); i += 3) {
		int idx1 = model->indices[i];
		int idx2 = model->indices[i+1];
		int idx3 = model->indices[i+2];
		const vec3& v1 = model->vertices[idx1].position;
		const vec3& v2 = model->vertices[idx2].position;
		const vec3& v3 = model->vertices[idx3].position;
		AABB box;
		box.expand(v1);
		box.expand(v2);
		box.expand(v3);
		vec3& min = box.min;
		vec3& max = box.max;

		// convert to cell coordinates
		min = (box.min - bbox.min) / cellDimension;
		max = (box.max - bbox.min) / cellDimension;
		uint32_t zmin = clamp(std::floor(min[2]), 0, resolution[2] - 1);
		uint32_t zmax = clamp(std::floor(max[2]), 0, resolution[2] - 1);
		uint32_t ymin = clamp(std::floor(min[1]), 0, resolution[1] - 1);
		uint32_t ymax = clamp(std::floor(max[1]), 0, resolution[1] - 1);
		uint32_t xmin = clamp(std::floor(min[0]), 0, resolution[0] - 1);
		uint32_t xmax = clamp(std::floor(max[0]), 0, resolution[0] - 1);

		// loop over all the cells the triangle overlaps and insert
		for (uint32_t z = zmin; z <= zmax; ++z) {
			for (uint32_t y = ymin; y <= ymax; ++y) {
				for (uint32_t x = xmin; x <= xmax; ++x) {
					uint32_t o = z * resolution[0] * resolution[1] + y * resolution[0] + x;
					//cout << o << '\n';
					if (cells[o] == NULL) cells[o] = new Cell(model);
					cells[o]->insert(i/3);
				}
			}
		}
	}

	int maxn = 0;
	for (int i = 0; i < ncell; i++)
		if (cells[i] != NULL && maxn < cells[i]->triangles.size())
			maxn = cells[i]->triangles.size();
	cout << "max # in grid: " << maxn << endl;
}

const float EPSILON2 = 0.00001;

bool intersectTriangle( Ray &r, vec3 &v0, vec3& v1, vec3& v2, float &t, float &u, float &v)
{
	static uint32_t ntimes = 0;
	ntimes++;
	//cout << ntimes << endl;
	vec3 edge1 = v1 - v0;
	vec3 edge2 = v2 - v0;
	vec3 pvec = glm::cross(r.direction, edge2);
	float det = glm::dot(edge1, pvec);
	if (det > -EPSILON2 && det < EPSILON2) return false;
	float invDet = 1 / det;
	vec3 tvec = r.source - v0;
	u = glm::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1) return false;
	vec3 qvec = glm::cross(tvec, edge1);
	v = glm::dot(r.direction, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;
	t = glm::dot(edge2, qvec) * invDet;
	return true;
} 

bool Grid::Cell::intersect(Ray&ray) const
{
	float uhit, vhit;
	for (uint32_t i = 0; i < triangles.size(); ++i) {
		//cout << "triangles.size() = " << triangles.size() << endl;
		uint32_t idx0 = model->indices[triangles[i] * 3];
		uint32_t idx1 = model->indices[triangles[i] * 3 + 1];
		uint32_t idx2 = model->indices[triangles[i] * 3 + 2];

		//assert(idx0 >=0 && idx0 < model->vertices.size() && idx1);
		vec3& v0 = model->vertices[idx0].position;
		vec3& v1 = model->vertices[idx1].position;
		vec3& v2 = model->vertices[idx2].position;

		if (ray.intersectsTriangle(v0, v1, v2)) {
			return true;
		}
		//float t, u, v;
		//if (intersectTriangle(ray, v0, v1, v2, t, u, v)) {
		//	if (t < ray.tmax) {
		//		ray.tmax = t;
		//		uhit = u;
		//		vhit = v;
		//		return true;
		//	}
		//}
	}
	return false;
}

bool Grid::intersect(Ray& ray) const 
{
	//cout << "Grid::intersect()" << endl;
	// if the ray doesn't intersect the grid return
	Ray r(ray);
	//if (!r.intersectsAABB(bbox))
	//	return false;

	// initialization step
	glm::ivec3 exit, step, cell;
	vec3 deltaT, nextCrossingT;
	for (uint8_t i = 0; i < 3; ++i) {
		// convert ray starting point to cell coordinates
		float rayOrigCell = ((r.source[i] + r.direction[i] * r.tmin) -  bbox.min[i]);
		cell[i] = clamp(std::floor(rayOrigCell / cellDimension[i]), 0, resolution[i] - 1);
		if (r.direction[i] < 0) {
			deltaT[i] = -cellDimension[i] / r.direction[i];
			nextCrossingT[i] = r.tmin + (cell[i] * cellDimension[i] - rayOrigCell) / r.direction[i];
			exit[i] = -1;
			step[i] = -1;
		}
		else {
			deltaT[i] = cellDimension[i] / r.direction[i];
			nextCrossingT[i] = r.tmin + ((cell[i] + 1)  * cellDimension[i] - rayOrigCell) / r.direction[i];
			exit[i] = resolution[i];
			step[i] = 1;
		}
	}

	// walk through each cell of the grid and test for an intersection if 
	// current cell contains geometry
	// const Object *hitObject = NULL;
	while (1) {
		uint32_t o = cell[2] * resolution[0] * resolution[1] + cell[1] * resolution[0] + cell[0];
		assert(o>=0 && o<ncell);
		if (cells[o] != NULL) {
			if (cells[o]->intersect(ray))
				return true;
			//if (hitObject != NULL) { ray.color = cells[o]->color; }
		}

		// to next cell
		uint8_t k = 
			((nextCrossingT[0] < nextCrossingT[1]) << 2) +
			((nextCrossingT[0] < nextCrossingT[2]) << 1) +
			((nextCrossingT[1] < nextCrossingT[2]));
		static const uint8_t map[8] = {2, 1, 2, 1, 2, 2, 0, 0};
		assert(k<8);
		uint8_t axis = map[k];

		if (ray.tmax < nextCrossingT[axis]) break;
		cell[axis] += step[axis];
		if (cell[axis] == exit[axis]) break;
		nextCrossingT[axis] += deltaT[axis];
	}
	return false;
	//return hitObject;
}