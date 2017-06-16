#pragma once
#include "GL/GLWindow.h"
#include "imgui.h"
#include "gl/Camera.h"
#include "Component.h"
#include "GL/Program.h"
#include "RotatingComponent.h"
#include "PulsatingComponent.h"
#include <map>
#include "Step.h"
#include "LinearMovementComponent.h"

class Scene;

class UI
{
public:
	UI();
	UI(GLWindow* window);
	~UI();
	void render();
	void setActiveScene(Scene *scene);
	void setActiveCam(Camera* cam);
	bool isCursorInUi(double x, double y);
	ImVec2 size, pos;
	char* backgroundPathBuffer, *cardTitleBuff, *cardNameBuff, *cardInsBuff, *selectedObjectNameBuffer, *importerPathBuffer;
	int activeItem = 1;
	float expectedSize = 1.0f;
	Camera* activeCam;
	int componentComboItem = 0;
	bool newComponentWindowActive = false;
	Program* renderProgram;
	bool componentSelectionWindowActive = false;

	bool stepWindowActive = false;
	int stepComboItem = 0, stepOrder = -1;
	Step *activeStep;
	bool stepAssignmentWindowActive = false;
	bool importerWindowActive = false;
	char* componentNameBuff;
	char* vumarkLableBuff;
private:
	Scene *activeScene;
	GLWindow* window;
	Component *selectedComponent;
	RotatingComponent *tempRotatingComponent = nullptr;
	PulsatingComponent* tempPulsatingComponent = nullptr;
	LinearMovementComponent *tempLinearMovementComponent = nullptr;
	std::vector<bool> componentActive;

	std::map<std::string, std::pair<ImVec2, ImVec2>> UICoordMap;

	int currentComponentTypeIndex = 0;
	int newObjectID = 0;
};

