#include "Coordinating.h"
#include <math.h>

unsigned int* Coordinates::gridIndices;

void Coordinates::SetGridIndices()
{
	gridIndices = new unsigned int[] {
		0, 1, 2,
		2, 3, 0
	};
}

unsigned int* Coordinates::GetGridIndices()
{
	return gridIndices;
}

void Coordinates::clearCoords()
{
	delete[] gridIndices;
}

float* Coordinates::SnapToGrid(const float x, const float y)
{
	auto* coords = new float[]
	{
		x - 16.0f, y - 9.0f,  0.0f,  0.0f,
		x - 15.0f, y - 9.0f,  1.0f,  0.0f,
		x - 15.0f, y - 8.0f,  1.0f,  1.0f,
		x - 16.0f, y - 8.0f,  0.0f,  1.0f
	};

	return coords;
}

float* Coordinates::SnapToBgGrid(float x, float y)
{
	auto* coords = new float[]
	{
		-16.0f , -9.0f , 0.0f, 0.0f,
		x	   , -9.0f , 1.0f, 0.0f,
		x	   , y	   , 1.0f, 1.0f,
		-16.0f , y	   , 0.0f, 1.0f
	};

	return coords;
}

int* Coordinates::UniformCoordinates(const float* coords)
{
	auto* uniformCoords = new int[]
	{
		static_cast<int>(ceil(coords[0])+16), static_cast<int>(ceil(coords[1])+9),
		static_cast<int>(ceil(coords[4])+16), static_cast<int>(ceil(coords[5])+9),
		static_cast<int>(ceil(coords[8])+16), static_cast<int>(ceil(coords[9])+9),
		static_cast<int>(ceil(coords[12])+16),static_cast<int>(ceil(coords[13]+9))
	};
	return uniformCoords;
}
