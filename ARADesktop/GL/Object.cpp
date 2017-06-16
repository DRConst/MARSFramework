#include <stdafx.h>
#include "Object.h"
#include "Program.h"
#include "Camera.h"
#include "Camera.h"

Object::Object(int type)
{
	components = new std::vector<Component*>;

	 
	this->mMat = translate(pos) * scale(sc) * rotate(angle, rot);

	this->mesh = mesh;

	this->type = type;
	
	if (type == 0)
		this->lable = "VuMark00";
}

void Object::render(Camera *cam, int currentProgram, std::map<std::pair<int, std::string>, int> uniformLocMap, double deltaTime)
{
	
	this->mMat = translate(pos) * scale(sc) * rotate(angle, rot);

	for(Component * component : *components)
	{
		mMat = component->transform(mMat, deltaTime);
	}

	for (Mesh* mesh : meshes)
		mesh->render(value_ptr(cam->proj * cam->view * mMat), currentProgram, uniformLocMap);
}

void Object::setCull(GLenum cull)
{
	this->culling = cull;
}

void Object::updateMMat()
{
	this->mMat = translate(pos) * scale(sc) * rotate(angle, rot);
}

void Object::resetComponents()
{
	if(this->components && components->size() > 0)
		this->components->clear();
}

json Object::exportVumarkToJson()
{
	/*
	std::string toRet;

	toRet += "{\n";
	toRet += "\"name\" : \"vumark\",\n";
	toRet += "\"id\" : " + std::to_string(id) + ",\n";
	toRet += "\"desc\" : \"vumark\",\n";
	toRet += "\"pos\" : [" + std::to_string(pos.x) + "," + std::to_string(pos.z) + "," + std::to_string(pos.y) + "],\n";
	toRet += "\"database\" : \"Vuforia.xml\"\n";
	toRet += "}";
	return toRet;*/

	json vumark;

	vumark["name"] = name;
	vumark["class"] = "vumark";
	vumark["id"] = id;
	vumark["desc"] = "vumark";
	vumark["lable"] = lable;
	vumark["pos"] = { pos.x, pos.z, pos.y };
	vumark["rot"] = { rot.x, rot.z, rot.y };
	vumark["sc"] = { sc.x, sc.z, sc.y };
	vumark["database"] = "Vuforia.xml";

	return vumark;
}

json Object::exportEntityToJson()
{

	/*std::string toRet;
	toRet += "{\n";
	toRet += "\"id\" : " + std::to_string(id) + ",\n";
	toRet += "\"pos\" : [" + std::to_string(pos.x) + "," + std::to_string(pos.z) + "," + std::to_string(pos.y) + "],\n";
	toRet += "\"components\" : [";
	bool firstLoop = true;
	for(auto comp : *components)
	{
		if(!firstLoop)
			toRet += ", ";
		else
			firstLoop = !firstLoop;
		toRet += "\"" + *comp->name + "\"";
		
	}

	toRet += "]\n}\n";
	return toRet;*/

	json entity;

	entity["id"] = id;
	entity["pos"] = { pos.x, pos.z, pos.y };
	entity["rotAngle"] = angle;
	entity["rot"] = { rot.x, rot.z, rot.y };
	entity["sc"] = { sc.x, sc.z, sc.y };
	if (type == 1)
		entity["class"] = "screw";
	else if (type == 2)
		entity["class"] = "ram";
	else if (type == 3)
		entity["class"] = "mitre";

	json componentArray = json::array();

	for (auto comp : *components)
	{
		componentArray.emplace_back(comp->name);
	}

	entity["components"] = componentArray;

	return entity;

}
void Object::addComponent(Component* component)
{
	this->components->emplace_back(component);
}

Object::~Object()
{
}
