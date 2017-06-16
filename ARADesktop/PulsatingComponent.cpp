#include "stdafx.h"
#include "PulsatingComponent.h"


PulsatingComponent::PulsatingComponent()
{
	this->maxScale = 0.0f;
	this->minScale = 0.0f;
	this->pulsesPerSecond = 0.0f;
	this->type = 1;
	this->currentScale = 1;
}


PulsatingComponent::~PulsatingComponent()
{
}

glm::mat4 PulsatingComponent::transform(glm::mat4 status, double delta)
{
	double scalePerSecond = (maxScale - minScale) / pulsesPerSecond;

	double tick = scalePerSecond * delta ;

	float x, y, z;

	if (this->currentScale > maxScale)
	{
		this->growing = false;
	}
	else if (this->currentScale < minScale) {
		this->growing = true;
	}
	if (this->growing) {
		this->currentScale += tick;

	}
	else {
		this->currentScale -= tick;
	}
	x = (this->currentScale)* scale[0];
	y = (this->currentScale)* scale[1];
	z = (this->currentScale)* scale[2];
	
	return glm::scale(status, glm::vec3(x,y,z));

}

json PulsatingComponent::exportToJson(int id)
{
	/*std::string toRet;

	toRet += "{\n";
	toRet += "\"name\" : \"" + *name + "\",\n";
	toRet += "\"id\" : " + std::to_string(id) + ",\n";
	toRet += "\"class\" : \"linearPulsating\",\n";
	toRet += "\"scale\" : [" + std::to_string(scale.x) + "," + std::to_string(scale.z) + "," + std::to_string(scale.y) + "],\n";
	toRet += "\"pulsesPerSecond\" : " + std::to_string(pulsesPerSecond) + ",\n";
	toRet += "\"maxScale\" : " + std::to_string(maxScale) + ",\n";
	toRet += "\"minScale\" : " + std::to_string(minScale) + "\n";
	toRet += "}";
	return toRet;*/

	json comp;

	comp["name"] = name;
	comp["id"] = id;
	comp["class"] = "linearPulsating";
	comp["scale"] = { scale.x, scale.z, scale.y };
	comp["pulsesPerSecond"] = pulsesPerSecond;
	comp["maxScale"] = maxScale;
	comp["minScale"] = minScale;

	return comp;
}
