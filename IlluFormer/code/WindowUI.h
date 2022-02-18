#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class WindowUI
{
private:
	static GLFWwindow* window;
public:
	static GLFWwindow* GetWindowPtr();
	static bool CreateWindow(unsigned int width, unsigned int height, unsigned int pollingRate);
};