#include"Entity.h"

#include <iomanip>

#include "Player.h"


Entity::Entity(EntityType type, DrawnObject* object, int levelIndex)
	: isOnGround(false), jumpVelocity(0.0f), jumpTick(0), tick(0), typeName(type), sprite(object), ownerIndex(levelIndex), isMoving(false), isJumping(0), isMidJump(false)
{
	const float* positions = object->GetPosition();
	for (int i = 0; i < 16; ++i)
		defaultPosition[i] = positions[i];
	for(int i = 0; i<4; ++i)
	{
		isMovingInDirection[i] = false;
		isBlockedInDirection[i] = false;
		velocity[i] = 0.0f;
		overrideVelocity[i] = 0.0f;
	}
	switch (type)
	{
		case EntityType::PLAYER:
			health = 3;
			minSpeed = 0.1f;
			maxSpeed = 0.15f;
			facing = RIGHT;
			break;
		case EntityType::GHOST:
			health = 1;
			minSpeed = 0.01f;
			maxSpeed = 0.04f;
			facing = LEFT;
			isMovingInDirection[LEFT] = true;
			isMoving = true;
			break;
		case EntityType::BLOCK:
			health = -1;
			minSpeed = 0.1f;
			maxSpeed = 0.1f;
			facing = DOWN;
			break;
		default:
			health = 0;
			minSpeed = 0.0f;
			maxSpeed = 0.0f;
			facing = RIGHT;
			break;
	}

}

Entity::~Entity()
{

}

bool Entity::PollEntityEvents()
{
	if (isJumping) Jump();
	if (isMoving)
	{
		if (isMovingInDirection[LEFT]) { velocity[LEFT] += 0.01f; }
		else { velocity[LEFT] -= 0.03f; }
		if (velocity[LEFT] > maxSpeed) { velocity[LEFT] = maxSpeed; }
		else if (velocity[LEFT] < 0.0f) { velocity[LEFT] = 0.0f; }

		if (isMovingInDirection[RIGHT]) { velocity[RIGHT] += 0.01f; }
		else { velocity[RIGHT] -= 0.03f; }
		if (velocity[RIGHT] > maxSpeed) { velocity[RIGHT] = maxSpeed; }
		else if (velocity[RIGHT] < 0.0f) { velocity[RIGHT] = 0.0f; }

		if (velocity[UP] > 0.0f) velocity[UP] = 0.0f;
	}
	//else if (velocity[DOWN] != 0.0f || velocity[RIGHT] != 0.0f || velocity[UP] != 0.0f || velocity[LEFT] != 0.0f) std::cout << "ERROR IN VELOCITY MANAGEMENT" << std::endl << std::endl << std::endl << std::endl;

	SetGravity();
	SetJumpVelocity();
	CheckCollisions();

	if (velocity[RIGHT] > 0.0f)
	{
		facing = RIGHT;
		float* pos = sprite->GetPosition();
		pos[2] = 1.0f;
		pos[6] = 0.0f;
		pos[10] = 0.0f;
		pos[14] = 1.0f;
	}
	else if (velocity[LEFT] > 0.0f) //Flips the texture Right X Left
	{
		facing = LEFT;
		float* pos = sprite->GetPosition();
		pos[2] = 0.0f;
		pos[6] = 1.0f;
		pos[10] = 1.0f;
		pos[14] = 0.0f;
	}

	if (isMoving && velocity[DOWN] <= 0.0f && velocity[RIGHT] <= 0.0f && velocity[LEFT] <= 0.0f && velocity[UP] <= 0.0f)
	{
		isMoving = false;
	}

	if (isMoving)
	{
		MoveEntity();
	}

	tick++;
	jumpTick++;
	return false;
}

bool Entity::MoveEntity()
{
	sprite->AddUpPosition(velocity[RIGHT] - velocity[LEFT], velocity[UP] - velocity[DOWN]);
	return {};
	
}

bool Entity::MoveEntity(Direction direction)
{
	switch (direction)
	{
		case LEFT:
			sprite->AddUpPosition(-maxSpeed,0.0f);
			break;
		case RIGHT:
			sprite->AddUpPosition(maxSpeed, 0.0f);
			break;
		case UP:
			sprite->AddUpPosition(0.0f, maxSpeed);
			break;
		case DOWN:
			sprite->AddUpPosition(0.0f, -maxSpeed);
			break;
	}
	return {};
}

bool Entity::ChangeVelocity(float speed, Direction direction)
{
	velocity[direction] += speed;
	return false;
}

