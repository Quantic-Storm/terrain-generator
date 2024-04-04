#include "map.h"
#include "Perlin.h"
#include "qdbmp.h"
#include <future>


// génération de la map (sans seed prédéfinie)
Map::Map() : Map(rand(), 1000, 1000) {}

// génération de la map (avec seed prédéfinie)
Map::Map(long int customSeed, size_t sX, size_t sY) {

	//gestion des paramètres et initialisation des variables
	sizeX = sX;
	sizeY = sY;
	seed = customSeed;
	srand(seed);
	HeightMap* hm;

	verbose.setRequiredLevel(1);

	std::vector<Perlin> perlins(6);
	std::vector<future<HeightMap*>> heightMaps(6);
	std::vector<unsigned int> chunkSizes = { 500, 500, 610, 200, 70, 40 };

	// lancement de la génération de Perlin sur des threads différents
	for (unsigned int i = 0; i < 6; i++) {
		perlins[i] = Perlin(rand());
		heightMaps[i] = std::async(std::launch::async, &Perlin::generate, &perlins[i], sizeX, sizeY, chunkSizes[i]);
	}

	// récupération des résultats
	temperature = *heightMaps[0].get();
	moisture = *heightMaps[1].get();
	HeightMap harm1 = *heightMaps[2].get();
	HeightMap harm2 = *heightMaps[3].get();
	HeightMap harm3 = *heightMaps[4].get();
	HeightMap harm4 = *heightMaps[5].get();

	// construction de la topologie et des climats
	terrain = harm1 * 30 + harm2 * 15 + harm3 * 6 + harm4 * 3;        // ne pas hesiter à changer les valeurs pour équilibrer
	temperature = temperature + harm1*2 + harm2*0.5 + harm3*0.2 + harm4*0.1;
	moisture = moisture + harm2*5 + harm3*0.2 + harm4*1;

	//calculs des extrémums
	terrain.computeMinMaxValues();
	temperature.computeMinMaxValues();
	moisture.computeMinMaxValues();

	cout << "Applying harsh erosion\n";
	// Application de l'erosion
	Erosion erosionH(0.5, 0.1, 0.05, 0.4, 0.00008, false);
	erosionH.applyOn(terrain, 0.02*sizeX*sizeY);

	Erosion erosionT= Erosion(0.8, 0.9, 0.25, 1, 0.9, true);
	erosionT.applyOn(temperature, 0.5 * sizeX * sizeY);
	

	Erosion erosionM = Erosion(0.2, 0.5, 0.05, 1, 0.5, false);
	erosionM.applyOn(moisture, 0.5 * sizeX * sizeY);

	cout << "Applying smooth erosion\n";

	Erosion erosionS(0.5, 0.1, 0.01, 0.25, 0.001, true);
	erosionS.applyOn(terrain, 0.03*sizeX*sizeY);
	cout << "Erosion done !\n";

	verbose.endRequiredLevel();

	// recalcul des extrémums après erosion
	terrain.computeMinMaxValues();
	moisture.computeMinMaxValues();
	temperature.computeMinMaxValues();

}

// application des couleurs en fonction du climat
std::vector<unsigned int> Map::getColor(unsigned int x, unsigned int y)
{

	// récupération des extrémums
	float min_height = terrain.getMinValue();
	float max_height = terrain.getMaxValue();

	float min_temperature = temperature.getMinValue();
	float max_temperature = temperature.getMaxValue();

	float min_moisture = moisture.getMinValue();
	float max_moisture = moisture.getMaxValue();

	// normalisation entre -1 et 1
	float heightRatio = (terrain.getHeightValue(x, y) - min_height) / (max_height - min_height);
	float tempRatio = (temperature.getHeightValue(x, y) - min_temperature) / (max_temperature - min_temperature);
	float moistRatio = (moisture.getHeightValue(x, y) - min_moisture) / (max_moisture - min_moisture);


	// disjonction de cas sur température, humidité et altitude pour déterminer le climat (couleur)
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

//sauvegarde de l'image de la carte
int Map::build_image(const char* filepath) {

	// création de l'image 
	BMP* img = BMP_Create(sizeX, sizeY, 24);

	// itération sur chaque pixel
	unsigned int x, y;
	for (x = 0; x < sizeX; x++) {
		for (y = 0; y < sizeY; y++) {
			//récupération de la couleur du pixel et écriture dans le fichier
			vector<unsigned int> color = getColor(x, y);
			BMP_SetPixelRGB(img, x, y, color[0], color[1], color[2]);
			//BMP_CHECK_ERROR(stdout, -1);
		}
	}
	// enregistrement du fichier image
	BMP_WriteFile(img, filepath);

	return 0;
}