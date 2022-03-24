#pragma once
#include "Texture.h"
#include "Shader.h"
#include "../Items/Consts.h"

#include <vector>


class DrawnObject
{
private:
	UniformPosition* uniformPosition;
	float* position;
	unsigned int positionsAmount;
	unsigned int* indices;
	unsigned int indicesAmount;
	int index;
	Texture* texture;
	Shader* currentShader;

public:
	bool isSolid;
	bool isDeadly;

	DrawnObject(UniformPosition* uniPos, float* pos, unsigned int posAmount, unsigned int* indis,
	            unsigned int indisAmount, Texture* texture, Shader* shader, int index);
	~DrawnObject();
	bool DrawObject();
	bool ChangePosition(float* pos);
	bool AddUpPosition(float x, float y);
	float* GetPosition() const;
	int GetObjectType(); //Returns index of the Texture in TextureList
	bool UnloadObject(std::vector<DrawnObject*>* listOfObjects);
	void SetTexture(TextureName newTexture);

	static int count;
	//static std::vector<DrawnObject*> objects;

	static int FindObjectAtCoordinates(float x, float y, std::vector<DrawnObject*>* listOfObjects);
	static int FindObjectAtCoordinates(int x, int y, std::vector<DrawnObject*>* listOfObjects);
	static bool UnloadObject(int index, std::vector<DrawnObject*>* listOfObjects);
	static bool UnloadAll(std::vector<DrawnObject*>* listOfObjects);
	static void ReIndexList(int index, std::vector<DrawnObject*>* listOfObjects);
};
