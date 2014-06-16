#ifndef _GRID_H_
#define _GRID_H_
#include "global.h"
#include "Ray.h"
#include "AABB.h"

typedef AABB BBox;
typedef glm::vec3 Point;
typedef glm::vec3 Vector;

class Object;

int clamp(int val, int low, int high);

// Voxel Declarations
struct Voxel {
    // Voxel Public Methods
    uint32_t size() const { return primitives.size(); }
    Voxel() { }
    Voxel(Reference<Primitive> op) {
        allCanIntersect = false;
        primitives.push_back(op);
    }
    void AddPrimitive(Reference<Primitive> prim) {
        primitives.push_back(prim);
    }
    bool Intersect(const Ray &ray, Intersection *isect);
    bool IntersectP(const Ray &ray);
private:
    vector<Reference<Primitive> > primitives;
    bool allCanIntersect;
};

// GridAccel Declarations
class GridAccel {
public:
    // GridAccel Public Methods
    GridAccel(Object* object);
    BBox WorldBound() const;
    bool CanIntersect() const { return true; }
    ~GridAccel();
    bool Intersect(const Ray &ray, Intersection *isect) const;
    bool IntersectP(const Ray &ray) const;
private:
    // GridAccel Private Methods
    int posToVoxel(const Point &P, int axis) const {
        int v = int((P[axis] - bounds.min[axis]) *
                          invWidth[axis]);
        return clamp(v, 0, nVoxels[axis]-1);
    }
    float voxelToPos(int p, int axis) const {
        return bounds.min[axis] + p * width[axis];
    }
    inline int offset(int x, int y, int z) const {
        return z*nVoxels[0]*nVoxels[1] + y*nVoxels[0] + x;
    }

    // GridAccel Private Data
    //vector<Reference<Primitive> > primitives;
    Object* object;
	int nVoxels[3];
    BBox bounds;
    Vector width, invWidth;
    Voxel **voxels; };

#endif
