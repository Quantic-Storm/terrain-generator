#include "map.h"
#include "Perlin.h"
#include "qdbmp.h"



Map::Map() : Map(rand(), 1000, 1000) {}

Map::Map(long int customSeed, size_t sX, size_t sY) {
	sizeX = sX;
	sizeY = sY;
	seed = customSeed;
	srand(seed);
	HeightMap* hm;

	Perlin p(rand());
	
	std::cout << "Temperature...\n";
	hm = p.generate(sizeX, sizeY, 500);
	temperature = HeightMap(*hm);
	delete hm;

	std::cout << "Moisture...\n";
	p.changeSeed();
	hm = p.generate(sizeX, sizeY, 500);
	moisture = HeightMap(*hm);
	delete hm;

	std::cout << "Altitude...\n";
	p.changeSeed();
	hm = p.generate(sizeX, sizeY, 610);
	HeightMap altitude(*hm);
	delete hm;

	std::cout << "Montains...\n";
	p.changeSeed();
	hm = p.generate(sizeX, sizeY, 200);
	HeightMap harm1(*hm);
	delete hm;


	std::cout << "Hills...\n";
	p.changeSeed();
	hm = p.generate(sizeX, sizeY, 75);
	HeightMap harm2(*hm);
	delete hm;

	std::cout << "small hills...\n";
	p.changeSeed();
	hm = p.generate(sizeX, sizeY, 40);
	HeightMap harm3(*hm);
	delete hm;

	terrain = altitude * 30 + harm1 * 15 + harm2 * 6 + harm3 * 3;				// ne pas hesiter à changer les valeurs pour équilibrer

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