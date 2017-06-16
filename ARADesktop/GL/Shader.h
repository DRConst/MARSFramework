#pragma once
class Shader
{
public:
	Shader(std::string path, GLenum type);
	~Shader();
	void load(std::string path);
	GLuint shaderHandle;
	GLenum type;
};

