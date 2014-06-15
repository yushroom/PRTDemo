#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "Sample.h"

class Light
{
public:
	Light(int numBands) : numBands(numBands), numFunctions(numBands*numBands) {
		coeffs = new float[numFunctions];
		rotatedCoeffs = new float[numFunctions];
		memset(coeffs, 0, sizeof(coeffs[0])*numFunctions);
		memset(rotatedCoeffs, 0, sizeof(coeffs[0])*numFunctions);
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

	static Light* directLight(Sampler& sampler, int numBands = 5, float lightIntensity = 0.4);							// directional light
	static Light* lightFromImage(const char* imagePath,Sampler& sampler, int numBands = 5);	// using light probe (Image Based Lighting)

	//void projectLight(Sampler& sampler, int numBands) {};
	// Update the rotated light coefficients
	void rotateSHCoefficients(float theta, float phi);

	int numBands;
	int numFunctions;	// = numBands * numBands
	float* coeffs;		// has already multiply by light intensity
	float* rotatedCoeffs;
};

#endif

