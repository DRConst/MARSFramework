#pragma once
#include "Shader.h"
class Program
{
public:
	Program();
	~Program();

	void bindVertexShader(Shader *shader);
	void bindFragmentShader(Shader *shader);
	void bindTCShader(Shader *shader);
	void bindTEShader(Shader *shader);
	void bindGeoShader(Shader *shader);
	void bind();
	void finalize();


	GLint getUniformLocation(std::string name);

	void setUniformMatrix4fv(std::string name, void *val);

	GLuint programHandle;

	GLuint vertexShader, fragmentShader, TCShader, TEShader, geoShader = -1;
};

