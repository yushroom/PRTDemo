#ifndef _SCENE_H_
#define _SCENE_H_
#include "global.h"
#include "Ray.h"
#include "Object.h"
#include "Sample.h"

class Scene
{
public:
	bool addModelFromFile(const char* path);
	bool generateDirectCoeffs(Sampler& sampler, int numBands);	// diffuse
	bool generateDirectCoeffsDS(Sampler& sampler, int numBands, int bounceTime);
	bool generateDirectCoeffsDS(Sampler& sampler, int numBands);
	bool generateDirectCoeffsGS(Sampler& sampler, int numBands );	// glossy surface self-transfer

	void bindBuffer();

	int isRayBlocked(Ray& ray);
	vector<Object*> object;

	GLuint VB;
	GLuint IB;
	int numIndices;

};

#endif //_SCENE_H_