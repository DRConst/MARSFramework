#include <stdafx.h>
#include "Texture2D.h"


Texture2D::Texture2D(std::string file, GLenum repeat, GLenum filtering)
{
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	

	unsigned char* image =
		SOIL_load_image(file.c_str(), &width, &height, nullptr, SOIL_LOAD_RGB);

	if (!image)
		return;

	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	if (filtering == GL_NEAREST_MIPMAP_LINEAR || filtering == GL_NEAREST_MIPMAP_NEAREST || filtering == GL_LINEAR_MIPMAP_LINEAR || filtering == GL_LINEAR_MIPMAP_NEAREST)
		glGenerateMipmap(handle);

	this->texLoaded = true;


	
}




Texture2D::~Texture2D()
{
}

void Texture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture2D::bind(GLint samplerID)
{
	glActiveTexture(GL_TEXTURE0 + samplerID);
	glBindTexture(GL_TEXTURE_2D, handle);
}
