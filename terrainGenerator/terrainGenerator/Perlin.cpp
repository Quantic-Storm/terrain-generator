#include "Perlin.h"
#include "Utils.h"
#include <exception>
#include <cmath>
#include <stdexcept>
#include <random>
#include <algorithm>


Perlin::Perlin() {
	randoms = vector<char>(256);
	int rdmSeed = rand();
	this->setSeed(rdmSeed);
}


Perlin::Perlin(long customSeed) {
	randoms = vector<char>(256);
	this->setSeed(customSeed);
}


void Perlin::changeSeed() {

	// Melange les valeurs al�atoires
	std::shuffle(randoms.begin(), randoms.end(), std::default_random_engine());

}

HeightMap* Perlin::generate(unsigned int xSize, unsigned int ySize, int chunkSize) {

	int x, y;
	HeightMap* res = new HeightMap(xSize, ySize);

	std::chrono::steady_clock::time_point timeStart = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point timeEnd;
	long duration;

	vector<float> v1(2), v2(2), v3(2), v4(2);

	//vector<vector<vector<vector<float>>>> chunkData(chunkSize);
	//for (int chunkX = 0; chunkX < chunkSize; chunkX++)
	//{
	//	vector<vector<vector<float>>> chunkLine(chunkSize);

	//	vector<float> v1Pos(2), v2Pos(2), v3Pos(2), v4Pos(2);
	//	v1Pos[0] = 1.0*chunkX / chunkSize;
	//	v2Pos[0] = 1.0*(chunkX - chunkSize) / chunkSize;
	//	v3Pos[0] = v1Pos[0];
	//	v4Pos[0] = v2Pos[0];

	//	for (int chunkY = 0; chunkY < chunkSize; chunkY++)
	//	{
	//		vector<vector<float>> vectors;
	//		v1Pos[1] = 1.0*chunkY/chunkSize;
	//		v2Pos[1] = v1Pos[1];
	//		v3Pos[1] = 1.0*(chunkY-chunkSize) / chunkSize;
	//		v4Pos[1] = v3Pos[1];

	//		vectors.push_back(v1Pos);
	//		vectors.push_back(v2Pos);
	//		vectors.push_back(v3Pos);
	//		vectors.push_back(v4Pos);
	//		chunkLine[chunkY] = vectors;
	//	}

	//	chunkData[chunkX] = chunkLine;
	//}


	//std::chrono::steady_clock::time_point timeEnd = std::chrono::high_resolution_clock::now();
	//long duration = std::chrono::duration_cast<std::chrono::seconds>(timeEnd - timeStart).count();

	//verbose << "Generated chunk data in " << duration << " second(s)\n";

	//// A rendre multi thread eventuellement
	//vector<float> v1Pos(2), v2Pos(2), v3Pos(2), v4Pos(2);
	//for (int posXChunk = 0; posXChunk < ceil(xSize / chunkSize); posXChunk++)
	//{
	//	for (int posYChunk = 0; posYChunk < ceil(ySize / chunkSize); posYChunk++)
	//	{
	//		v1[0] = getVecX(posXChunk, posYChunk) / 128.;
	//		v1[1] = getVecY(posXChunk, posYChunk) / 128.;

	//		v2[0] = getVecX(posXChunk + 1, posYChunk) / 128.;
	//		v2[1] = getVecY(posXChunk + 1, posYChunk) / 128.;

	//		v3[0] = getVecX(posXChunk, posYChunk + 1) / 128.;
	//		v3[1] = getVecY(posXChunk, posYChunk + 1) / 128.;

	//		v4[0] = getVecX(posXChunk + 1, posYChunk + 1) / 128.;
	//		v4[1] = getVecY(posXChunk + 1, posYChunk + 1) / 128.;

	//		for (int chunkDataX = 0; chunkDataX < chunkSize; chunkDataX++)
	//		{
	//			float x = chunkDataX + posXChunk * chunkSize;

	//			if (x >= xSize) { break; }

	//			for (int chunkDataY = 0; chunkDataY < chunkSize; chunkDataY++)
	//			{
	//				float y = chunkDataY + posYChunk * chunkSize;

	//				if (y >= ySize)
	//				{
	//					break;
	//				}

	//				v1Pos = chunkData[chunkDataX][chunkDataY][0];
	//				v2Pos = chunkData[chunkDataX][chunkDataY][1];
	//				v3Pos = chunkData[chunkDataX][chunkDataY][2];
	//				v4Pos = chunkData[chunkDataX][chunkDataY][3];

	//				float t1 = v1Pos[0]; // v1Pos[0]
	//				float t2 = v1Pos[1]; // v1Pos[1]
	//				float h1 = Utils::interpolate(Utils::dotProduct(v1Pos, v1), Utils::dotProduct(v2Pos, v2), 3 * t1 * t1 - 2 * t1 * t1 * t1);
	//				float h2 = Utils::interpolate(Utils::dotProduct(v3Pos, v3), Utils::dotProduct(v4Pos, v4), 3 * t1 * t1 - 2 * t1 * t1 * t1);
	//				res->setHeightValue(x, y, Utils::interpolate(h1, h2, 3 * t2 * t2 - 2 * t2 * t2 * t2));
	//			}
	//		}
	//	}
	//}

	//timeEnd = std::chrono::high_resolution_clock::now();
	//duration = std::chrono::duration_cast<std::chrono::seconds>(timeEnd - timeStart).count();

	//verbose << "Total time " << duration << " second(s)\n";
	//return res;

	vector<float> v1Pos(2), v2Pos(2), v3Pos(2), v4Pos(2);
	for (x = 0; x < xSize; x++) {

		v1Pos[0] = (x % chunkSize);

		v2Pos[0] = (x % chunkSize) - chunkSize;

		v3Pos[0] = v1Pos[0];

		v4Pos[0] = v2Pos[0];

		v1Pos[0] /= chunkSize;
		v2Pos[0] /= chunkSize;
		v3Pos[0] /= chunkSize;
		v4Pos[0] /= chunkSize;


		float t1 = v1Pos[0];

		float w1 = 3 * t1 * t1 - 2 * t1 * t1 * t1;

		for (y = 0; y < ySize; y++) {

			v1[0] = getVecX(floor((float)x / chunkSize), floor((float)y / chunkSize)) / 128.;
			v1[1] = getVecY(floor((float)x / chunkSize), floor((float)y / chunkSize)) / 128.;

			v2[0] = getVecX(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize)) / 128.;
			v2[1] = getVecY(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize)) / 128.;

			v3[0] = getVecX(floor((float)x / chunkSize), floor((float)y / chunkSize) + 1) / 128.;
			v3[1] = getVecY(floor((float)x / chunkSize), floor((float)y / chunkSize) + 1) / 128.;

			v4[0] = getVecX(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize) + 1) / 128.;
			v4[1] = getVecY(floor((float)x / chunkSize) + 1, floor((float)y / chunkSize) + 1) / 128.;


			v1Pos[1] = (y % chunkSize);

			v2Pos[1] = v1Pos[1];

			v3Pos[1] = (y % chunkSize) - chunkSize;

			v4Pos[1] = v3Pos[1];

			v1Pos[1] /= chunkSize;
			v2Pos[1] /= chunkSize;
			v3Pos[1] /= chunkSize;
			v4Pos[1] /= chunkSize;

			float t2 = (float)(y % chunkSize) / chunkSize;

			float w2 = 3 * t2 * t2 - 2 * t2 * t2 * t2;

			float h1 = Utils::interpolate(Utils::dotProduct(v1Pos, v1), Utils::dotProduct(v2Pos, v2), w1);
			float h2 = Utils::interpolate(Utils::dotProduct(v3Pos, v3), Utils::dotProduct(v4Pos, v4), w1);
			res->setHeightValue(x, y, Utils::interpolate(h1, h2, w2));
		}
	}

	timeEnd = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();

	verbose << duration << " ms \n";

	return res;
}