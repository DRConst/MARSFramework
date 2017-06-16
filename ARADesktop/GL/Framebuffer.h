#pragma once
class Framebuffer
{
public:
	Framebuffer(int width, int height);
	void bind();
	~Framebuffer();

	GLuint fbo;
	GLuint colourTex, depthTex;
};

