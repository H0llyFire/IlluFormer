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
	PollEntityEvents();
	PrintStatus();
	return false;
}

void Player::StartMovement(const Direction direction)
{
	player->isMoving = true;
	switch (direction)
	{
		case Direction::RIGHT:
			isMovingInDirection[static_cast<int>(Direction::RIGHT)] = true;
			break;
		case Direction::LEFT:
			isMovingInDirection[static_cast<int>(Direction::LEFT)] = true;
			break;
	}
}

void Player::StopMovement(Direction direction)
{
	switch (direction)
	{
	case Direction::RIGHT:
		isMovingInDirection[static_cast<int>(Direction::RIGHT)] = false;
		break;
	case Direction::LEFT:
		isMovingInDirection[static_cast<int>(Direction::LEFT)] = false;
		break;
	}
}



Player* Player::GetPlayerPtr()
{
	return player;
}

Player* Player::player;