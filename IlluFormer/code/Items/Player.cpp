#include "Player.h"

Player::Player(DrawnObject* object)
	: Entity(EntityType::PLAYER, object)
{
	player = this;
}

Player::~Player()
{
	
}

bool Player::PollPlayerEvents()
{
	if (!(isMovingDown || isMovingRight || isMovingLeft || isMovingUp)) player->isMoving = false;
	if (isMoving)
	{
		MoveEntity();
	}
	PollEntityEvents();
	return false;
}

void Player::StartMovement(const Direction direction)
{
	player->isMoving = true;
	switch (direction)
	{
		case Direction::RIGHT:
			isMovingRight = true;
			break;
		case Direction::LEFT:
			isMovingLeft = true;
			break;
		case Direction::UP:
			isMovingUp = true;
			break;
		case Direction::DOWN:
			isMovingDown = true;
			break;
	}
}

void Player::StopMovement(Direction direction)
{
	switch (direction)
	{
	case Direction::RIGHT:
		isMovingRight = false;
		break;
	case Direction::LEFT:
		isMovingLeft = false;
		break;
	case Direction::UP:
		isMovingUp = false;
		break;
	case Direction::DOWN:
		isMovingDown = false;
		break;
	}
}

Player* Player::GetPlayerPtr()
{
	return player;
}

Player* Player::player;