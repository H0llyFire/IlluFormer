#pragma once
#include <string>
#include <vector>
#include "Graphics/Texture.h"
#include "Items/Consts.h"

class Level
{
private:
	std::vector<Sprite*> sprites;
	Sprite background;
	
	
public:
	Level(std::string filepath);
	bool Load(const Shader& shader);
	bool Render();
};