#pragma once
class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();
	void bind();
private:
	GLuint vao;
};

