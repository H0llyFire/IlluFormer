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

double* Coordinates::GetResolutionCoordinates(const float* coords)
{
	const auto resCoords = new double[8];
	for (int i = 0; i < 4; ++i)
	{
		resCoords[2 * i]	 = (coords[4 * i] + 16) * 50;
		resCoords[2 * i + 1] = -1 * (coords[4 * i + 1] - 9) * 50;
	}
	return resCoords;
}

double* Coordinates::GetResolutionCoordinates(float x, float y)
{
	const auto resCoords = new double[8];
	resCoords[0] = (x + 16) * 50;
	resCoords[1] = -1 * (y - 9) * 50;
	return resCoords;
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
	auto coords = new float[]
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

float* Coordinates::AddToCoords(float* coords, float x, float y)
{
	auto newCoords = new float[16]
	{
		coords[ 0] + x, coords[ 1] + y, coords[ 2], coords[ 3],
		coords[ 4] + x, coords[ 5] + y, coords[ 6], coords[ 7],
		coords[ 8] + x, coords[ 9] + y, coords[10], coords[11],
		coords[12] + x, coords[13] + y, coords[14], coords[15]
	};
	return newCoords;
}