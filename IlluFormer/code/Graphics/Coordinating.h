#pragma once

class Coordinates
{
private:
public:
	static unsigned int* gridIndices;

	static void SetGridIndices();
	static unsigned int* GetGridIndices();
	static void clearCoords();

	static float* SnapToGrid(float x, float y);
	static float* SnapToBgGrid(float x, float y);
	static int* UniformCoordinates(const float* coords);
};