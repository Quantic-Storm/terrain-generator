#include "Perlin.h"
#include <exception>
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

HeightMap& Perlin::generate(int xMin, int yMin, int xMax, int yMax, size_t chunkSize) {
	if (xMin > xMax || yMin > yMax) throw invalid_argument("negative size");
	unsigned int xSize = xMax - xMin, ySize = yMax - yMin;
	int x, y;

	for (x = xMin; x < xMax; x++) {
		for (y = yMin; y < yMax; y++) {
			vector<unsigned char> v1(2), v2(2), v3(2), v4(2);
			vector<float> v1Norm(2), v2Norm(2), v3Norm(2), v4Norm(2);

			v1[0] = (x > 0) ? getVecX(x / chunkSize) : getVecX((x / chunkSize) - 1);
			v1[1] = (y > 0) ? getVecY(y / chunkSize) : getVecY((y / chunkSize) - 1);

			v2[0] = (x > 0) ? getVecX((x / chunkSize + 1)) : getVecX(x / chunkSize);
			v2[1] = (y > 0) ? getVecY(y / chunkSize) : getVecY((y / chunkSize) - 1);

			v3[0] = (x > 0) ? getVecX(x / chunkSize) : getVecX((x / chunkSize) - 1);
			v3[1] = (y > 0) ? getVecY((y / chunkSize) + 1) : getVecY(y / chunkSize);

			v4[0] = (x > 0) ? getVecX((x / chunkSize) + 1) : getVecX(x / chunkSize);
			v4[1] = (y > 0) ? getVecY((y / chunkSize) + 1) : getVecY(y / chunkSize);

			v1Norm[0] = (float)(v1[0] / sqrt(v1[0] * v1[0] + v1[1] * v1[1]));
			v1Norm[0] = (float)(v1[1] / sqrt(v1[0] * v1[0] + v1[1] * v1[1]));

			v2Norm[0] = (float)(v2[0] / sqrt(v2[0] * v2[0] + v2[1] * v2[1]));
			v2Norm[0] = (float)(v2[1] / sqrt(v2[0] * v2[0] + v2[1] * v2[1]));

			v3Norm[0] = (float)(v3[0] / sqrt(v3[0] * v3[0] + v3[1] * v3[1]));
			v3Norm[0] = (float)(v3[1] / sqrt(v3[0] * v3[0] + v3[1] * v3[1]));

			v4Norm[0] = (float)(v4[0] / sqrt(v4[0] * v4[0] + v4[1] * v4[1]));
			v4Norm[0] = (float)(v4[1] / sqrt(v4[0] * v4[0] + v4[1] * v4[1]));
		}
	}

	HeightMap k(xSize, ySize);

	
}