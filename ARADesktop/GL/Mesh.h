#pragma once
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Texture2D.h"
#include "Material.h"

class Mesh
{
public:
	Mesh();
	void genBuffers();
	void render(const float* mvpMat, int currentProgram, std::map<std::pair<int, std::string>, int> uniformLocMap);
	void render(const float* mvpMat, int mvpLocation, int texLocation);
	~Mesh();

	VertexArrayObject *vao;

	VertexBufferObject *vertexBuffer;
	VertexBufferObject* normalBuffer;
	VertexBufferObject* uvBuffer;


	GLfloat *vecPos = nullptr;
	GLfloat *normals = nullptr;
	GLfloat* uvs = nullptr;
	int numVerts = 0;
	int numNorm = 0;
	int numUVs = 0;

	Texture2D *tex;
	int buffersGened = 0;
	Material* material;
};

