#pragma once
class GLMHelper
{
public:
	GLMHelper();
	~GLMHelper();
	static glm::vec3 vec3FromArray(float *arr);
	static float* arrayFromVec3(glm::vec3 vec);
};

