#pragma once
class Texture2D
{
public:
	Texture2D::Texture2D(std::string file, GLenum repeat = GL_REPEAT, GLenum filtering = GL_LINEAR);
	~Texture2D();

	void bind();
	void bind(GLint samplerID);
	GLuint handle;

	int width, height;
	bool texLoaded;
};

