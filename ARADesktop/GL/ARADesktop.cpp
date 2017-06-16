// ARADesktop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GLWindow.h"
#include "UI.h"
#include "Shader.h"
#include "Program.h"
#include "Camera.h"
#include "Loader.h"
#include "Object.h"
#include "Scene.h"
#include "InputHandler.h"

Camera *callbackCam;


void openglCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam) {
	
	if(type != GL_DEBUG_TYPE_ERROR)
		return;

	std::cout << "---------------------opengl-callback-start------------" << std::endl;
	std::cout << "message: " << message << std::endl;
	std::cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER";
		break;
	}
	std::cout << std::endl;

	std::cout << "id: " << id << std::endl;
	std::cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "HIGH";
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "NOTIFICATION";
		break;
	}
	std::cout << std::endl;
	std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

int main()
{
	float windowWidth = 1920;
	float windowHeight = 1080;
	GLWindow *window = new GLWindow(windowWidth, windowHeight);


	if (glDebugMessageCallback) {
		std::cout << "Register OpenGL debug callback " << std::endl;
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			&unusedIds,
			true);
	}
	else
		std::cout << "glDebugMessageCallback not available" << std::endl;

	UI *ui = new UI(window);
	
	Shader *pickingVS = new Shader("./Shaders/pickingVS.glsl", GL_VERTEX_SHADER);
	Shader *pickingFS = new Shader("./Shaders/pickingFS.glsl", GL_FRAGMENT_SHADER);

	Program *pickingProgram = new Program();
	pickingProgram->bindVertexShader(pickingVS);
	pickingProgram->bindFragmentShader(pickingFS);
	pickingProgram->finalize();
	pickingProgram->bind();

	GLint pickingColourLoc = glGetUniformLocation(pickingProgram->programHandle, "pickingColour");
	Shader *vs = new Shader("./Shaders/simple_VS.glsl", GL_VERTEX_SHADER);
	Shader *fs = new Shader("./Shaders/simple_FS.glsl", GL_FRAGMENT_SHADER);

	Program *renderProgram = new Program();
	renderProgram->bindVertexShader(vs);
	renderProgram->bindFragmentShader(fs);
	renderProgram->finalize();
	renderProgram->bind();

	ui->renderProgram = renderProgram;

	

	Loader *loader = new Loader("cube.dae");
	Mesh *mesh = new Mesh();
	loader->populatePos(mesh, "Cube");

	Camera *cam = new Camera(glm::vec3(0.0f, 10.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	cam->verticalAngle = -45.0f;
	callbackCam = cam; //God damn glfw and it's C'ness
	cam->window = window->window;
	mat4 projMat = perspectiveFov(59.0f, windowWidth, windowHeight, 0.1f, 500.0f);
	cam->registerProj(59.0f, 0.1f, 500.0f, windowWidth / windowHeight, projMat);

	Scene *scene = new Scene();
	scene->window = window;
	scene->updateUnifromLocMap(renderProgram);
	scene->updateUnifromLocMap(pickingProgram);

	ui->setActiveScene(scene);
	ui->setActiveCam(cam);
	//scene->registerObject(obj, false);
	InputHandler *inputHandler = new InputHandler(window, cam);
	scene->registerInputHandler(inputHandler);
	inputHandler->registerUI(ui);

	Framebuffer *pickingFB = new Framebuffer(1920, 1080);


	double delta, oldTime = glfwGetTime();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	int showPickfb = 0;
	while (!window->shouldClose())
	{
		double current = glfwGetTime();
		delta = current - oldTime;
		oldTime = current;

		GLfloat grey[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat pick[] = { 0.2f, 0.0f, 0.2f, 1.0f };

		pickingProgram->bind();
		if(!showPickfb)
			pickingFB->bind(); 
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, pick);
				
		if(showPickfb)
			scene->pick(cam, pickingColourLoc, pickingProgram,delta);
		if(inputHandler->pickingPendent)
		{
			scene->pick(cam, pickingColourLoc, pickingProgram,delta);
			inputHandler->pickingPendent = 0;
		}
		
		
		if(!showPickfb){
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


		
		
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glClearBufferfv(GL_COLOR, 0, grey);
		
			//cam->handleRotation(delta);

		

			renderProgram->bind();

			//obj->render(cam);
			scene->render(cam, pickingFB, renderProgram, delta);

			ui->render();

			pickingFB->bind();
		}

		glfwSwapBuffers(window->window);

		glfwPollEvents();
	}

    return 0;
}

