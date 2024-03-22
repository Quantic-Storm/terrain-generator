#include "map.h"
#include "Perlin.h"
extern "C" {
	#include "qdbmp/qdbmp.h"
}



Map::Map() : Map(rand(), 1000, 1000){}

Map::Map(long int customSeed, size_t sX, size_t sY) {
	sizeX = sX;
	sizeY = sY;
	seed = customSeed;
	srand(seed);
	Perlin p(rand());
	temperature = *p.generate(sizeX, sizeY, 500);
	p.changeSeed(rand());
	moisture = *p.generate(sizeX, sizeY, 500);
	p.changeSeed(rand());
	HeightMap altitude = *p.generate(sizeX, sizeY, 610);
	p.changeSeed(rand());
	HeightMap harm1 = *p.generate(sizeX, sizeY, 200);
	p.changeSeed(rand());
	HeightMap harm2 = *p.generate(sizeX, sizeY, 75);
	p.changeSeed(rand());
	HeightMap harm3 = *p.generate(sizeX, sizeY, 40);
	terrain = altitude * 30 + harm1 * 15 + harm2 * 6 + harm3 * 3;				// ne pas hesiter à changer les valeurs pour équilibrer
}


int Map::build_image() {
	BMP* img = BMP_Create(sizeX, sizeY, 24);
	float min = terrain.getMinValue();
	float max = terrain.getMaxValue();

	unsigned int x, y;
	for (x = 0; x < sizeX; x++) {
		for (y = 0; y < sizeY; y++) {
			float val = terrain.getHeightValue(x, y);
			
			unsigned char color = (unsigned char)((val - min) / (max - min) * 8);
			color *= 32;
			BMP_SetPixelRGB(img, x, y, color, color, color);
			BMP_CHECK_ERROR(stdout, -1);
		}
	}
	BMP_WriteFile(img, "map.bmp");
	
}
