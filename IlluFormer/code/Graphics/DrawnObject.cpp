#include "DrawnObject.h"

DrawnObject::DrawnObject(UniformPosition* uniPos, float* pos, unsigned int posAmount, unsigned int* indis,
                         unsigned int indisAmount, Texture* texture, Shader* shader, int index)
	: uniformPosition(uniPos), position(pos), positionsAmount(posAmount), indices(indis), indicesAmount(indisAmount),
	  index(index), texture(texture), currentShader(shader)
{
	//position = new float(posAmount);
	texture->Unbind();
	if (texture == TextureList::textures[TEXTURE_WALL] || texture == TextureList::textures[TEXTURE_GROUND])
	{
		isSolid = true;
	}
	else { isSolid = false; }
	if (texture == TextureList::textures[TEXTURE_SPIKE_UP] || texture == TextureList::textures[TEXTURE_SPIKE_RIGHT] ||
		texture == TextureList::textures[TEXTURE_SPIKE_DOWN] || texture == TextureList::textures[TEXTURE_SPIKE_LEFT])
		isDeadly = true;
	else
		isDeadly = false;
}

DrawnObject::~DrawnObject()
{
	delete[] position;
	delete[] uniformPosition;
	//delete uniformPosition;
}

bool DrawnObject::DrawObject()
{
	texture->Bind();
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	VertexArray va;
	IndexBuffer ib(indices, indicesAmount);

	VertexBuffer vb(position, positionsAmount * sizeof(float));
	va.AddBuffer(vb, layout);

	Renderer::Draw(va, ib, *currentShader);

	texture->Unbind();
	return false;
}

bool DrawnObject::ChangePosition(float* pos)
{
	for (int i = 0; i < 16; ++i)
	{
		position[i] = pos[i];
	}

	return false;
}

bool DrawnObject::AddUpPosition(const float x, const float y)
{
	position[0] += x;
	position[4] += x;
	position[8] += x;
	position[12] += x;

	position[1] += y;
	position[5] += y;
	position[9] += y;
	position[13] += y;

	return true;
}

float* DrawnObject::GetPosition() const
{
	return position;
}

int DrawnObject::GetObjectType()
{
	for (int i = 0; i < TEXTURE_LAST; ++i)
	{
		if (TextureList::textures[i] == texture) return i;
	}
	return -1;
}

bool DrawnObject::UnloadObject(std::vector<DrawnObject*>* listOfObjects)
{
	int tempIndex = index;
	listOfObjects->erase(listOfObjects->begin() + index);
	ReIndexList(tempIndex, listOfObjects);
	return false;
}

void DrawnObject::SetTexture(TextureName newTexture)
{
	texture = TextureList::textures[newTexture];
}


int DrawnObject::FindObjectAtCoordinates(const float x, const float y, std::vector<DrawnObject*>* listOfObjects)
{
	int tempX = static_cast<int>(floorf(x)) + 16;
	int tempY = static_cast<int>(floorf(y)) + 9;

	for (const auto obj : *listOfObjects)
	{
		if (tempX == obj->uniformPosition->x)
		{
			if (tempY == obj->uniformPosition->y)
			{
				return obj->index;
			}
		}
	}
	return -1;
}

int DrawnObject::FindObjectAtCoordinates(const int x, const int y, std::vector<DrawnObject*>* listOfObjects)
{
	for (const auto obj : *listOfObjects)
	{
		if (x == obj->uniformPosition->x)
		{
			if (y == obj->uniformPosition->y)
			{
				return obj->index;
			}
		}
	}
	return -1;
}

bool DrawnObject::UnloadObject(const int index, std::vector<DrawnObject*>* listOfObjects)
{
	listOfObjects->erase(listOfObjects->begin() + index);
	ReIndexList(index, listOfObjects);
	return false;
}

bool DrawnObject::UnloadAll(std::vector<DrawnObject*>* listOfObjects)
{
	listOfObjects->clear();
	return false;
}

void DrawnObject::ReIndexList(int index, std::vector<DrawnObject*>* listOfObjects)
{
	for (unsigned int i = index; i < listOfObjects->size(); ++i)
	{
		(*listOfObjects)[i]->index--;
	}
}

int DrawnObject::count{0};
//std::vector<DrawnObject*> DrawnObject::objects{};