bool Entity::SetVelocity(float speed, Direction direction)
{
	velocity[direction] = speed;
	return false;
}

/*float tempX = truncf((positions[4 * i] + velocity[RIGHT] - velocity[LEFT]) * 100) / 100;
 *float tempY = truncf((positions[4 * i + 1] + velocity[UP] - velocity[DOWN]) * 100) / 100;
 *
 *int const index = DrawnObject::FindObjectAtCoordinates(uniCoords[direction * 2 + 2 * i], uniCoords[direction * 2 + 1 + 2 * i]);
 *
 *
 *
 */


int Entity::GetCollisionIndex(float posX, float posY, int direction, int directionCorner)
{ //BUG WORKS LIKE 95%, walking in single block wide corridors is sometimes impossible, jumping into corners still bug the shit out of the player
	float tempX = posX;
	float tempY = posY;
	switch (direction)
	{
	case DOWN:
		tempY = posY - velocity[DOWN];
		if (tempX == floorf(tempX) && directionCorner == 1) tempX -= 1.0f;
		break;
	case RIGHT:
		tempX = posX + velocity[RIGHT];
		if (tempX == floorf(tempX)) tempX -= 1.0f;
		if (tempY == floorf(tempY) && directionCorner==1)   tempY -= 1.0f;
		break;
	case UP:
		tempY = posY + velocity[UP];
		if (tempY == floorf(tempY)) tempY -= 1.0f;
		if (tempX == floorf(tempX) && directionCorner == 0) tempX -= 1.0f;
		break;
	case LEFT:
		tempX = posX - velocity[LEFT];
		if (tempY == floorf(tempY) && directionCorner == 0) tempY -= 1.0f;
		break;
	default:
		break;
	}

	const int index = DrawnObject::FindObjectAtCoordinates(tempX, tempY, &Level::levelList[ownerIndex]->objects);
	if (index >= 0 && velocity[DOWN] > 0.0f && direction == 0 && Level::levelList[ownerIndex]->objects[index]->isSolid)
	{
		isOnGround = true;
		isMidJump = false;
	}
	return index;
}
bool Entity::CheckBlock(int direction, int blockIndex, float position)
{ //BUG CORNER  COLLISIONS WRONG
	if(Level::levelList[ownerIndex]->objects[blockIndex]->isSolid)
	{
		float vel = abs(abs(position) - abs(Level::levelList[ownerIndex]->objects[blockIndex]->GetPosition()[direction > 1 ? (direction - 2) * 4 + (direction + 1) % 2 : (direction + 2) * 4 + (direction + 1) % 2]));
		//std::cout << "temp: " << vel << std::endl;
		
		overrideVelocity[direction] = vel;
		if (vel == 0.0f) velocity[direction] = vel;

		if (typeName == EntityType::GHOST && direction == RIGHT)
		{
			isMovingInDirection[RIGHT] = false;
			isMovingInDirection[LEFT] = true;
		}
		if (typeName == EntityType::GHOST && direction == LEFT)
		{
			isMovingInDirection[RIGHT] = true;
			isMovingInDirection[LEFT] = false;
		}
	}
	else if(Level::levelList[ownerIndex]->objects[blockIndex]->GetObjectType()==TEXTURE_COIN && typeName==EntityType::PLAYER)
	{
		Player::GetPlayerPtr()->CollectCoin(Level::levelList[ownerIndex]->objects[blockIndex]);
	}
	else if(Level::levelList[ownerIndex]->objects[blockIndex]->GetObjectType() == TEXTURE_FLAG && typeName==EntityType::PLAYER)
	{
		int newLevel = Level::GetActiveLevel()->index+1;
		if(Level::levelList[ownerIndex]->index != Level::GetActiveLevel()->index)
		{
				
		}
		else if (newLevel == 3)
		{
			Menu::isEndMenuActive = true;
		}
		else
		{
			new Level{ (std::to_string(newLevel)) + ".level" };
			Level::SetActiveLevel(newLevel);
		}
	}
	return true;
}

void Entity::OverrideVelocity()
{
	for (int i = DOWN; i <= LEFT; ++i)
	{
		if(overrideVelocity[i]!=0.0f)
		{
			velocity[i] = overrideVelocity[i];
			overrideVelocity[i] = 0.0f;
			isMoving = true;
			if (i == UP) jumpVelocity = 0.0f;
		}
	}
}

