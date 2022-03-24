#include "Player.h"

Player::Player(DrawnObject* object, int levelIndex)
	: Entity(EntityType::PLAYER, object, levelIndex), collectedCoins(0), animationFrame(0), animationTick(0)
{
	//delete player;
	player = this;
}

Player::~Player()
{
	delete player;
}

bool Player::PollPlayerEvents()
{
	CheckEntityCollisions();
	CheckOutOfBounds();
	ProcessAnimation();
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
	default:
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

void Player::CollectCoin(DrawnObject* object)
{
	object->UnloadObject(&Level::levelList[ownerIndex]->objects);
	collectedCoins++;
}

void Player::ProcessAnimation()
{
	if (!isOnGround)
	{
		sprite->SetTexture(TEXTURE_CHARACTER_JUMP);
	}
	else if (velocity[RIGHT] > 0.0f || velocity[LEFT] > 0.0f)
	{
		animationTick++;
		if (animationTick >= 10)
		{
			animationFrame++;
			animationTick = 0;
		}
		if (animationFrame >= 8) animationFrame = 0;
		sprite->SetTexture(static_cast<TextureName>(TEXTURE_CHARACTER_WALK_FRAME_0 + animationFrame));
	}
	else
	{
		sprite->SetTexture(TEXTURE_CHARACTER_WALK_FRAME_7);
		animationFrame = 0;
		animationTick = 0;
	}
}


Player* Player::GetPlayerPtr()
{
	return player;
}

Player* Player::player;
