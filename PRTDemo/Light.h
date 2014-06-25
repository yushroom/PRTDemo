#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "Sample.h"

struct Image {
	Image() : width(0), height(0){
		pixel[0] = NULL;
		pixel[1] = NULL;
		pixel[2] = NULL;
	}
	~Image() {
		for (int i = 0; i < 3; i++)
			if (!pixel[i]) { delete []pixel[i]; pixel[i] = NULL;}
	}

	int width;
	int height;
	float* pixel[3];
	bool loadFromFile(const char* filename);
};

struct SHCoeffs {
	float* r;
	float* g;
	float* b;
	int numFunctions;
	SHCoeffs(int numFunctions) : numFunctions(numFunctions) {
		r = new float[numFunctions];
		g = new float[numFunctions];
		b = new float[numFunctions];
	}
	~SHCoeffs() {
		if (NULL != r) delete[] r;
		if (NULL != g) delete[] g;
		if (NULL != b) delete[] b;
		r = g = b = NULL;
	}
	float* operator[] (int idx) {
		assert(idx < 3 && idx >=0);
		if (idx == 0) return r;
		if (idx == 1) return g;
		if (idx == 2) return b;
	}
};

class Light
{
public:
	Light(int numBands) : numBands(numBands), numFunctions(numBands*numBands) {
		coeffs = new SHCoeffs(numFunctions);
		rotatedCoeffs = new SHCoeffs(numFunctions);
		//memset(coeffs, 0, sizeof(coeffs[0])*numFunctions);
		//memset(rotatedCoeffs, 0, sizeof(coeffs[0])*numFunctions);
		//for (unsigned int i = 0; i < numFunctions; i++)
		//	coeffs[i] = 0.f;
	};
	~Light() {
		if (NULL != coeffs)
			delete []coeffs;
		coeffs = NULL;
		if (NULL != rotatedCoeffs)
			delete []rotatedCoeffs;
		rotatedCoeffs = NULL;
	};

	static Light* directLight(Sampler& sampler, int numBands = 5, float lightIntensity = 0.4f);							// directional light
	static Light* lightFromImage(const char* imagePath,Sampler& sampler, int numBands = 5, float lightIntensity = 0.4f);	// using light probe (Image Based Lighting)

	//void projectLight(Sampler& sampler, int numBands) {};
	// Update the rotated light coefficients
	void rotateSHCoefficients(float theta, float phi);

	unsigned int numBands;
	unsigned int numFunctions;			// = numBands * numBands

	SHCoeffs* coeffs;			// has already multiply by light intensity
	SHCoeffs* rotatedCoeffs;
};

#endif

