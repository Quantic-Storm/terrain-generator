#pragma once
#include "HeightMap.h"
#include "Erosion.h"
#include "Verbose.h"

class Map {
private:
	size_t sizeX;				// taille sur l'axe X
	size_t sizeY;				// taille sur l'axe Y
	long int seed;				// seed de g�n�ration
	HeightMap terrain;			// heightmap contenant les valeurs topologiques
	HeightMap temperature;		// heightmap des temp�ratures
	HeightMap moisture;			// heightmap de l'humidit�

	Verbose verbose;			// gestionnaire de verbosit� 

	std::vector<unsigned int> getColor(unsigned int x, unsigned int y);

	// d�finition des couleurs pour chaque climat
	std::vector<unsigned int> snow = {255, 255, 255};
	std::vector<unsigned int> rockMountain = {173, 173, 173};
	std::vector<unsigned int> highPlain = {218, 242, 208};
	std::vector<unsigned int> highForest = {60, 125, 34};
	std::vector<unsigned int> barrenMountain = {204, 153, 0};
	std::vector<unsigned int> jungleMountain = {0, 153, 0};
	std::vector<unsigned int> tundraForest = {51, 153, 102};
	std::vector<unsigned int> plains = {146, 208, 80};
	std::vector<unsigned int> forest = {0, 204, 0};
	std::vector<unsigned int> savanna = {204, 204, 0};
	std::vector<unsigned int> denseForest = {0, 153, 0};
	std::vector<unsigned int> desert = {255, 255, 0};
	std::vector<unsigned int> jungle = {51, 102, 0};
	std::vector<unsigned int> icePlain = {218, 233, 248};
	std::vector<unsigned int> swamp = {70, 112, 27};
	std::vector<unsigned int> coldWater = {166, 201, 236};
	std::vector<unsigned int> freshWater = {77, 147, 217};
	std::vector<unsigned int> coldOcean = {21, 61, 100};
	std::vector<unsigned int> freshOcean = {33, 92, 152};

public:
	Map();
	Map(Map& map) = default;
	Map(long int customSeed, size_t sX, size_t sY);

	Map& operator=(Map& map) = default;

	int build_image(const char* filepath);
	long int getSeed() { return seed; };
};