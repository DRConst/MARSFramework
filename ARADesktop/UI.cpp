#include "stdafx.h"
#include "UI.h"
#include "GL/GLWindow.h"
#include "imgui.h"
#include "examples/opengl3_example/imgui_impl_glfw_gl3.h"
#include "Scene.h"
#include "gl/Loader.h"
#include <memory>
#include "GLMHelper.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "RotatingComponent.h"
#include "PulsatingComponent.h"
#include <thread>


UI::UI(GLWindow *window)
{
	this->window = window;
	this->backgroundPathBuffer = (char*)calloc(1024, sizeof(char));
	this->componentNameBuff = (char*)calloc(1024, sizeof(char));
	this->selectedObjectNameBuffer = (char*)calloc(1024, sizeof(char));
	this->importerPathBuffer = (char*)calloc(1024, sizeof(char));
	this->cardNameBuff = (char*)calloc(1024, sizeof(char));
	this->cardTitleBuff = (char*)calloc(1024, sizeof(char));
	this->cardInsBuff = (char*)calloc(1024, sizeof(char));
	this->vumarkLableBuff = (char*)calloc(1024, sizeof(char));
	ImGui_ImplGlfwGL3_Init(window->window, true);

}


UI::~UI()
{
}

void UI::render()
{
	bool asd = false;
	ImGui_ImplGlfwGL3_NewFrame();


	/*ImGui::ShowTestWindow(&asd);*/

	ImGui::Begin("ARA", nullptr);
	if (ImGui::CollapsingHeader("Camera"))
	{


		if (ImGui::Button("Top"))
		{

			activeCam->verticalAngle = -90.f;
			activeCam->horizontalAngle = 0.f;
			activeCam->up = vec3(1.0f, 0.0f, 0.0f);
			activeCam->handleMove(vec3(0.0f, 5.0f, 0.0f));
		}

		if (ImGui::Button("Front"))
		{
			activeCam->verticalAngle = 0.f;
			activeCam->horizontalAngle = -180.f;
			activeCam->up = vec3(0.0f, 1.0f, 0.0f);
			activeCam->handleMove(vec3(0.0f, 0.0f, 5.0f));
		}

		if (ImGui::Button("Right"))
		{
			activeCam->verticalAngle = 0.f;
			activeCam->horizontalAngle = 90.f;
			activeCam->up = vec3(0.0f, 1.0f, 0.0f);
			activeCam->handleMove(vec3(5.0f, 0.0f, 0.0f));
		}



	}
	if (ImGui::CollapsingHeader("Entity"))
	{
		if (activeScene->selectedStep)
		{
			if (ImGui::Button("Screw"))
			{
				Loader *loader = new Loader("screw.dae");
				Mesh *mesh = new Mesh();
				
				Object *obj = new Object(1);
				loader->loadModel(obj);
				activeScene->registerObject(obj, true);
				obj->id = newObjectID++;
				//obj->sc = vec3(0.1f);
			}

			if (ImGui::Button("Mitre"))
			{
				Loader *loader = new Loader("mitre.dae");
				Mesh *mesh = new Mesh();

				Object *obj = new Object(3);
				loader->loadModel(obj);
				activeScene->registerObject(obj, true);
				obj->id = newObjectID++;
				//obj->sc = vec3(0.1f);
			}

			if (ImGui::Button("RAM"))
			{
				Loader *loader = new Loader("RAM.dae");
				Mesh *mesh = new Mesh();
				Object *obj = new Object(2);
				loader->loadModel(obj);
				activeScene->registerObject(obj, true);
				obj->id = newObjectID++;
				//obj->sc = vec3(0.1f);
			}


			if (ImGui::Button("VuMark"))
			{
				Loader *loader = new Loader("plane.dae");
				Mesh *mesh = new Mesh();
				loader->populatePos(mesh, "Plane");
				Texture2D *tex = new Texture2D("Textures/VuMark.png");
				
				if (tex->texLoaded)
					mesh->tex = tex;
				
				Object *obj = new Object(0);
				obj->meshes.emplace_back(mesh);
				activeScene->registerObject(obj, true);
				obj->id = newObjectID++;
			}
		}else
		{
			ImGui::Text("Please select or create a step");
		}
	}
	if (ImGui::CollapsingHeader("Selected Object")) {
		Object *selectedObject = activeScene->getSelectedObject();
		if (selectedObject) {

			if(!selectedObjectNameBuffer[0])
				memcpy(selectedObjectNameBuffer, selectedObject->name.c_str(), 1024);

			if(ImGui::InputText("Name", selectedObjectNameBuffer, 1024))
			{
				selectedObject->name = std::string(selectedObjectNameBuffer);
				free(selectedObjectNameBuffer);
				this->selectedObjectNameBuffer = (char*)calloc(1024, sizeof(char));
			}

			float pos[3] = { selectedObject->pos.x, selectedObject->pos.y, selectedObject->pos.z };
			float rot[3] = { selectedObject->rot.x, selectedObject->rot.y, selectedObject->rot.z };
			float sc[3] = { selectedObject->sc.x, selectedObject->sc.y, selectedObject->sc.z };
			float angle = selectedObject->angle;
			angle = glm::radians(angle);
			if (ImGui::InputFloat3("Position", pos))
			{
				selectedObject->pos.x = pos[0];
				selectedObject->pos.y = pos[1];
				selectedObject->pos.z = pos[2];
				selectedObject->updateMMat();
			}
			if (ImGui::InputFloat3("Rotation", rot))
			{
				selectedObject->rot.x = rot[0];
				selectedObject->rot.y = rot[1];
				selectedObject->rot.z = rot[2];
				selectedObject->updateMMat();
			}
			if (ImGui::InputFloat3("Scale", sc))
			{
				selectedObject->sc.x = sc[0];
				selectedObject->sc.y = sc[1];
				selectedObject->sc.z = sc[2];
				selectedObject->updateMMat();
			}
			if (ImGui::SliderAngle("Angle", &angle))
			{
				selectedObject->angle = (angle) * 360.0f / (2 * 3.14);;
				selectedObject->updateMMat();
			}
			
			if(selectedObject->type == 0)
			{
				//if it's a vumark set it's lable
				
				memcpy(vumarkLableBuff, (void*)selectedObject->lable.c_str(), 1024);

				ImGui::InputText("VuMark Lable", vumarkLableBuff, 1024);

				selectedObject->lable = std::string(vumarkLableBuff);

				this->vumarkLableBuff = (char*)calloc(1024, sizeof(char));

			}

			if (ImGui::Button("Delete"))
				activeScene->deleteObject(selectedObject);
			ImGui::SameLine();
			if (ImGui::Button("Set Active Components"))
			{
				this->componentSelectionWindowActive = true;
				componentActive.resize(0);
			}
		}
	}

	if (ImGui::CollapsingHeader("Background"))
	{
		char *items[] = { "4:3", "16:9" };

		ImGui::InputText("Path", backgroundPathBuffer, 1024);
		ImGui::Combo("Image Format", &activeItem, items, 2, -1);
		if (ImGui::Button("Load"))
		{
			activeScene->loadBase(backgroundPathBuffer, activeItem);
		}

		if (activeScene->selectedStep && activeScene->selectedStep->selectedBackground)
		{
			if (ImGui::Button("Auto Scale"))
			{
				activeScene->autoScaleActive = true;
			}
		}
	}
	if (ImGui::CollapsingHeader("Component Manager"))
	{
		int count;
		if (ImGui::Button("New Component"))
		{
			if (tempRotatingComponent)
			{
				tempRotatingComponent = nullptr;
			}

			if (tempPulsatingComponent) {
				tempRotatingComponent = nullptr;
			}

			if(tempLinearMovementComponent)
			{
				tempLinearMovementComponent = nullptr;
			}

			newComponentWindowActive = true;
		}
	}

	if (ImGui::CollapsingHeader("Steps"))
	{

		std::vector<Step *> steps = activeScene->getSteps();

		int numSteps = steps.size();

		const char **stepCombo = new const char*[numSteps + 1];

		const char * c = "[New Step]";
		

		int i = 0;
		for (; i < numSteps; i++)
		{
			const char * c = steps[i]->name.c_str();
			stepCombo[i] = c;
		}		
		stepCombo[i] = c;

		if (ImGui::Combo("Steps", &stepComboItem, stepCombo, numSteps + 1))
			if (stepComboItem != numSteps)
				activeScene->selectedStep = steps[stepComboItem];
			else
				activeScene->selectedStep = nullptr;

		if (ImGui::Button("Edit Step"))
		{
			this->stepWindowActive = true;
			if (!strcmp("[New Step]", stepCombo[stepComboItem]))
				this->activeStep = nullptr;
			else
				this->activeStep = steps[stepComboItem];
		}
		/*ImGui::SameLine();
		if (ImGui::Button("Add objects to steps"))
		{
			stepAssignmentWindowActive = true;
		}*/
	}


	if (ImGui::Button("Import"))
		this->importerWindowActive = true;
	ImGui::SameLine();

	if (ImGui::Button("Export"))
		activeScene->exportToJson();

	size = ImGui::GetWindowSize();
	pos = ImGui::GetWindowPos();
	UICoordMap.insert_or_assign("ARA", std::make_pair(pos, size));

	ImGui::End();
	this->size = ImGui::GetWindowSize();
	this->pos = ImGui::GetWindowPos();

	if (activeScene->autoScaleActive)
	{
		ImGui::Begin("Auto Scale", &activeScene->autoScaleActive);


		size = ImGui::GetWindowSize();
		pos = ImGui::GetWindowPos();
		UICoordMap.insert_or_assign("Auto Scale", std::make_pair(pos, size));


		if (ImGui::Button("Point 1"))
			activeScene->activePointer = 1;

		ImGui::SameLine();

		if (ImGui::Button("Point 2"))
			activeScene->activePointer = 2;

		ImGui::InputFloat("Expected Size", &expectedSize);
		if (ImGui::Button("Scale"))
		{
			activeScene->scaleBackground(expectedSize);
			activeScene->activePointer = 0;
			activeScene->autoScaleActive = false;
		}


		ImGui::End();
	}


	if (newComponentWindowActive)
	{

		ImGui::Begin("Component Manager", &newComponentWindowActive);

		size = ImGui::GetWindowSize();
		pos = ImGui::GetWindowPos();
		UICoordMap.insert_or_assign("Component Manager", std::make_pair(pos, size));

		int count;

		ImGui::InputText("Component Name", componentNameBuff, 1024);

		char *componentTypes[] = { "Rotating", "Pulsating", "Movement" };
		ImGui::Combo("Component Types", &currentComponentTypeIndex, componentTypes, 3, -1);

		std::string type = componentTypes[currentComponentTypeIndex];

		if (type == "Rotating")
		{
			if (!tempRotatingComponent)
				tempRotatingComponent = new RotatingComponent();

			selectedComponent = tempRotatingComponent;

			float rot[3] = { tempRotatingComponent->rot.x, tempRotatingComponent->rot.y, tempRotatingComponent->rot.z };

			if (ImGui::InputFloat3("Rotation Axis", rot))
			{
				tempRotatingComponent->rot.x = rot[0];
				tempRotatingComponent->rot.y = rot[1];
				tempRotatingComponent->rot.z = rot[2];
			}

			float angle = tempRotatingComponent->anglePerTick;

			if (ImGui::InputFloat("Angle Per Tick", &angle))
			{
				tempRotatingComponent->anglePerTick = angle;
			}
		}
		else if (type == "Pulsating")
		{
			if (!tempPulsatingComponent)
				tempPulsatingComponent = new PulsatingComponent();

			selectedComponent = tempPulsatingComponent;

			float scale[3] = { tempPulsatingComponent->scale.x, tempPulsatingComponent->scale.y, tempPulsatingComponent->scale.z };

			if (ImGui::InputFloat3("Scale Axis", scale))
			{
				tempPulsatingComponent->scale.x = scale[0];
				tempPulsatingComponent->scale.y = scale[1];
				tempPulsatingComponent->scale.z = scale[2];
			}

			float scales[2] = { tempPulsatingComponent->minScale, tempPulsatingComponent->maxScale };

			if (ImGui::InputFloat2("Min/Max Scales", scales))
			{
				tempPulsatingComponent->minScale = scales[0];
				tempPulsatingComponent->maxScale = scales[1];
			}

			float angle = tempPulsatingComponent->pulsesPerSecond;

			if (ImGui::InputFloat("Scale Per Tick", &angle))
			{
				tempPulsatingComponent->pulsesPerSecond = angle;
			}



		}
		else if (type == "Movement")
		{
			if (!tempLinearMovementComponent)
				tempLinearMovementComponent = new LinearMovementComponent();

			selectedComponent = tempLinearMovementComponent;
			
			float axis[3] = { tempLinearMovementComponent->axis.x, tempLinearMovementComponent->axis.y, tempLinearMovementComponent->axis.z };

			if (ImGui::InputFloat3("Movement Axis", axis))
			{
				tempLinearMovementComponent->axis.x = axis[0];
				tempLinearMovementComponent->axis.y = axis[1];
				tempLinearMovementComponent->axis.z = axis[2];
			}

			float maxDist = tempLinearMovementComponent->maxDist;

			if(ImGui::InputFloat("Maximum Distance", &maxDist))
			{
				tempLinearMovementComponent->maxDist = maxDist;
			}

			float distPerTick = tempLinearMovementComponent->distPerTick;

			if (ImGui::InputFloat("Speed", &distPerTick))
			{
				tempLinearMovementComponent->distPerTick = distPerTick;
			}
		}

		

		if (ImGui::Button("Save Component"))
		{
			selectedComponent->name = std::string(componentNameBuff);
			activeScene->registerComponent(selectedComponent);
			newComponentWindowActive = false;
			//componentNameBuff = "";
			//selectedComponent = nullptr;
		}


		ImGui::End();


	}


	if (componentSelectionWindowActive)
	{
		ImGui::Begin("Component Selection", &componentSelectionWindowActive);


		size = ImGui::GetWindowSize();
		pos = ImGui::GetWindowPos();
		UICoordMap.insert_or_assign("Component Selection", std::make_pair(pos, size));

		std::vector<Component *> components = activeScene->getComponents();
		if (componentActive.size() == 0) {
			componentActive.resize(components.size());
			Object *selectedObject = activeScene->getSelectedObject();

			for (int i = 0; i < components.size(); i++)
			{
				if (std::find(selectedObject->components->begin(), selectedObject->components->end(), components[i]) != selectedObject->components->end())
				{
					componentActive[i] = true;
				}
			}

		}
		for(int i = 0; i < components.size(); i++)
		{
			auto comp = components[i];
			bool active = componentActive[i];
			ImGui::Text(comp->name.c_str());
			ImGui::SameLine(150);
			if(ImGui::Checkbox(std::string("Active##" + std::to_string(i)).c_str(), &active))
			{
				componentActive[i] = active;
			}
		}

		if(ImGui::Button("Save Changes"))
		{
			Object *selectedObject = activeScene->getSelectedObject();
			selectedObject->resetComponents();
			for (int i = 0; i < components.size(); i++)
			{
				if(componentActive[i])
				{
					selectedObject->addComponent(components[i]);
				}
			}
			componentSelectionWindowActive = false;
		}

		ImGui::End();

	}
	/*
	if (stepAssignmentWindowActive)
	{
		
		ImGui::Begin("Step Selection", &stepAssignmentWindowActive);

		
		size = ImGui::GetWindowSize();
		pos = ImGui::GetWindowPos();
		UICoordMap.insert_or_assign("Component Selection", std::make_pair(pos, size));

		std::vector<Object *> objects = activeScene->getObjects();
		
		for (int i = 0; i < objects.size(); i++)
		{
			auto obj = objects[i];
			ImGui::Text(obj->name.c_str());
			ImGui::SameLine(150);
			if (ImGui::InputInt(std::string("Step##" + std::to_string(i)).c_str(), &obj->order))
			{
				int asd = 1;
			}
		}

		ImGui::End();

	}*/

	if (stepWindowActive)
	{
		ImGui::Begin("Step Manager", &stepWindowActive);


		size = ImGui::GetWindowSize();
		pos = ImGui::GetWindowPos();
		UICoordMap.insert_or_assign("Step Manager", std::make_pair(pos, size));
		
		if (!cardNameBuff[0] && activeStep)
		{

			memcpy(cardNameBuff, activeStep->name.c_str(), 1024);
			memcpy(cardTitleBuff, activeStep->cardTitle.c_str(), 1024);
			memcpy(cardInsBuff, activeStep->instructions.c_str(), 1024);
		}

		ImGui::InputText("Step name", cardNameBuff, 1024);
		ImGui::InputText("Card title", cardTitleBuff, 1024);
		ImGui::InputText("Card intructions", cardInsBuff, 1024);
		ImGui::InputInt("Order", &stepOrder);

		if (ImGui::Button("Save"))
		{
			if (!activeStep)
			{
				activeStep = new Step();
				activeScene->addStep(activeStep);
				activeScene->selectedStep = activeStep;
			}

			activeStep->name = std::string(cardNameBuff);
			activeStep->cardTitle = std::string(cardTitleBuff);
			activeStep->instructions = std::string(cardInsBuff);
			activeStep->order = stepOrder;

			//Clear the buffers
			free(cardNameBuff);
			free(cardTitleBuff);
			free(cardInsBuff);
			this->cardNameBuff = (char*)calloc(1024, sizeof(char));
			this->cardTitleBuff = (char*)calloc(1024, sizeof(char));
			this->cardInsBuff = (char*)calloc(1024, sizeof(char));
		}



		ImGui::End();

	}

	if (this->importerWindowActive)
	{
		ImGui::Begin("Importer", &this->importerWindowActive);

		ImGui::InputText("Path", importerPathBuffer, 1024);

		if(ImGui::Button("Import"))
		{
			activeScene->importJSON(importerPathBuffer);

			//std::thread t(&Scene::importJSON, activeScene, importerPathBuffer);
			
		}

		ImGui::End();
	}

	ImGui::Render();
}

void UI::setActiveScene(Scene* scene)
{
	this->activeScene = scene;
}

void UI::setActiveCam(Camera* cam)
{
	this->activeCam = cam;
}

bool UI::isCursorInUi(double x, double y)
{
	for (std::map<std::string, std::pair<ImVec2, ImVec2>>::iterator it = UICoordMap.begin(); it != UICoordMap.end(); ++it)
	{
		auto pair = it->second;

		if (x > pair.first.x && x < pair.first.x + pair.second.x && y > pair.first.y && y < pair.first.y + pair.second.y)
			return true;

	}

	return false;
}
