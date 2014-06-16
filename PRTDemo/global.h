#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <chrono>
using std::cout;
using std::endl;
using glm::vec3;
using std::vector;
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

#define PI		3.1415926f
#define EPSILON 0.01f

typedef std::chrono::high_resolution_clock Clock;


enum LIGHTING_TYPE
{
	LIGHTING_TYPE_GL = 0,			// old way
	LIGHTING_TYPE_SH_UNSHADOWED,
	LIGHTING_TYPE_SH_SHADOWED
};


#endif