bool Entity::CheckCollisions() //W.I.P
{
	const float* position = sprite->GetPosition(); //array of 16 floats, 4 per each point => x;y;position;index
	isOnGround = false;
	for (int direction = DOWN; direction <= LEFT; ++direction)
	{
		for (int n = 0; n < 2; ++n)
		{
			int index = GetCollisionIndex(position[((direction+n==4?-1:direction) + n) * 4], position[((direction + n == 4 ? -1 : direction) + n) * 4 + 1], direction, n);
			if(index>=0)
			{
				CheckBlock(direction, index, position[direction * 4 + (direction + 1) % 2]);
			}
		}
	}
	OverrideVelocity();
	return true;
}

bool Entity::CheckEntityCollisions()
{
	const float* checkedPos = sprite->GetPosition();
	for (auto entity : Level::GetActiveLevel()->entities)
	{
		float* pos = entity->sprite->GetPosition();
		if (((checkedPos[0] >= pos[0] && checkedPos[0] <= pos[4]) || (checkedPos[4] >= pos[0] && checkedPos[4] <= pos[4])) &&
			((checkedPos[1] >= pos[1] && checkedPos[1] <= pos[9]) || (checkedPos[9] >= pos[1] && checkedPos[9] <= pos[9])))
		{
			std::cout << "Collision with entity" << std::endl;
			Level::GetActiveLevel()->ResetLevel();
		}
	}
	return false;
}

bool Entity::CheckOutOfBounds() const
{
	const float* positions = sprite->GetPosition();
	if (positions[8] < -16.0f || positions[9] < -9.0f || positions[0] > 16.0f)
	{
		Level::GetActiveLevel()->ResetLevel();
		return true;
	}
	return false;
}

void Entity::ResetPosition()
{
	sprite->ChangePosition(defaultPosition);
}

void Entity::Jump()
{
	if (isOnGround)
	{
		jumpVelocity = 0.28f;
		isMidJump = true;
		isMoving = true;
	}
	//else if (isMidJump && jumpTick < 10);
}

void Entity::SetGravity()
{
	//if (isOnGround)
		//velocity[DOWN] = 0.0f;
	if (jumpVelocity > 0.0f)
	{
		if (jumpTick >=3)
		{
			jumpTick = 0;
			jumpVelocity -= 0.03f;
			if (jumpVelocity <= 0.0f)
			{
				jumpVelocity = 0.0f;
				isMidJump = false;
			}
		}
	}
	else if (velocity[DOWN] <= 0.0f)
	{
		velocity[DOWN] = 0.025f;
		isMoving = true;
	}
	else
	{
		velocity[DOWN] += 0.0085f;
		isMoving = true;
	}

	if (velocity[DOWN] > 0.25f)
		velocity[DOWN] = 0.25f;

}
void Entity::SetGravity(float modifier)
{
}

void Entity::SetJumpVelocity()
{
	velocity[UP] += jumpVelocity;
}

void Entity::PrintStatus()
{
	float* positions = sprite->GetPosition();
	std::cout << "Is On Ground: " << isOnGround << std::endl;
	std::cout << "Is Mid Jump: " << isMidJump << std::endl;
	std::cout << "Is Moving: " << isMoving << std::endl;
	std::cout << "Position: " << sprite->GetPosition()[0] << " " << sprite->GetPosition()[1] << std::endl;
	std::cout << std::setprecision(9) << "Velocity D: " << velocity[DOWN] << " R: " << velocity[RIGHT] << " U: " << velocity[UP] << " L: " << velocity[LEFT] << std::endl;
	std::cout << "Jump Velocity: " << jumpVelocity << " Jump Tick: " << jumpTick << std::endl;
	for (int i = 0; i < 4; ++i)
		{ std::cout << std::setprecision(9) << "Point " << i << ":  " << positions[4 * i] << "; " << positions[4 * i + 1] << std::endl; }
	std::cout << std::endl;
} 

bool Entity::PollEntitiesEvents()
{
	for (auto entity : Level::GetActiveLevel()->entities)
	{
		entity->PollEntityEvents();
	}
	return false;
}

int Entity::CreateEntity(EntityType type, DrawnObject* object, int levelIndex)
{
	Level::GetActiveLevel()->entities.push_back(new Entity(type, object, levelIndex));
	return Level::GetActiveLevel()->entities.size() - 1;
}

bool Entity::UnloadEntity(const int index)
{

	Level::GetActiveLevel()->entities.erase(Level::GetActiveLevel()->entities.begin() + index);
	return false;
}

//std::vector<Entity*> Entity::entityList;