#include "stdafx.h"
#include "Scene.h"
#include "gl/Loader.h"
#include "GL/Texture2D.h"
double oldX, oldY;

int recentlyAdded = 1;

Scene::Scene()
{
	Loader *loader = new Loader("scaleMitre.dae");
	Mesh *mesh = new Mesh();

	selectorPip = new Object();
	loader->loadModel(selectorPip);
	selectorPip->sc = vec3(0.01f);

	selectorPip->updateMMat();

	selectorPip2 = new Object();
	loader->loadModel(selectorPip2);
	selectorPip2->sc = vec3(0.01f);

	selectorPip2->updateMMat();


	selectedStep = nullptr;

	Step *defaultStep = new Step();
	defaultStep->order = -1;
	defaultStep->name = "Default Step";
	defaultStep->cardTitle = "Default Step";
	defaultStep->instructions = "Default Step";

	this->selectedStep = defaultStep;

	steps.emplace_back(defaultStep);
}


Scene::~Scene()
{
}

void Scene::registerObject(Object *obj, bool setAsActive = false)
{
	if (selectedStep) {
		selectedStep->visibleObjects.emplace_back(obj);
		if (setAsActive)
			selectedObject = obj;
	}
}

void Scene::registerInputHandler(InputHandler* ih)
{
	this->inputHandler = ih;
}
bool intersectPlane(const vec3 &n, const vec3 &p0, const vec3 &l0, const vec3 &l, float &t)
{
	vec3 normal = normalize(n);
	vec3 light = normalize(l);
	// assuming vectors are all normalized
	float denom = dot(normal, light);
	if (abs(denom) > 1e-6) {
		vec3 p0l0 = p0 - l0;
		t = dot(p0l0, normal) / denom;
		return (t >= 0);
	}

	return false;
}

void Scene::render(Camera *cam, Framebuffer *fbo, Program *renderProgram, double delta)
{

	int progHandle = renderProgram->programHandle;

	int mvpLocation = uniformLocMap[std::make_pair(progHandle, "mvpMatrix")];
	int texLoc = uniformLocMap[std::make_pair(progHandle, "isTextured")];


	int width = window->width;
	int height = window->height;

	if (selectedObject && inputHandler->isDragging)
	{
		

		if(recentlyAdded)
		{
			vec3 pos = glm::project(selectedObject->pos, cam->view * selectedObject->mMat, cam->proj, vec4(0, 0, width, height));
			inputHandler->resetMousePos(pos.x, pos.y);
			recentlyAdded = !recentlyAdded;
		}
		double x, y;
		inputHandler->getMousePos(&x, &y);
		

		y = height - y;
		vec3 nearPoint = glm::unProject(vec3(x, y, 0.0f), cam->view, cam->proj, vec4(0, 0, width, height));
		vec3 farPoint = glm::unProject(vec3(x, y, 1.0f), cam->view, cam->proj, vec4(0, 0, width, height));
		float t;

		//Check where the ray instersects the plane centered on the object origin with the normal equal to the cam dir
		vec3 origin = selectedObject->pos;
		vec3 dir = cam->direction;

		if (inputHandler->breakPoint())
			dir = dir;

		if(intersectPlane(dir, origin, nearPoint, farPoint - nearPoint, t))
		{
			vec3 pos = nearPoint + normalize(farPoint - nearPoint) * t;
			pos.y = 0; //Force objects to sit on the image plane, good for now??
			selectedObject->pos = pos;
			selectedObject->updateMMat();
		}



	}

	if (autoScaleActive)
	{
		if (activePointer) {
			double x, y;
			inputHandler->getMousePos(&x, &y);


			y = window->height - y;
			vec3 nearPoint = glm::unProject(vec3(x, y, 0.0f), cam->view, cam->proj, vec4(0, 0, width, height));
			vec3 farPoint = glm::unProject(vec3(x, y, 1.0f), cam->view, cam->proj, vec4(0, 0, width, height));
			float t;


			vec3 origin = selectedStep->selectedBackground->pos;
			vec3 dir = vec3(0.0f, 1.0f, 0.0f);

			if (intersectPlane(dir, origin, nearPoint, farPoint - nearPoint, t))
			{
				vec3 pos = nearPoint + normalize(farPoint - nearPoint) * t;

				if (activePointer == 1)
				{
					selectorPip->pos = pos;
					selectorPip->updateMMat();
				}
				else if (activePointer == 2) {
					selectorPip2->pos = pos;
					selectorPip2->updateMMat();
				}
			}

		}
		selectorPip->render(cam, progHandle, uniformLocMap,delta);
		selectorPip2->render(cam, progHandle, uniformLocMap,delta);
	}



	if (selectedStep) {

		if (selectedStep->selectedBackground)
			selectedStep->selectedBackground->render(cam, progHandle, uniformLocMap,delta);

		for (auto obj : selectedStep->visibleObjects)
			obj->render(cam, progHandle, uniformLocMap,delta);
	}
}

