#pragma once
#include <vector>
#include "HeightMap.h"

using namespace std;

class Perlin {
private :
	long seed;
	vector<unsigned char> randoms;


	//allow to shuffle the values of randoms
	unsigned char getVecX(int xVec, int yVec) {
		return randoms[abs((xVec * 3 + yVec-2) % 256)];
	}
	unsigned char getVecY(int xVec, int yVec) {
		return randoms[abs((yVec * -5 + xVec) % 256)];
	}

public :
	Perlin();
	Perlin(long customSeed);
	Perlin(Perlin& p) = default;
	~Perlin() = default;

	HeightMap* generate(unsigned int xSize, unsigned int ySize, int chunkSize);		// free le pointeur !
	void changeSeed(long newSeed);
	long getSeed() { return seed; }


};

