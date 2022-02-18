#include "LevelLoad.h"
#include <fstream>

#include "Graphics/Coordinating.h"
#include "Graphics/DrawnObject.h"
#include "Graphics/Texture.h"

Level::Level(std::string filepath)
{
	std::ifstream data;
	std::vector<std::string> lines;
	data.open("res\\level\\"+filepath);

	if (data.is_open())
	{
		std::string line;
		while (std::getline(data, line))
		{
			lines.push_back(line);
		}
		data.close();
	}


	background.x = std::stoi(lines[0].substr(0, lines[0].find(';')));
	background.y = std::stoi(lines[0].substr(lines[0].find(';')+1));
	background.texture = TextureList::textures[TEXTURE_BG];

	lines.erase(lines.begin());
	if (lines[0] == "Premade")
	{
		lines.erase(lines.begin());
		for(std::string line : lines)
		{
			int x = stoi(line.substr(0, line.find(';')));
			int y = stoi(line.substr(line.find(';')+1, line.find_last_of(';')- line.find(';') + 1));
			Texture* tex = TextureList::EvalTextureName(line.substr(line.find_last_of(';')+1));
			sprites.push_back(new Sprite{x, y, tex}); // finish up
		}
	}

	
}

bool Level::Load(const Shader& shader)
{
	DrawnObject::CreateObject(new UniformPosition{1,1}, Coordinates::SnapToBgGrid(background.x, background.y), 16, Coordinates::GetGridIndices(), 6, background.texture, shader);
	for (Sprite* sprite : sprites)
	{
		DrawnObject::CreateObject(new UniformPosition{sprite->x, sprite->y}, Coordinates::SnapToGrid(sprite->x, sprite->y), 16, Coordinates::GetGridIndices(), 6, sprite->texture, shader);
	}
	return false;
}

bool Level::Render()
{
	for (auto object : DrawnObject::objects)
	{
		object->DrawObject();
	}
	return false;
}

