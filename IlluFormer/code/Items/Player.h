#pragma once
#include "Entity.h"
#include "../LevelLoad.h"

class Player : public Entity
{
private:
	static Player* player;
	int collectedCoins;
	int animationFrame;
	int animationTick;
public:
	Player(DrawnObject* object, int levelIndex);
	~Player();
	bool PollPlayerEvents();
	void StartMovement(Direction direction);
	void StopMovement(Direction direction);
	void CollectCoin(DrawnObject* object);
	void ProcessAnimation();

	static Player* GetPlayerPtr();
};
