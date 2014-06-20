#include "global.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include "Scene.h"
#include "Sample.h"
#include "PRTRenderer.h"

const int numBands		 = 5;
const int sqrtNumSamples = 20;

int main()
{
	srand(time(NULL));

	cout << "Preprocess\n";
	auto startTime = Clock::now();

	//printf("    Generating samples (%d * %d)...\n", sqrtNumSamples, sqrtNumSamples); 
	Sampler sampler;
	sampler.generateSamples(sqrtNumSamples, numBands);

	const char* lightName = "Environments/grace_probe.pfm";
	//const char* lightName = "grace_probe.jpg";
	cout << "    Load environment light " << endl;
	// get environment light
	Light* light1 = Light::directLight(sampler, numBands, 0.5);
	Light* light2 = Light::lightFromImage(lightName, sampler, numBands);

	cout << "    Load model and precompute SH functions...\n";
	Scene scene;
	//scene.addModelFromFile("models/Buddha4.obj"); 
	//scene.addModelFromFile("models/bunny.obj");
	//scene.addModelFromFile("models/head_sad.x");
	scene.addModelFromFile("models/dragon.obj");
	cout << "    Ray tracing...\n        ";
	scene.generateDirectCoeffs(sampler, numBands);
	cout << "\n    Generate SH coefficients...\n";
	scene.generateDirectCoeffsDS(sampler, numBands);

	auto endTime = Clock::now();
	auto ms = duration_cast<milliseconds>(endTime-startTime).count();
	cout << "Preproc time: " << ms << " ms" << '\n';

	
	PRTRenderer renderer(700, 700);
	renderer.compileShader("shader/unShadowed.vert", "shader/unShadowed.frag");
	renderer.setUniform();
	renderer.renderSceneWithLight(scene, *light2);
	return 0;
}

