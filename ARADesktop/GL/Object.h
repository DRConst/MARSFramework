#pragma once
#include <stdafx.h>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture2D.h"
#include "Program.h"
#include "Component.h"

class Object
{
public:
	~Object();
	Object(int type = -1);
	void render(Camera* cam, int currentProgram, std::map<std::pair<int, std::string>, int> uniformLocMap, double deltaTime);
	void setMesh(Mesh *mesh);

	void setCull(GLenum cull);
	void updateMMat();
	void resetComponents();
	json exportVumarkToJson();
	json exportEntityToJson();
	void addComponent(Component* component);
	glm::vec3 pos = vec3(0.0f), sc = vec3(1.0f), rot = vec3(1.0f);
	float angle = 0.0f;
	float size; //Radius of the bounding sphere
	std::string name;

	VertexArrayObject *vertexArray;
	VertexBufferObject *vertexBuffer;
	VertexBufferObject* normalBuffer;
	VertexBufferObject* uvBuffer;
	glm::mat4 mMat;
	Camera *cam;
	Mesh *mesh;
	GLenum culling = GL_BACK;

	std::vector<Mesh*> meshes;
	// 0 = vumark, 1 = screw
	int type, id;
	std::vector<Component*> *components;
	int order = -1;
	std::string lable;
};

