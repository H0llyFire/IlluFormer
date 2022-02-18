#pragma once
#include "../Graphics/Texture.h"

enum class Direction
{
	RIGHT = 0,
	LEFT = 1,
	UP = 2,
	DOWN = 3
};
enum class EntityType
{
	PLAYER = 0,
	GHOST = 1,
	BLOCK = 2
};

struct Position
{
	float x;
	float y;
};
struct UniformPosition
{
	int x;
	int y;
};
struct Velocity
{
	float speedX;
	float speedY;
};

struct Sprite
{
	int x;
	int y;
	Texture* texture;
};