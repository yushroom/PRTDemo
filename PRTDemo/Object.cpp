#include "Object.h"

Object::Object(const char* path)
{
	Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(path,  aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	 const aiScene* scene = importer.ReadFile( path, 
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType			 |
		aiProcess_GenNormals);
	if (!scene) {
		cout << "[ERROR] can not open model " << path << endl;
		exit(1);
		//return false;
	}

	// get each mesh
	int nvertices = 0;
	int ntriangles = 0;
	//cout << "scene->mNumMeshes = " << scene->mNumMeshes << endl; 
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		nvertices += mesh->mNumVertices;
		ntriangles += mesh->mNumFaces;
	}
	//cout << "nvirtices = " << nvertices << " ntriangles = " << ntriangles << endl;

	vertices.reserve(nvertices);
	indices.reserve(ntriangles);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			aiVector3D& v = mesh->mVertices[j];
			aiVector3D& n = mesh->mNormals[j];
			//aiVector3D pTexCoord = Zero3D; // set zero if not has texture
			vertices.push_back(Vertex(v.x, v.y, v.z, n.x, n.y, n.z));
			aabb.expand(glm::vec3(v.x, v.y, v.z));
		}
		for(unsigned int j = 0; j < mesh->mNumFaces; j++) {
			const aiFace& Face = mesh->mFaces[j];
			assert(Face.mNumIndices == 3);
			indices.push_back(Face.mIndices[0]);
			indices.push_back(Face.mIndices[1]);
			indices.push_back(Face.mIndices[2]);
		}
	}

	// adjust, not robust meshod, but it's simple...
	float fScale = aabb.max.x - aabb.min.x;
	fScale = std::max(aabb.max.y - aabb.min.y, fScale);
	fScale = std::max(aabb.max.z - aabb.min.z, fScale);
	fScale = 1.f/fScale * 7;
	//cout << "fScale = " << fScale << endl;

	vec3 center = aabb.max+aabb.min;
	center.x /= 2.f; center.y /= 2.f; center.z /= 2.f;
	aabb.max -= center;
	aabb.min -= center;
	//cout << "center =  ( " << center.x << ", " << center.y << ", " << center.z << " )\n";
	//cout << "fscale = " << fScale << endl;
	for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
		it->position -= center;
		it->position *= fScale;
	}
	aabb.max *= fScale;
	aabb.min *= fScale;

	// apply uniform grid
	grid = new Grid(this);
}

bool Object::doesRayHitObject(Ray& ray) const
{
	//cout << "Object::doesRayHitObject" << endl;
	if (!aabb.intersect(ray))
		return false;

	// test each triangle individually
	return grid->intersect(ray);
	//for (unsigned int i = 0; i < indices.size(); i+=3)
	//{
	//	vec3 v[3];
	//	v[0] = vertices[indices[i  ]].position;
	//	v[1] = vertices[indices[i+1]].position;
	//	v[2] = vertices[indices[i+3]].position;

	//	if (ray.intersectsTriangle(v[0], v[1], v[2]))
	//		return true;
	//}
	//return false;
}


void Object::render()
{
	//gl
}