#ifndef _VERTEX_H_
#define _VERTEX_H_
#include "global.h"

class Vertex
{
public:
	Vertex() : litColor(1.f, 1.f, 1.f), unshadowedCoeffs(NULL), diffuseMaterial(1.f, 1.f, 1.f), isBlocked(NULL), blockIdx(NULL)  
	{
		for (int i = 0; i < 4; i++)
			shadowedCoeffs[i] = NULL;
	}

	Vertex(float px, float py, float pz, float nx, float ny, float nz) 
		: position(px, py, pz), normal(nx, ny, nz), litColor(1.f, 1.f, 1.f), unshadowedCoeffs(NULL), 
		diffuseMaterial(1.f, 1.f, 1.f), isBlocked(NULL), blockIdx(NULL)  
	{
		normal = glm::normalize(normal);
		for (int i = 0; i < 4; i++)
			shadowedCoeffs[i] = NULL;
	}

	~Vertex() {
		delete []unshadowedCoeffs;
		unshadowedCoeffs=NULL;

		for (int i = 0; i < 4; i++) {
			delete [] shadowedCoeffs[i];
			shadowedCoeffs[i] = 0;
		}
		delete[] isBlocked;
		delete[] blockIdx;
		isBlocked =NULL;
		blockIdx = NULL;
	}

	vec3 position;
	vec3 normal;
	vec3 litColor;
	vec3 diffuseMaterial;
	vec3* unshadowedCoeffs;
	vec3* shadowedCoeffs[4];	// 0-shadowed, 1~3 shadowed and self-transfer
	//vec3* shadowedCoeffsDS[3];

	bool* isBlocked;	// true if the ray in this direction is blocked;
	int* blockIdx;		// tirangle index (if blocked)
};

#endif

