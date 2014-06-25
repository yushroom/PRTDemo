#ifndef _SAMPLER_H_
#define _SAMPLER_H_
#include "global.h"
#include "SHEval.h"

class Sample
{
public:
	vec3 direction;
	float* shValues;
	float phi;
	float theta;
	Sample() : shValues(NULL) {}
	~Sample() {};
};

class Sampler
{
public:
	Sampler() {};
	~Sampler() {};
	void generateSamples(int sqrtNumSamples, int numBans) 
	{
		int numSamples = sqrtNumSamples*sqrtNumSamples;
		int numFunctions = numBans*numBans;
		samples.resize(numSamples);

		SHEvalFunc SHEval[] = {SHEval3, SHEval3, SHEval3, SHEval3, SHEval4, SHEval5, SHEval6, SHEval7, SHEval8, SHEval9, SHEval10};
		for (int i = 0; i < sqrtNumSamples; i++)
		{
			for (int j = 0; j < sqrtNumSamples; j++)
			{
				float a = ((float)i + rand()/RAND_MAX) / (float) sqrtNumSamples;
				float b = ((float)j + rand()/RAND_MAX) / (float) sqrtNumSamples;
				float theta = 2*acos(sqrt(1-a));
				float phi = 2*PI*b;
				float x = sin(theta)*cos(phi);
				float y = sin(theta)*sin(phi);
				float z = cos(theta);
				int k = i*sqrtNumSamples + j;
				samples[k].direction.x = x;
				samples[k].direction.y = y;
				samples[k].direction.z = z;
				samples[k].phi = phi;
				samples[k].theta = theta;
				if (NULL == samples[k].shValues) {
					samples[k].shValues = new float[numFunctions];
					// compute SH coefficients for this sample
					SHEval[numBans](x, y, z, samples[k].shValues);
				}
			}
		}
	}
	unsigned int size() { return samples.size(); }


	Sample& operator[](unsigned int idx) {
		assert(idx >=0 && idx < samples.size());
		return samples[idx];
	}
	
	vector<Sample> samples;
};

#endif // _SAMPLER_H_