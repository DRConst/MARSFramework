#pragma once
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include "Mesh.h"
#include "Object.h"

#pragma comment(lib,"assimp.lib")

//#define DEGUG 

class Loader
{
public:
	Loader(std::string name);
	~Loader();
	void loadModel(Object* obj);
	void populatePos(Mesh *mesh, std::string name);


	Assimp::Importer importer;
	const aiScene *scene;
	
	aiMesh *mesh;
	aiVector3D vec;

	
};

