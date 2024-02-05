#pragma once
#include <vector>

using namespace std;

class HeightMap {
private :
	vector<vector<float>> terrain;

public :
	HeightMap() {};
	HeightMap(unsigned int width, unsigned int length) {
		terrain = vector<vector<float>>(width);
		unsigned int i;
		for (i = 0; i < terrain.size(); i++) {
			terrain[i] = vector<float>(length);
		}
	}
	HeightMap(HeightMap& hm) = default;
	HeightMap(HeightMap&& hm) = default;
	~HeightMap() = default;


	float getHeightValue(unsigned int x, unsigned int y) { return terrain[x][y]; }
	float getLength() { return terrain[0].size(); }
	float getWidth() { return terrain.size(); }
	void setHeightValue(unsigned int x, unsigned int y, float value) { terrain[x][y] = value; }
	void addHeightValue(unsigned int x, unsigned int y, float value) { terrain[x][y] += value; }

};