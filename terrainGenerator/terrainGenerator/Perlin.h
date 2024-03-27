#pragma once
#include <vector>
#include "HeightMap.h"
#include <chrono>

using namespace std;

class Perlin {
private:
	long seed;
	vector<char> randoms;

	Verbose verbose;


	//allow to shuffle the values of randoms
	unsigned char getVecX(int xVec, int yVec) {
		return randoms[abs((xVec * 3 + yVec - 2) % 256)];
	}
	unsigned char getVecY(int xVec, int yVec) {
		return randoms[abs((yVec * -5 + xVec) % 256)];
	}

public:
	Perlin();
	Perlin(long customSeed);
	Perlin(Perlin& p) = default;
	~Perlin() = default;

	void setSeed(long newSeed) {
		seed = newSeed;
		srand(seed);

		for (unsigned int i = 0; i < 256; i++)
		{
			randoms.push_back((rand() % 256));
		}
	}

	// free le pointeur !
	HeightMap* generate(unsigned int xSize, unsigned int ySize, int chunkSize);		
	void changeSeed();
	long getSeed() { return seed; }


};