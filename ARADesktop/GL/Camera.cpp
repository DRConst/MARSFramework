#include <stdafx.h>
#include "Camera.h"
//

Camera::Camera(glm::vec3 pos, glm::vec3 center, glm::vec3 up)
{
	this->pos = pos;
	this->center = center;
	this->up = glm::normalize(up);
	this->right = cross(up, normalize(pos - center));
	this->direction = center - pos;

	this->view = glm::lookAt(pos, center, up);
	xpos = 400;
	ypos = 300;
	horizontalAngle = 3.14;
}

void Camera::registerProj(float fov, float nearP, float farP, float aspect, glm::mat4 proj)
{
	this->farP = farP;
	this->nearP = nearP;
	this->fov = fov;
	this->aspect = aspect;
	this->proj = proj;
}


void Camera::handleRotation(float delta)
{
	double horizontalDelta = 0.05 * delta * float(800 / 2 - xpos);
	double verticalDelta = 0.05 * delta * float(600 / 2 - ypos);

	handleRotation(horizontalDelta, verticalDelta);
	
}

void Camera::handleMove(vec3 newPos)
{
	this->pos = newPos;
	this->right = cross(up, normalize(pos - center));
	this->direction = center - pos;
	this->view = glm::lookAt(pos, center, up);
}

void Camera::handleRotation(double horizontalDelta, double verticalDelta)
{
	verticalAngle += verticalDelta;
	horizontalAngle += horizontalDelta;

	this->direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	this->right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	this->up = glm::cross(right, direction);

	this->view = glm::lookAt(pos, pos + direction, up);
}

void Camera::handlePan(double horizontalDelta, double verticalDelta)
{
	vec3 hDelta = right * vec3(horizontalDelta);
	vec3 vDelta = up * vec3(verticalDelta);
	this->pos += hDelta - vDelta;
	this->view = glm::lookAt(pos, pos + direction, up);
}

void Camera::handleZoom(double delta)
{
	vec3 d = direction * vec3(delta);
	this->pos += d;
	this->view = glm::lookAt(pos, pos + direction, up);
}

Camera::~Camera()
{
}
