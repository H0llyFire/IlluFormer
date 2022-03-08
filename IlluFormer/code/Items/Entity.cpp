#include"Entity.h"

#include <iomanip>


Entity::Entity(EntityType type, DrawnObject* object)
	: isOnGround(false), currentSpeedBonus(0.0f), tick(0), typeName(type), sprite(object), isMoving(false), isMidJump(false), jumpVelocity(0.0f)
{
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
			minSpeed = 0.05f;
			maxSpeed = 0.08f;
			facing = LEFT;
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
	else if (velocity[DOWN] != 0.0f || velocity[RIGHT] != 0.0f || velocity[UP] != 0.0f || velocity[LEFT] != 0.0f) std::cout << "ERROR IN VELOCITY MANAGEMENT" << std::endl << std::endl << std::endl << std::endl;
	SetGravity();
	//TODO Add jump velocity variable and function to add the velocity
	SetJumpVelocity();
	CheckCollisions(); //Bugging into sprites

	if (isMoving && velocity[DOWN] <= 0.0f && velocity[RIGHT] <= 0.0f && velocity[LEFT] <= 0.0f && velocity[UP] <= 0.0f)
	{
		isMoving = false;
	}

	if (isMoving)
	{
		MoveEntity();
	}

	tick++;

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
{ //BUG WORKS LIKE 95%, walking in single block wide corridors is impossible
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
		if (tempY == floorf(tempY) && directionCorner==1) 
			tempY -= 1.0f;
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

	const int index = DrawnObject::FindObjectAtCoordinates(tempX, tempY);
	if (index >= 0 && velocity[DOWN] > 0.0f && direction == 0 && DrawnObject::objects[index]->isSolid)
	{
		isOnGround = true;
	}
	return index;
}
bool Entity::CheckBlock(int direction, int blockIndex, float position)
{ //BUG RIGHT SIDE BUGS OUT COMPLETELY, JUMPING ON SECOND PLATFORM GOES YEET
	if(DrawnObject::objects[blockIndex]->isSolid)
	{
		float vel = abs(abs(position) - abs(DrawnObject::objects[blockIndex]->GetPosition()[direction > 1 ? (direction - 2) * 4 + (direction + 1) % 2 : (direction + 2) * 4 + (direction + 1) % 2]));
		//std::cout << "temp: " << vel << std::endl;
		
		overrideVelocity[direction] = vel;
		if (vel == 0.0f) velocity[direction] = vel;
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
			if(index>=0 && DrawnObject::objects[index]->isSolid)
			{
				CheckBlock(direction, index, position[direction * 4 + (direction + 1) % 2]);
			}
		}
	}
	OverrideVelocity();
	return true;
}
void Entity::Jump()
{
	if (isOnGround) 
	{
		jumpVelocity = 0.5f;
		isMidJump = true;
		isMoving = true;
	}
	// BUG jumping off of any tile entity
}

void Entity::SetGravity()
{
	//if (isOnGround)
		//velocity[DOWN] = 0.0f;
	if (velocity[DOWN] <= 0.0f)
	{
		velocity[DOWN] = 0.02f;
		isMoving = true;
	}
	else
	{
		velocity[DOWN] += 0.004f;
		isMoving = true;
	}

	if (velocity[DOWN] > maxSpeed)
		velocity[DOWN] = maxSpeed;

	if (jumpVelocity > 0.0f)
	{
		jumpVelocity -= 0.04f;
		if (jumpVelocity <= 0.0f)
		{
			jumpVelocity = 0.0f;
			isMidJump = false;
		}
	}
}
void Entity::SetGravity(float modifier)
{
}

void Entity::SetJumpVelocity()
{
	velocity[UP] += jumpVelocity;
}

void Entity::SetCollision(int i, int ptA, int ptB, int* afterMoveCords) //transfer a part of CheckCollisions
{
}

void Entity::PrintStatus()
{
	float* positions = sprite->GetPosition();
	std::cout << "Is On Ground: " << isOnGround << std::endl;
	std::cout << "Is Mid Jump: " << isMidJump << std::endl;
	std::cout << "Is Moving: " << isMoving << std::endl;
	std::cout << "Position: " << sprite->GetPosition()[0] << " " << sprite->GetPosition()[1] << std::endl;
	std::cout << std::setprecision(9) << "Velocity D: " << velocity[DOWN] << " R: " << velocity[RIGHT] << " U: " << velocity[UP] << " L: " << velocity[LEFT] << std::endl;
	for (int i = 0; i < 4; ++i)
		{ std::cout << std::setprecision(9) << "Point " << i << ":  " << positions[4 * i] << "; " << positions[4 * i + 1] << std::endl; }
	std::cout << std::endl;
} 

bool Entity::PollEntitiesEvents()
{
	for (auto entity : entityList)
	{
		entity->PollEntityEvents();
	}
	return false;
}

int Entity::CreateEntity(EntityType type, DrawnObject* object)
{
	entityList.push_back(new Entity(type, object));
	return entityList.size() - 1;
}

bool Entity::UnloadEntity(const int index)
{

	entityList.erase(entityList.begin() + index);
	return false;
}

bool Entity::UnloadAll()
{
	entityList.clear();
	return false;
}

std::vector<Entity*> Entity::entityList;