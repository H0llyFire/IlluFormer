#include"Entity.h"
#include "../Graphics/Coordinating.h"



Entity::Entity(EntityType type, DrawnObject* object)
	: isOnGround(false), currentSpeedBonus(0.0f), tick(0), velocity({ 0.0f, 0.0f }), typeName(type), sprite(object), isMoving(false)
{
	switch (type)
	{
		case EntityType::PLAYER:
			health = 3;
			minSpeed = 0.1f;
			maxSpeed = 0.15f;
			facing = Direction::RIGHT;
			break;
		case EntityType::GHOST:
			health = 1;
			minSpeed = 0.05f;
			maxSpeed = 0.08f;
			facing = Direction::LEFT;
			break;
		case EntityType::BLOCK:
			health = -1;
			minSpeed = 0.1f;
			maxSpeed = 0.1f;
			facing = Direction::DOWN;
			break;
		default:
			health = 0;
			minSpeed = 0.0f;
			maxSpeed = 0.0f;
			facing = Direction::RIGHT;
			break;
	}

}

Entity::~Entity()
{
}

bool Entity::PollEntityEvents()
{
	//if (!isOnGround) (MoveEntity({0.0f, -0.2f}));
	//float* pos = sprite->GetPosition();

	//std::cout << "Entity tick: " << tick << std::endl;
	tick++;
	if(tick%60==0) { std::cout << "seconds: " << tick/60 << std::endl; }
	if(tick%5==0)
	{
		//if (velocity.speedX > 0 && velocity.speedX <= maxSpeed)		velocity.speedX += 0.01f;
		//if (velocity.speedX < 0 && velocity.speedX >= -1.0f*maxSpeed)  velocity.speedX -= 0.01f;
	}
	

	



	return false;
}

bool Entity::MoveEntity()
{
	CheckCollisions();

	SetVelocity({ static_cast<float>(isMovingRight) * 1.0f + static_cast<float>(isMovingLeft) * -1.0f,
						   static_cast<float>(isMovingUp) * 1.0f + static_cast<float>(isMovingDown) * -1.0f });

	sprite->AddUpPosition(velocity.speedX * (minSpeed + currentSpeedBonus), velocity.speedY * minSpeed);
	return {};
}

bool Entity::MoveEntity(Direction direction)
{
	switch (direction)
	{
		case Direction::LEFT:
			sprite->AddUpPosition(-maxSpeed,0.0f);
			break;
		case Direction::RIGHT:
			sprite->AddUpPosition(maxSpeed, 0.0f);
			break;
		case Direction::UP:
			sprite->AddUpPosition(0.0f, maxSpeed);
			break;
		case Direction::DOWN:
			sprite->AddUpPosition(0.0f, -maxSpeed);
			break;
	}
	return {};
}

bool Entity::ChangeVelocity(Velocity vel)
{
	velocity.speedX += vel.speedX;
	velocity.speedY += vel.speedY;
	return false;
}

bool Entity::SetVelocity(Velocity vel)
{
	velocity.speedX = vel.speedX;
	velocity.speedY = vel.speedY;
	return false;
}

bool Entity::CheckCollisions() //W.I.P
{
	const float* position = sprite->GetPosition();

	//int* uniformCords = Coordinates::UniformCoordinates(position);
	//if ( velocity.speedY < 0.0f ) std::cout<< "Crossed" << std::endl;
	//std::cout << position[4] << " / " << round(position[4]) << std::endl;

	//Check direction => choose two to three pts which to check 

	int afterMoveCords[4][2];
	for (int i = 0; i < 4; ++i)
	{
		float tempX = truncf((position[4 * i]     + velocity.speedX * (minSpeed + currentSpeedBonus))*100)/100;
		float tempY = truncf((position[4 * i + 1] + velocity.speedY * minSpeed)*100)/100;

		afterMoveCords[i][0] = static_cast<int>(floorf(tempX)) + 16;
		afterMoveCords[i][1] = static_cast<int>(floorf(tempY)) + 9;
	}

	for (int i = 1; i < 5; ++i)
	{
		const int tempPtA = afterMoveCords[i - 1][i % 2 == 0 ? 1 : 0];
		const int tempPtB = afterMoveCords[i == 4 ? 0 : i][i % 2 == 0 ? 1 : 0];
		if (i % 2 != 0)
		{
			int const index = DrawnObject::FindObjectAtCoordinates(tempPtA, afterMoveCords[i - 1][i % 2 == 0 ? 0 : 1]);
			if (index >= 0 && DrawnObject::objects[index]->isSolid)
			{
				isMovingRight = false;
				isMovingLeft = false;
				currentSpeedBonus = 0.0f;
			}
		}
		else
		{
			int const index = DrawnObject::FindObjectAtCoordinates(afterMoveCords[i - 1][i % 2 == 0 ? 0 : 1], tempPtA);
			if (index >= 0 && DrawnObject::objects[index]->isSolid)
			{
				isMovingDown = false;
				isMovingUp = false;
			}
		}

		if (tempPtA != tempPtB)
		{
			if (i % 2 != 0)
			{
				int const index = DrawnObject::FindObjectAtCoordinates(tempPtB, afterMoveCords[i == 4 ? 0 : i][i % 2 == 0 ? 0 : 1]);
				if (index >= 0 && DrawnObject::objects[index]->isSolid)
				{
					isMovingRight = false;
					isMovingLeft = false;
					currentSpeedBonus = 0.0f;
				}
			}
			else
			{
				int const index = DrawnObject::FindObjectAtCoordinates(afterMoveCords[i == 4 ? 0 : i][i % 2 == 0 ? 0 : 1], tempPtB);
				if (index >= 0 && DrawnObject::objects[index]->isSolid)
				{
					isMovingDown = false;
					isMovingUp = false;
				}
			}
		}
	}

	return false;
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