#pragma once
#include "Component.h"

class PulsatingComponent : public Component
{
public:
	PulsatingComponent();
	~PulsatingComponent();

	glm::mat4 transform(glm::mat4 status, double delta) override;
	json exportToJson(int i);
	glm::vec3 scale;
	float pulsesPerSecond;
	float maxScale;
	float minScale;
	double currentScale;
	bool growing;
};

