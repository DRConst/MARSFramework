#include "stdafx.h"
#include "LinearMovementComponent.h"


LinearMovementComponent::LinearMovementComponent()
{
	this->maxDist = 0.0f;
	this->distPerTick = 0.0f;
	this->type = 2;

	pos = 0;
}


LinearMovementComponent::~LinearMovementComponent()
{
}

glm::mat4 LinearMovementComponent::transform(glm::mat4 status, double delta)
{
	float tick = (float)(distPerTick * delta );
	float x, y, z;

	if (pos >= maxDist)
	{
		growing = false;
	}
	else if (pos <= 0.0) {
		growing = true;
	}
	if (growing) {
		pos += tick;

	}
	else {
		pos -= tick;
	}
	x = (pos)* axis[0];
	y = (pos)* axis[1];
	z = (pos)* axis[2];
	
	return glm::translate(status, glm::vec3(x, y, z));
}


json LinearMovementComponent::exportToJson(int id)
{

	json comp;

	comp["name"] = name;
	comp["id"] = id;
	comp["class"] = "linearMovement";
	comp["axis"] = { axis.x, axis.z, axis.y };
	comp["maxDist"] = maxDist;
	comp["distPerTick"] = distPerTick;

	return comp;
}
