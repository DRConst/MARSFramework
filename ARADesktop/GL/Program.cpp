#include <stdafx.h>
#include "Program.h"


Program::Program()
{
	programHandle = glCreateProgram();
}


Program::~Program()
{
}

void Program::bindVertexShader(Shader * shader)
{
	this->vertexShader = shader->shaderHandle;
	glAttachShader(this->programHandle, vertexShader);
}

void Program::bindFragmentShader(Shader * shader)
{
	this->fragmentShader = shader->shaderHandle;
	glAttachShader(this->programHandle, fragmentShader);
}

void Program::bindTCShader(Shader * shader)
{
	this->TCShader = shader->shaderHandle;
	glAttachShader(this->programHandle, TCShader);
}

void Program::bindTEShader(Shader * shader)
{
	this->TEShader = shader->shaderHandle;
	glAttachShader(this->programHandle, TEShader);
}

void Program::bindGeoShader(Shader * shader)
{
	this->geoShader = shader->shaderHandle;
	glAttachShader(this->programHandle, geoShader);
}

void Program::finalize()
{
	glLinkProgram(this->programHandle);

	GLint isLinked = 0;
	glGetProgramiv(this->programHandle, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(this->programHandle, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(this->programHandle, maxLength, &maxLength, &infoLog[0]);
		std::cerr << "ERROR::Program::Program Linking Failed : " ;
		for (int i = 0; i < infoLog.size(); i++)
			std::cerr << infoLog.at(i);
		std::cerr << std::endl;
		//The shadowProgram is useless now. So delete it.
		glDeleteProgram(this->programHandle);

		//Provide the infolog in whatever manner you deem best.
		//Exit with failure.
		return;
	}

}

GLint Program::getUniformLocation(std::string name)
{
	return glGetUniformLocation(this->programHandle, name.c_str());
}

void Program::setUniformMatrix4fv(std::string name, void *val)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, (GLfloat *) val);
}

void Program::bind()
{
	glUseProgram(this->programHandle);
}
