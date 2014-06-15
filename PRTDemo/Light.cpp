#include "Light.h"

// directional light
Light* Light::directLight(Sampler& sampler, int numBands, float lightIntensity)							
{
	//float lightIntensity = 0.4;

	Light* light = new Light(numBands);
	for (unsigned int i = 0; i < light->numFunctions; i++) {
		light->coeffs[i] = 0.f;
		for (unsigned int j = 0; j < sampler.size(); j++) {
			if (sampler[j].theta < PI/6.0f)
				light->coeffs[i] += lightIntensity*sampler[j].shValues[i];
		}
		light->coeffs[i] *= 4*PI/sampler.size();
	}
	return light;
}

// using light probe (Image Based Lighting)
Light* Light::lightFromImage(const char* imagePath, Sampler& sampler, int numBands)
{

}

void Light::rotateSHCoefficients(float theta, float phi)
{
	for (int i = 0; i < numFunctions; i++)
		rotatedCoeffs[i] = coeffs[i];

	// Band 0 coefficient is unchanged
	rotatedCoeffs[0] = coeffs[0];

	// Rotate band 1 coefficients
	if (numBands > 1) {
		float band1X[9];
		float band1Z1[9];
		float band1Z2[9];
		
		// TODO
	}

}
