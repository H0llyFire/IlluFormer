#pragma once
#include <string>
#include <vector>

#include "Graphics/DrawnObject.h"
#include "Items/Consts.h"
#include "Items/Entity.h"

class Level
{
private:
	int objectAmount;
	bool isInitialized;
	Sprite background;
	std::vector<Sprite*> sprites;

	static int levelAmount;
	static Level* activeLevel;
public:
	int index;
	std::vector<DrawnObject*> objects;
	std::vector<Entity*> entities;
	static std::vector<Level*> levelList;

	Level(std::string filepath);
	~Level();
	bool Load(Shader* shader);
	bool Render() const;
	void ResetLevel();

	static void InitLevels(Shader* shader);
	static void SetActiveLevel(Level*);
	static void SetActiveLevel(int levelIndex);
	static Level* GetActiveLevel() { return activeLevel; }
	static void UnloadAllLevels();
};

class Menu
{
private:
	static std::vector<DrawnObject*> mainMenuObjects;
	static std::vector<DrawnObject*> pauseMenuObjects;
	static std::vector<DrawnObject*> endMenuObjects;

public:
	static bool isPauseMenuActive;
	static bool isEndMenuActive;
	static void InitMenus(Shader* shader);
	static void ShowMainMenu(double cursorX, double cursorY);
	static void ShowPauseMenu(double cursorX, double cursorY);
	static void ShowEndMenu(double cursorX, double cursorY);

	static bool ProcessClick(double cursorX, double cursorY);
	static void UnloadMenus();
};
