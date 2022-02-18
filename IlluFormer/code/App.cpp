#include <iostream>

#include "WindowUI.h"
#include "Loader.h"
#include "GameLoop.h"


int main()
{
	//Load Necessary Data
	Loader::LoadAll();
	//Create Game Loop
	GameLoop::StartLoop();
	return 0;
}