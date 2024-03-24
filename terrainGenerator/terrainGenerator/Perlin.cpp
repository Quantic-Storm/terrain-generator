#include "Perlin.h"
#include "Utils.h"
#include <exception>
#include <cmath>
#include <stdexcept>


Perlin::Perlin() {
	randoms = vector<char>(256);
	int rdmSeed = rand();
	this->changeSeed(rdmSeed);
}


Perlin::Perlin(long customSeed) {
	randoms = vector<char>(256);
	this->changeSeed(customSeed);
}


void Perlin::changeSeed(long newSeed) {
	unsigned int i;
	seed = newSeed;
	srand(seed);
	
	for (i = 0; i < 255; i++) {
		randoms[i] = (rand() % 256);// -128;
	}
}

HeightMap* Perlin::generate(unsigned int xSize, unsigned int ySize, int chunkSize) {

	int x, y;
	HeightMap* res = new HeightMap(xSize, ySize);

	for (x = 0; x < xSize; x++) {
		for (y = 0; y < ySize; y++) {
			vector<float> v1(2), v2(2), v3(2), v4(2);
			vector<float> v1Pos(2), v2Pos(2), v3Pos(2), v4Pos(2);

			v1[0] = getVecX(floor((float)x / chunkSize), floor((float)y / chunkSize)) / 128.;
			v1[1] = getVecY(floor((float)x / chunkSize), floor((float)y / chunkSize)) / 128.;

			v2[0] = getVecX(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize)) / 128.;
			v2[1] = getVecY(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize)) / 128.;

			v3[0] = getVecX(floor((float)x / chunkSize), floor((float)y / chunkSize) + 1) / 128.;
			v3[1] = getVecY(floor((float)x / chunkSize), floor((float)y / chunkSize) + 1) / 128.;

			v4[0] = getVecX(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize) + 1) / 128.;
			v4[1] = getVecY(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize) + 1) / 128.;


			v1Pos[0] = (x % chunkSize);
			v1Pos[1] = (y % chunkSize);

			v2Pos[0] = (x % chunkSize) - chunkSize;
			v2Pos[1] = (y % chunkSize);

			v3Pos[0] = (x % chunkSize);
			v3Pos[1] = (y % chunkSize) - chunkSize;

			v4Pos[0] = (x % chunkSize) - chunkSize;
			v4Pos[1] = (y % chunkSize) - chunkSize;

			v1Pos[0] /= chunkSize; v1Pos[1] /= chunkSize;
			v2Pos[0] /= chunkSize; v2Pos[1] /= chunkSize;
			v3Pos[0] /= chunkSize; v3Pos[1] /= chunkSize;
			v4Pos[0] /= chunkSize; v4Pos[1] /= chunkSize;

			float t1 = (float) abs((int) (x % chunkSize)) / chunkSize;
			float t2 = (float) abs((int) (y % chunkSize)) / chunkSize;

			float w1 = 3 * t1 * t1 - 2 * t1 * t1 * t1;
			float w2 = 3 * t2 * t2 - 2 * t2 * t2 * t2;

			float h1 = Utils::interpolate(Utils::dotProduct(v1Pos, v1), Utils::dotProduct(v2Pos, v2), w1);
			float h2 = Utils::interpolate(Utils::dotProduct(v3Pos, v3), Utils::dotProduct(v4Pos, v4), w1);
			res->setHeightValue(x, y, Utils::interpolate(h1, h2, w2));
		}
	}
	return res;
}