void Scene::pick(Camera *cam, GLint colourLoc, Program *pickingProgram, double delta)
{
	int progHandle = pickingProgram->programHandle;

	int mvpLocation = uniformLocMap[std::make_pair(progHandle, "mvpMatrix")];
	int texLoc = uniformLocMap[std::make_pair(progHandle, "isTextured")];
	float col[3];



	double x, y;
	inputHandler->getMousePos(&x, &y);

	if (x<0 || x > window->width)
		return;

	if (y<0 || y > window->height)
		return;

	y = window->height - y;

	if (selectedStep) {
		int numObjects = selectedStep->visibleObjects.size();

		float rangePerObject = 1.0f / static_cast<float>(numObjects);

		float currentCol = 0.0f;

		for (auto obj : selectedStep->visibleObjects)
		{
			glUniform3f(colourLoc, currentCol + rangePerObject*0.1, currentCol + rangePerObject*0.1, currentCol + rangePerObject*0.1);
			obj->render(cam, progHandle, uniformLocMap,delta);
			currentCol += rangePerObject;
		}

		glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, col);

		selectedObject = nullptr;
		vec3 pos;
		
		if(!autoScaleActive)
		{
			if ((abs(col[0] - 1.0f) < 0.01f && col[1] == 0) || (abs(col[1] - 1.0f) < 0.01f && col[0] == 0))
				return;
			if (abs(col[0] - 0.2f) > 0.0001f &&  abs(col[1] - 0.2f) > 0.0001f && abs(col[2] - 0.2f) > 0.0001f)
			{

				int index = col[0] / rangePerObject;
				selectedObject = selectedStep->visibleObjects[index];
				pos = glm::project(selectedObject->pos, cam->view, cam->proj, vec4(0, 0, window->width, window->height));

				inputHandler->resetMousePos(pos.x, window->height - pos.y);
			}

		}

	}

	vec3 pos;
	if (autoScaleActive) {


		glUniform3f(colourLoc, 1.0f, 0.0f, 0.0f);
		selectorPip->render(cam, progHandle, uniformLocMap,delta);
		glUniform3f(colourLoc, 0.0f, 1.0f, 0.0f);
		selectorPip2->render(cam, progHandle, uniformLocMap,delta);

		glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, col);

		if (activePointer != 1 && abs(col[0] - 1.0f) < 0.01f && col[1] == 0)
		{
			//Pip 1
			activePointer = 1;
			pos = glm::project(selectorPip->pos, cam->view, cam->proj, vec4(0, 0, window->width, window->height));
		}
		else if (activePointer != 2 && abs(col[1] - 1.0f) < 0.01f && col[0] == 0)
		{
			//Pip 2
			activePointer = 2;
			pos = glm::project(selectorPip2->pos, cam->view, cam->proj, vec4(0, 0, window->width, window->height));
		}
		else
		{
			activePointer = 0;
		}
	}

}

Object* Scene::getSelectedObject()
{
	return selectedObject;
}

void Scene::loadBase(char str[1024], int isWideScreen)
{
	Loader *loader = new Loader("plane.dae");
	Mesh *mesh = new Mesh();

	loader->populatePos(mesh, "Plane");

	Texture2D *tex = new Texture2D(str);


	if (tex->texLoaded)
		mesh->tex = tex;

	selectedStep->selectedBackground = new Object();
	selectedStep->selectedBackground->meshes.emplace_back(mesh);

	if (isWideScreen)
		selectedStep->selectedBackground->sc.z = 9.0f / 16.0f;
	else
		selectedStep->selectedBackground->sc.z = 3.0f / 4.0f;

	selectedStep->selectedBackground->rot = vec3(1.0f, 0.0f, 0.0f);
	selectedStep->selectedBackground->angle = -90.0f;

	selectedStep->selectedBackground->sc *= vec3(3.0f);

	selectedStep->selectedBackground->updateMMat();
}

void Scene::deleteObject(Object* object)
{
	if (selectedStep) {
		auto it = std::find(selectedStep->visibleObjects.begin(), selectedStep->visibleObjects.end(), object);

		if (it != selectedStep->visibleObjects.end()) {
			using std::swap;
			swap(*it, selectedStep->visibleObjects.back());
			selectedStep->visibleObjects.pop_back();
		}

		selectedObject = nullptr;

		delete object;
	}
}

void Scene::scaleBackground(float expected)
{
	vec3 delta = selectorPip->pos - selectorPip2->pos;

	float dist = glm::length(delta);

	float scale = expected / dist;

	selectedStep->selectedBackground->sc *= scale;
}


