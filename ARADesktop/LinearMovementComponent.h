#pragma once
#include "Component.h"

class LinearMovementComponent : public Component
{
public:
	LinearMovementComponent();
	~LinearMovementComponent();

	glm::mat4 transform(glm::mat4 status, double delta) override;

	json exportToJson(int id);

	glm::vec3 axis;
	float maxDist;
	float distPerTick;

private:
	float pos;
	bool growing;

};

