#include "map.h"
#include "Perlin.h"
extern "C" {
	#include "qdbmp/qdbmp.h"
}
#include <future>



Map::Map() : Map(rand(), 1000, 1000){}

Map::Map(long int customSeed, size_t sX, size_t sY) {
	sizeX = sX;
	sizeY = sY;
	seed = customSeed;
	srand(seed);
	Perlin p1(rand());


	std::future<HeightMap*> f1 = std::async(std::launch::async, &Perlin::generate, &p1, sizeX, sizeY, 500);
	Perlin p2(rand());
	std::future<HeightMap*> f2 = std::async(std::launch::async, &Perlin::generate, &p2, sizeX, sizeY, 500);
	Perlin p3(rand());
	std::future<HeightMap*> f3 = std::async(std::launch::async, &Perlin::generate, &p3, sizeX, sizeY, 610);
	Perlin p4(rand());
	std::future<HeightMap*> f4 = std::async(std::launch::async, &Perlin::generate, &p4, sizeX, sizeY, 200);
	Perlin p5(rand());
	std::future<HeightMap*> f5 = std::async(std::launch::async, &Perlin::generate, &p5, sizeX, sizeY, 75);
	Perlin p6(rand());
	std::future<HeightMap*> f6 = std::async(std::launch::async, &Perlin::generate, &p6, sizeX, sizeY, 40);


	temperature = *f1.get();
	moisture = *f2.get();
	HeightMap harm1 = *f3.get();
	HeightMap harm2 = *f4.get();
	HeightMap harm3 = *f5.get();
	HeightMap harm4 = *f6.get();

	terrain = harm1 * 30 + harm2 * 15 + harm3 * 6 + harm4 * 3;		// ne pas hesiter à changer les valeurs pour équilibrer
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
