#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "global.h"
#include "Ray.h"
#include "AABB.h"
#include "Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Grid.h"
//class Grid;

class Object
{
public:
	Object(const char* path);		// load form file
	int doesRayHitObject(Ray& ray) const;

	void render();

	vector<Vertex> vertices;
	vector<GLuint> indices;
	
	Grid* grid;
	AABB aabb;
};

#endif