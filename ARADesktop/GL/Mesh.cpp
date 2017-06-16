#include <stdafx.h>
#include "Mesh.h"


Mesh::Mesh()
{
	numNorm = 0;
	numUVs = 0;
	numVerts = 0;
	uvs = nullptr;
	normals = nullptr;
	vecPos = nullptr;
	tex = nullptr;
	buffersGened = 0;
	material = nullptr;
}

void Mesh::genBuffers()
{
	
	vao = new VertexArrayObject();
	vao->bind();

	vertexBuffer = new VertexBufferObject(GL_ARRAY_BUFFER);
	vertexBuffer->setBufferData(vecPos, numVerts * sizeof(GLfloat) * 3, GL_STATIC_DRAW);
	vertexBuffer->setAttribPointer(0, 3, GL_FLOAT, GL_FALSE);

	normalBuffer = new VertexBufferObject(GL_ARRAY_BUFFER);
	normalBuffer->setAttribPointer(1, 3, GL_FLOAT, GL_FALSE);
	normalBuffer->setBufferData(normals, numNorm * sizeof(GLfloat) * 3, GL_STATIC_DRAW);

	if (numUVs) {
		uvBuffer = new VertexBufferObject(GL_ARRAY_BUFFER);
		uvBuffer->setAttribPointer(2, 2, GL_FLOAT, GL_FALSE);
		uvBuffer->setBufferData(uvs, numUVs * sizeof(GLfloat) * 2, GL_STATIC_DRAW);
	}

	this->buffersGened = 1;

}


void Mesh::render(const float *mvpMat, int currentProgram, std::map<std::pair<int, std::string>, int> uniformLocMap)
{

	int mvpLocation = uniformLocMap[std::make_pair(currentProgram, "mvpMatrix")];
	int texLocation = uniformLocMap[std::make_pair(currentProgram, "isTextured")];
	int hasMaterialLocation = uniformLocMap[std::make_pair(currentProgram, "hasMaterial")];
	int materialLocation = uniformLocMap[std::make_pair(currentProgram, "material")];

	if (!buffersGened)
		genBuffers();

	glDisable(GL_CULL_FACE);
	vao->bind();
	vertexBuffer->bind();
	normalBuffer->bind();
	if (tex) {
		glUniform1i(texLocation, 1);
		tex->bind();
	}
	else if (texLocation != -1)
	{
		glUniform1i(texLocation, 0);
	}
	if (numUVs)
		uvBuffer->bind();

	if (material && hasMaterialLocation != -1)
	{
		glUniform1i(hasMaterialLocation, 1);
		glUniform3f(materialLocation, material->difColour.r, material->difColour.g, material->difColour.b);
	}
	if (mvpLocation != -1)
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvpMat);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
}

Mesh::~Mesh()
{
	free(vecPos);
}