void Scene::exportToJson()
{

	json scene;

	json vumarkArray = json::array();

	std::ofstream file;
	file.open("scene.json");

	int i = 0;
	for(auto step : steps)
		for (auto obj : step->visibleObjects)
		{
			if (obj->type == 0)
			{
				vumarkArray.emplace_back(obj->exportVumarkToJson());
			}
		}
	scene["vumarks"] = vumarkArray;
	i = 0;

	json entityArray = json::array();

	for (auto step : steps)
		for (auto obj : step->visibleObjects)
		{
			if (obj->type)
			{
				entityArray.emplace_back(obj->exportEntityToJson());
			}
		}
	scene["entities"] = entityArray;

	json componentArray = json::array();

	i = 0;

	for (auto obj : components)
	{
		if (obj->type == 0)
		{
			RotatingComponent *rot = dynamic_cast<RotatingComponent*>(obj);
			componentArray.emplace_back(rot->exportToJson(i++));
		}
		else if (obj->type == 1)
		{
			PulsatingComponent *pls = dynamic_cast<PulsatingComponent*>(obj);
			componentArray.emplace_back(pls->exportToJson(i++));
		}
		else if (obj->type == 2)
		{
			LinearMovementComponent *lmc = dynamic_cast<LinearMovementComponent*>(obj);
			componentArray.emplace_back(lmc->exportToJson(i++));
		}

	}

	scene["components"] = componentArray;

	json stepArray = json::array();

	for(auto step : steps)
	{
		stepArray.emplace_back(step->exportToJson());
	}
	
	scene["steps"] = stepArray;

	file << scene.dump(4);
}

const char** Scene::getComponentNames(int *count)
{
	const char **toRet = new const char*[components.size()];
	int i;
	for (i = 0; i < components.size(); i++)
		toRet[i] = components[i]->name.c_str();

	*count = i;

	if (i == 0)
	{
		toRet = new const char*[1];
		toRet[0] = nullptr;
	}

	return toRet;
}

void Scene::registerComponent(Component *component)
{
	components.emplace_back(component);
}

std::vector<Component *> Scene::getComponents()
{
	return components;
}

void Scene::updateUnifromLocMap(Program *program)
{

	int currentProgram = program->programHandle;
	int mvpLocation = glGetUniformLocation(currentProgram, "mvpMatrix");
	int texLocation = glGetUniformLocation(currentProgram, "isTextured");
	int hasMaterialLocation = glGetUniformLocation(currentProgram, "hasMaterial");
	int materialLocation = glGetUniformLocation(currentProgram, "material");

	auto pair = std::make_pair(currentProgram, "mvpMatrix");
	uniformLocMap[pair] = mvpLocation;

	pair = std::make_pair(currentProgram, "isTextured");
	uniformLocMap[pair] = texLocation;

	pair = std::make_pair(currentProgram, "hasMaterial");
	uniformLocMap[pair] = hasMaterialLocation;

	pair = std::make_pair(currentProgram, "material");
	uniformLocMap[pair] = materialLocation;
}

std::vector<Step*> Scene::getSteps()
{
	return steps;
}

void Scene::addStep(Step* step)
{
	steps.emplace_back(step);
}

std::vector<Object*> Scene::getObjects()
{
	return objects;
}

