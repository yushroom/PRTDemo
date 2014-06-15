#include "global.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include "Scene.h"
#include "Sample.h"
#include "PRTRenderer.h"

const int numBands   = 5;
const int sqrtNumSamples = 20;



int main()
{
	srand(time(NULL));

	cout << "Preprocess\n";
	auto startTime = Clock::now();

	printf("    Generating samples (%d * %d)...\n", sqrtNumSamples, sqrtNumSamples); 
	Sampler sampler;
	sampler.generateSamples(sqrtNumSamples, numBands);

	printf("    Load model and precompute SH functions...\n");
	Scene scene;
	scene.addModelFromFile("models/Buddha4.obj");
	scene.generateDirectCoeffs(sampler, numBands);

	auto endTime = Clock::now();
	auto ms = duration_cast<milliseconds>(endTime-startTime).count();
	cout << "Preproc time: " << ms << " ms" << '\n';

	
	PRTRenderer renderer(700, 700);
	renderer.compileShader("shader/unShadowed.vert", "shader/unShadowed.frag");
	//renderer.prog.printActiveAttribs();
	//renderer.prog.printActiveUniforms();
	Light* light = Light::directLight(sampler, numBands, 0.5);
	// TODO
	//cout << "precomputeColor()\n";
	//precomputeColor(scene, *light);
	//renderer.renderSceneWithLight(scene, *light);
	//cout << "bindBuffer()\n";
	//scene.bindBuffer();
	//cout << "setUniform()\n";
	renderer.setUniform();
	//cout << "render2()\n";
	//renderer.render2(scene);
	renderer.renderSceneWithLight(scene, *light);
	return 0;
}

