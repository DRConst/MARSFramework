#pragma once
#include "GL/GLWindow.h"
#include "gl/Camera.h"
#include "UI.h"

class InputHandler
{
public:
	InputHandler(GLWindow* window, Camera* cam);
	void getMousePos(double* x, double* y);
	void resetCam(double x, double y);
	~InputHandler();
	int breakPoint();
	void resetMousePos(float x, float y);
	void registerUI(UI* ui2);
	GLWindow* window;
	bool isCamTiltDeltaActive = false;
	double oldX, oldY, oldScroll = 0;
	Camera* cam;
	bool isCamPosDeltaActive = false;
	bool isCamZoomDeltaActive = false;
	int isDragging = 0;
	int pickingPendent = 0;
};

