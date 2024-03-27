#include "map.h"
#include "Perlin.h"
#include "qdbmp.h"
#include <future>



Map::Map() : Map(rand(), 1000, 1000) {}

Map::Map(long int customSeed, size_t sX, size_t sY) {
	sizeX = sX;
	sizeY = sY;
	seed = customSeed;
	srand(seed);
	HeightMap* hm;

	Perlin p(rand());

	seed = customSeed;
	srand(seed);
	Perlin p1(rand());

	std::vector<future<HeightMap*>> perlins(6);
	std::vector<unsigned int> chunkSizes = { 500, 500, 610, 200, 70, 40 };

	for (unsigned int i = 0; i < 6; i++) {
		Perlin p(rand());
		perlins[i] = std::async(std::launch::async, &Perlin::generate, &p, sizeX, sizeY, chunkSizes[i]);
	}


	temperature = *perlins[0].get();
	moisture = *perlins[1].get();
	HeightMap harm1 = *perlins[2].get();
	HeightMap harm2 = *perlins[3].get();
	HeightMap harm3 = *perlins[4].get();
	HeightMap harm4 = *perlins[5].get();

	terrain = harm1 * 30 + harm2 * 15 + harm3 * 6 + harm4 * 3;        // ne pas hesiter à changer les valeurs pour équilibrer

	build_image("C:/Users/benhi/Desktop/map1.bmp");

	cout << "Applying harsh erosion\n";
	// Applying erosion
	Erosion erosionH(0.5, 0.1, 0.05, 0.4, 0.00008, false);
	//erosionH.applyOn(terrain, 0.02*sizeX*sizeY);

	cout << "Applying smooth erosion\n";
	// Applying erosion
	Erosion erosionS(0.5, 0.1, 0.01, 0.25, 0.001, true);
	erosionS.applyOn(terrain, 0.03*sizeX*sizeY);
	cout << "Erosion done !\n";

	verbose.endRequiredLevel();

	build_image("C:/Users/benhi/Desktop/map2.bmp");

}


int Map::build_image(const char* filepath) {
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
			//BMP_CHECK_ERROR(stdout, -1);
		}
	}
	BMP_WriteFile(img, filepath);

	return 0;
}