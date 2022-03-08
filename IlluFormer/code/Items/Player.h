#pragma once
#include "Entity.h"

class Player : public Entity
{
private:
	static Player* player;

public:
	Player(DrawnObject* object);
	~Player();
	bool PollPlayerEvents();
	void StartMovement(Direction direction);
	void StopMovement(Direction direction);

	static Player* GetPlayerPtr();
};