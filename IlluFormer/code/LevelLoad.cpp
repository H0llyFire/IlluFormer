#include "LevelLoad.h"
#include <fstream>

#include "Graphics/Coordinating.h"
#include "Graphics/Texture.h"
#include "Items/Player.h"

Level::Level(std::string filepath)
	:objectAmount(0), index(levelAmount), isInitialized(false)
{
	levelList.push_back(this);
	levelAmount++;
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

Level::~Level()
{
	levelAmount--;
	for (auto entity : entities)
		delete entity;
	entities.clear();
	
	for (auto sprite : sprites)
		delete sprite;
	sprites.clear();

	for (auto object : objects)
		delete object;
	objects.clear();
}

bool Level::Load(Shader* shader)
{
	isInitialized = true;
	objects.push_back(new DrawnObject(new UniformPosition{1,1}, Coordinates::SnapToBgGrid(background.x, background.y), 16, Coordinates::GetGridIndices(), 6, background.texture, shader, objectAmount));
	objectAmount++;
	for (Sprite* sprite : sprites)
	{
		objects.push_back(new DrawnObject(new UniformPosition{sprite->x, sprite->y}, Coordinates::SnapToGrid(sprite->x, sprite->y), 16, Coordinates::GetGridIndices(), 6, sprite->texture, shader, objectAmount));
		if (sprite->texture == TextureList::textures[TEXTURE_ENEMY])
			entities.push_back(new Entity(EntityType::GHOST, objects.back(), index));
		else if (sprite->texture == TextureList::textures[TEXTURE_PLAYER])
			auto player = new Player(objects.back(), index);
		objectAmount++;

	}
	return false;
}

bool Level::Render() const
{
	for (auto object : objects)
	{
		object->DrawObject();
	}
	return false;
}

void Level::ResetLevel()
{
	for (auto entity : entities)
	{
		entity->ResetPosition();
	}
	Player::GetPlayerPtr()->ResetPosition();

}

void Level::InitLevels(Shader* shader)
{
	for (auto lvl : levelList)
	{
		if (!lvl->isInitialized)
			lvl->Load(shader);
	}	
}

void Level::SetActiveLevel(Level* level)
{
	activeLevel = level;
}
void Level::SetActiveLevel(int levelIndex)
{
	activeLevel = levelList[levelIndex];
}

void Level::UnloadAllLevels()
{
	for (auto level : levelList)
	{
		delete level;
	}
	levelList.clear();
}

void Menu::InitMenus(Shader* shader)
{
	const auto positions = new float[16]
	{
		-5.0f, 0.5f, 0.0f, 0.0f,
		 5.0f, 0.5f, 1.0f, 0.0f,
		 5.0f, 2.5f, 1.0f, 1.0f,
		-5.0f, 2.5f, 0.0f, 1.0f
	};

	mainMenuObjects.push_back(new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::SnapToBgGrid(16, 9), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_BG_MENU], shader, 0));
	mainMenuObjects.push_back(new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, 0.0f),	  16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_START_BUTTON],			shader, 0));
	mainMenuObjects.push_back(new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, 0.0f),	  16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_START_BUTTON_HOVER],	shader, 0));
	mainMenuObjects.push_back(new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, -3.0f), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_EXIT_BUTTON],			shader, 0));
	mainMenuObjects.push_back(new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, -3.0f), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_EXIT_BUTTON_HOVER],	shader, 0));

	pauseMenuObjects.push_back(	  new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::SnapToBgGrid(16, 9), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_DARKEN_BG], shader, 0));
	pauseMenuObjects.push_back(	  new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, 0.0f),    16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_RESTART_BUTTON],			shader, 0));
	pauseMenuObjects.push_back(	  new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, 0.0f),    16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_RESTART_BUTTON_HOVER],	shader, 0));
	pauseMenuObjects.push_back(	  new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, -3.0f), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_MENU_BUTTON],		shader, 0));
	pauseMenuObjects.push_back(	  new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, -3.0f), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_MENU_BUTTON_HOVER],	shader, 0));

	endMenuObjects.push_back(new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::SnapToBgGrid(16, 9), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_DARKEN_BG], shader, 0));
	endMenuObjects.push_back(new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, -3.0f), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_MENU_BUTTON], shader, 0));
	endMenuObjects.push_back(new DrawnObject(new UniformPosition{ 1,1 }, Coordinates::AddToCoords(positions, 0.0f, -3.0f), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_MENU_BUTTON_HOVER], shader, 0));
	delete[] positions;

	const auto positionsText = new float[16]
	{
		-7.5f, 0.5f, 0.0f, 0.0f,
		 7.5f, 0.5f, 1.0f, 0.0f,
		 7.5f, 3.5f, 1.0f, 1.0f,
		-7.5f, 3.5f, 0.0f, 1.0f
	};
	endMenuObjects.push_back(new DrawnObject(new UniformPosition{ 1,1 }, positionsText, 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_TEXT_GJ], shader, 0));
}

