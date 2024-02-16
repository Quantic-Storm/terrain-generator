#include "Perlin.h"
#include "Utils.h"
#include <exception>
#include <cmath>
#include <stdexcept>


Perlin::Perlin() {
	randoms = vector<unsigned char>(256);
	int rdmSeed = rand();
	this->changeSeed(rdmSeed);
}


Perlin::Perlin(long customSeed) {
	randoms = vector<unsigned char>(256);
	this->changeSeed(customSeed);
}


void Perlin::changeSeed(long newSeed) {
	unsigned int i;
	seed = newSeed;
	srand(seed);
	
	for (i = 0; i < 255; i++) {
		randoms[i] = rand() % 256;
	}
}

HeightMap* Perlin::generate(unsigned int xSize, unsigned int ySize, int chunkSize) {

	int x, y;
	HeightMap* res = new HeightMap(xSize, ySize);

	for (x = 0; x < xSize; x++) {
		for (y = 0; y < ySize; y++) {
			vector<unsigned char> v1(2), v2(2), v3(2), v4(2);
			vector<float> v1Norm(2), v2Norm(2), v3Norm(2), v4Norm(2);
			vector<float> v1Pos(2), v2Pos(2), v3Pos(2), v4Pos(2);

			v1[0] = getVecX(floor((float)x / chunkSize), floor((float)y / chunkSize));
			v1[1] = getVecY(floor((float)x / chunkSize), floor((float)y / chunkSize));

			v2[0] = getVecX(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize));
			v2[1] = getVecY(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize));

			v3[0] = getVecX(floor((float)x / chunkSize), floor((float)y / chunkSize) + 1);
			v3[1] = getVecY(floor((float)x / chunkSize), floor((float)y / chunkSize) + 1);

			v4[0] = getVecX(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize) + 1);
			v4[1] = getVecY(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize) + 1);

			v1Norm[0] = (float) cos(atan((float)v1[1] / v1[0]));
			v1Norm[1] = (float) sin(atan((float)v1[1] / v1[0]));

			v2Norm[0] = (float) cos(atan((float)v2[1] / v2[0]));
			v2Norm[1] = (float) sin(atan((float)v2[1] / v2[0]));

			v3Norm[0] = (float) cos(atan((float)v3[1] / v3[0]));
			v3Norm[1] = (float) sin(atan((float)v3[1] / v3[0]));

			v4Norm[0] = (float) cos(atan((float)v4[1] / v4[0]));
			v4Norm[1] = (float) sin(atan((float)v4[1] / v4[0]));

			v1Pos[0] = (x % chunkSize);
			v1Pos[1] = (y % chunkSize);

			v2Pos[0] = (x % chunkSize) - chunkSize - 1;
			v2Pos[1] = (y % chunkSize);

			v3Pos[0] = (x % chunkSize);
			v3Pos[1] = (y % chunkSize) - chunkSize - 1;

			v4Pos[0] = (x % chunkSize) - chunkSize - 1;
			v4Pos[1] = (y % chunkSize) - chunkSize - 1;

			v1Pos[0] /= chunkSize; v1Pos[1] /= chunkSize;
			v2Pos[0] /= chunkSize; v2Pos[1] /= chunkSize;
			v3Pos[0] /= chunkSize; v3Pos[1] /= chunkSize;
			v4Pos[0] /= chunkSize; v4Pos[1] /= chunkSize;

			float t1 = (float) abs((int) (x % chunkSize)) / chunkSize;
			float t2 = (float) abs((int) (y % chunkSize)) / chunkSize;

			float w1 = 3 * t1 * t1 - 2 * t1 * t1 * t1;
			float w2 = 3 * t2 * t2 - 2 * t2 * t2 * t2;

			float h1 = Utils::interpolate(Utils::dotProduct(v1Pos, v1Norm), Utils::dotProduct(v2Pos, v2Norm), w1);
			float h2 = Utils::interpolate(Utils::dotProduct(v3Pos, v3Norm), Utils::dotProduct(v4Pos, v4Norm), w1);
			res->setHeightValue(x, y, Utils::interpolate(h1, h2, w2));
		}
	}
	return res;
}