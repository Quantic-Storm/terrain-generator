#pragma once
#include "HeightMap.h"

class Map {
private :
	size_t sizeX;
	size_t sizeY;
	long int seed;
	HeightMap terrain;
	HeightMap temperature;

public  :
	Map();
	Map(Map& map);
	Map(long int seed);
	Map(std::string filename);

	void serialize();
	long int getSeed();


};
