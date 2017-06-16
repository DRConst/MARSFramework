#include "stdafx.h"
#include "GLMHelper.h"


GLMHelper::GLMHelper()
{
}


GLMHelper::~GLMHelper()
{
}

glm::vec3 GLMHelper::vec3FromArray(float* arr)
{
	return glm::vec3(arr[0], arr[1], arr[2]);
}

float* GLMHelper::arrayFromVec3(glm::vec3 vec)
{
	//float ret[3] = { vec.x, vec.y, vec.z };
	float *ret = new float[3];
	ret[0] = vec.x;
	ret[1] = vec.y;
	ret[2] = vec.z;
	return ret;
}
