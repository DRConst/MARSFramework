#include <stdafx.h>
#include "VertexBufferObject.h"


VertexBufferObject::VertexBufferObject(GLenum bufferTarget)
{
	glGenBuffers(1, &this->vbo);
	this->bufferTarget = bufferTarget;
}




VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &this->vbo);
}

void VertexBufferObject::setBufferData(GLvoid * data, GLuint dataSize, GLenum usage)
{
	glBindBuffer(bufferTarget, vbo);
	glBufferData(bufferTarget, dataSize, data, usage);
	bufferUsage = usage;
}

void VertexBufferObject::setAttribPointersArray(int cnt, GLuint * indexes, GLint * sizes, GLenum * dataTypes, GLboolean * normalized, GLsizei * strides, GLvoid ** data)
{
	for (int i = 0; i < cnt; i++)
	{
		this->setAttribPointer(indexes[i], sizes[i], dataTypes[i], normalized[i], strides[i], data[i]);
	}
}

void VertexBufferObject::setAttribPointer(GLuint index, GLint size, GLenum dataType, GLboolean normalized, GLsizei stride, GLvoid * data)
{
	glBindBuffer(bufferTarget, vbo);
	
	glVertexAttribPointer(index, size, dataType, normalized, stride, data);
	glEnableVertexAttribArray(index);
	attribIndexes.push_back(index);
}

void VertexBufferObject::bind()
{
	glBindBuffer(bufferTarget, vbo);
	for each (GLuint index in attribIndexes)
	{
		glEnableVertexAttribArray(index);
	}
}
