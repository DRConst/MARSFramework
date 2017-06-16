#pragma once
class GLWindow
{
public:
	GLWindow(int width, int height);
	~GLWindow();

	bool shouldClose();
	int width, height;

	GLFWwindow *window;
};

