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
	std::vector<unsigned int> chunkSizes = { 50, 50, 61, 20, 7, 4 };

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


std::vector<unsigned int> Map::getColor(unsigned int x, unsigned int y)
{

	float min_height = terrain.getMinValue();
	float max_height = terrain.getMaxValue();

	float min_temperature = temperature.getMinValue();
	float max_temperature = temperature.getMaxValue();

	float min_moisture = moisture.getMinValue();
	float max_moisture = moisture.getMaxValue();

	float heightRatio = (terrain.getHeightValue(x, y) - min_height) / (max_height - min_height);
	float tempRatio = (temperature.getHeightValue(x, y) - min_temperature) / (max_temperature - min_temperature);
	float moistRatio = (moisture.getHeightValue(x, y) - min_moisture) / (max_moisture - min_moisture);


	if (heightRatio < 1.0 / 7)
	{
		if (tempRatio < 3.0 / 6) { return coldOcean; }

		return freshOcean;
	}

	if (heightRatio < 2.0 / 7)
	{
		if (tempRatio < 2.0 / 6) { return coldWater; }

		return freshWater;
	}


	if (moistRatio < 0.5)
	{

		if (heightRatio < 3.0 / 7)
		{
			if (tempRatio < 1.0 / 6) { return icePlain; }

			if (tempRatio < 3.0 / 6) { return coldWater; }

			return freshWater;
		}

		if (heightRatio < 4.0 / 7)
		{
			if (tempRatio < 2.0 / 6) { return icePlain; }

			if (tempRatio < 5.0 / 6) { return plains; }

			return desert;
		}

		if (heightRatio < 5.0 / 7)
		{
			if (tempRatio < 2.0 / 6) { return highPlain; }

			if (tempRatio < 4.0 / 6) { return plains; }

			if (tempRatio < 5.0 / 6) { return forest; }

			return desert;
		}

		if (heightRatio < 6.0 / 7)
		{
			if (tempRatio < 2.0 / 6) { return rockMountain; }

			if (tempRatio < 5.0 / 6) { return highPlain; }

			return barrenMountain;
		}

		return snow;

	}

	if (heightRatio < 3.0 / 7)
	{
		if (tempRatio < 3.0 / 6) { return coldWater; }

		return freshWater;
	}

	if (heightRatio < 4.0 / 7)
	{
		if (tempRatio < 2.0 / 6) { return tundraForest; }

		if (tempRatio < 5.0 / 6) { return swamp; }

		return jungle;
	}

	if (heightRatio < 5.0 / 7)
	{
		if (tempRatio < 2.0 / 6) { return tundraForest; }

		if (tempRatio < 4.0 / 6) { return forest; }

		if (tempRatio < 5.0 / 6) { return denseForest; }

		return jungle;
	}

	if (heightRatio < 6.0 / 7)
	{
		if (tempRatio < 2.0 / 6) { return snow; }

		if (tempRatio < 5.0 / 6) { return highForest; }

		return jungleMountain;
	}

	return snow;

}

int Map::build_image(const char* filepath) {
	BMP* img = BMP_Create(sizeX, sizeY, 24);
	//float min = terrain.getMinValue();
	//float max = terrain.getMaxValue();

	//verbose.setRequiredLevel(0);

	unsigned int x, y;
	for (x = 0; x < sizeX; x++) {
		for (y = 0; y < sizeY; y++) {

			//verbose << x * sizeY + y << "/" << sizeX * sizeY << "\r";
			//float val = terrain.getHeightValue(x, y);

			/*unsigned char color = (unsigned char)((val - min) / (max - min) * 8);
			color *= 32;*/
			vector<unsigned int> color = getColor(x, y);
			BMP_SetPixelRGB(img, x, y, color[0], color[1], color[2]);
			//BMP_CHECK_ERROR(stdout, -1);
		}
	}
	BMP_WriteFile(img, filepath);
	//verbose << sizeX * sizeY << "/" << sizeX * sizeY << "\r";
	//verbose.endRequiredLevel();

	return 0;
}