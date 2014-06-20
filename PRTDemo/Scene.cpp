#include "Scene.h"

bool Scene::addModelFromFile(const char* path)
{
	Object* obj = new Object(path);
	cout << "	Model: # of vertices = " << obj->vertices.size() << ", # of triangles = " << obj->indices.size()/3 << endl;
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
			if (NULL == currentVertex.isBlocked)
				currentVertex.isBlocked = new bool[sampler.size()];
			if (NULL == currentVertex.blockIdx)
				currentVertex.blockIdx = new int[sampler.size()];
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

				if (cosineTerm > 0.f) 
				{
					Ray ray(currentVertex.position + 2*EPSILON*currentVertex.normal, sampler[j].direction);
					int index = isRayBlocked(ray);
					//bool rayBlocked = false;
					if (index > 0) {
						currentVertex.isBlocked[j] = true;
						currentVertex.blockIdx[j] = index;
					} else {
						currentVertex.isBlocked[j] = false;
						currentVertex.blockIdx[j] = -1;
					}

					for (int k = 0; k < numFunctions; k++)
					{
						float contribution = cosineTerm * sampler[j].shValues[k];
						currentVertex.unshadowedCoeffs[k] += contribution;
						if (index == -1) {
							currentVertex.shadowedCoeffs[k] += contribution;
						}
					}	
				} else {
					currentVertex.isBlocked[j] = false;
					currentVertex.blockIdx[j] = -2;	
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

bool Scene::generateDirectCoeffsDS(Sampler& sampler, int numBands, int bounceTime)
{
	assert(bounceTime >= 1 && bounceTime <=3);
	const int numFunctions = numBands*numBands;
	const int numSamples = sampler.size();
	
	for (unsigned int objIdx = 0; objIdx < object.size(); objIdx++)
	{
		const int numVertices = object[objIdx]->vertices.size();
		float intervel = 0;
		
		for (unsigned int i = 0; i < numVertices; i++)
		{
			Vertex& v = object[objIdx]->vertices[i];
			for (unsigned int j = 0; j < numFunctions; j++) {
				if (bounceTime ==1)
					v.shadowedCoeffsDS[bounceTime-1][j] = v.shadowedCoeffs[j];
				else
					v.shadowedCoeffsDS[bounceTime-1][j] = v.shadowedCoeffsDS[bounceTime-2][j];
			}
		}

		for (int i = 0; i < numVertices; i++)
		{
			if (i == intervel) {
				cout <<  i*100/numVertices << "% ";
				intervel += numVertices/10;
			}
			Vertex& currentVertex = object[objIdx]->vertices[i];

			for (int j = 0; j < sampler.size(); j ++)
			{
				if (currentVertex.isBlocked[j]) {
					int index = currentVertex.blockIdx[j];
					//cout << index << endl;
					int vidx0 = object[objIdx]->indices[index];
					int vidx1 = object[objIdx]->indices[index+1];
					int vidx2 = object[objIdx]->indices[index+2];
					Vertex& v0 = object[objIdx]->vertices[vidx0]; 
					Vertex& v1 = object[objIdx]->vertices[vidx1]; 
					Vertex& v2 = object[objIdx]->vertices[vidx2];
					float fScale = 0.0001f;
					float cosineTerm0 = -glm::dot(sampler[j].direction, v0.normal);
					float cosineTerm1 = -glm::dot(sampler[j].direction, v1.normal);
					float cosineTerm2 = -glm::dot(sampler[j].direction, v2.normal);
					for (int  k = 0; k < numFunctions; k++) {
						if (bounceTime == 1) {
							v0.shadowedCoeffsDS[bounceTime-1][k] += fScale * currentVertex.shadowedCoeffs[k] * cosineTerm0;
							v1.shadowedCoeffsDS[bounceTime-1][k] += fScale * currentVertex.shadowedCoeffs[k] * cosineTerm1;
							v2.shadowedCoeffsDS[bounceTime-1][k] += fScale * currentVertex.shadowedCoeffs[k] * cosineTerm2;
						}
						else {
							v0.shadowedCoeffsDS[bounceTime-1][k] += fScale * currentVertex.shadowedCoeffsDS[bounceTime-2][k] * cosineTerm0;
							v1.shadowedCoeffsDS[bounceTime-1][k] += fScale * currentVertex.shadowedCoeffsDS[bounceTime-2][k] * cosineTerm1;
							v2.shadowedCoeffsDS[bounceTime-1][k] += fScale * currentVertex.shadowedCoeffsDS[bounceTime-2][k] * cosineTerm2;
						}
					}
				}
			}
		}
	}
	return true;
}

bool Scene::generateDirectCoeffsDS(Sampler& sampler, int numBands)
{
	const int numFunctions = numBands*numBands;
	const int numSamples = sampler.size();

	for (unsigned int objIdx = 0; objIdx < object.size(); objIdx++)
	{
		const int numVertices = object[objIdx]->vertices.size();
		for (int i = 0; i < numVertices; i++)
		{
			Vertex& currentVertex = object[objIdx]->vertices[i];
			for (int j = 0; j < 3; j++) {
				if (NULL == currentVertex.shadowedCoeffsDS[j])
					currentVertex.shadowedCoeffsDS[j] = new vec3[numFunctions];
			}
		}
	}

	for (int i = 1; i <= 3; i++) {
		generateDirectCoeffsDS(sampler, numBands, i);
		cout << endl;
	}
	return 0;
}


bool Scene::generateDirectCoeffsGS(Sampler& sampler, int numBands )
{
	return true;
}

int Scene::isRayBlocked(Ray& ray)
{
	//cout << "Scene::isRayBlocked\n";
	for (unsigned int i = 0; i < object.size(); i++) {
		int index = object[i]->doesRayHitObject(ray);
		if (index > 0)
			return index;
	}
	return -1;
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