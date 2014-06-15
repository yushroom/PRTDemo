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
	bool generateDirectCoeffs(Sampler& sampler, int numBands);

	void bindBuffer();

//private:
	bool isRayBlocked(Ray& ray);
	vector<Object*> object;

	GLuint VB;
	GLuint IB;
	int numIndices;

private:
	//struct bufferBinder {
	//	static void bindBuffer(const std::vector<Vertex>& Vertices,
	//					const std::vector<unsigned int>& Indices);
 //       
 //       static GLuint VB;		//For vertex buffer
 //       static GLuint IB;		//For index buffer
 //       //unsigned int numIndices;
	//private:
 //       bufferBinder();
 //       ~bufferBinder();
 //   };
};

#endif //_SCENE_H_