#pragma once
class Component
{
public:
	Component();
	virtual ~Component();

	virtual glm::mat4 transform(glm::mat4 status, double delta) { return glm::mat4(); }

	std::string name;
	//Type = 0 (rot) = 1 (pulse)
	int type;
};

