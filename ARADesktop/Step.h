#pragma once
#include "GL/Object.h"

class Step
{
public:
	Step();
	~Step();

	std::string name, instructions, cardTitle;
	std::vector<Object*> visibleObjects;
	int order;

	Object *selectedBackground;	

	json exportToJson();
};

