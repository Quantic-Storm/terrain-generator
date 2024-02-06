#pragma once
#include <vector>
#include "HeightMap.h"

using namespace std;

class Perlin {
private :
	long seed;
	vector<unsigned char> randoms;


	//allow to shuffle the values of randoms
	unsigned char getVecX(int xVec) {
		return randoms[abs(xVec * 3 % 256)];
	}
	unsigned char getVecY(int yVec) {
		return randoms[abs(yVec * -5 % 256)];
	}

public :
	Perlin();
	Perlin(long customSeed);
	Perlin(Perlin& p) = default;
	~Perlin() = default;

	HeightMap& generate(int xMin, int yMin, int xMax, int yMax, size_t chunkSize);
	void changeSeed(long newSeed);
	long getSeed() { return seed; }


};

