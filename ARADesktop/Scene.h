#pragma once
#include <vector>
#include "gl/Object.h"
#include "InputHandler.h"
#include "gl/Framebuffer.h"
#include "Component.h"
#include <map>
#include "Step.h"

class Scene
{
public:
	Scene();
	~Scene();
	void registerObject(Object* obj, bool setAsActive);
	void registerInputHandler(InputHandler *ih);
	void render(Camera* cam, Framebuffer* fbo, Program* renderProgram, double delta);
	void renderDepth(Camera* cam, Framebuffer* fbo);
	void pick(Camera* cam, GLint colourLoc, Program* pickingProgram, double delta);
	Object* getSelectedObject();
	void loadBase(char str[], int isWideScreen);
	void deleteObject(Object* object);
	void scaleBackground(float expected);
	void exportToJson();
	const char** getComponentNames( int *count );
	void registerComponent(Component* component);
	std::vector<Component*> getComponents();
	void updateUnifromLocMap(Program* program);
	std::vector<Step*> getSteps();
	void addStep(Step* step);
	std::vector<Object*> getObjects();
	void importJSON(char* importer_path_buffer);
	bool dragBusy = false;
	GLWindow *window;
	Object* background = nullptr;
	int activePointer = 0;
	bool autoScaleActive = false;
//private:
	std::vector<Object *> objects;
	std::vector<Component *> components;
	std::vector<Step *> steps;
	Object *selectedObject = nullptr;
	InputHandler *inputHandler;
	Framebuffer *depthFB;

	Step *selectedStep = nullptr;

	Object *selectorPip;
	Object *selectorPip2;

	std::map<std::pair<int, std::string>, int> uniformLocMap;
};

