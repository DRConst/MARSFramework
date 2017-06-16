#include "stdafx.h"
#include "InputHandler.h"
#include "GLFW/glfw3.h"
#include "GL/GLWindow.h"
#include "gl/Camera.h"
#include "imgui.h"
#include "UI.h"


InputHandler *inputHandler;
UI *ui;

InputHandler::InputHandler(GLWindow *window, Camera *cam)
{
	this->window = window;
	this->cam = cam;
	oldX = 0;
	oldY = 0;
	inputHandler = this;
	glfwSetCursorPosCallback(window->window, 
		[](GLFWwindow *window, double x, double y)
		{
			double deltaX = inputHandler->oldX - x;
			double deltaY = inputHandler->oldY - y;
			if(inputHandler->isCamTiltDeltaActive)
			{				
				inputHandler->cam->handleRotation(deltaX * 0.001, deltaY * 0.001);
			}else if(inputHandler->isCamPosDeltaActive)
			{
				inputHandler->cam->handlePan(deltaX * 0.01, deltaY * 0.01);
			}
			else if (inputHandler->isCamZoomDeltaActive)
			{
				inputHandler->cam->handleZoom(deltaY * 0.01);
			}

			inputHandler->oldX = x;
			inputHandler->oldY = y;
		}
	);

	glfwSetMouseButtonCallback(window->window, [](GLFWwindow* window, int button, int action, int mods)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) 
		{			
			inputHandler->resetCam(x, y);
			inputHandler->isCamTiltDeltaActive = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) 
		{
			inputHandler->isCamTiltDeltaActive = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE && mods == GLFW_MOD_SHIFT && action == GLFW_PRESS)
		{
			inputHandler->isCamZoomDeltaActive = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE && mods == GLFW_MOD_SHIFT && action == GLFW_RELEASE)
		{
			inputHandler->isCamZoomDeltaActive = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		{
			inputHandler->resetCam(x, y);
			inputHandler->isCamPosDeltaActive = true;			
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		{
			inputHandler->isCamPosDeltaActive = false;
		}		
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			if (ui->isCursorInUi(x, y))
				return;
			inputHandler->pickingPendent = true;
			inputHandler->isDragging = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			inputHandler->isDragging = false;
		}
	});

	glfwSetScrollCallback(window->window, [](GLFWwindow* window, double xoffset, double yoffset)
	{
		double delta = yoffset - inputHandler->oldScroll;

		inputHandler->cam->handleZoom(delta*0.1);

		inputHandler->oldScroll = delta;
	});
	
}

void InputHandler::getMousePos(double *x, double *y)
{
	glfwGetCursorPos(window->window, x, y);
}

int InputHandler::breakPoint()
{
	return glfwGetKey(window->window, GLFW_KEY_SPACE);
}

void InputHandler::resetMousePos(float x, float y)
{
	glfwSetCursorPos(window->window, x, y);
}

void InputHandler::registerUI(UI *ui2)
{
	ui = ui2;
}
void InputHandler::resetCam(double x, double y)
{
	this->oldX = x;
	this->oldY = y;
	//this->cam->xpos = x;
	//this->cam->ypos = y;
}
InputHandler::~InputHandler()
{
}
