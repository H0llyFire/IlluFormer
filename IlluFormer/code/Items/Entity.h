#pragma once
#include "../Graphics/DrawnObject.h"
#include "Consts.h"
#include <vector>

class Entity
{
private:
	bool isOnGround;
	int health;
	float maxSpeed;
	float minSpeed;
	float currentSpeedBonus;
	unsigned int tick;

	Velocity velocity;
	EntityType typeName;
	Direction facing;
	DrawnObject* sprite;


public:
	bool isMoving;
	bool isMovingRight;
	bool isMovingLeft;
	bool isMovingUp;
	bool isMovingDown;

	Entity(EntityType type, DrawnObject* object);
	~Entity();
	bool PollEntityEvents();
	bool MoveEntity();
	bool MoveEntity(Direction direction);
	bool ChangeVelocity(Velocity vel);
	bool SetVelocity(Velocity vel);
	bool CheckCollisions();

	bool UnloadEntity(const int index);

	static std::vector<Entity*> entityList;

	static bool PollEntitiesEvents();
	static int CreateEntity(EntityType type, DrawnObject* object);
	static bool UnloadAll();
};