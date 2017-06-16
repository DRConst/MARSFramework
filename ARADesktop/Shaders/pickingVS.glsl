#version 440 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normals;
uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * position;
}
