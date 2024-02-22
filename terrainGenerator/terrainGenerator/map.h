#pragma once
#include "HeightMap.h"

class Map {
private :
	long int seed;
	HeightMap height;
	HeightMap temperature;

public  :
	Map();
	Map(Map& map);
	Map(long int seed);
	Map(std::string filename);

	void serialize();
	long int getSeed();


};
