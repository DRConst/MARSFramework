#pragma once

class VertexBufferObject
{
public:
	VertexBufferObject(GLenum bufferTarget);
	~VertexBufferObject();
	void setBufferData(GLvoid *data, GLuint dataSize, GLenum usage = GL_STATIC_DRAW);
	void setAttribPointersArray(int cnt, GLuint *indexes, GLint *sizes, GLenum *dataTypes, GLboolean *normalized,GLsizei *strides, GLvoid **data);
	void setAttribPointer(GLuint index, GLint size, GLenum dataType, GLboolean normalized, GLsizei stride = 0, GLvoid *data = NULL);
	void bind();
private:
	GLuint vbo;
	GLenum bufferTarget;
	GLenum bufferUsage;
	
	std::vector<GLuint> attribIndexes;
};

