#include "DrawnObject.h"

DrawnObject::DrawnObject(UniformPosition* uniPos, float* pos, unsigned int posAmount, unsigned int* indis, unsigned int indisAmount, Texture* texture, Shader shader, int index)
	: uniformPosition(uniPos), position(pos), positionsAmount(posAmount), indices(indis), indicesAmount(indisAmount), index(index), texture(texture), currentShader(shader)
{
	//position = new float(posAmount);
	texture->Unbind();
	if (texture == TextureList::textures[TEXTURE_WALL]) { isSolid = true; }
	else { isSolid = false; }
}

DrawnObject::~DrawnObject()
{
	delete texture;
	delete[] position;
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

	Renderer::Draw(va, ib, currentShader);

	texture->Unbind();
	return false;
}

bool DrawnObject::ChangePosition(float* pos)
{
	delete[] position;
	position = pos;
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


int DrawnObject::FindObjectAtCoordinates(const float x, const float y)
{
	for (const auto obj : objects)
	{
		if (static_cast<int>(ceilf(x)) == obj->uniformPosition->x)
		{
			if (static_cast<int>(ceilf(y)) == obj->uniformPosition->y)
			{
				return obj->index;
			}
		}
	}
	return -1;
}

int DrawnObject::FindObjectAtCoordinates(const int x, const int y)
{
	for (const auto obj : objects)
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

int DrawnObject::CreateObject(UniformPosition* uniPos, float* pos, unsigned int posAmount, unsigned int* indis, unsigned int indisAmount, Texture* texture, Shader shader)
{
	objects.push_back(new DrawnObject(uniPos, pos, posAmount, indis, indisAmount, texture, shader, count));
	count += 1;
	return count - 1;
}

bool DrawnObject::UnloadObject(const int index)
{

	objects.erase(objects.begin() + index);
	return false;
}

bool DrawnObject::UnloadAll()
{
	objects.clear();
	return false;
}

int DrawnObject::count{ 0 };
std::vector<DrawnObject*> DrawnObject::objects{};