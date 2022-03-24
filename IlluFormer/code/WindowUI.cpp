#include "WindowUI.h"
#include "Graphics/Logging.h"
//#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* WindowUI::window;


GLFWwindow* WindowUI::GetWindowPtr()
{
	return window;
}

bool WindowUI::CreateWindow(unsigned int width, unsigned int height, unsigned int pollingRate)
{
	if (!glfwInit())
		return true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Windown", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return true;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(pollingRate);
	return false;
}
