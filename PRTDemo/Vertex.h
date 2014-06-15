#ifndef _VERTEX_H_
#define _VERTEX_H_
#include "global.h"

class Vertex
{
public:
	Vertex() : unshadowedCoeffs(NULL), shadowedCoeffs(NULL), diffuseMaterial(1.f, 1.f, 1.f)  {}
	Vertex(float px, float py, float pz, float nx, float ny, float nz) 
		: position(px, py, pz), normal(nx, ny, nz), unshadowedCoeffs(NULL), shadowedCoeffs(NULL), diffuseMaterial(1.f, 1.f, 1.f) {
			normal = glm::normalize(normal);
	}


	~Vertex() {
		if(unshadowedCoeffs)
			delete [] unshadowedCoeffs;
		unshadowedCoeffs=NULL;

		if(shadowedCoeffs)
			delete [] shadowedCoeffs;
		shadowedCoeffs=NULL;
	}

	vec3 position;
	vec3 normal;
	vec3 litColor;
	vec3 diffuseMaterial;
	float* unshadowedCoeffs;
	float* shadowedCoeffs;
};

#endif

