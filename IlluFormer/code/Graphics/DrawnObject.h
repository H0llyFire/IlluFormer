#pragma once
#include "Texture.h"
#include "Shader.h"
#include "../Items/Consts.h"

#include <vector>


class DrawnObject
{
private:
   UniformPosition*	uniformPosition;
			 float* position;
	unsigned int	positionsAmount;
	unsigned int*	indices;
	unsigned int	indicesAmount;
			 int	index;
	Texture*		texture;
	Shader			currentShader;

public:
	bool isSolid;

	DrawnObject(UniformPosition* uniPos, float* pos, unsigned int posAmount, unsigned int* indis, unsigned int indisAmount, Texture* texture, Shader shader, int index);
	~DrawnObject();
	bool DrawObject();
	bool ChangePosition(float* pos);
	bool AddUpPosition(float x, float y);
	float* GetPosition() const;

	static int count;
	static std::vector<DrawnObject*> objects;

	static int FindObjectAtCoordinates(float x, float y);
	static int FindObjectAtCoordinates(int x, int y);
	static int CreateObject(UniformPosition* uniPos, float* pos, unsigned int posAmount, unsigned int* indis, unsigned int indisAmount, Texture* texture, Shader shader);
	static bool UnloadObject(int index);
	static bool UnloadAll();
};