#pragma once
#include "HeightMap.h"

class Map {
private :
	size_t sizeX;
	size_t sizeY;
	long int seed;
	HeightMap terrain;
	HeightMap temperature;
	HeightMap humidity;

public  :
	Map();
	Map(Map& map) = default;
	Map(long int customSeed, size_t sX, size_t sY);
	Map(std::string filename);	

	Map& operator=(Map& map) = default;

	void serialize(std::string name);		// determiner le format (idem pour le dernier constructeur)
	long int getSeed() { return seed; };


};
