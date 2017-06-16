#include "stdafx.h"
#include "RotatingComponent.h"


RotatingComponent::RotatingComponent()
{
	this->type = 0;
	this->anglePerTick = 0.0f;
	this->angle = 0.0f;
}


RotatingComponent::~RotatingComponent()
{
}

glm::mat4 RotatingComponent::transform(glm::mat4 status, double delta)
{
	angle += anglePerTick * 60 * delta;
	angle = fmod(angle, 360.0f);

	return glm::rotate(status, angle, rot);
}

json RotatingComponent::exportToJson(int id)
{
	/*std::string toRet;

	toRet += "{\n";
	toRet += "\"name\" : \"" + *name + "\",\n";
	toRet += "\"id\" : " + std::to_string(id) + ",\n";
	toRet += "\"class\" : \"rotating\",\n";
	toRet += "\"axis\" : [" + std::to_string(rot.x) + "," + std::to_string(rot.z) + "," + std::to_string(rot.y) + "],\n";
	toRet += "\"anglePerTick\" : " + std::to_string(anglePerTick) + "\n";
	toRet += "}";
	return toRet;*/

	json comp;

	comp["name"] = name;
	comp["id"] = id;
	comp["class"] = "rotating";
	comp["axis"] = { rot.x, rot.z, rot.y };
	comp["anglePerTick"] = anglePerTick;

	return comp;
}
