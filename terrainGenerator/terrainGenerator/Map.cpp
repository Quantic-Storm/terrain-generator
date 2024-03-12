#include "map.h"
#include "Perlin.h"
#include "TurboJSON/FileParser.h"
#include "TurboJSON/FileWriter.h"


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
	HeightMap altitude = *p.generate(sizeX, sizeY, 300);
	p.changeSeed(rand());
	HeightMap erosion = *p.generate(sizeX, sizeY, 150);
	p.changeSeed(rand());
	HeightMap local = *p.generate(sizeX, sizeY, 25);
	terrain = altitude * 30 + erosion * 10 + local * 3;				// ne pas hesiter à changer les valeurs pour équilibrer
}


Map::Map(std::string filename) {
	json::FileParser fp(filename);
	fp.parse();
	json::Value data = fp.getParsed();
	sizeX = (size_t) data.getObject()->operator[]("sizeX").getDouble();
	sizeY = (size_t)data.getObject()->operator[]("sizeY").getDouble();
	seed = (size_t)data.getObject()->operator[]("seed").getDouble();
	std::vector<json::Value>* map = data.getObject()->operator[]("map").getArray();

	unsigned int i = 0;
	for (json::Value el : *map) {
		temperature.setHeightValue(i % sizeX, i / sizeX, *el.getObject()->operator[]("temp").getDouble());
		moisture.setHeightValue(i % sizeX, i / sizeX, *el.getObject()->operator[]("mois").getDouble());
		terrain.setHeightValue(i % sizeX, i / sizeX, *el.getObject()->operator[]("terr").getDouble());
	}
}

void Map::serialize(std::string filename) {
	json::FileWriter fw(filename);
	fw.getValue().addElement("sizeX", json::Value((double) sizeX));
	fw.getValue().addElement("sizeY", json::Value((double)sizeY));
	fw.getValue().addElement("seed", json::Value((double)seed));
	std::vector<json::Value> map;

	unsigned int i, j;
	for (j = 0; j < sizeY; j++) {
		for (i = 0; i < sizeX; i++) {
			unordered_map<std::string, json::Value> sample;
			sample["temp"] = json::Value((double)temperature.getHeightValue(i, j));
			sample["mois"] = json::Value((double)moisture.getHeightValue(i, j));
			sample["terr"] = json::Value((double)terrain.getHeightValue(i, j));
			map.emplace_back(sample);
		}
	}

	fw.getValue().addElement("map", json::Value(map));
}
