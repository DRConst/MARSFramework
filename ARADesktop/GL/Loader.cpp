#include <stdafx.h>
#include "Loader.h"
#include "Object.h"
#include "Material.h"


Loader::Loader(std::string name)
{
	scene = importer.ReadFile(std::string("./Models/") + name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	
}


Loader::~Loader()
{
	
}
//Fast sqrt aprox using SSE
float sqrt_recip(float x) {
	return 1.0f;// _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ps1(x))); //same as _mm_set1_ps
}


void Loader::loadModel(Object *obj)
{
	int numMeshes = scene->mNumMeshes;


	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		Mesh *mesh = new Mesh();

		this->populatePos(mesh, scene->mMeshes[i]->mName.C_Str());

		obj->meshes.emplace_back(mesh);
	}
}

void Loader::populatePos(Mesh *myMesh, std::string name)
{

	float largestRadius = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		if (scene->mMeshes[i]->mName == aiString(name))
			mesh = scene->mMeshes[i];
	}
	int numFaces = mesh->mNumFaces;

	myMesh->vecPos = new float[3 * 3 * numFaces];
	myMesh->numVerts = numFaces * 3;
	int index = 0;
	float rad;
	for (int c = 0; c < numFaces; c++)
	{
		aiFace face = mesh->mFaces[c];
		for (int j = 0; j < 3; j++)
		{
			vec = mesh->mVertices[face.mIndices[j]];
			myMesh->vecPos[index++] = vec[0];
			myMesh->vecPos[index++] = vec[1];
			myMesh->vecPos[index++] = vec[2];
			rad = sqrt_recip(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
			if (rad > largestRadius)
				largestRadius = rad;
		}
	}
	
	
	myMesh->normals = new float[3 * 3 * numFaces];
	myMesh->numNorm = numFaces * 3;

	index = 0;

	for (int c = 0; c < numFaces; c++)
	{
		aiFace face = mesh->mFaces[c];
		for (int j = 0; j < 3; j++)
		{
			vec = mesh->mNormals[face.mIndices[j]];
			myMesh->normals[index++] = vec[0];
			myMesh->normals[index++] = vec[1];
			myMesh->normals[index++] = vec[2];
		}
	}

	if (mesh->HasTextureCoords(0)) {
		myMesh->uvs = new float[2 * 3 * numFaces];
		myMesh->numUVs = numFaces * 3;

		index = 0;

		for (int c = 0; c < numFaces; c++)
		{
			aiFace face = mesh->mFaces[c];
			for (int j = 0; j < 3; j++)
			{
				vec = mesh->mTextureCoords[0][face.mIndices[j]];
				myMesh->uvs[index++] = 1.0f - vec[0];
				myMesh->uvs[index++] = 1.0f - vec[1];
			}
		}
	}
	
	if (scene->mNumMaterials) {
		aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D difCol;
		aiString asd;
		mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), asd);

		if(asd != aiString(""))
		{
			Texture2D *tex = new Texture2D(std::string("./Models/" + std::string(asd.C_Str())));

			if (tex->texLoaded)
				myMesh->tex = tex;
			else
				delete(tex);
		}

		/*
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, difCol);
		Material *material = new Material;
		material->difColour.r = difCol.r;
		material->difColour.g = difCol.g;
		material->difColour.b = difCol.b;

		myMesh->material = material;*/
	}

	/*if (DEGUG)
	{
		while (i < size * 3 + 1)
		{
			printf("%f %f %f \n", myMesh->vecPos[i++], myMesh->vecPos[i++], myMesh->vecPos[i++]);			
		}
	}*/
}
