#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
using std::cout;
using std::endl;
using glm::vec3;
using std::vector;

// timer
#include <chrono>
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;
typedef std::chrono::high_resolution_clock Clock;

// const
#define PI		3.1415926f
#define EPSILON 0.01f


enum LIGHTING_TYPE
{
	LIGHTING_TYPE_GL = 0,				// old way, not implemented
	LIGHTING_TYPE_SH_UNSHADOWED,
	LIGHTING_TYPE_SH_SHADOWED,
	LIGHTING_TYPE_SH_SHADOWED_BOUNCE_1,	// interreflected(bounce 1)
	LIGHTING_TYPE_SH_SHADOWED_BOUNCE_2,
	LIGHTING_TYPE_SH_SHADOWED_BOUNCE_3,
};

#endif