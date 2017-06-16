#pragma once
#include <stdafx.h>
using namespace glm;

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up);
	
	~Camera();


	void registerProj(float fov, float nearP, float farP, float aspect, glm::mat4 proj);
	void handleRotation(float delta);
	void handleMove(vec3 newPos);
	glm::vec3 direction, right, up;
	void handleRotation(double horizontalAngle, double verticalAngle);
	void handlePan(double horizontalDelta, double verticalDelta);
	void handleZoom(double delta);
	void updateCursor(GLFWwindow* window, double xpos, double ypos);
	glm::vec3 pos, center;
	glm::mat4 view;
	glm::mat4 proj;
	float fov, nearP, farP, aspect;

	int xpos = 400;
	int ypos = 300;

	bool moved = false;

	float horizontalAngle = 0.0f;
	float verticalAngle = 0.0f;

	GLFWwindow *window;
};

