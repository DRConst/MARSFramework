#include <stdafx.h>
#include "VertexArrayObject.h"


VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &vao);
}

void VertexArrayObject::bind()
{
	glBindVertexArray(vao);
}


VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &vao);
}
