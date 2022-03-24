#pragma once
#include "../Graphics/DrawnObject.h"
#include "Consts.h"

class Entity
{
private:
	int health;
	float maxSpeed;
	float minSpeed;
	float jumpVelocity;
	float overrideVelocity[4]{};
	float defaultPosition[16]{};
protected:
	bool isOnGround;
	unsigned int jumpTick;
	unsigned int tick;
	float velocity[4]{};
	EntityType typeName;
	Direction facing;
	DrawnObject* sprite;
public:
	int ownerIndex;
	bool isMoving;
	bool isJumping;
	bool isMidJump;
	bool isMovingInDirection[4]{};

	Entity(EntityType type, DrawnObject* object, int levelIndex);
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
	bool CheckEntityCollisions();
	bool CheckOutOfBounds() const;

	void ResetPosition();
	void Jump();
	void SetGravity();
	void SetGravity(float modifier);
	void SetJumpVelocity();
	void PrintStatus();

	bool UnloadEntity(int index);

	//static std::vector<Entity*> entityList;

	static bool PollEntitiesEvents();
	static int CreateEntity(EntityType type, DrawnObject* object, int levelIndex);
	static bool UnloadAll();
};
