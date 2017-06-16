#include "stdafx.h"
#include "Step.h"


Step::Step()
{
	selectedBackground = nullptr;
}


Step::~Step()
{
}

json Step::exportToJson()
{
	json step;

	step["order"] = order;
	step["name"] = cardTitle;
	step["instructions"] = instructions;
	
	json ar = json::array();

	for(auto obj : visibleObjects)
	{
			ar.emplace_back(obj->id);
	}

	step["visibleEntities"] = ar;

	////Switch objects storing step to step storing object

	return step;
}
