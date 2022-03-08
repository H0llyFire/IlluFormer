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
	float jumpVelocity;
	unsigned int tick;

	float velocity[4]{};
	float overrideVelocity[4]{};
	EntityType typeName;
	Direction facing;
	DrawnObject* sprite;


public:
	bool isMoving;
	bool isMidJump;
	bool isMovingInDirection[4]{};
	bool isBlockedInDirection[4]{};

	Entity(EntityType type, DrawnObject* object);
	~Entity();
	bool PollEntityEvents();
	bool MoveEntity();
	bool MoveEntity(Direction direction);

	bool ChangeVelocity(float speed, Direction direction);
	bool SetVelocity(float speed, Direction direction);
	int GetCollisionIndex(float posX, float posY, int direction, int directionCorner);
	bool CheckBlock(int direction, int blockIndex, float position);
	void OverrideVelocity();
	bool CheckCollisions();

	void Jump();
	void SetGravity();
	void SetGravity(float modifier);
	void SetJumpVelocity();
	void SetCollision(int i, int ptA, int ptB, int* afterMoveCords);
	void PrintStatus();

	bool UnloadEntity(const int index);

	static std::vector<Entity*> entityList;

	static bool PollEntitiesEvents();
	static int CreateEntity(EntityType type, DrawnObject* object);
	static bool UnloadAll();
};