void Menu::ShowMainMenu(double cursorX, double cursorY)
{
	mainMenuObjects[0]->DrawObject();
	mainMenuObjects[1]->DrawObject();
	mainMenuObjects[3]->DrawObject();

	double* tempPosition = Coordinates::GetResolutionCoordinates( mainMenuObjects[1]->GetPosition());
	if (cursorX > tempPosition[0] && cursorX < tempPosition[2] && cursorY < tempPosition[1] && cursorY > tempPosition[5])
		mainMenuObjects[2]->DrawObject();
	delete[] tempPosition;

	tempPosition = Coordinates::GetResolutionCoordinates(mainMenuObjects[3]->GetPosition());
	if (cursorX > tempPosition[0] && cursorX < tempPosition[2] && cursorY < tempPosition[1] && cursorY > tempPosition[5])
		mainMenuObjects[4]->DrawObject();
	delete[] tempPosition;
}

void Menu::ShowPauseMenu(double cursorX, double cursorY)
{
	pauseMenuObjects[0]->DrawObject();
	pauseMenuObjects[1]->DrawObject();
	pauseMenuObjects[3]->DrawObject();

	double* tempPosition = Coordinates::GetResolutionCoordinates(pauseMenuObjects[1]->GetPosition());
	if (cursorX > tempPosition[0] && cursorX < tempPosition[2] && cursorY < tempPosition[1] && cursorY > tempPosition[5])
		pauseMenuObjects[2]->DrawObject();
	delete[] tempPosition;

	tempPosition = Coordinates::GetResolutionCoordinates(pauseMenuObjects[3]->GetPosition());
	if (cursorX > tempPosition[0] && cursorX < tempPosition[2] && cursorY < tempPosition[1] && cursorY > tempPosition[5])
		pauseMenuObjects[4]->DrawObject();
	delete[] tempPosition;
}

void Menu::ShowEndMenu(double cursorX, double cursorY)
{
	endMenuObjects[0]->DrawObject();
	endMenuObjects[1]->DrawObject();
	endMenuObjects[3]->DrawObject();

	double* tempPosition = Coordinates::GetResolutionCoordinates(endMenuObjects[1]->GetPosition());
	if (cursorX > tempPosition[0] && cursorX < tempPosition[2] && cursorY < tempPosition[1] && cursorY > tempPosition[5])
		endMenuObjects[2]->DrawObject();
	delete[] tempPosition;
}

bool Menu::ProcessClick(double cursorX, double cursorY)
{
	bool shouldClose = false;
	double* tempPosition = Coordinates::GetResolutionCoordinates(mainMenuObjects[1]->GetPosition());
	if (cursorX > tempPosition[0] && cursorX < tempPosition[2] && cursorY < tempPosition[1] && cursorY > tempPosition[5])
	{
		if (Level::GetActiveLevel() == nullptr) //Start Level
		{
			Level* one = new Level("0.level");

			Level::SetActiveLevel(one);
			isPauseMenuActive = false;
			isEndMenuActive = false;
		}
		else if (isPauseMenuActive) //TODO Restart Level
		{
			Level::GetActiveLevel()->ResetLevel();
			isPauseMenuActive = false;
		}

	}
	delete[] tempPosition;

	tempPosition = Coordinates::GetResolutionCoordinates(mainMenuObjects[3]->GetPosition());
	if (cursorX > tempPosition[0] && cursorX < tempPosition[2] && cursorY < tempPosition[1] && cursorY > tempPosition[5])
	{
		if (Level::GetActiveLevel() == nullptr) //Exit Game
		{
			shouldClose = true;
		}
		else if (isPauseMenuActive || isEndMenuActive) //Exit Level To Menu
		{
			Level::SetActiveLevel(nullptr);
			for (auto lvl : Level::levelList)
			{
				delete lvl;
			}
			Level::levelList.clear();
		}
	}
	delete[] tempPosition;

	return shouldClose;
}

void Menu::UnloadMenus()
{
	for (auto object : mainMenuObjects)
		delete object;
	for (auto object : pauseMenuObjects)
		delete object;
	for (auto object : endMenuObjects)
		delete object;
}

bool Menu::isPauseMenuActive{ false };
bool Menu::isEndMenuActive{ false };
int Level::levelAmount{ 0 };
Level* Level::activeLevel{ nullptr };
std::vector<DrawnObject*> Menu::mainMenuObjects;
std::vector<DrawnObject*> Menu::pauseMenuObjects;
std::vector<DrawnObject*> Menu::endMenuObjects;
std::vector<Level*> Level::levelList;