void Scene::importJSON(char* importer_path_buffer)
{
	std::string path(importer_path_buffer);

	std::ifstream fileStream;

	fileStream.open(path);

	std::string contents((std::istreambuf_iterator<char>(fileStream)),std::istreambuf_iterator<char>());

	json jsonContents = json::parse(contents.c_str());

	

	//Start parsing steps, if no steps exist in file, all additions are to active step, otherwise, a default step is created.
	json steps = jsonContents["steps"];

	//Store if json contained step to controll insertions
	bool containsSteps = !steps.is_null();
	std::vector<int> *visibleObjects;
	if(!steps.is_null())
	{
		int stepCnt = steps.size();
		visibleObjects = new std::vector<int>[stepCnt];
		for (json::iterator it = steps.begin(); it != steps.end(); ++it) {

			json stepJSON = it.value();

			Step *step = new Step;
			
			step->order = stepJSON["order"];
			step->name = stepJSON["name"].get<std::string>();
			step->cardTitle = stepJSON["name"].get<std::string>();
			step->instructions = stepJSON["instructions"].get<std::string>();
			
			json visibleObjectsArray = stepJSON["visibleEntities"];

			for(int i = 0; i < visibleObjectsArray.size(); i++)
			{
				visibleObjects[step->order].emplace_back(visibleObjectsArray[i]);
			}

			this->steps.emplace_back(step);
		}

	}else
	{
		//Select first available step or create a new one
		if(!this->selectedStep)
		{
			if (this->steps.size())
				this->selectedStep = this->steps[0];
			else
			{
				Step *step = new Step();
				step->name = "Default Step";
				step->cardTitle = "Default Step";
				step->instructions = "Default Step";
				this->steps.emplace_back(step);
				this->selectedStep = step;
			}
		}
	}


	json vumarks = jsonContents["vumarks"];
	if(!vumarks.is_null())
	{
		// iterate the array

		Mesh *vumarkMesh = new Mesh();
		Loader *loader = new Loader("plane.dae");
		loader->populatePos(vumarkMesh, "Plane");
		delete(loader);

		
		for (json::iterator it = vumarks.begin(); it != vumarks.end(); ++it) {
			std::cout << *it << '\n';


			Object *vumark = new Object(0);
			vumark->meshes.emplace_back(vumarkMesh);

			vumark->id = it.value()["id"];

			float x = it.value()["pos"][0];
			float y = it.value()["pos"][1];
			float z = it.value()["pos"][2];

			vumark->pos = { x,y,z };

			if (!it.value()["sc"].is_null()) {
				x = it.value()["sc"][0];
				y = it.value()["sc"][1];
				z = it.value()["sc"][2];

				vumark->sc = { x,y,z };
			}

			if (!it.value()["rot"].is_null()) {
				x = it.value()["rot"][0];
				y = it.value()["rot"][1];
				z = it.value()["rot"][2];

				vumark->rot = { x,y,z };
			}

			if(containsSteps)
			{
				//Find step that contains this vumark
				for(int  i = 0; i < steps.size(); i++)
					for(int visibleObject : visibleObjects[i])
					{
						if (vumark->id == visibleObject)
							this->steps[i]->visibleObjects.emplace_back(vumark);
					}
			}else
			{
				//Here selectedStep is either the only existant step or the user selected one, so add this object to it;
				this->selectedStep->visibleObjects.emplace_back(vumark);
			}
		}
	}

	json entities = jsonContents["entities"];
	
	if (!entities.is_null())
	{
		// iterate the array

		Mesh *entityMesh = new Mesh();
		Loader *loader = new Loader("screw.dae");
		loader->populatePos(entityMesh, "screw");
		delete(loader);


		for (json::iterator it = entities.begin(); it != entities.end(); ++it) {
			std::cout << *it << '\n';


			Object *entity = new Object(1);
			entity->meshes.emplace_back(entityMesh);

			entity->id = it.value()["id"];

			float x = it.value()["pos"][0];
			float y = it.value()["pos"][2];
			float z = it.value()["pos"][1];

			entity->pos = { x,y,z };

			if (!it.value()["sc"].is_null()) {
				x = it.value()["sc"][0];
				y = it.value()["sc"][2];
				z = it.value()["sc"][1];

				entity->sc = { x,y,z };
			}

			if (!it.value()["rot"].is_null()) {
				x = it.value()["rot"][0];
				y = it.value()["rot"][2];
				z = it.value()["rot"][1];

				entity->rot = { x,y,z };
			}

			if (containsSteps)
			{
				//Find step that contains this entity
				for (int i = 0; i < steps.size(); i++)
					for (int visibleObject : visibleObjects[i])
					{
						if (entity->id == visibleObject)
							this->steps[i]->visibleObjects.emplace_back(entity);
					}
			}
			else
			{
				//Here selectedStep is either the only existant step or the user selected one, so add this object to it;
				this->selectedStep->visibleObjects.emplace_back(entity);
			}
		}
	}

	json components = jsonContents["components"];

	if(!components.is_null())
	{
		for (json::iterator it = components.begin(); it != components.end(); ++it) {

			json comp = it.value();

			std::string type = comp["class"];
			float x, y, z;

			if (!type.compare("rotating"))
			{
				RotatingComponent *rot = new RotatingComponent();

				rot->name = comp["name"].get<std::string>();
				rot->type = 0;
				rot->anglePerTick = comp["anglePerTick"];
				x = comp["axis"][0];
				y = comp["axis"][2];
				z = comp["axis"][1];

				rot->rot = { x,y,z };
				this->registerComponent(rot);
			}

			if (!type.compare("pulsating"))
			{
				PulsatingComponent *pls = new PulsatingComponent();

				pls->name = comp["name"].get<std::string>();
				pls->type = 1;
				pls->maxScale = comp["maxScale"];
				pls->minScale = comp["minScale"];
				pls->pulsesPerSecond = comp["pulsesPerSecond"];

				x = comp["scale"][0];
				y = comp["scale"][2];
				z = comp["scale"][1];
				this->registerComponent(pls);
			}
		}
	}
}

