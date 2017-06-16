#include <stdafx.h>
#include "Shader.h"


Shader::Shader(std::string path, GLenum type)
{
	shaderHandle = glCreateShader(type);
	this->type = type;
	this->load(path.c_str());
}


Shader::~Shader()
{
}

void Shader::load(std::string path)
{
	std::string code;
	std::stringstream stream;
	std::ifstream file;
	const char * src;
	GLint success;

	file.open(path);

	if(file.is_open())
	{
		stream << file.rdbuf();
		code = stream.str();
		file.close();
		src = code.c_str();
		glShaderSource(shaderHandle, 1, &src, nullptr);

		glCompileShader(shaderHandle);

		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);


		if(success != GL_TRUE)
		{
			std::cerr << "ERROR::SHADER::Shader Compilation Failed" << std::endl;
		}

	}else
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}