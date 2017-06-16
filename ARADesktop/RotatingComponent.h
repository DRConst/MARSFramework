#pragma once
#include "Component.h"

class RotatingComponent : public Component
{
public:
	RotatingComponent();
	~RotatingComponent();
	glm::mat4 transform(glm::mat4 status, double delta) override;
	json exportToJson(int i);
	glm::vec3 rot;
	float anglePerTick = 0.0f;

private :
	float angle;
};

