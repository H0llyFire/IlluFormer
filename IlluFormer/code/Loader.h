#pragma once
#include "WindowUI.h"

class Loader
{
private:
	static bool LoadWindow();
	static bool LoadGlew();
	static bool LoadMainMenu();
	static bool LoadSettings();
public:
	static bool LoadAll();
};
