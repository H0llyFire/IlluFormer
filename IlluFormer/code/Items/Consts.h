#pragma once
#include "../Graphics/Texture.h"

enum Direction
{
	DOWN = 0,
	RIGHT = 1,
	UP = 2,
	LEFT = 3
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