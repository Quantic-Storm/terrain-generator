#pragma once
#include "HeightMap.h"
#include "Erosion.h"
#include "Verbose.h"

class Map {
private:
	size_t sizeX;
	size_t sizeY;
	long int seed;
	HeightMap terrain;
	HeightMap temperature;
	HeightMap moisture;

	Verbose verbose;

public:
	Map();
	Map(Map& map) = default;
	Map(long int customSeed, size_t sX, size_t sY);

	Map& operator=(Map& map) = default;

	int build_image(const char* filepath);
	long int getSeed() { return seed; };
};