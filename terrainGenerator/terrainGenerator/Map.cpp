#include "map.h"
#include "Perlin.h"


Map::Map() {
	sizeX = sizeY = 1000;
	seed = rand();
	srand(seed);
	Perlin p(rand());
	temperature = *p.generate(sizeX, sizeY, 500);
	p.changeSeed(rand());
	HeightMap altitude = *p.generate(sizeX, sizeY, 300);
	p.changeSeed(rand());
	HeightMap erosion = *p.generate(sizeX, sizeY, 150);
	p.changeSeed(rand());
	HeightMap local = *p.generate(sizeX, sizeY, 25);
	terrain = altitude * 30 + erosion * 10 + local * 3;				// ne pas hesiter à changer les valeurs pour équilibrer
}