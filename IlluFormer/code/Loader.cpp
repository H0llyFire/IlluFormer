#include "Loader.h"
#include "Graphics/Logging.h"
#include "Graphics/DrawnObject.h"

bool Loader::LoadWindow()
{
	WindowUI::CreateWindow(1600, 900, 0);
	return false;
}

bool Loader::LoadAll()
{
	LoadWindow();
	LoadGlew();
	LoadSettings();
	LoadMainMenu();
	TextureList::GenerateTextures();
	return false;
}

bool Loader::LoadGlew()
{
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error with GLEW" << std::endl;
		return true;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
	return false;
}

bool Loader::LoadMainMenu()
{
	return false;
}

bool Loader::LoadSettings()
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	return false;
}
