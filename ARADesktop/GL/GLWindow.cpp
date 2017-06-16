#include <stdafx.h>
#include "GLWindow.h"

GLWindow *callbackWindowPtr;


void window_size_callback(GLFWwindow* window, int width, int height)
{
	callbackWindowPtr->width = width;
	callbackWindowPtr->height = height;

	glViewport(0, 0, width, height);
}

GLWindow::GLWindow(int width = 800, int height = 600)
{
	glfwInit();

	//glfwWindowHint(GLFW_DECORATED, 0);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Atmospheric Scattering", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		printf("gladLoadGL went wrong!\n");
		exit(-1);
	}
	else {
		printf("%s\n", glGetString(GL_VERSION));
	}

	this->width = width;
	this->height = height;
	callbackWindowPtr = this;

	glfwSetWindowSizeCallback(window, window_size_callback);
}


GLWindow::~GLWindow()
{
}

bool GLWindow::shouldClose()
{
	return glfwWindowShouldClose(window);
}
