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
	bool generateCoeffs(Sampler& sampler, int numBands);					 // prepare to generate coeffs
	void bindBuffer();

	int isRayBlocked(Ray& ray);
	vector<Object*> object;

	GLuint VB;
	GLuint IB;
	int numIndices;

private:
	bool generateCoeffsUnshadowedAndShadowed(Sampler& sampler, int numBands);// diffuse unshadowed and shadowed transfer
	bool generateCoeffsDS(Sampler& sampler, int numBands, int bounceTime);	 // diffuse interreflected transfer
	//bool generateDirectCoeffsDS(Sampler& sampler, int numBands);	
	bool generateDirectCoeffsGS(Sampler& sampler, int numBands );	// glossy surface self-transfer. not implemented
};

#endif //_SCENE_H_