#pragma once
#include "glad/glad.h"
#include <glfw3.h>
#include <iostream>

class Window
{
private:
	GLFWwindow* window;

public:
	Window(GLuint SCREEN_WIDTH, GLuint SCREEN_HEIGHT, GLushort MAJOR, GLushort MINOR, const char* name, bool vSync)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, name, NULL, NULL);
		if (!window)
		{
			std::cout << "Failed to create the GLFW window\n";
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(vSync);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize OpenGL context" << std::endl;
		}
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	~Window() { glfwDestroyWindow(window); }

	GLFWwindow* getWindow() { return window; }
};