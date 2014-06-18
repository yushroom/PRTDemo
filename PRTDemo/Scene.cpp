#include "Scene.h"

bool Scene::addModelFromFile(const char* path)
{
	Object* obj = new Object(path);
	cout << "	Model: # of vetices = " << obj->vertices.size() << ", # of triangles = " << obj->indices.size()/3 << endl;
	object.push_back(obj);
	return true;
}

bool Scene::generateDirectCoeffs(Sampler& sampler, int numBands)
{
	const int numFunctions = numBands*numBands;
	const int numSamples = sampler.size();

	for (unsigned int objIdx = 0; objIdx < object.size(); objIdx++)
	{
		const int numVertices = object[objIdx]->vertices.size();
		for (int i = 0; i < numVertices; i++)
		{
			Vertex& currentVertex = object[objIdx]->vertices[i];
			if (NULL == currentVertex.unshadowedCoeffs)
				currentVertex.unshadowedCoeffs = new vec3[numFunctions];
			if (NULL == currentVertex.shadowedCoeffs)
				currentVertex.shadowedCoeffs = new vec3[numFunctions];
			if (NULL == currentVertex.unshadowedCoeffs || NULL == currentVertex.shadowedCoeffs) {
				cout << "[ERROR] Unable to create space for vertex SH coefficients" << endl;
				exit(1);
			}
		}
	}

	for (unsigned int objIdx = 0; objIdx < object.size(); objIdx++)
	{
		const int numVertices = object[objIdx]->vertices.size();
		float intervel = 0;
		for (int i = 0; i < numVertices; i++)
		{
			if (i == intervel) {
				cout <<  i*100/numVertices << "% ";
				intervel += numVertices/10;
			}
			Vertex& currentVertex = object[objIdx]->vertices[i];

			for (int j = 0; j < numFunctions; j++) {
				for (int k = 0; k < 3; k++) {
					currentVertex.unshadowedCoeffs[j][k] = 0.f;
					currentVertex.shadowedCoeffs[j][k] = 0.f;
				}
			}

			for (int j = 0; j < sampler.size(); j ++)
			{
				float cosineTerm = glm::dot(sampler[j].direction, currentVertex.normal);

				if (cosineTerm > 0.f) {
					Ray ray(currentVertex.position + 2*EPSILON*currentVertex.normal, sampler[j].direction);
					//Ray ray(currentVertex.position, sampler[j].direction);
					bool rayBlocked = isRayBlocked(ray);
					//bool rayBlocked = false;
					currentVertex.isBlocked[j] = rayBlocked;
					for (int k = 0; k < numFunctions; k++)
					{
						float contribution = cosineTerm * sampler[j].shValues[k];
						currentVertex.unshadowedCoeffs[k] += contribution;
						if (!rayBlocked) {
							//cout << "1";
							currentVertex.shadowedCoeffs[k] += contribution;
						} else  {
							//cout << "0";
						}
					}	
				}
			}
			// Rescale the coefficients
			float scale = 4*PI/numSamples;
			for (int j = 0; j < numFunctions; j++) {
				currentVertex.unshadowedCoeffs[j] *= scale;
				currentVertex.shadowedCoeffs[j]   *= scale;
			}
		}
	}
	return true;
}

bool Scene::generateDirectCoeffsGS(Sampler& sampler, int numBands )
{
	return true;
}

bool Scene::isRayBlocked(Ray& ray)
{
	//cout << "Scene::isRayBlocked\n";
	for (unsigned int i = 0; i < object.size(); i++) {
		if (object[i]->doesRayHitObject(ray))
			return true;
	}
	return false;
}

void Scene::bindBuffer()
{
	if (object.size() < 1)
		return;
	// ! only bind the object 
	// TODO
	//unsigned int numIndices = object[0]->indices.size();
	numIndices = object[0]->indices.size();
	//GLuint VB;
	//GLuint IB;

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * object[0]->vertices.size(), &object[0]->vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &object[0]->indices[0], GL_STATIC_DRAW);
}