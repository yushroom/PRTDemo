#ifndef _VERTEX_H_
#define _VERTEX_H_
#include "global.h"

class Vertex
{
public:
	Vertex() : unshadowedCoeffs(NULL), shadowedCoeffs(NULL), diffuseMaterial(1.f, 1.f, 1.f), isBlocked(NULL), blockIdx(NULL)  {
		for (int i = 0; i < 3; i++)
			shadowedCoeffsDS[i] = NULL;
	}
	Vertex(float px, float py, float pz, float nx, float ny, float nz) 
		: position(px, py, pz), normal(nx, ny, nz), unshadowedCoeffs(NULL), shadowedCoeffs(NULL), 
		diffuseMaterial(1.f, 1.f, 1.f), isBlocked(NULL), blockIdx(NULL)  
	{
		normal = glm::normalize(normal);
		for (int i = 0; i < 3; i++)
			shadowedCoeffsDS[i] = NULL;
	}

	~Vertex() {
		if(unshadowedCoeffs)
			delete [] unshadowedCoeffs;
		unshadowedCoeffs=NULL;

		if(shadowedCoeffs)
			delete [] shadowedCoeffs;
		shadowedCoeffs=NULL;
		if (isBlocked != NULL)
			delete[] isBlocked;
		if (blockIdx != NULL)
			delete[] blockIdx;
		isBlocked =NULL;
		blockIdx = NULL;
	}

	vec3 position;
	vec3 normal;
	vec3 litColor;
	vec3 diffuseMaterial;
	//float* unshadowedCoeffs;
	//float* shadowedCoeffs;
	vec3* unshadowedCoeffs;
	vec3* shadowedCoeffs;
	vec3* shadowedCoeffsDS[3];

	bool* isBlocked;	// true if the ray in this direction is blocked;
	int* blockIdx;		// tirangle index (if blocked)
};

#endif

